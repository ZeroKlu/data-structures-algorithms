package code_samples.section13.example_2_prefix_counting;

import java.io.BufferedReader; // Efficient line-by-line reading of text files
import java.io.FileReader;     // Character stream for reading a file
import java.io.IOException;    // Exception type for I/O failures

/*
 * Trie Prefix Counting (Java)
 *
 * This example demonstrates a trie (prefix tree) that supports:
 * - Inserting words (case-insensitive)
 * - Counting how many inserted words start with a given prefix
 *
 * Key constraints:
 * - Only letters 'a'–'z' are accepted (case-insensitive).
 * - If any character in a word is not a–z, insert() rejects the entire word.
 * - prefixCount(prefix) returns 0 if the prefix is invalid or not found.
 *
 * Dictionary loading:
 * - Reads words from a file (default: "..\\data\\words.txt")
 * - Relative paths resolve from the working directory where Java is executed.
 */
public class trie_prefix_count {

    /*
     * Number of supported lowercase English letters.
     */
    private static final int ALPHABET_SIZE = 26;

    /*
     * Trie node definition.
     *
     * Each node contains:
     * - children: references to child nodes for each letter 'a'–'z'
     * - prefixCount: number of inserted words that share this prefix node
     * - isEndOfWord: end-of-word marker (not used by prefix counting output,
     *   but set during insert for completeness / consistency with other examples)
     */
    private static class TrieNode {
        TrieNode[] children = new TrieNode[ALPHABET_SIZE];

        // Number of words whose insertion path includes this node
        int prefixCount = 0;      // number of words sharing this prefix

        // Included for compatibility with word-search examples; not used here
        @SuppressWarnings("unused")
        boolean isEndOfWord = false;
    }

    /*
     * Root node of the trie.
     * Always exists and does not correspond to any particular character.
     */
    private final TrieNode root = new TrieNode();

    /*
     * Convert a lowercase character to its child index.
     *
     * Parameters:
     * - c: character expected to be between 'a' and 'z'
     *
     * Returns:
     * - 0..25 if valid
     * - -1 if invalid
     */
    private static int index(char c) {
        if (c < 'a' || c > 'z') return -1;
        return c - 'a';
    }

    /**
     * Inserts a word into the trie and increments prefixCount along the path.
     *
     * Behavior:
     * - Normalizes characters to lowercase (case-insensitive)
     * - Rejects the entire word if any character is not 'a'–'z'
     * - Creates nodes lazily as needed
     * - Increments prefixCount after advancing into the child node, meaning
     *   the node's prefixCount represents how many words share that prefix node
     *
     * @param word Word to insert into the trie.
     */
    public void insert(String word) {
        TrieNode current = root;

        // Traverse each character in the word
        for (int i = 0; i < word.length(); i++) {
            char c = Character.toLowerCase(word.charAt(i));
            int idx = index(c);

            // Reject entire word if invalid character is encountered
            if (idx < 0) return; // reject word

            // Create the child node if this path does not yet exist
            if (current.children[idx] == null) {
                current.children[idx] = new TrieNode();
            }

            // Move to the next node in the trie
            current = current.children[idx];

            // Increment prefixCount for this prefix node
            current.prefixCount++;
        }

        // Mark end-of-word (not used by prefixCount(), but kept for completeness)
        current.isEndOfWord = true;
    }

    /**
     * Returns how many words in the trie start with the given prefix.
     *
     * Behavior:
     * - Returns 0 if the prefix contains invalid characters (non a–z)
     * - Returns 0 if the prefix path does not exist in the trie
     * - Otherwise returns prefixCount stored in the node representing the prefix
     *
     * @param prefix Prefix to query.
     * @return Count of inserted words that start with the prefix.
     */
    public int prefixCount(String prefix) {
        TrieNode current = root;

        // Walk the trie according to the prefix characters
        for (int i = 0; i < prefix.length(); i++) {
            char c = Character.toLowerCase(prefix.charAt(i));
            int idx = index(c);

            // Invalid character -> prefix is invalid by design
            if (idx < 0) return 0;

            // Missing child -> prefix does not exist in trie
            if (current.children[idx] == null) return 0;

            // Advance to the next node
            current = current.children[idx];
        }

        // The node's prefixCount stores how many words share this prefix
        return current.prefixCount;
    }

    /*
     * Loads words from a dictionary file into the trie.
     *
     * Parameters:
     * - trie: trie instance to populate
     * - path: filesystem path to dictionary file
     *
     * Returns:
     * - number of non-empty lines processed (insert attempted for each)
     * - 0 if the file cannot be opened/read
     *
     * Notes:
     * - insert() may reject words containing invalid characters, but the loader
     *   still increments the count for each non-empty trimmed line (as written).
     */
    private static int loadDictionary(trie_prefix_count trie, String path) {
        int count = 0;

        // Try-with-resources ensures the reader is closed even on error
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String line;

            // Read the file line by line
            while ((line = br.readLine()) != null) {
                line = line.trim();

                // Skip empty lines
                if (line.isEmpty()) continue;

                // Insert word into the trie (may be rejected if invalid)
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
     * Program entry point.
     *
     * Demonstrates:
     * - Dictionary loading
     * - Prefix-count queries
     *
     * Arguments:
     * - args[0] (optional): dictionary path; defaults to "..\\data\\words.txt"
     */
    public static void main(String[] args) {
        trie_prefix_count trie = new trie_prefix_count();

        // Default path matches lesson convention; allow override via CLI arg
        String dictPath = (args.length > 0) ? args[0] : "..\\data\\words.txt";

        // Load dictionary words into the trie
        int loaded = loadDictionary(trie, dictPath);

        System.out.println("Loaded " + loaded + " words from " + dictPath);
        System.out.println();

        // Prefixes to query
        String[] prefixes = { "a", "ab", "alg", "aard", "z", "nope" };
        for (String p : prefixes) {
            System.out.println("prefixCount(\"" + p + "\") = " + trie.prefixCount(p));
        }
    }
}
