#include <array>      // std::array for fixed-size child pointer storage
#include <cctype>     // std::tolower for character normalization
#include <fstream>   // std::ifstream for file input
#include <iostream>  // std::cout, std::cerr for console I/O
#include <memory>    // std::unique_ptr for automatic memory management
#include <string>    // std::string for word handling
#include <vector>    // std::vector for test collections

/*
 * Trie (prefix tree) implementation for lowercase English words.
 *
 * Features:
 * - Case-insensitive insertion and lookup
 * - Rejects words containing non 'a'–'z' characters
 * - Automatic memory management via std::unique_ptr
 */
class Trie {
public:
    /*
     * Constructs an empty trie with a single root node.
     */
    Trie() : root(std::make_unique<Node>()) {}

    /*
     * Inserts a word into the trie.
     *
     * Behavior:
     * - Converts all characters to lowercase
     * - Rejects words containing characters outside 'a'–'z'
     * - Creates nodes lazily as needed
     *
     * Parameters:
     * - word: string to insert
     */
    void insert(const std::string& word) {
        Node* current = root.get();

        // Iterate over each character in the input word
        for (unsigned char ch : word) {
            // Normalize character to lowercase safely
            char c = static_cast<char>(std::tolower(ch));

            // Convert character to child index
            int idx = index(c);

            // Abort insertion if character is invalid
            if (idx < 0) return; // reject words containing non a-z characters

            // Allocate child node if it does not yet exist
            if (!current->children[idx]) {
                current->children[idx] = std::make_unique<Node>();
            }

            // Move to the next node in the trie
            current = current->children[idx].get();
        }

        // Mark the final node as representing a complete word
        current->isEnd = true;
    }

    /*
     * Searches for a complete word in the trie.
     *
     * Parameters:
     * - word: word to search for
     *
     * Returns:
     * - true if the word exists and is marked as complete
     * - false otherwise
     */
    bool search(const std::string& word) const {
        const Node* node = walk(word);
        return node && node->isEnd;
    }

    /*
     * Checks whether any word in the trie starts with the given prefix.
     *
     * Parameters:
     * - prefix: prefix string to test
     *
     * Returns:
     * - true if the prefix exists in the trie
     * - false otherwise
     */
    bool startsWith(const std::string& prefix) const {
        return walk(prefix) != nullptr;
    }

private:
    /*
     * Internal trie node structure.
     *
     * Each node contains:
     * - 26 child pointers (one per lowercase English letter)
     * - A flag indicating whether this node terminates a word
     */
    struct Node {
        std::array<std::unique_ptr<Node>, 26> children{}; // Child nodes
        bool isEnd{false};                                // End-of-word marker
    };

    // Root node of the trie
    std::unique_ptr<Node> root;

    /*
     * Maps a lowercase character to its corresponding child index.
     *
     * Parameters:
     * - c: character expected to be between 'a' and 'z'
     *
     * Returns:
     * - Index in range [0, 25] if valid
     * - -1 if character is invalid
     */
    static int index(char c) {
        if (c < 'a' || c > 'z') return -1;
        return c - 'a';
    }

    /*
     * Traverses the trie according to the given string.
     *
     * Used internally by both search() and startsWith().
     *
     * Parameters:
     * - s: word or prefix to traverse
     *
     * Returns:
     * - Pointer to the final node if traversal succeeds
     * - nullptr if traversal fails at any point
     */
    const Node* walk(const std::string& s) const {
        const Node* current = root.get();

        // Follow the trie path for each character
        for (unsigned char ch : s) {
            char c = static_cast<char>(std::tolower(ch));
            int idx = index(c);

            // Fail on invalid characters
            if (idx < 0) return nullptr;

            const auto& child = current->children[idx];

            // Fail if required child node does not exist
            if (!child) return nullptr;

            current = child.get();
        }

        return current;
    }
};

/*
 * Loads words from a dictionary file into a trie.
 *
 * Parameters:
 * - trie: trie instance to populate
 * - path: filesystem path to the dictionary file
 *
 * Returns:
 * - Number of words successfully loaded
 * - 0 if the file could not be opened
 */
static int loadDictionary(Trie& trie, const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Failed to open dictionary: " << path << "\n";
        return 0;
    }

    int count = 0;
    std::string line;

    // Read file line by line
    while (std::getline(in, line)) {
        // Trim trailing Windows carriage return if present
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // Skip empty lines
        if (line.empty()) continue;

        // Insert word into trie
        trie.insert(line);
        count++;
    }

    return count;
}

/*
 * Program entry point.
 *
 * Demonstrates:
 * - Trie construction
 * - Dictionary loading
 * - Exact word searching
 * - Prefix searching
 */
int main() {
    // Create an empty trie
    Trie trie;

    // Path to dictionary file
    const std::string dictPath = "..\\data\\words.txt";

    // Load dictionary words
    int loaded = loadDictionary(trie, dictPath);
    std::cout << "Loaded " << loaded << " words from " << dictPath << "\n\n";

    // Words to test for exact matches
    const std::vector<std::string> tests = {
        "aardvark",
        "abandon",
        "zebra",
        "notaword"
    };

    // Perform exact-word searches
    for (const auto& w : tests) {
        std::cout << "search(\"" << w << "\") = "
                  << (trie.search(w) ? "true" : "false") << "\n";
    }

    // Prefix tests
    std::cout << "\nPrefix tests:\n";
    const std::vector<std::string> prefixes = {"ab", "alg", "aard", "nope"};

    for (const auto& p : prefixes) {
        std::cout << "startsWith(\"" << p << "\") = "
                  << (trie.startsWith(p) ? "true" : "false") << "\n";
    }

    return 0;
}
