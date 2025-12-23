package code_samples.section13.example_1_insert_and_search;

import java.io.BufferedReader; // Efficient line-by-line file reading
import java.io.FileReader;     // File input stream
import java.io.IOException;    // I/O exception handling

/*
 * Trie Insert + Search (Java)
 *
 * This class demonstrates a simple trie (prefix tree) implementation
 * supporting:
 * - Case-insensitive insertion and lookup
 * - Exact word search
 * - Prefix search
 * - Dictionary loading from a text file
 *
 * Only lowercase English letters ('a'–'z') are supported.
 */
public class trie_insert_search {

    /*
     * Number of supported lowercase English letters.
     */
    private static final int ALPHABET_SIZE = 26;

    /*
     * Trie node definition.
     *
     * Each node contains:
     * - An array of child references (one per letter 'a'–'z')
     * - A flag indicating whether this node terminates a valid word
     */
    private static class TrieNode {
        TrieNode[] children = new TrieNode[ALPHABET_SIZE]; // Child nodes
        boolean isEndOfWord = false;                        // End-of-word marker
    }

    /*
     * Root node of the trie.
     * Always exists, even for an empty trie.
     */
    private final TrieNode root = new TrieNode();

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
    private static int index(char c) {
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
     *
     * Parameters:
     * - word: word to insert
     */
    public void insert(String word) {
        TrieNode current = root;

        // Traverse each character in the word
        for (int i = 0; i < word.length(); i++) {
            char c = Character.toLowerCase(word.charAt(i));
            int idx = index(c);

            // Abort insertion if an invalid character is encountered
            if (idx < 0) return; // reject words containing non a-z characters

            // Allocate a new node if the path does not exist
            if (current.children[idx] == null) {
                current.children[idx] = new TrieNode();
            }

            // Move to the next node in the trie
            current = current.children[idx];
        }

        // Mark the final node as representing a complete word
        current.isEndOfWord = true;
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
    public boolean search(String word) {
        TrieNode node = walk(word);
        return node != null && node.isEndOfWord;
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
    public boolean startsWith(String prefix) {
        return walk(prefix) != null;
    }

    /*
     * Traverses the trie following the given word or prefix.
     *
     * Used internally by search() and startsWith().
     *
     * Parameters:
     * - s: word or prefix to traverse
     *
     * Returns:
     * - The final TrieNode reached if traversal succeeds
     * - null if traversal fails due to invalid characters or missing nodes
     */
    private TrieNode walk(String s) {
        TrieNode current = root;

        // Traverse character by character
        for (int i = 0; i < s.length(); i++) {
            char c = Character.toLowerCase(s.charAt(i));
            int idx = index(c);

            // Fail if character is invalid
            if (idx < 0) return null;

            // Fail if required child node does not exist
            if (current.children[idx] == null) return null;

            current = current.children[idx];
        }

        return current;
    }

    /*
     * Loads words from a dictionary file into the trie.
     *
     * Parameters:
     * - trie: trie instance to populate
     * - path: filesystem path to the dictionary file
     *
     * Returns:
     * - Number of words successfully loaded
     * - 0 if the file could not be opened or read
     */
    private static int loadDictionary(trie_insert_search trie, String path) {
        int count = 0;

        // Try-with-resources ensures the reader is always closed
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String line;

            // Read file line by line
            while ((line = br.readLine()) != null) {
                line = line.trim();

                // Skip empty lines
                if (line.isEmpty()) continue;

                // Insert word into the trie
                trie.insert(line);
                count++;
            }
        } catch (IOException e) {
            // Report file access or I/O errors
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
     * - Trie construction
     * - Dictionary loading
     * - Exact word search
     * - Prefix search
     */
    public static void main(String[] args) {
        trie_insert_search trie = new trie_insert_search();

        // Path to dictionary file
        String dictPath = "..\\data\\words.txt";

        // Load dictionary words into the trie
        int loaded = loadDictionary(trie, dictPath);
        System.out.println("Loaded " + loaded + " words from " + dictPath);
        System.out.println();

        // Words to test for exact matches
        String[] tests = {"aardvark", "abandon", "zebra", "notaword"};
        for (String w : tests) {
            System.out.println("search(\"" + w + "\") = "
                    + (trie.search(w) ? "true" : "false"));
        }

        // Prefix tests
        System.out.println();
        System.out.println("Prefix tests:");
        String[] prefixes = {"ab", "alg", "aard", "nope"};
        for (String p : prefixes) {
            System.out.println("startsWith(\"" + p + "\") = "
                    + (trie.startsWith(p) ? "true" : "false"));
        }
    }
}
