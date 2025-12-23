#include <stdio.h>   // Standard I/O functions (printf, fopen, fgets, perror)
#include <stdlib.h>  // Memory allocation and utility functions (calloc, free)
#include <string.h>  // String handling utilities (strcspn)
#include <ctype.h>   // Character classification/conversion (tolower)

/*
 * Number of lowercase English letters supported by the trie.
 * This implementation only supports 'a'–'z'.
 */
#define ALPHABET_SIZE 26

/*
 * Maximum length of a word read from the dictionary file.
 * Includes space for the null terminator.
 */
#define WORD_MAX 128

/*
 * Trie node structure.
 *
 * Each node contains:
 * - An array of child pointers, one for each letter 'a'–'z'
 * - A flag indicating whether this node marks the end of a valid word
 */
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE]; // Pointers to child nodes
    int isEndOfWord;                          // 1 if this node ends a word
} TrieNode;

/*
 * Allocates and initializes a new trie node.
 *
 * calloc is used so:
 * - All child pointers are initialized to NULL
 * - isEndOfWord is initialized to 0
 *
 * Returns:
 * - Pointer to a newly allocated TrieNode
 */
TrieNode *createNode(void) {
    return (TrieNode *)calloc(1, sizeof(TrieNode));
}

/*
 * Converts a lowercase character into a trie array index.
 *
 * Parameters:
 * - c: character expected to be between 'a' and 'z'
 *
 * Returns:
 * - Index in range [0, 25] if valid
 * - -1 if the character is outside 'a'–'z'
 */
int charIndex(char c) {
    if (c < 'a' || c > 'z') return -1;
    return c - 'a';
}

/*
 * Inserts a word into the trie.
 *
 * Behavior:
 * - Converts characters to lowercase
 * - Rejects words containing non-alphabetic characters
 * - Creates nodes as needed
 *
 * Parameters:
 * - root: root node of the trie
 * - word: null-terminated string to insert
 */
void trieInsert(TrieNode *root, const char *word) {
    TrieNode *current = root;

    // Iterate over each character in the word
    for (int i = 0; word[i]; i++) {
        // Convert character to lowercase safely
        char c = tolower((unsigned char)word[i]);

        // Map character to child index
        int idx = charIndex(c);

        // Abort insertion if an invalid character is encountered
        if (idx == -1) return;

        // Create a new node if this path does not yet exist
        if (!current->children[idx]) {
            current->children[idx] = createNode();
        }

        // Move to the next node in the trie
        current = current->children[idx];
    }

    // Mark the final node as representing a complete word
    current->isEndOfWord = 1;
}

/*
 * Searches for a complete word in the trie.
 *
 * Parameters:
 * - root: root node of the trie
 * - word: word to search for
 *
 * Returns:
 * - 1 if the word exists and is marked as complete
 * - 0 otherwise
 */
int trieSearch(TrieNode *root, const char *word) {
    TrieNode *current = root;

    // Traverse the trie following each character
    for (int i = 0; word[i]; i++) {
        char c = tolower((unsigned char)word[i]);
        int idx = charIndex(c);

        // Fail if character is invalid or path does not exist
        if (idx == -1 || !current->children[idx]) return 0;

        current = current->children[idx];
    }

    // Word exists only if final node is marked as an end-of-word
    return current && current->isEndOfWord;
}

/*
 * Checks whether any word in the trie starts with the given prefix.
 *
 * Parameters:
 * - root: root node of the trie
 * - prefix: prefix string to check
 *
 * Returns:
 * - 1 if the prefix exists in the trie
 * - 0 otherwise
 */
int trieStartsWith(TrieNode *root, const char *prefix) {
    TrieNode *current = root;

    // Traverse the trie for each prefix character
    for (int i = 0; prefix[i]; i++) {
        char c = tolower((unsigned char)prefix[i]);
        int idx = charIndex(c);

        // Fail if character is invalid or missing from trie
        if (idx == -1 || !current->children[idx]) return 0;

        current = current->children[idx];
    }

    // Successfully reached the end of the prefix
    return 1;
}

/*
 * Recursively frees all nodes in the trie.
 *
 * Parameters:
 * - node: current node to free
 *
 * This performs a post-order traversal to ensure all children
 * are freed before the parent node.
 */
void freeTrie(TrieNode *node) {
    if (!node) return;

    // Recursively free all child subtrees
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(node->children[i]);
    }

    // Free the current node
    free(node);
}

/*
 * Loads words from a dictionary file into the trie.
 *
 * Parameters:
 * - root: root node of the trie
 * - path: file system path to the dictionary file
 *
 * Returns:
 * - Number of words successfully loaded
 * - 0 if the file could not be opened
 */
int loadDictionary(TrieNode *root, const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Failed to open dictionary");
        return 0;
    }

    char buffer[WORD_MAX]; // Buffer to hold each line/word
    int count = 0;         // Number of words loaded

    // Read file line by line
    while (fgets(buffer, sizeof(buffer), fp)) {
        // Remove trailing newline and carriage return characters
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Insert non-empty lines into the trie
        if (buffer[0]) {
            trieInsert(root, buffer);
            count++;
        }
    }

    fclose(fp);
    return count;
}

/*
 * Program entry point.
 *
 * Demonstrates:
 * - Trie creation
 * - Dictionary loading
 * - Word search
 * - Prefix search
 * - Memory cleanup
 */
int main(void) {
    // Create the root node of the trie
    TrieNode *trie = createNode();

    // Path to dictionary file
    const char *dictPath = "..\\data\\words.txt";

    // Load dictionary words into the trie
    int loaded = loadDictionary(trie, dictPath);
    printf("Loaded %d words from %s\n\n", loaded, dictPath);

    // Test words to search in the trie
    const char *tests[] = {"aardvark", "abandon", "zebra", "notaword"};

    // Perform exact-word searches
    for (int i = 0; i < 4; i++) {
        printf("search(\"%s\") = %s\n",
               tests[i],
               trieSearch(trie, tests[i]) ? "true" : "false");
    }

    // Perform a prefix search
    printf("\nstartsWith(\"ab\") = %s\n",
           trieStartsWith(trie, "ab") ? "true" : "false");

    // Free all allocated trie memory
    freeTrie(trie);
    return 0;
}
