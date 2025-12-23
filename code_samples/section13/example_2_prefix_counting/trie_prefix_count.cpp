#include <array>      // std::array for fixed-size child storage (26 letters)
#include <cctype>     // std::tolower for case-insensitive normalization
#include <fstream>    // std::ifstream for reading the dictionary file
#include <iostream>   // std::cout / std::cerr for console output
#include <memory>     // std::unique_ptr for automatic node memory management
#include <string>     // std::string for words/prefixes
#include <vector>     // std::vector for test prefix list

/*
 * Trie implementation that supports prefix counting.
 *
 * Key behavior:
 * - Only letters 'a'–'z' are accepted (case-insensitive).
 * - If any character in an inserted word is not a–z, the entire insert aborts.
 * - Each node maintains prefixCount: how many inserted words share that prefix.
 */
class Trie {
public:
    /*
     * Construct an empty trie with a root node.
     *
     * The root node does not correspond to any character; it is simply
     * the entry point for all traversals.
     */
    Trie() : root(std::make_unique<Node>()) {}

    /*
     * Insert a word into the trie.
     *
     * Behavior:
     * - Lowercases each character (case-insensitive trie)
     * - Rejects the entire word if any character is not 'a'–'z'
     * - Creates nodes lazily (only when needed)
     * - Increments prefixCount on each node visited along the word path
     *
     * Note:
     * - prefixCount is incremented AFTER advancing to the child node, meaning
     *   it counts words for that prefix node (not for the parent).
     */
    void insert(const std::string& word) {
        Node* current = root.get();

        // Walk each character in the word
        for (unsigned char ch : word) {
            // Normalize to lowercase in a safe way for potential signed char values
            char c = static_cast<char>(std::tolower(ch));

            // Convert character to an index in [0, 25]
            int idx = index(c);

            // Reject entire word if it contains any non a-z character
            if (idx < 0) return; // reject word

            // Create the child node if it doesn't exist yet
            if (!current->children[idx]) {
                current->children[idx] = std::make_unique<Node>();
            }

            // Advance to the next node in the trie
            current = current->children[idx].get();

            // This node represents a prefix of the word; increment its count
            current->prefixCount++;
        }

        // Mark the final node as the end of a complete inserted word
        current->isEnd = true;
    }

    /*
     * Count how many inserted words start with the given prefix.
     *
     * Behavior:
     * - Returns 0 if prefix contains invalid characters (non a–z) OR
     *   if the prefix path does not exist in the trie.
     * - Otherwise returns the prefixCount stored in the node representing
     *   the final character of the prefix.
     *
     * Parameters:
     * - prefix: prefix string to query
     *
     * Returns:
     * - number of words sharing that prefix
     */
    int prefixCount(const std::string& prefix) const {
        const Node* node = walk(prefix);
        return node ? node->prefixCount : 0;
    }

private:
    /*
     * Internal trie node type.
     *
     * children:
     * - 26 possible next nodes (a–z), managed by unique_ptr for safe cleanup
     *
     * prefixCount:
     * - number of words whose path includes this node (i.e., share this prefix)
     *
     * isEnd:
     * - marks that a complete word terminates at this node
     *   (not used by prefixCount(), but kept for completeness/consistency)
     */
    struct Node {
        std::array<std::unique_ptr<Node>, 26> children{}; // child pointers
        int  prefixCount{0};                              // words sharing this prefix
        bool isEnd{false};                                // end-of-word marker
    };

    // Root pointer owning the full trie structure
    std::unique_ptr<Node> root;

    /*
     * Convert a lowercase character 'a'–'z' into an index [0, 25].
     *
     * Returns:
     * - -1 if c is not in 'a'–'z'
     */
    static int index(char c) {
        if (c < 'a' || c > 'z') return -1;
        return c - 'a';
    }

    /*
     * Traverse the trie according to the provided string.
     *
     * Used by prefixCount() to locate the node corresponding to the final
     * character of the prefix.
     *
     * Behavior:
     * - Lowercases each character
     * - Returns nullptr if any character is invalid (non a–z)
     * - Returns nullptr if any required child pointer is missing
     *
     * Returns:
     * - pointer to the final node if traversal succeeds
     * - nullptr otherwise
     */
    const Node* walk(const std::string& s) const {
        const Node* current = root.get();

        for (unsigned char ch : s) {
            char c = static_cast<char>(std::tolower(ch));
            int idx = index(c);

            // Reject invalid characters
            if (idx < 0) return nullptr;

            // Follow child pointer; fail if missing
            const auto& child = current->children[idx];
            if (!child) return nullptr;

            current = child.get();
        }

        return current;
    }
};

/*
 * Load words from a dictionary file into the trie.
 *
 * Parameters:
 * - trie: trie to populate
 * - path: dictionary file path
 *
 * Returns:
 * - number of non-empty lines processed (insert attempted for each)
 * - 0 if the file cannot be opened
 *
 * Notes:
 * - Lines are trimmed for Windows CR ('\r') when reading text files that
 *   may have CRLF line endings.
 * - Words containing invalid characters will be rejected by trie.insert(),
 *   but this loader still increments count for each non-empty line (as written).
 */
static int loadDictionary(Trie& trie, const std::string& path) {
    std::ifstream in(path);

    // Validate the file stream opened successfully
    if (!in) {
        std::cerr << "Failed to open dictionary: " << path << "\n";
        return 0;
    }

    int count = 0;
    std::string line;

    // Read file line-by-line
    while (std::getline(in, line)) {
        // Trim Windows carriage return if present (common with CRLF files)
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // Skip empty lines
        if (line.empty()) continue;

        // Insert the word; insert() may reject it if it contains invalid chars
        trie.insert(line);
        count++;
    }

    return count;
}

/*
 * Program entry point.
 *
 * Demonstrates:
 * - Dictionary loading into the trie
 * - prefixCount queries for several prefixes
 *
 * Arguments:
 * - argv[1] (optional): dictionary path; defaults to "..\\data\\words.txt"
 */
int main(int argc, char** argv) {
    Trie trie;

    // Default path matches the lesson convention; can be overridden by CLI arg.
    const std::string dictPath = (argc > 1) ? argv[1] : "..\\data\\words.txt";

    // Load dictionary words into trie
    int loaded = loadDictionary(trie, dictPath);

    std::cout << "Loaded " << loaded << " words from " << dictPath << "\n\n";

    // Prefixes to query against the loaded trie
    const std::vector<std::string> prefixes = {
        "a",
        "ab",
        "alg",
        "aard",
        "z",
        "nope"
    };

    // Print prefixCount results
    for (const auto& p : prefixes) {
        std::cout << "prefixCount(\"" << p << "\") = " << trie.prefixCount(p) << "\n";
    }

    return 0;
}
