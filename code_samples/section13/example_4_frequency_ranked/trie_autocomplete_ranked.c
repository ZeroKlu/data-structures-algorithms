#include <stdio.h>   // Standard I/O (printf, fopen, fgets, perror)
#include <stdlib.h>  // Memory management and conversions (calloc, malloc, free, strtol, qsort)
#include <string.h>  // String utilities (strcspn, strncpy, strcmp)
#include <ctype.h>   // Character conversion (tolower)

/*
 * Constants:
 * - ALPHABET_SIZE: number of supported letters ('a'–'z')
 * - WORD_MAX: maximum word length (including null terminator)
 * - DEFAULT_LIMIT: default number of autocomplete suggestions to output
 */
#define ALPHABET_SIZE 26
#define WORD_MAX 128
#define DEFAULT_LIMIT 20

/*
 * Trie node structure for ranked autocomplete.
 *
 * Each node contains:
 * - children: pointers to child nodes for each character 'a'..'z'
 * - isEndOfWord: indicates whether a complete word terminates at this node
 * - frequency: score/frequency for the complete word ending here
 *              (only meaningful when isEndOfWord == 1)
 *
 * Notes:
 * - This implementation stores frequency only on terminal nodes (complete words).
 * - Non-terminal nodes may have frequency==0 and isEndOfWord==0.
 */
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE]; // Child pointers
    int isEndOfWord;                          // End-of-word marker (1/0)
    int frequency;                            // Word frequency (for terminal nodes)
} TrieNode;

/*
 * Allocate and zero-initialize a new trie node.
 *
 * calloc() ensures:
 * - children[] pointers are NULL
 * - isEndOfWord == 0
 * - frequency == 0
 */
static TrieNode *createNode(void) {
    return (TrieNode *)calloc(1, sizeof(TrieNode));
}

/*
 * Convert a lowercase character to its child array index.
 *
 * Parameters:
 * - c: expected to be between 'a' and 'z'
 *
 * Returns:
 * - 0..25 if valid
 * - -1 if invalid
 */
static int charIndex(char c) {
    if (c < 'a' || c > 'z') return -1;
    return c - 'a';
}

/*
 * Insert a word into the trie.
 *
 * Parameters:
 * - root: trie root node
 * - word: null-terminated string to insert
 * - bumpFrequency: if non-zero, increments the terminal node's frequency
 *
 * Behavior:
 * - Case-insensitive: lowercases each character
 * - Rejects the entire word if any character is not 'a'..'z'
 * - Creates missing nodes lazily
 * - Marks the final node as end-of-word
 * - Optionally increments frequency for the terminal node
 *
 * Important nuance:
 * - When bumpFrequency is 0, this still marks the word as present (isEndOfWord=1)
 *   but leaves frequency unchanged.
 */
static void trieInsert(TrieNode *root, const char *word, int bumpFrequency) {
    TrieNode *current = root;

    // Traverse each character in the word
    for (int i = 0; word[i]; i++) {
        // Normalize to lowercase safely
        char c = (char)tolower((unsigned char)word[i]);
        int idx = charIndex(c);

        // Reject entire word if invalid character is encountered
        if (idx == -1) return;

        // Create child node if missing
        if (!current->children[idx]) current->children[idx] = createNode();

        // Advance to next node
        current = current->children[idx];
    }

    // Mark that this path forms a complete word
    current->isEndOfWord = 1;

    // Optionally update usage/frequency count
    if (bumpFrequency) current->frequency += 1;
}

/*
 * Walk down the trie following a prefix and return the node at the end of the prefix.
 *
 * Parameters:
 * - root: trie root node
 * - prefix: prefix string
 *
 * Returns:
 * - TrieNode* corresponding to the last character of prefix
 * - NULL if prefix contains invalid characters or the path does not exist
 */
static TrieNode *trieWalk(TrieNode *root, const char *prefix) {
    TrieNode *current = root;

    for (int i = 0; prefix[i]; i++) {
        char c = (char)tolower((unsigned char)prefix[i]);
        int idx = charIndex(c);

        // Invalid character or missing node -> prefix not found
        if (idx == -1 || !current->children[idx]) return NULL;

        current = current->children[idx];
    }

    return current;
}

/* -------- Top-K min-heap (by frequency, then reverse-lex so we can pop "worst") -------- */

/*
 * Suggestion record stored in the heap.
 *
 * - word: the full suggestion string (null-terminated)
 * - frequency: its associated score
 */
typedef struct Suggestion {
    char word[WORD_MAX];
    int frequency;
} Suggestion;

/*
 * Determine whether suggestion `a` is "worse" than suggestion `b`.
 *
 * "Worse" ordering defines the heap priority (min-heap of the worst item):
 * - Lower frequency is worse
 * - If frequencies tie, lexicographically larger word is worse
 *   (so that among ties, we keep the lexicographically smaller items)
 *
 * This ordering ensures:
 * - heap[0] is always the worst among the current top-K,
 *   making replacement decisions efficient.
 */
static int isWorseThan(const Suggestion *a, const Suggestion *b) {
    if (a->frequency != b->frequency) return a->frequency < b->frequency;
    return strcmp(a->word, b->word) > 0;
}

/*
 * Swap two Suggestion structs in-place.
 *
 * Used by heap sift operations.
 */
static void heapSwap(Suggestion *a, Suggestion *b) {
    Suggestion tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
 * Restore heap property by moving an element up toward the root.
 *
 * Parameters:
 * - heap: array representing the heap
 * - idx: index of the element to sift upward
 *
 * Heap invariant:
 * - Parent must be <= child in "worse" ordering (i.e., root is worst).
 */
static void heapSiftUp(Suggestion *heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;

        // If child is not worse than parent, heap property holds
        if (!isWorseThan(&heap[idx], &heap[parent])) break;

        // Otherwise swap and continue upward
        heapSwap(&heap[idx], &heap[parent]);
        idx = parent;
    }
}

/*
 * Restore heap property by moving an element down away from the root.
 *
 * Parameters:
 * - heap: array representing the heap
 * - size: number of valid elements in heap
 * - idx: index of the element to sift downward
 */
static void heapSiftDown(Suggestion *heap, int size, int idx) {
    for (;;) {
        int left = idx * 2 + 1;
        int right = left + 1;
        int smallest = idx; // "smallest" here means "worst" per isWorseThan ordering

        // Choose the worse child if it violates the heap invariant
        if (left < size && isWorseThan(&heap[left], &heap[smallest])) smallest = left;
        if (right < size && isWorseThan(&heap[right], &heap[smallest])) smallest = right;

        // If no child is worse than current, heap is valid
        if (smallest == idx) break;

        heapSwap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/*
 * Consider a candidate suggestion for inclusion in the top-K heap.
 *
 * Parameters:
 * - heap: array representing the heap
 * - size: in/out size of heap (number of valid elements)
 * - capacity: heap capacity (K)
 * - cand: candidate suggestion
 *
 * Behavior:
 * - If heap is not full, push candidate.
 * - If heap is full and candidate is better than the current root (worst),
 *   replace root with candidate and sift down.
 *
 * "Better" is the inverse of "worse":
 * - Higher frequency wins
 * - If tie, lexicographically smaller wins
 */
static void heapConsider(Suggestion *heap, int *size, int capacity, const Suggestion *cand) {
    if (capacity <= 0) return;

    // If heap not yet full, append and sift up into position
    if (*size < capacity) {
        heap[*size] = *cand;
        heapSiftUp(heap, *size);
        (*size)++;
        return;
    }

    // Root is the worst among current top-K.
    // Replace it only if the candidate is better than that worst.
    if (isWorseThan(&heap[0], cand)) {
        heap[0] = *cand;
        heapSiftDown(heap, *size, 0);
    }
}

/*
 * Comparator for final output ordering.
 *
 * Sort best-first:
 * - Higher frequency first
 * - If tie, lexicographically smaller first
 *
 * Used with qsort() after we finish building the heap.
 */
static int betterForOutput(const void *pa, const void *pb) {
    const Suggestion *a = (const Suggestion *)pa;
    const Suggestion *b = (const Suggestion *)pb;

    // Higher frequency should come earlier
    if (a->frequency != b->frequency) return (b->frequency - a->frequency);

    // For ties, lexicographically smaller should come earlier
    return strcmp(a->word, b->word);
}

/*
 * Depth-first traversal collecting top-K suggestions (by frequency).
 *
 * Parameters:
 * - node: subtree root to explore
 * - buffer: mutable buffer holding the current word under construction
 * - depth: current length of the word in buffer
 * - heap: top-K heap storage
 * - heapSize: in/out number of items currently in heap
 * - limit: maximum number of suggestions to keep (K)
 *
 * Behavior:
 * - When a terminal node is encountered, it forms a candidate Suggestion.
 * - The candidate is inserted into the top-K heap via heapConsider().
 * - DFS explores children in 'a'..'z' order, but ranking is driven by frequency.
 *
 * Note:
 * - This DFS does not stop after limit candidates; it traverses the entire
 *   subtree to find the actual top-K by frequency.
 */
static void dfsRanked(TrieNode *node,
                      char *buffer,
                      int depth,
                      Suggestion *heap,
                      int *heapSize,
                      int limit) {
    if (!node) return;

    // If this node represents a complete word, consider it as a candidate
    if (node->isEndOfWord) {
        buffer[depth] = '\0'; // Null-terminate for safe copying/comparison

        Suggestion s;

        // Copy the current word into the suggestion struct
        strncpy(s.word, buffer, WORD_MAX - 1);
        s.word[WORD_MAX - 1] = '\0'; // Ensure null-termination

        // Capture the terminal node's frequency as the ranking score
        s.frequency = node->frequency;

        // Consider candidate for inclusion in the top-K heap
        heapConsider(heap, heapSize, limit, &s);
    }

    // Explore all child nodes
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            buffer[depth] = (char)('a' + i);
            dfsRanked(node->children[i], buffer, depth + 1, heap, heapSize, limit);
        }
    }
}

/*
 * Ranked autocomplete:
 * - Collect up to `limit` best matches under the prefix
 * - Rank by frequency (descending), then by word (ascending)
 * - Print each suggestion and its frequency
 *
 * Parameters:
 * - root: trie root node
 * - prefix: prefix to match
 * - limit: max number of suggestions to print
 *
 * Steps:
 * 1) Walk to the prefix node.
 * 2) DFS the subtree to gather top-K candidates using a min-heap.
 * 3) Sort the heap items into best-first output order.
 * 4) Print results.
 */
static void trieAutocompleteRanked(TrieNode *root, const char *prefix, int limit) {
    TrieNode *start = trieWalk(root, prefix);

    // No matches if prefix not found or limit is non-positive
    if (!start || limit <= 0) return;

    char buffer[WORD_MAX];

    // Seed the buffer with the lowercased prefix so DFS can append suffixes
    int depth = 0;
    for (; prefix[depth] && depth < WORD_MAX - 1; depth++) {
        buffer[depth] = (char)tolower((unsigned char)prefix[depth]);
    }

    // Allocate heap storage for up to `limit` suggestions
    Suggestion *heap = (Suggestion *)malloc(sizeof(Suggestion) * (size_t)limit);
    int heapSize = 0;

    // Traverse subtree and build the top-K heap
    dfsRanked(start, buffer, depth, heap, &heapSize, limit);

    // Sort collected heap items into best-first output order
    qsort(heap, (size_t)heapSize, sizeof(Suggestion), betterForOutput);

    // Print final ranked suggestions with their frequencies
    for (int i = 0; i < heapSize; i++) {
        printf("%s\t(freq=%d)\n", heap[i].word, heap[i].frequency);
    }

    // Clean up heap allocation
    free(heap);
}

/*
 * Recursively free all nodes in the trie.
 *
 * Performs a post-order traversal:
 * - free children first, then free the node itself.
 */
static void freeTrie(TrieNode *node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) freeTrie(node->children[i]);
    free(node);
}

/*
 * Load dictionary: inserts every word and sets baseline frequency=1 for each inserted word.
 *
 * Parameters:
 * - root: trie root
 * - path: path to dictionary file
 *
 * Returns:
 * - number of non-empty lines processed (insert attempted for each)
 * - 0 if file open fails
 *
 * Notes:
 * - If the dictionary contains duplicates, each insertion bumps frequency again.
 * - Words containing invalid characters are rejected by trieInsert().
 */
static int loadDictionary(TrieNode *root, const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Failed to open dictionary");
        return 0;
    }

    char buffer[WORD_MAX];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (!buffer[0]) continue;

        // bumpFrequency=1 => baseline frequency is 1 per inserted dictionary word
        trieInsert(root, buffer, 1);
        count++;
    }

    fclose(fp);
    return count;
}

/*
 * OPTIONAL: load a "usage" file where each line is a word the user typed.
 * Each occurrence bumps that word's frequency (and creates it if missing).
 *
 * Parameters:
 * - root: trie root
 * - path: usage file path
 *
 * Returns:
 * - number of non-empty lines processed (insert attempted for each)
 * - 0 if file open fails
 *
 * Notes:
 * - This uses trieInsert(..., bumpFrequency=1), so it both ensures the word exists
 *   and increments its frequency.
 * - If usage includes words not in the dictionary, they will be added to the trie.
 */
static int loadUsageFile(TrieNode *root, const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Failed to open usage file");
        return 0;
    }

    char buffer[WORD_MAX];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (!buffer[0]) continue;

        // Treat each line as one usage event for that word
        trieInsert(root, buffer, 1);
        count++;
    }

    fclose(fp);
    return count;
}

/*
 * Parse a positive integer from a C string or return a default.
 *
 * Parameters:
 * - s: numeric string (base 10)
 * - def: fallback value if parsing fails or produces an invalid value
 *
 * Returns:
 * - parsed int if valid and within constraints
 * - def otherwise
 *
 * Constraints:
 * - must be > 0
 * - must be <= 1,000,000 (arbitrary safety cap)
 */
static int parseIntOrDefault(const char *s, int def) {
    if (!s || !*s) return def;
    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s || v <= 0 || v > 1000000) return def;
    return (int)v;
}

/*
 * Program entry point.
 *
 * Usage:
 * - argv[1] (optional): dictionary path (default: "..\\data\\frequency.txt")
 * - argv[2] (optional): prefix to autocomplete (default: "th")
 * - argv[3] (optional): limit (default: DEFAULT_LIMIT)
 * - argv[4] (optional): usage file path (each line bumps frequency for that word)
 *
 * Demonstrates:
 * - Loading dictionary words (baseline frequency bumps)
 * - Optionally loading usage events to further adjust frequencies
 * - Printing ranked autocomplete suggestions for a prefix
 */
int main(int argc, char **argv) {
    // Create trie root
    TrieNode *trie = createNode();

    // Read CLI args with fallbacks to lesson defaults
    const char *dictPath = (argc > 1) ? argv[1] : "..\\data\\frequency.txt";
    const char *prefix   = (argc > 2) ? argv[2] : "th";
    int limit            = (argc > 3) ? parseIntOrDefault(argv[3], DEFAULT_LIMIT) : DEFAULT_LIMIT;

    // Load dictionary words into the trie (baseline frequency bumps)
    int loaded = loadDictionary(trie, dictPath);
    printf("Loaded %d words from %s\n", loaded, dictPath);

    // Optional 4th arg: usage file (further bumps word frequencies)
    if (argc > 4) {
        int used = loadUsageFile(trie, argv[4]);
        printf("Loaded %d usage events from %s\n", used, argv[4]);
    }

    // Run ranked autocomplete query and print results
    printf("\nRanked Autocomplete(\"%s\") [limit=%d]:\n", prefix, limit);
    trieAutocompleteRanked(trie, prefix, limit);

    // Free allocated trie memory
    freeTrie(trie);
    return 0;
}
