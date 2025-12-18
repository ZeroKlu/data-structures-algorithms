#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define WORD_MAX 128

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
    }
    current->isEndOfWord = 1;
}

int trieSearch(TrieNode *root, const char *word) {
    TrieNode *current = root;
    for (int i = 0; word[i]; i++) {
        char c = tolower((unsigned char)word[i]);
        int idx = charIndex(c);
        if (idx == -1 || !current->children[idx]) return 0;
        current = current->children[idx];
    }
    return current && current->isEndOfWord;
}

int trieStartsWith(TrieNode *root, const char *prefix) {
    TrieNode *current = root;
    for (int i = 0; prefix[i]; i++) {
        char c = tolower((unsigned char)prefix[i]);
        int idx = charIndex(c);
        if (idx == -1 || !current->children[idx]) return 0;
        current = current->children[idx];
    }
    return 1;
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
        if (buffer[0]) {
            trieInsert(root, buffer);
            count++;
        }
    }

    fclose(fp);
    return count;
}

int main(void) {
    TrieNode *trie = createNode();

    const char *dictPath = "..\\data\\words.txt";
    int loaded = loadDictionary(trie, dictPath);
    printf("Loaded %d words from %s\n\n", loaded, dictPath);

    const char *tests[] = {"aardvark", "abandon", "zebra", "notaword"};
    for (int i = 0; i < 4; i++) {
        printf("search(\"%s\") = %s\n",
               tests[i],
               trieSearch(trie, tests[i]) ? "true" : "false");
    }

    printf("\nstartsWith(\"ab\") = %s\n",
           trieStartsWith(trie, "ab") ? "true" : "false");

    freeTrie(trie);
    return 0;
}
