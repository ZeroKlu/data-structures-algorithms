#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define WORD_MAX 128

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int prefixCount;      // number of words sharing this prefix
    int isEndOfWord;
} TrieNode;

TrieNode *createNode(void) {
    return (TrieNode *)calloc(1, sizeof(TrieNode));
}

int charIndex(char c) {
    if (c < 'a' || c > 'z') return -1;
    return c - 'a';
}

/*
 * Insert a word.
 * Each node on the path increments prefixCount.
 * Rejects the entire word if any char is not a–z.
 */
void trieInsert(TrieNode *root, const char *word) {
    TrieNode *current = root;

    for (int i = 0; word[i]; i++) {
        char c = tolower((unsigned char)word[i]);
        int idx = charIndex(c);
        if (idx == -1) return;

        if (!current->children[idx]) {
            current->children[idx] = createNode();
        }

        current = current->children[idx];
        current->prefixCount++;
    }

    current->isEndOfWord = 1;
}

/*
 * Count how many words start with the given prefix.
 * Returns 0 if the prefix is invalid or not found.
 */
int triePrefixCount(TrieNode *root, const char *prefix) {
    TrieNode *current = root;

    for (int i = 0; prefix[i]; i++) {
        char c = tolower((unsigned char)prefix[i]);
        int idx = charIndex(c);
        if (idx == -1 || !current->children[idx])
            return 0;

        current = current->children[idx];
    }

    return current->prefixCount;
}

void freeTrie(TrieNode *node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(node->children[i]);
    }
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
        if (buffer[0] == '\0') continue;
        trieInsert(root, buffer);
        count++;
    }

    fclose(fp);
    return count;
}

int main(void) {
    TrieNode *trie = createNode();

    const char *dictPath = "..\\data\\words.txt";
    int loaded = loadDictionary(trie, dictPath);
    printf("Loaded %d words from %s\n\n", loaded, dictPath);

    const char *prefixes[] = {
        "a",
        "ab",
        "alg",
        "aard",
        "z",
        "nope"
    };

    for (int i = 0; i < 6; i++) {
        printf("prefixCount(\"%s\") = %d\n",
               prefixes[i],
               triePrefixCount(trie, prefixes[i]));
    }

    freeTrie(trie);
    return 0;
}
