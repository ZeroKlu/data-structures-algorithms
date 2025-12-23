package code_samples.section13.example_3_auto_complete;

import java.io.BufferedReader;   // Efficient line-by-line reading of text files
import java.io.FileReader;       // Character stream for reading a file
import java.io.IOException;      // Exception type for I/O failures
import java.util.ArrayList;      // Dynamic list for returning autocomplete results

/*
 * Trie Autocomplete (Java)
 *
 * This example demonstrates a trie (prefix tree) that supports:
 * - Inserting words (case-insensitive)
 * - Autocompleting a prefix by returning up to N suggestions
 *
 * Key behavior / constraints:
 * - Only letters 'a'–'z' are supported (case-insensitive).
 * - If any character in a word/prefix is not a–z:
 *   - insert() rejects the entire word
 *   - walk() returns null
 *   - autocomplete() returns an empty list
 *
 * Autocomplete ordering:
 * - Suggestions are generated in lexicographic order because DFS visits
 *   children in 'a'..'z' order.
 *
 * Dictionary loading:
 * - Default dictionary path is "..\\data\\words.txt"
 * - Relative paths resolve from the working directory where Java runs.
 */
public class trie_autocomplete {

    /*
     * Number of supported lowercase English letters.
     */
    private static final int ALPHABET_SIZE = 26;

    /*
     * Maximum word length constant (present for parity with other examples).
     * Not used in this implementation because StringBuilder grows dynamically.
     */
    @SuppressWarnings("unused")
    private static final int WORD_MAX = 128;

    /*
     * Trie node definition.
     *
     * Each node contains:
     * - children: references to child nodes for each letter 'a'–'z'
     * - isEndOfWord: indicates whether a complete inserted word ends at this node
     */
    private static class TrieNode {
        TrieNode[] children = new TrieNode[ALPHABET_SIZE]; // Child pointers
        boolean isEndOfWord = false;                       // End-of-word marker
    }

    /*
     * Root node of the trie.
     * Always exists and does not correspond to any particular character.
     */
    private final TrieNode root = new TrieNode();

    /*
     * Convert a lowercase character into an array index.
     *
     * Parameters:
     * - c: expected to be between 'a' and 'z'
     *
     * Returns:
     * - 0..25 if valid
     * - -1 if invalid
     */
    private static int index(char c) {
        if (c < 'a' || c > 'z') return -1;
        return c - 'a';
    }

    /*
     * Insert a word into the trie.
     *
     * Behavior:
     * - Normalizes to lowercase
     * - Rejects the entire word if any character is not 'a'–'z'
     * - Creates nodes lazily as needed
     * - Marks the final node as an end-of-word
     *
     * Parameters:
     * - word: word to insert
     */
    public void insert(String word) {
        TrieNode current = root;

        // Traverse each character of the word
        for (int i = 0; i < word.length(); i++) {
            char c = Character.toLowerCase(word.charAt(i));
            int idx = index(c);

            // Reject word if any character is invalid
            if (idx < 0) return;

            // Allocate child node if missing
            if (current.children[idx] == null) current.children[idx] = new TrieNode();

            // Advance in the trie
            current = current.children[idx];
        }

        // Mark the final node as representing a complete word
        current.isEndOfWord = true;
    }

    /*
     * Walk the trie according to a prefix and return the final node.
     *
     * Behavior:
     * - Lowercases each character
     * - Returns null if prefix contains invalid characters or missing path
     *
     * Parameters:
     * - prefix: prefix to traverse
     *
     * Returns:
     * - TrieNode at the end of the prefix, or null if not found/invalid
     */
    private TrieNode walk(String prefix) {
        TrieNode current = root;

        // Traverse each character of the prefix
        for (int i = 0; i < prefix.length(); i++) {
            char c = Character.toLowerCase(prefix.charAt(i));
            int idx = index(c);

            // Invalid character -> prefix not supported
            if (idx < 0) return null;

            // Missing path -> prefix not present in trie
            if (current.children[idx] == null) return null;

            // Advance in the trie
            current = current.children[idx];
        }

        return current;
    }

    /*
     * Return up to `limit` autocomplete suggestions for the given prefix.
     *
     * Steps:
     * 1) Find the prefix node using walk(prefix).
     * 2) Seed a StringBuilder with the normalized (lowercased) prefix.
     * 3) DFS from the prefix node, appending letters to the buffer as we explore.
     * 4) Add completed words (end-of-word nodes) to the output list.
     *
     * Parameters:
     * - prefix: prefix to autocomplete
     * - limit: maximum number of suggestions to return
     *
     * Returns:
     * - ArrayList<String> of suggestions, possibly empty
     *
     * Notes:
     * - The prefix itself is included in results only if it is also a complete word.
     */
    public ArrayList<String> autocomplete(String prefix, int limit) {
        TrieNode start = walk(prefix);

        // Output list (returned even when empty)
        ArrayList<String> out = new ArrayList<>();

        // If prefix is invalid or not found, return empty list
        if (start == null) return out;

        // Seed buffer with the lowercased prefix; extra capacity helps reduce reallocations
        StringBuilder buf = new StringBuilder(prefix.length() + 16);
        for (int i = 0; i < prefix.length(); i++) {
            buf.append(Character.toLowerCase(prefix.charAt(i)));
        }

        // DFS beneath the prefix node to collect suggestions
        dfsCollect(start, buf, out, limit);
        return out;
    }

    /*
     * Depth-first traversal to collect autocomplete suggestions.
     *
     * Parameters:
     * - node: current subtree root
     * - buf: mutable buffer holding the current word (prefix + explored suffix)
     * - out: output list of suggestions
     * - limit: maximum number of suggestions to collect
     *
     * Behavior:
     * - If node.isEndOfWord is true, add buf.toString() as a suggestion.
     * - Visit children in 'a'..'z' order to produce lexicographic results.
     * - Backtrack by removing the last character after each recursive call.
     * - Stop early once out.size() reaches limit.
     */
    private void dfsCollect(TrieNode node, StringBuilder buf, ArrayList<String> out, int limit) {
        // Base cases: nothing to explore, or we've already collected enough results
        if (node == null || out.size() >= limit) return;

        // If this node terminates a word, capture the current buffer as a suggestion
        if (node.isEndOfWord) {
            out.add(buf.toString());
            if (out.size() >= limit) return;
        }

        // Explore children in lexicographic order
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node.children[i] != null) {
                // Append this character to the current word buffer
                buf.append((char)('a' + i));

                // Recurse into the child subtree
                dfsCollect(node.children[i], buf, out, limit);

                // Backtrack: remove the last appended character
                buf.deleteCharAt(buf.length() - 1);

                // Stop early if limit reached
                if (out.size() >= limit) return;
            }
        }
    }

    /*
     * Load words from a dictionary file into the trie.
     *
     * Parameters:
     * - trie: trie instance to populate
     * - path: dictionary file path
     *
     * Returns:
     * - number of non-empty trimmed lines processed (insert attempted for each)
     * - 0 if the file cannot be opened/read
     *
     * Notes:
     * - insert() rejects words with invalid characters, but this loader still
     *   increments count for each non-empty trimmed line (as written).
     */
    private static int loadDictionary(trie_autocomplete trie, String path) {
        int count = 0;

        // Try-with-resources ensures the reader closes even if an exception occurs
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String line;

            // Read file line-by-line
            while ((line = br.readLine()) != null) {
                line = line.trim();

                // Skip empty lines
                if (line.isEmpty()) continue;

                // Insert word into trie (may be rejected if invalid)
                trie.insert(line);
                count++;
            }
        } catch (IOException e) {
            // Report file access / I/O errors
            System.err.println("Failed to open dictionary: " + path);
            System.err.println(e.getMessage());
            return 0;
        }

        return count;
    }

    /*
     * Parse a positive integer from a string, or return a default.
     *
     * Parameters:
     * - s: input string representing an integer
     * - def: default to return if parsing fails or value is non-positive
     *
     * Returns:
     * - parsed value if > 0
     * - def otherwise
     */
    private static int parseIntOrDefault(String s, int def) {
        try {
            int v = Integer.parseInt(s);
            return v > 0 ? v : def;
        } catch (Exception e) {
            return def;
        }
    }

    /*
     * Program entry point.
     *
     * Usage:
     * - args[0] (optional): dictionary path (default: "..\\data\\words.txt")
     * - args[1] (optional): prefix (default: "ab")
     * - args[2] (optional): limit (default: 20)
     *
     * Demonstrates:
     * - Dictionary loading
     * - Autocomplete query for a prefix
     * - Printing returned suggestions
     */
    public static void main(String[] args) {
        trie_autocomplete trie = new trie_autocomplete();

        // Default path matches lesson convention; allow override via CLI args
        String dictPath = (args.length > 0) ? args[0] : "..\\data\\words.txt";
        String prefix   = (args.length > 1) ? args[1] : "ab";
        int limit       = (args.length > 2) ? parseIntOrDefault(args[2], 20) : 20;

        // Load dictionary into trie
        int loaded = loadDictionary(trie, dictPath);
        System.out.println("Loaded " + loaded + " words from " + dictPath);
        System.out.println();

        // Run autocomplete and print results
        System.out.println("Autocomplete(\"" + prefix + "\") [limit=" + limit + "]");
        for (String w : trie.autocomplete(prefix, limit)) {
            System.out.println(w);
        }
    }
}
