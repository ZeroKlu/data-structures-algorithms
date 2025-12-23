package code_samples.section13.example_4_frequency_ranked;

import java.io.*;   // I/O primitives (BufferedReader, FileReader, IOException)
import java.util.*; // Collections (List, ArrayList, Map, Entry, etc.)

/*
 * trie_autocomplete_ranked
 *
 * This example implements a trie (prefix tree) that supports "frequency-ranked" autocomplete.
 * It loads words from two files:
 * - words.txt: a baseline dictionary
 * - frequency.txt: additional occurrences / usage events (each occurrence bumps frequency)
 *
 * Ranking rules (as implemented):
 * - Higher frequency first
 * - If frequencies tie, lexicographically smaller word first
 *
 * Key constraints:
 * - Only letters 'a'–'z' are accepted (case-insensitive).
 * - If any character in an inserted word is not a–z, insert() rejects that word.
 * - If any character in the prefix is not a–z, autocomplete() returns an empty list.
 *
 * Implementation approach:
 * - Collect ALL completions under the prefix using DFS
 * - Sort the collected list using the ranking rules
 * - Return the first `limit` items
 *
 * Note:
 * - This is simple but can be expensive for prefixes with many matches because it
 *   stores and sorts the entire subtree’s completions.
 */
public class trie_autocomplete_ranked {

    /*
     * Trie node.
     *
     * Fields:
     * - children: 26 child references (a–z)
     * - frequency: how many times this complete word has been inserted
     * - isEnd: whether a complete word ends at this node
     *
     * Frequency is only meaningful when isEnd == true.
     */
    private static class Node {
        Node[] children = new Node[26]; // Child pointers for letters a..z
        int frequency = 0;              // Word frequency counter (terminal nodes)
        boolean isEnd = false;          // End-of-word marker
    }

    // Root node of the trie (does not correspond to any character)
    private final Node root = new Node();

    /*
     * Convert a lowercase character to a trie child index.
     *
     * Parameters:
     * - c: expected to be 'a'..'z'
     *
     * Returns:
     * - 0..25 if valid
     * - -1 if invalid
     */
    private int idx(char c) {
        return (c >= 'a' && c <= 'z') ? c - 'a' : -1;
    }

    /*
     * Insert a word into the trie and bump its frequency.
     *
     * Behavior:
     * - Lowercases the word (case-insensitive)
     * - Rejects the entire word if any character is not 'a'..'z'
     * - Creates missing nodes lazily
     * - Marks terminal node as end-of-word and increments frequency
     *
     * Parameters:
     * - word: word to insert
     *
     * Frequency meaning:
     * - Each call to insert(word) increments that word’s frequency by 1.
     * - Loading multiple files means the same word can accumulate frequency across inputs.
     */
    public void insert(String word) {
        Node cur = root;

        // Normalize to lowercase and traverse each character
        for (char ch : word.toLowerCase().toCharArray()) {
            int i = idx(ch);

            // Reject word if invalid character encountered
            if (i < 0) return;

            // Allocate child node if missing
            if (cur.children[i] == null) cur.children[i] = new Node();

            // Advance in trie
            cur = cur.children[i];
        }

        // Mark complete word and bump frequency
        cur.isEnd = true;
        cur.frequency++;
    }

    /*
     * Return ranked autocomplete suggestions for a prefix.
     *
     * Parameters:
     * - prefix: prefix to autocomplete
     * - limit: maximum number of results to return
     *
     * Returns:
     * - List of Map.Entry(word, frequency), best-first
     * - Empty list if prefix is invalid or not found
     *
     * Steps:
     * 1) Walk from root to the node corresponding to the prefix.
     * 2) DFS the subtree under that node, collecting (word, frequency) for each terminal node.
     * 3) Sort collected results by:
     *    - frequency descending
     *    - word ascending for ties
     * 4) Return the first `limit` results (via subList).
     *
     * Note:
     * - This method currently does not guard against non-positive `limit`.
     *   If limit <= 0, Math.min(limit, out.size()) can produce a negative end index,
     *   which would cause an IndexOutOfBoundsException when calling subList().
     *   (No code changes requested—this is just an explanatory note.)
     */
    public List<Map.Entry<String,Integer>> autocomplete(String prefix, int limit) {
        // Walk to prefix node
        Node start = root;

        // Normalize prefix to lowercase and traverse it
        for (char ch : prefix.toLowerCase().toCharArray()) {
            int i = idx(ch);

            // Invalid character or missing path => no suggestions
            if (i < 0 || start.children[i] == null) return List.of();

            start = start.children[i];
        }

        // Collect all completions under the prefix node
        List<Map.Entry<String,Integer>> out = new ArrayList<>();

        // Seed the buffer with the lowercased prefix so DFS can append suffix letters
        dfs(start, new StringBuilder(prefix.toLowerCase()), out);

        // Sort best-first:
        // - higher frequency first
        // - lexicographically smaller word first for ties
        out.sort((a,b) -> {
            if (!a.getValue().equals(b.getValue()))
                return b.getValue() - a.getValue();
            return a.getKey().compareTo(b.getKey());
        });

        // Return only the top `limit` results (or fewer if not enough matches)
        return out.subList(0, Math.min(limit, out.size()));
    }

    /*
     * DFS traversal collecting all terminal words under a subtree.
     *
     * Parameters:
     * - node: current trie node
     * - buf: mutable buffer containing the current word (prefix + explored suffix)
     * - out: output list of (word, frequency) pairs
     *
     * Behavior:
     * - If node.isEnd is true, emit current buffer as a suggestion with node.frequency.
     * - Visit children in 'a'..'z' order:
     *   append letter -> recurse -> backtrack (delete last char).
     *
     * Ordering note:
     * - DFS emits words in lexicographic order, but final output order is determined
     *   by the sort() step in autocomplete().
     */
    private void dfs(Node node, StringBuilder buf,
                     List<Map.Entry<String,Integer>> out) {
        // Emit a suggestion if this node terminates a complete word
        if (node.isEnd)
            out.add(Map.entry(buf.toString(), node.frequency));

        // Explore children in lexicographic order
        for (int i = 0; i < 26; i++) {
            if (node.children[i] != null) {
                // Append next letter
                buf.append((char)('a' + i));

                // Recurse into child subtree
                dfs(node.children[i], buf, out);

                // Backtrack: remove last appended letter
                buf.deleteCharAt(buf.length() - 1);
            }
        }
    }

    /*
     * Load all non-empty lines from a file and insert them into the trie.
     *
     * Parameters:
     * - t: trie instance to populate
     * - path: filesystem path to read
     *
     * Throws:
     * - IOException if file cannot be opened/read
     *
     * Notes:
     * - Uses try-with-resources to ensure the reader is closed.
     * - Does not trim lines; it only checks !w.isEmpty().
     *   Lines containing whitespace-only strings would be treated as "non-empty"
     *   and passed to insert(), which will reject them due to invalid characters.
     *   (No code changes requested—this is just an explanatory note.)
     */
    private static void loadFile(trie_autocomplete_ranked t, String path)
            throws IOException {
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String w;

            // Read each line; insert if non-empty
            while ((w = br.readLine()) != null)
                if (!w.isEmpty()) t.insert(w);
        }
    }

    /*
     * Program entry point.
     *
     * Current behavior:
     * - Uses hard-coded default paths and prefix:
     *   dict = "..\\data\\words.txt"
     *   freq = "..\\data\\frequency.txt"
     *   prefix = "th"
     * - Loads both files into the trie.
     * - Prints the top 20 ranked autocomplete suggestions for the prefix.
     *
     * Exception policy:
     * - Declares `throws Exception`, so I/O and other errors will propagate and
     *   terminate the program unless caught by the runtime.
     */
    public static void main(String[] args) throws Exception {
        trie_autocomplete_ranked trie = new trie_autocomplete_ranked();

        String dict = "..\\data\\words.txt";
        String freq = "..\\data\\frequency.txt";
        String prefix = "th";

        // Load baseline dictionary words, then additional frequency/usage events
        loadFile(trie, dict);
        loadFile(trie, freq);

        // Print ranked suggestions as "word\t(freq=N)"
        for (var e : trie.autocomplete(prefix, 20))
            System.out.println(e.getKey() + "\t(freq=" + e.getValue() + ")");
    }
}
