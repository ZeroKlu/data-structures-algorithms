#include <stdio.h>   // Standard I/O (printf, fopen, fgets, perror)
#include <stdlib.h>  // Memory management and conversions (calloc, free, strtol)
#include <string.h>  // String utilities (strcspn)
#include <ctype.h>   // Character handling (tolower)

/*
 * Constants:
 * - ALPHABET_SIZE: number of supported letters ('a'–'z')
 * - WORD_MAX: maximum length of a word buffer (includes space for '\0')
 * - DEFAULT_LIMIT: default number of autocomplete suggestions to print
 */
#define ALPHABET_SIZE 26
#define WORD_MAX 128
#define DEFAULT_LIMIT 20

/*
 * Trie node structure (basic version for autocomplete).
 *
 * Each node contains:
 * - children: pointers to next nodes for each letter
 * - isEndOfWord: flag indicating whether a full word ends at this node
 *
 * Note:
 * - No prefixCount / frequency data here; suggestions are enumerated by DFS.
 */
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE]; // Child pointers for letters a..z
    int isEndOfWord;                          // 1 if this node terminates a word
} TrieNode;

/*
 * Allocate and initialize a new trie node.
 *
 * calloc() zero-initializes memory so:
 * - all children pointers become NULL
 * - isEndOfWord becomes 0
 */
TrieNode *createNode(void) {
    return (TrieNode *)calloc(1, sizeof(TrieNode));
}

/*
 * Convert a lowercase character to an array index.
 *
 * Parameters:
 * - c: expected to be 'a'..'z'
 *
 * Returns:
 * - 0..25 if valid
 * - -1 if invalid
 */
int charIndex(char c) {
    if (c < 'a' || c > 'z') return -1;
    return c - 'a';
}

/* Insert a word; reject the entire word if any char is not a-z (case-insensitive). */
void trieInsert(TrieNode *root, const char *word) {
    TrieNode *current = root;

    // Traverse each character in the word
    for (int i = 0; word[i]; i++) {
        // Normalize to lowercase safely (cast to unsigned char for tolower)
        char c = tolower((unsigned char)word[i]);

        // Map to 0..25
        int idx = charIndex(c);

        // Reject entire word if any invalid character is encountered
        if (idx == -1) return;

        // Create child node if missing
        if (!current->children[idx]) current->children[idx] = createNode();

        // Move to next node
        current = current->children[idx];
    }

    // Mark final node as a complete word
    current->isEndOfWord = 1;
}

/* Walk down the trie for a prefix; returns node or NULL (invalid char or missing path). */
TrieNode *trieWalk(TrieNode *root, const char *prefix) {
    TrieNode *current = root;

    // Traverse each character in the prefix
    for (int i = 0; prefix[i]; i++) {
        char c = tolower((unsigned char)prefix[i]);
        int idx = charIndex(c);

        // Fail if invalid character or missing child path
        if (idx == -1 || !current->children[idx]) return NULL;

        current = current->children[idx];
    }

    // Successfully reached the node that corresponds to the prefix
    return current;
}

/*
 * Depth-first enumeration of words under a node.
 *
 * Parameters:
 * - node: subtree root to enumerate from
 * - buffer: mutable buffer holding the current word under construction
 * - depth: current length of the word in buffer
 * - limit: maximum number of words to output
 * - emitted: in/out count of how many words have been printed so far
 *
 * Behavior:
 * - Performs a DFS, visiting children in increasing letter order (a..z).
 * - Prints each word as it is found (when isEndOfWord is true).
 * - Stops early once `*emitted` reaches `limit`.
 */
void dfsCollect(TrieNode *node,
                char *buffer,
                int depth,
                int limit,
                int *emitted) {
    // Base cases: no node to explore, or we've already emitted enough results
    if (!node || *emitted >= limit) return;

    // If this node ends a word, print the current buffer as a completed word
    if (node->isEndOfWord) {
        buffer[depth] = '\0';     // Null-terminate the constructed word
        printf("%s\n", buffer);   // Output suggestion
        (*emitted)++;             // Track how many have been emitted
        if (*emitted >= limit) return;
    }

    // Explore all children in lexicographic order ('a' to 'z')
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            // Append this character to the buffer at the current depth
            buffer[depth] = (char)('a' + i);

            // Recurse to extend the word
            dfsCollect(node->children[i], buffer, depth + 1, limit, emitted);

            // Stop immediately once the limit is reached (avoids extra traversal)
            if (*emitted >= limit) return;
        }
    }
}

/*
 * Print up to `limit` autocomplete suggestions for `prefix`.
 *
 * Behavior:
 * - Locates the trie node for the prefix via trieWalk()
 * - Seeds a buffer with the prefix (lowercased)
 * - Performs DFS enumeration of suffixes under that node
 *
 * Ordering:
 * - Suggestions are printed in lexicographic order because dfsCollect()
 *   visits child pointers in 'a'..'z' order.
 *
 * Notes:
 * - If the prefix is invalid or not found, this function prints nothing.
 * - The current implementation does not print the prefix itself unless
 *   the prefix node is marked as isEndOfWord (i.e., the prefix is also a word).
 */
void trieAutocomplete(TrieNode *root, const char *prefix, int limit) {
    // Find the trie node that corresponds to the full prefix
    TrieNode *start = trieWalk(root, prefix);

    // If prefix is invalid or not present, no suggestions exist
    if (!start) return;

    char buffer[WORD_MAX];

    // Seed the buffer with the prefix (lowercased), and compute its length.
    // This ensures DFS appends suffix characters after the prefix.
    int depth = 0;
    for (; prefix[depth] && depth < WORD_MAX - 1; depth++) {
        buffer[depth] = (char)tolower((unsigned char)prefix[depth]);
    }

    // Track number of suggestions printed so far
    int emitted = 0;

    // Enumerate words under the prefix node, continuing from the seeded buffer
    dfsCollect(start, buffer, depth, limit, &emitted);
}

/*
 * Recursively free all nodes in the trie.
 *
 * This performs a post-order traversal:
 * - free children first, then free the current node
 */
void freeTrie(TrieNode *node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) freeTrie(node->children[i]);
    free(node);
}

/*
 * Load words from a dictionary file into the trie.
 *
 * Parameters:
 * - root: trie root node
 * - path: dictionary file path
 *
 * Returns:
 * - number of non-empty lines processed (insert attempted for each)
 * - 0 if file open fails
 *
 * Notes:
 * - Each line is stripped of trailing '\r' and '\n' characters.
 * - Words containing invalid characters are rejected by trieInsert().
 */
int loadDictionary(TrieNode *root, const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Failed to open dictionary");
        return 0;
    }

    char buffer[WORD_MAX];
    int count = 0;

    // Read file line-by-line
    while (fgets(buffer, sizeof(buffer), fp)) {
        // Remove newline / carriage return
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Skip empty lines
        if (!buffer[0]) continue;

        // Insert into trie (may be rejected if invalid)
        trieInsert(root, buffer);
        count++;
    }

    fclose(fp);
    return count;
}

/*
 * Parse a positive integer from a string, or return a default value.
 *
 * Parameters:
 * - s: string containing an integer (base 10)
 * - def: default value if parsing fails or constraints are violated
 *
 * Returns:
 * - parsed integer if valid and within constraints
 * - def otherwise
 *
 * Constraints:
 * - Must be > 0
 * - Must be <= 1,000,000 (arbitrary safety cap to avoid extreme limits)
 */
static int parseIntOrDefault(const char *s, int def) {
    if (!s || !*s) return def;

    char *end = NULL;
    long v = strtol(s, &end, 10);

    // end == s indicates no digits were parsed
    // v <= 0 disallows zero/negative limits
    // v > 1000000 enforces an upper bound
    if (end == s || v <= 0 || v > 1000000) return def;

    return (int)v;
}

/*
 * Program entry point.
 *
 * Usage:
 * - argv[1] (optional): dictionary path (default: "..\\data\\words.txt")
 * - argv[2] (optional): prefix to autocomplete (default: "ab")
 * - argv[3] (optional): suggestion limit (default: DEFAULT_LIMIT)
 *
 * Demonstrates:
 * - Loading a dictionary into a trie
 * - Printing up to N autocomplete suggestions for a prefix
 * - Proper cleanup of allocated trie memory
 */
int main(int argc, char **argv) {
    // Create trie root
    TrieNode *trie = createNode();

    // Read CLI arguments with fallbacks to lesson defaults
    const char *dictPath = (argc > 1) ? argv[1] : "..\\data\\words.txt";
    const char *prefix   = (argc > 2) ? argv[2] : "ab";
    int limit            = (argc > 3) ? parseIntOrDefault(argv[3], DEFAULT_LIMIT) : DEFAULT_LIMIT;

    // Load dictionary data
    int loaded = loadDictionary(trie, dictPath);
    printf("Loaded %d words from %s\n", loaded, dictPath);

    // Print autocomplete suggestions
    printf("\nAutocomplete(\"%s\") [limit=%d]:\n", prefix, limit);
    trieAutocomplete(trie, prefix, limit);

    // Free allocated trie memory
    freeTrie(trie);
    return 0;
}
