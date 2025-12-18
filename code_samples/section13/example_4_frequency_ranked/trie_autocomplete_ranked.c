
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define WORD_MAX 128
#define DEFAULT_LIMIT 20

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    int frequency;              // frequency for this complete word (only meaningful if isEndOfWord=1)
} TrieNode;

static TrieNode *createNode(void) {
    return (TrieNode *)calloc(1, sizeof(TrieNode));
}

static int charIndex(char c) {
    if (c < 'a' || c > 'z') return -1;
    return c - 'a';
}

/*
 * Insert a word.
 * If `bumpFrequency` is non-zero, increments the terminal node's frequency.
 * Rejects the entire word if any char is not a-z (case-insensitive).
 */
static void trieInsert(TrieNode *root, const char *word, int bumpFrequency) {
    TrieNode *current = root;
    for (int i = 0; word[i]; i++) {
        char c = (char)tolower((unsigned char)word[i]);
        int idx = charIndex(c);
        if (idx == -1) return;
        if (!current->children[idx]) current->children[idx] = createNode();
        current = current->children[idx];
    }
    current->isEndOfWord = 1;
    if (bumpFrequency) current->frequency += 1;
}

/* Walk down the trie for a prefix; returns node or NULL (invalid char or missing path). */
static TrieNode *trieWalk(TrieNode *root, const char *prefix) {
    TrieNode *current = root;
    for (int i = 0; prefix[i]; i++) {
        char c = (char)tolower((unsigned char)prefix[i]);
        int idx = charIndex(c);
        if (idx == -1 || !current->children[idx]) return NULL;
        current = current->children[idx];
    }
    return current;
}

/* -------- Top-K min-heap (by frequency, then reverse-lex so we can pop "worst") -------- */

typedef struct Suggestion {
    char word[WORD_MAX];
    int frequency;
} Suggestion;

/*
 * "Worse" means: lower frequency, or same frequency and lexicographically larger.
 * We keep a min-heap where the root is the worst item among the current top-K.
 */
static int isWorseThan(const Suggestion *a, const Suggestion *b) {
    if (a->frequency != b->frequency) return a->frequency < b->frequency;
    return strcmp(a->word, b->word) > 0;
}

static void heapSwap(Suggestion *a, Suggestion *b) {
    Suggestion tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heapSiftUp(Suggestion *heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (!isWorseThan(&heap[idx], &heap[parent])) break;
        heapSwap(&heap[idx], &heap[parent]);
        idx = parent;
    }
}

static void heapSiftDown(Suggestion *heap, int size, int idx) {
    for (;;) {
        int left = idx * 2 + 1;
        int right = left + 1;
        int smallest = idx;

        if (left < size && isWorseThan(&heap[left], &heap[smallest])) smallest = left;
        if (right < size && isWorseThan(&heap[right], &heap[smallest])) smallest = right;

        if (smallest == idx) break;
        heapSwap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/* If heap not full, push. If full and candidate is better than root, replace root. */
static void heapConsider(Suggestion *heap, int *size, int capacity, const Suggestion *cand) {
    if (capacity <= 0) return;

    if (*size < capacity) {
        heap[*size] = *cand;
        heapSiftUp(heap, *size);
        (*size)++;
        return;
    }

    // root is the "worst" among the current top-K
    if (isWorseThan(&heap[0], cand)) {
        heap[0] = *cand;
        heapSiftDown(heap, *size, 0);
    }
}

static int betterForOutput(const void *pa, const void *pb) {
    const Suggestion *a = (const Suggestion *)pa;
    const Suggestion *b = (const Suggestion *)pb;
    // Sort best-first: higher frequency first, then lexicographically smaller
    if (a->frequency != b->frequency) return (b->frequency - a->frequency);
    return strcmp(a->word, b->word);
}

/* Depth-first traversal collecting top-K suggestions (by frequency). */
static void dfsRanked(TrieNode *node,
                      char *buffer,
                      int depth,
                      Suggestion *heap,
                      int *heapSize,
                      int limit) {
    if (!node) return;

    if (node->isEndOfWord) {
        buffer[depth] = '\0';
        Suggestion s;
        strncpy(s.word, buffer, WORD_MAX - 1);
        s.word[WORD_MAX - 1] = '\0';
        s.frequency = node->frequency;
        heapConsider(heap, heapSize, limit, &s);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            buffer[depth] = (char)('a' + i);
            dfsRanked(node->children[i], buffer, depth + 1, heap, heapSize, limit);
        }
    }
}

/*
 * Ranked autocomplete:
 * - Collect up to `limit` best matches under the prefix, ranked by frequency desc, then word asc.
 * - Prints each suggestion with its frequency.
 */
static void trieAutocompleteRanked(TrieNode *root, const char *prefix, int limit) {
    TrieNode *start = trieWalk(root, prefix);
    if (!start || limit <= 0) return;

    char buffer[WORD_MAX];
    int depth = 0;
    for (; prefix[depth] && depth < WORD_MAX - 1; depth++) {
        buffer[depth] = (char)tolower((unsigned char)prefix[depth]);
    }

    Suggestion *heap = (Suggestion *)malloc(sizeof(Suggestion) * (size_t)limit);
    int heapSize = 0;

    dfsRanked(start, buffer, depth, heap, &heapSize, limit);

    qsort(heap, (size_t)heapSize, sizeof(Suggestion), betterForOutput);

    for (int i = 0; i < heapSize; i++) {
        printf("%s\t(freq=%d)\n", heap[i].word, heap[i].frequency);
    }

    free(heap);
}

static void freeTrie(TrieNode *node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) freeTrie(node->children[i]);
    free(node);
}

/*
 * Load dictionary: inserts every word and sets baseline frequency=1 for each inserted word.
 * (If the dictionary contains duplicates, frequency increases accordingly.)
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
        trieInsert(root, buffer, 1);
        count++;
    }

    fclose(fp);
    return count;
}

/*
 * OPTIONAL: load a "usage" file where each line is a word the user typed.
 * Each occurrence bumps that word's frequency (and creates it if missing).
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
        trieInsert(root, buffer, 1);
        count++;
    }

    fclose(fp);
    return count;
}

static int parseIntOrDefault(const char *s, int def) {
    if (!s || !*s) return def;
    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s || v <= 0 || v > 1000000) return def;
    return (int)v;
}

int main(int argc, char **argv) {
    TrieNode *trie = createNode();

    const char *dictPath = (argc > 1) ? argv[1] : "..\\data\\frequency.txt";
    const char *prefix   = (argc > 2) ? argv[2] : "th";
    int limit            = (argc > 3) ? parseIntOrDefault(argv[3], DEFAULT_LIMIT) : DEFAULT_LIMIT;

    int loaded = loadDictionary(trie, dictPath);
    printf("Loaded %d words from %s\n", loaded, dictPath);

    // Optional 4th arg: usage file
    if (argc > 4) {
        int used = loadUsageFile(trie, argv[4]);
        printf("Loaded %d usage events from %s\n", used, argv[4]);
    }

    printf("\nRanked Autocomplete(\"%s\") [limit=%d]:\n", prefix, limit);
    trieAutocompleteRanked(trie, prefix, limit);

    freeTrie(trie);
    return 0;
}
