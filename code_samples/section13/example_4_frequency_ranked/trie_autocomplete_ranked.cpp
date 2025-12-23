// trie_autocomplete_ranked.cpp
//
// This program implements a trie (prefix tree) that supports ranked autocomplete.
// It loads words from two input files (a dictionary and a "frequency"/usage list),
// inserts them into the trie, and then prints the top ranked completions for a prefix.
//
// Ranking rules (as implemented):
// - Higher frequency first
// - If frequencies tie, lexicographically smaller word first
//
// Key constraints:
// - Only letters 'a'–'z' are accepted (case-insensitive).
// - If any character in an inserted word is not a–z, insert() rejects that word.
// - If any character in a searched prefix is not a–z, walk() fails and returns no results.

#include <array>      // std::array for fixed-size child storage (26 letters)
#include <cctype>     // std::tolower for case-insensitive normalization
#include <fstream>    // std::ifstream for reading input files
#include <iostream>   // std::cout for output
#include <memory>     // std::unique_ptr for node ownership / automatic cleanup
#include <queue>      // (Not used in current code) would typically support a heap/top-K approach
#include <string>     // std::string for words and buffers
#include <vector>     // std::vector for collecting results
#include <algorithm>  // std::sort for ranking results

/*
 * Trie class:
 * - insert(word): insert a word and increment its terminal frequency
 * - autocompleteRanked(prefix, limit): return up to `limit` suggestions under prefix,
 *   ranked by frequency (desc) then word (asc)
 */
class Trie {
public:
    /*
     * Construct an empty trie with a root node.
     * The root does not correspond to a character.
     */
    Trie() : root(std::make_unique<Node>()) {}

    /*
     * Insert a word into the trie and increment its frequency.
     *
     * Behavior:
     * - Normalizes each character to lowercase
     * - Rejects the entire word if any character is not 'a'–'z'
     * - Creates nodes lazily as needed
     * - Marks the terminal node as end-of-word and increments frequency
     *
     * Note:
     * - frequency counts how many times this exact word has been inserted
     *   across all input files (dict + freq file).
     */
    void insert(const std::string& word) {
        Node* cur = root.get();

        // Traverse each character in the input word
        for (unsigned char ch : word) {
            // Normalize safely: tolower expects unsigned char domain
            char c = static_cast<char>(std::tolower(ch));

            // Map character to child index
            int i = idx(c);

            // Reject word if invalid character
            if (i < 0) return;

            // Create missing child node
            if (!cur->children[i]) cur->children[i] = std::make_unique<Node>();

            // Advance into child node
            cur = cur->children[i].get();
        }

        // Mark word termination and bump frequency count
        cur->isEnd = true;
        cur->frequency++;
    }

    /*
     * Return ranked autocomplete suggestions for `prefix`.
     *
     * Parameters:
     * - prefix: prefix to match
     * - limit: maximum number of results to return
     *
     * Returns:
     * - vector of (word, frequency) pairs, sorted best-first
     *
     * Steps:
     * 1) walk(prefix) to find the trie node for the prefix.
     * 2) DFS the subtree under that node, collecting all matches into `all`.
     * 3) Sort results by:
     *    - frequency descending
     *    - word ascending (lexicographic) for tie-break
     * 4) Truncate to `limit`.
     *
     * Important nuance:
     * - This implementation collects *all* completions under the prefix, then sorts.
     *   For very large tries/subtrees, this can be expensive compared to a top-K heap.
     */
    std::vector<std::pair<std::string,int>>
    autocompleteRanked(const std::string& prefix, size_t limit) const {
        // Find the node corresponding to the prefix
        const Node* start = walk(prefix);

        // Invalid prefix or missing path -> no suggestions
        if (!start) return {};

        // Collect all matching words under the prefix
        std::vector<std::pair<std::string,int>> all;

        // Buffer seeded with prefix (lowercased) that DFS will extend
        std::string buf = prefix;
        for (char& c : buf) c = std::tolower(static_cast<unsigned char>(c));

        // DFS traversal to gather completions and their frequencies
        dfs(start, buf, all);

        // Sort best-first (highest frequency, then lexicographically smallest)
        std::sort(all.begin(), all.end(),
            [](auto& a, auto& b) {
                if (a.second != b.second) return a.second > b.second;
                return a.first < b.first;
            });

        // Truncate to limit (if needed)
        if (all.size() > limit) all.resize(limit);

        return all;
    }

private:
    /*
     * Trie node structure.
     *
     * children: 26 pointers for a–z stored as unique_ptr for ownership
     * isEnd: marks that a complete word ends at this node
     * frequency: how many times this complete word has been inserted
     */
    struct Node {
        std::array<std::unique_ptr<Node>,26> children{}; // Child nodes
        bool isEnd{false};                               // End-of-word marker
        int frequency{0};                                // Word frequency counter
    };

    // Root node; owns the entire trie
    std::unique_ptr<Node> root;

    /*
     * Map a lowercase letter to an index [0..25].
     *
     * Returns:
     * - 0..25 if valid
     * - -1 if invalid
     */
    static int idx(char c) { return (c >= 'a' && c <= 'z') ? c - 'a' : -1; }

    /*
     * Walk the trie according to `s` and return the final node.
     *
     * Behavior:
     * - Lowercases each character
     * - Returns nullptr if any character is invalid or any required child is missing
     *
     * Parameters:
     * - s: word or prefix to traverse
     *
     * Returns:
     * - pointer to final node, or nullptr on failure
     */
    const Node* walk(const std::string& s) const {
        const Node* cur = root.get();

        for (unsigned char ch : s) {
            char c = static_cast<char>(std::tolower(ch));
            int i = idx(c);

            // Fail if invalid char or missing child
            if (i < 0 || !cur->children[i]) return nullptr;

            cur = cur->children[i].get();
        }

        return cur;
    }

    /*
     * DFS traversal that collects all completed words under a node.
     *
     * Parameters:
     * - node: subtree root
     * - buf: mutable buffer containing the current word (prefix + suffix)
     * - out: output vector of (word, frequency)
     *
     * Behavior:
     * - If node is terminal, emit (buf, node->frequency)
     * - For each child in a..z order, append letter to buf, recurse, then pop_back()
     *
     * Ordering note:
     * - DFS visits children in lexicographic order, but final output ordering
     *   is determined by sorting in autocompleteRanked().
     */
    static void dfs(const Node* node, std::string& buf,
                    std::vector<std::pair<std::string,int>>& out) {
        // Emit a suggestion when a complete word ends here
        if (node->isEnd) out.emplace_back(buf, node->frequency);

        // Explore children in 'a'..'z' order
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                // Append next letter to current buffer
                buf.push_back(char('a' + i));

                // Recurse into child subtree
                dfs(node->children[i].get(), buf, out);

                // Backtrack to restore buffer for next sibling
                buf.pop_back();
            }
        }
    }
};

/*
 * Load all non-empty lines from a file and insert them into the trie.
 *
 * Parameters:
 * - t: trie to populate
 * - path: file path to read
 *
 * Notes:
 * - This function does not explicitly check whether the file opened successfully.
 *   If opening fails, std::getline() will immediately fail and nothing will be inserted.
 * - Words are inserted exactly as read; insert() will reject words containing invalid characters.
 */
static void loadFile(Trie& t, const std::string& path) {
    std::ifstream in(path);
    std::string w;

    // Read each line; insert non-empty lines
    while (std::getline(in, w)) if (!w.empty()) t.insert(w);
}

/*
 * Program entry point.
 *
 * CLI arguments:
 * - argv[1] (optional): dictionary file path (default: "..\\data\\words.txt")
 * - argv[2] (optional): frequency/usage file path (default: "..\\data\\frequency.txt")
 * - argv[3] (optional): prefix (default: "th")
 *
 * Behavior:
 * - Loads both files into the trie; each insertion increments a word's frequency.
 * - Prints the top 20 ranked autocomplete suggestions for the prefix.
 */
int main(int argc, char** argv) {
    Trie trie;

    // Resolve inputs with defaults
    std::string dict = argc > 1 ? argv[1] : "..\\data\\words.txt";
    std::string freq = argc > 2 ? argv[2] : "..\\data\\frequency.txt";
    std::string prefix = argc > 3 ? argv[3] : "th";

    // Insert dictionary words and usage/frequency words into the trie
    loadFile(trie, dict);
    loadFile(trie, freq);

    // Print ranked suggestions (word + frequency)
    for (auto& [w,f] : trie.autocompleteRanked(prefix, 20))
        std::cout << w << "\t(freq=" << f << ")\n";
}
