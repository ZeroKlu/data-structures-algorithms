#include <stdio.h>   // Standard I/O (printf, fopen, fgets, perror)
#include <stdlib.h>  // Memory allocation (calloc, free)
#include <string.h>  // String utilities (strcspn)
#include <ctype.h>   // Character classification/conversion (tolower)

/*
 * Number of supported lowercase English letters.
 */
#define ALPHABET_SIZE 26

/*
 * Maximum length of a dictionary word (including null terminator).
 */
#define WORD_MAX 128

/*
 * Trie node structure with prefix counting.
 *
 * Each node contains:
 * - An array of child pointers (one per letter 'a'–'z')
 * - prefixCount: number of words that share this prefix
 * - isEndOfWord: flag indicating whether this node terminates a word
 */
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE]; // Child nodes
    int prefixCount;                          // Words sharing this prefix
    int isEndOfWord;                          // End-of-word marker
} TrieNode;

/*
 * Allocates and initializes a new trie node.
 *
 * calloc is used to ensure:
 * - All child pointers are initialized to NULL
 * - prefixCount and isEndOfWord are initialized to 0
 *
 * Returns:
 * - Pointer to a newly allocated TrieNode
 */
TrieNode *createNode(void) {
    return (TrieNode *)calloc(1, sizeof(TrieNode));
}

/*
 * Converts a lowercase character to its corresponding trie index.
 *
 * Parameters:
 * - c: character expected to be between 'a' and 'z'
 *
 * Returns:
 * - Index in range [0, 25] if valid
 * - -1 if the character is outside the supported range
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
 * - Rejects the entire word if any character is not 'a'–'z'
 * - Creates nodes lazily as needed
 * - Increments prefixCount for each node along the word path
 *
 * Parameters:
 * - root: root node of the trie
 * - word: null-terminated string to insert
 */
void trieInsert(TrieNode *root, const char *word) {
    TrieNode *current = root;

    // Traverse each character in the word
    for (int i = 0; word[i]; i++) {
        // Normalize character to lowercase safely
        char c = tolower((unsigned char)word[i]);
        int idx = charIndex(c);

        // Abort insertion if an invalid character is encountered
        if (idx == -1) return;

        // Allocate a new child node if the path does not exist
        if (!current->children[idx]) {
            current->children[idx] = createNode();
        }

        // Move to the next node in the trie
        current = current->children[idx];

        // Increment the prefix count for this node
        current->prefixCount++;
    }

    // Mark the final node as representing a complete word
    current->isEndOfWord = 1;
}

/*
 * Counts how many words in the trie start with the given prefix.
 *
 * Parameters:
 * - root: root node of the trie
 * - prefix: prefix string to evaluate
 *
 * Returns:
 * - Number of words sharing the prefix
 * - 0 if the prefix is invalid or not found
 */
int triePrefixCount(TrieNode *root, const char *prefix) {
    TrieNode *current = root;

    // Traverse the trie according to the prefix
    for (int i = 0; prefix[i]; i++) {
        char c = tolower((unsigned char)prefix[i]);
        int idx = charIndex(c);

        // Fail if character is invalid or path does not exist
        if (idx == -1 || !current->children[idx])
            return 0;

        current = current->children[idx];
    }

    // The prefixCount stored at the final node represents
    // how many words share this prefix
    return current->prefixCount;
}

/*
 * Recursively frees all nodes in the trie.
 *
 * Parameters:
 * - node: current node to free
 *
 * Performs a post-order traversal to ensure children
 * are freed before their parent nodes.
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
 * - path: filesystem path to the dictionary file
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

    char buffer[WORD_MAX]; // Buffer for reading each word
    int count = 0;         // Number of words loaded

    // Read file line by line
    while (fgets(buffer, sizeof(buffer), fp)) {
        // Strip trailing newline and carriage return characters
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Skip empty lines
        if (buffer[0] == '\0') continue;

        // Insert word into the trie
        trieInsert(root, buffer);
        count++;
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
 * - Prefix-count queries
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

    // Prefixes to test
    const char *prefixes[] = {
        "a",
        "ab",
        "alg",
        "aard",
        "z",
        "nope"
    };

    // Perform prefix-count queries
    for (int i = 0; i < 6; i++) {
        printf("prefixCount(\"%s\") = %d\n",
               prefixes[i],
               triePrefixCount(trie, prefixes[i]));
    }

    // Free all allocated trie memory
    freeTrie(trie);
    return 0;
}
