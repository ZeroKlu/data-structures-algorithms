#include <array>      // std::array for fixed-size child pointer storage (26 letters)
#include <cctype>     // std::tolower for case-insensitive character normalization
#include <fstream>    // std::ifstream for reading dictionary files
#include <iostream>   // std::cout / std::cerr for console output
#include <memory>     // std::unique_ptr for automatic lifetime management of nodes
#include <string>     // std::string for words/prefixes/buffers
#include <vector>     // std::vector for returning autocomplete results

/*
 * Trie (prefix tree) implementation that supports autocomplete.
 *
 * Key behavior:
 * - Only letters 'a'–'z' are accepted (case-insensitive).
 * - If any character in a word/prefix is not a–z:
 *   - insert() rejects the entire word (no nodes are added)
 *   - walk() fails and autocomplete() returns an empty result set
 *
 * Autocomplete behavior:
 * - Finds the node representing the given prefix
 * - Enumerates words beneath that node with a depth-first search (DFS)
 * - Returns up to `limit` suggestions
 * - Suggestions are in lexicographic order because children are visited a..z
 */
class Trie {
public:
    /*
     * Construct an empty trie with a root node.
     *
     * The root node does not correspond to any character; it is the entry point.
     */
    Trie() : root(std::make_unique<Node>()) {}

    /*
     * Insert a word into the trie.
     *
     * Behavior:
     * - Normalizes characters to lowercase (case-insensitive)
     * - Rejects the entire word if any character is not 'a'–'z'
     * - Creates nodes lazily as needed along the insertion path
     * - Marks the final node as an end-of-word
     *
     * Parameters:
     * - word: word to insert
     */
    void insert(const std::string& word) {
        Node* current = root.get();

        // Traverse each character in the word
        for (unsigned char ch : word) {
            // Normalize safely: iterate as unsigned char then convert to char
            char c = static_cast<char>(std::tolower(ch));

            // Convert to trie index
            int idx = index(c);

            // Reject the entire word if it contains invalid characters
            if (idx < 0) return;

            // Create child node if missing
            if (!current->children[idx]) current->children[idx] = std::make_unique<Node>();

            // Move forward in the trie
            current = current->children[idx].get();
        }

        // Mark the final node as representing a complete word
        current->isEnd = true;
    }

    /*
     * Return up to `limit` autocomplete suggestions for a given prefix.
     *
     * Steps:
     * 1) Walk the trie to find the node representing the prefix.
     * 2) Seed a buffer string with the normalized (lowercased) prefix.
     * 3) DFS over the subtree below that node, appending letters to the buffer.
     * 4) Collect words when end-of-word nodes are encountered.
     *
     * Ordering:
     * - DFS visits children in increasing letter order (a..z), so suggestions
     *   come back in lexicographic order.
     *
     * Parameters:
     * - prefix: prefix to autocomplete
     * - limit: maximum number of suggestions to return
     *
     * Returns:
     * - vector of suggestions (possibly empty)
     */
    std::vector<std::string> autocomplete(const std::string& prefix, std::size_t limit) const {
        // Locate the trie node corresponding to the prefix
        const Node* start = walk(prefix);

        // Invalid prefix or missing path -> no suggestions
        if (!start) return {};

        // Create a normalized prefix buffer that DFS can extend
        std::string seed;
        seed.reserve(prefix.size());

        // Lowercase the prefix for consistent output
        for (unsigned char ch : prefix) seed.push_back(static_cast<char>(std::tolower(ch)));

        // Output collection
        std::vector<std::string> out;
        out.reserve(limit); // Reserve to reduce reallocations when limit is known
        dfsCollect(start, seed, out, limit);
        return out;
    }

private:
    /*
     * Internal trie node structure.
     *
     * children:
     * - 26 possible next nodes (a–z), owned via unique_ptr
     *
     * isEnd:
     * - marks that a full inserted word ends at this node
     */
    struct Node {
        std::array<std::unique_ptr<Node>, 26> children{}; // Child pointers
        bool isEnd{false};                                // End-of-word marker
    };

    // Root node owning the entire trie
    std::unique_ptr<Node> root;

    /*
     * Convert a lowercase character to an index [0, 25].
     *
     * Returns:
     * - -1 for invalid characters
     */
    static int index(char c) {
        if (c < 'a' || c > 'z') return -1;
        return c - 'a';
    }

    /*
     * Walk the trie according to a word/prefix.
     *
     * Behavior:
     * - Normalizes characters to lowercase
     * - Returns nullptr if any character is invalid or any required child is missing
     *
     * Parameters:
     * - s: string to traverse
     *
     * Returns:
     * - pointer to the node representing the final character in s
     * - nullptr if traversal fails
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

    /*
     * Depth-first enumeration of words beneath a node.
     *
     * Parameters:
     * - node: subtree root to enumerate
     * - buffer: current word under construction (includes prefix + suffix)
     * - out: output vector of collected suggestions
     * - limit: maximum number of suggestions to collect
     *
     * Behavior:
     * - Adds buffer to out whenever node->isEnd is true.
     * - Visits children in 'a'..'z' order for lexicographic output.
     * - Stops early when out.size() reaches limit.
     *
     * Implementation detail:
     * - This function mutates `buffer` by push_back/pop_back while exploring.
     */
    static void dfsCollect(const Node* node,
                           std::string& buffer,
                           std::vector<std::string>& out,
                           std::size_t limit) {
        // Base cases: nothing to explore, or already reached output limit
        if (!node || out.size() >= limit) return;

        // If this node ends a word, capture the current buffer
        if (node->isEnd) {
            out.push_back(buffer);
            if (out.size() >= limit) return;
        }

        // Explore children in lexicographic order
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                // Append letter corresponding to child index
                buffer.push_back(static_cast<char>('a' + i));

                // Recurse into child
                dfsCollect(node->children[i].get(), buffer, out, limit);

                // Undo the append to restore buffer state for next sibling
                buffer.pop_back();

                // Stop as soon as we've collected enough suggestions
                if (out.size() >= limit) return;
            }
        }
    }
};

/*
 * Load words from a dictionary file into a trie.
 *
 * Parameters:
 * - trie: trie instance to populate
 * - path: dictionary file path
 *
 * Returns:
 * - number of non-empty lines processed (insert attempted for each)
 * - 0 if the file cannot be opened
 *
 * Notes:
 * - Removes a trailing '\r' if present (common in CRLF line endings).
 * - insert() may reject words with invalid characters, but this loader still
 *   increments count for each non-empty line (as written).
 */
static int loadDictionary(Trie& trie, const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Failed to open dictionary: " << path << "\n";
        return 0;
    }

    int count = 0;
    std::string line;

    // Read file line-by-line
    while (std::getline(in, line)) {
        // Trim Windows CR if present
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // Skip empty lines
        if (line.empty()) continue;

        // Insert word into trie (may be rejected if invalid)
        trie.insert(line);
        count++;
    }

    return count;
}

/*
 * Parse a limit from a C-string, or return a default.
 *
 * Parameters:
 * - s: numeric string (expected unsigned)
 * - def: default value if parsing fails or yields 0
 *
 * Returns:
 * - parsed value if successful and > 0
 * - def otherwise
 *
 * Notes:
 * - Uses std::stoul and catches any exceptions (invalid_argument, out_of_range).
 */
static std::size_t parseLimit(const char* s, std::size_t def) {
    if (!s || !*s) return def;
    try {
        std::size_t v = static_cast<std::size_t>(std::stoul(s));
        return v == 0 ? def : v;
    } catch (...) {
        return def;
    }
}

/*
 * Program entry point.
 *
 * Usage:
 * - argv[1] (optional): dictionary path (default: "..\\data\\words.txt")
 * - argv[2] (optional): prefix to autocomplete (default: "ab")
 * - argv[3] (optional): limit (default: 20)
 *
 * Demonstrates:
 * - Loading a dictionary into a trie
 * - Printing up to N autocomplete suggestions for a prefix
 */
int main(int argc, char** argv) {
    Trie trie;

    // Default path matches the lesson convention; can be overridden by CLI arg.
    const std::string dictPath = (argc > 1) ? argv[1] : "..\\data\\words.txt";
    const std::string prefix   = (argc > 2) ? argv[2] : "ab";
    const std::size_t limit    = (argc > 3) ? parseLimit(argv[3], 20) : 20;

    // Load dictionary words into the trie
    int loaded = loadDictionary(trie, dictPath);
    std::cout << "Loaded " << loaded << " words from " << dictPath << "\n\n";

    // Run autocomplete and print results
    std::cout << "Autocomplete(\"" << prefix << "\") [limit=" << limit << "]\n";
    auto results = trie.autocomplete(prefix, limit);
    for (const auto& w : results) std::cout << w << "\n";

    return 0;
}
