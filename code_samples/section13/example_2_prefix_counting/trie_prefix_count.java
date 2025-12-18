package code_samples.section13.example_2_prefix_counting;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class trie_prefix_count {

    private static final int ALPHABET_SIZE = 26;

    private static class TrieNode {
        TrieNode[] children = new TrieNode[ALPHABET_SIZE];
        int prefixCount = 0;      // number of words sharing this prefix
        @SuppressWarnings("unused")
        boolean isEndOfWord = false;
    }

    private final TrieNode root = new TrieNode();

    private static int index(char c) {
        if (c < 'a' || c > 'z') return -1;
        return c - 'a';
    }

    /**
     * Inserts a word into the trie and increments prefixCount along the path.
     * Rejects the entire word if any character is not a-z (case-insensitive).
     */
    public void insert(String word) {
        TrieNode current = root;
        for (int i = 0; i < word.length(); i++) {
            char c = Character.toLowerCase(word.charAt(i));
            int idx = index(c);
            if (idx < 0) return; // reject word
            if (current.children[idx] == null) {
                current.children[idx] = new TrieNode();
            }
            current = current.children[idx];
            current.prefixCount++;
        }
        current.isEndOfWord = true;
    }

    /**
     * Returns how many words in the trie start with the given prefix.
     * Returns 0 if the prefix is invalid or not found.
     */
    public int prefixCount(String prefix) {
        TrieNode current = root;
        for (int i = 0; i < prefix.length(); i++) {
            char c = Character.toLowerCase(prefix.charAt(i));
            int idx = index(c);
            if (idx < 0) return 0;
            if (current.children[idx] == null) return 0;
            current = current.children[idx];
        }
        return current.prefixCount;
    }

    private static int loadDictionary(trie_prefix_count trie, String path) {
        int count = 0;
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String line;
            while ((line = br.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty()) continue;
                trie.insert(line);
                count++;
            }
        } catch (IOException e) {
            System.err.println("Failed to open dictionary: " + path);
            System.err.println(e.getMessage());
            return 0;
        }
        return count;
    }

    public static void main(String[] args) {
        trie_prefix_count trie = new trie_prefix_count();

        String dictPath = (args.length > 0) ? args[0] : "..\\data\\words.txt";
        int loaded = loadDictionary(trie, dictPath);

        System.out.println("Loaded " + loaded + " words from " + dictPath);
        System.out.println();

        String[] prefixes = { "a", "ab", "alg", "aard", "z", "nope" };
        for (String p : prefixes) {
            System.out.println("prefixCount(\"" + p + "\") = " + trie.prefixCount(p));
        }
    }
}
