package code_samples.section13.example_1_insert_and_search;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class trie_insert_search {

    private static final int ALPHABET_SIZE = 26;

    private static class TrieNode {
        TrieNode[] children = new TrieNode[ALPHABET_SIZE];
        boolean isEndOfWord = false;
    }

    private final TrieNode root = new TrieNode();

    private static int index(char c) {
        if (c < 'a' || c > 'z') return -1;
        return c - 'a';
    }

    /** Inserts a word into the trie. Rejects the entire word if any char is not a-z. */
    public void insert(String word) {
        TrieNode current = root;
        for (int i = 0; i < word.length(); i++) {
            char c = Character.toLowerCase(word.charAt(i));
            int idx = index(c);
            if (idx < 0) return; // reject words containing non a-z characters
            if (current.children[idx] == null) {
                current.children[idx] = new TrieNode();
            }
            current = current.children[idx];
        }
        current.isEndOfWord = true;
    }

    /** Returns true if the exact word exists in the trie. Rejects words containing non a-z. */
    public boolean search(String word) {
        TrieNode node = walk(word);
        return node != null && node.isEndOfWord;
    }

    /** Returns true if any word in the trie starts with the given prefix. Rejects prefixes containing non a-z. */
    public boolean startsWith(String prefix) {
        return walk(prefix) != null;
    }

    private TrieNode walk(String s) {
        TrieNode current = root;
        for (int i = 0; i < s.length(); i++) {
            char c = Character.toLowerCase(s.charAt(i));
            int idx = index(c);
            if (idx < 0) return null;
            if (current.children[idx] == null) return null;
            current = current.children[idx];
        }
        return current;
    }

    private static int loadDictionary(trie_insert_search trie, String path) {
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
        trie_insert_search trie = new trie_insert_search();

        String dictPath = "..\\data\\words.txt";
        int loaded = loadDictionary(trie, dictPath);
        System.out.println("Loaded " + loaded + " words from " + dictPath);
        System.out.println();

        String[] tests = {"aardvark", "abandon", "zebra", "notaword"};
        for (String w : tests) {
            System.out.println("search(\"" + w + "\") = " + (trie.search(w) ? "true" : "false"));
        }

        System.out.println();
        System.out.println("Prefix tests:");
        String[] prefixes = {"ab", "alg", "aard", "nope"};
        for (String p : prefixes) {
            System.out.println("startsWith(\"" + p + "\") = " + (trie.startsWith(p) ? "true" : "false"));
        }
    }
}
