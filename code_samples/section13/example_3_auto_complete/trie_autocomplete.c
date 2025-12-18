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
} TrieNode;

TrieNode *createNode(void) {
    return (TrieNode *)calloc(1, sizeof(TrieNode));
}

int charIndex(char c) {
    if (c < 'a' || c > 'z') return -1;
    return c - 'a';
}

/* Insert a word; reject the entire word if any char is not a-z (case-insensitive). */
void trieInsert(TrieNode *root, const char *word) {
    TrieNode *current = root;
    for (int i = 0; word[i]; i++) {
        char c = tolower((unsigned char)word[i]);
        int idx = charIndex(c);
        if (idx == -1) return;
        if (!current->children[idx]) current->children[idx] = createNode();
        current = current->children[idx];
    }
    current->isEndOfWord = 1;
}

/* Walk down the trie for a prefix; returns node or NULL (invalid char or missing path). */
TrieNode *trieWalk(TrieNode *root, const char *prefix) {
    TrieNode *current = root;
    for (int i = 0; prefix[i]; i++) {
        char c = tolower((unsigned char)prefix[i]);
        int idx = charIndex(c);
        if (idx == -1 || !current->children[idx]) return NULL;
        current = current->children[idx];
    }
    return current;
}

/* Depth-first enumeration of words under a node. */
void dfsCollect(TrieNode *node,
                char *buffer,
                int depth,
                int limit,
                int *emitted) {
    if (!node || *emitted >= limit) return;

    if (node->isEndOfWord) {
        buffer[depth] = '\0';
        printf("%s\n", buffer);
        (*emitted)++;
        if (*emitted >= limit) return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            buffer[depth] = (char)('a' + i);
            dfsCollect(node->children[i], buffer, depth + 1, limit, emitted);
            if (*emitted >= limit) return;
        }
    }
}

/*
 * Print up to `limit` autocomplete suggestions for `prefix`.
 * Suggestions are returned in lexicographic order due to DFS over children a..z.
 */
void trieAutocomplete(TrieNode *root, const char *prefix, int limit) {
    TrieNode *start = trieWalk(root, prefix);
    if (!start) return;

    char buffer[WORD_MAX];
    // seed the buffer with the prefix (lowercased)
    int depth = 0;
    for (; prefix[depth] && depth < WORD_MAX - 1; depth++) {
        buffer[depth] = (char)tolower((unsigned char)prefix[depth]);
    }

    int emitted = 0;
    dfsCollect(start, buffer, depth, limit, &emitted);
}

void freeTrie(TrieNode *node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) freeTrie(node->children[i]);
    free(node);
}

int loadDictionary(TrieNode *root, const char *path) {
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
        trieInsert(root, buffer);
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

    const char *dictPath = (argc > 1) ? argv[1] : "..\\data\\words.txt";
    const char *prefix   = (argc > 2) ? argv[2] : "ab";
    int limit            = (argc > 3) ? parseIntOrDefault(argv[3], DEFAULT_LIMIT) : DEFAULT_LIMIT;

    int loaded = loadDictionary(trie, dictPath);
    printf("Loaded %d words from %s\n", loaded, dictPath);

    printf("\nAutocomplete(\"%s\") [limit=%d]:\n", prefix, limit);
    trieAutocomplete(trie, prefix, limit);

    freeTrie(trie);
    return 0;
}
