package code_samples.section13.example_3_auto_complete;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class trie_autocomplete {

    private static final int ALPHABET_SIZE = 26;
    @SuppressWarnings("unused")
    private static final int WORD_MAX = 128;

    private static class TrieNode {
        TrieNode[] children = new TrieNode[ALPHABET_SIZE];
        boolean isEndOfWord = false;
    }

    private final TrieNode root = new TrieNode();

    private static int index(char c) {
        if (c < 'a' || c > 'z') return -1;
        return c - 'a';
    }

    public void insert(String word) {
        TrieNode current = root;
        for (int i = 0; i < word.length(); i++) {
            char c = Character.toLowerCase(word.charAt(i));
            int idx = index(c);
            if (idx < 0) return;
            if (current.children[idx] == null) current.children[idx] = new TrieNode();
            current = current.children[idx];
        }
        current.isEndOfWord = true;
    }

    private TrieNode walk(String prefix) {
        TrieNode current = root;
        for (int i = 0; i < prefix.length(); i++) {
            char c = Character.toLowerCase(prefix.charAt(i));
            int idx = index(c);
            if (idx < 0) return null;
            if (current.children[idx] == null) return null;
            current = current.children[idx];
        }
        return current;
    }

    public ArrayList<String> autocomplete(String prefix, int limit) {
        TrieNode start = walk(prefix);
        ArrayList<String> out = new ArrayList<>();
        if (start == null) return out;

        StringBuilder buf = new StringBuilder(prefix.length() + 16);
        for (int i = 0; i < prefix.length(); i++) buf.append(Character.toLowerCase(prefix.charAt(i)));

        dfsCollect(start, buf, out, limit);
        return out;
    }

    private void dfsCollect(TrieNode node, StringBuilder buf, ArrayList<String> out, int limit) {
        if (node == null || out.size() >= limit) return;

        if (node.isEndOfWord) {
            out.add(buf.toString());
            if (out.size() >= limit) return;
        }

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node.children[i] != null) {
                buf.append((char)('a' + i));
                dfsCollect(node.children[i], buf, out, limit);
                buf.deleteCharAt(buf.length() - 1);
                if (out.size() >= limit) return;
            }
        }
    }

    private static int loadDictionary(trie_autocomplete trie, String path) {
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

    private static int parseIntOrDefault(String s, int def) {
        try {
            int v = Integer.parseInt(s);
            return v > 0 ? v : def;
        } catch (Exception e) {
            return def;
        }
    }

    public static void main(String[] args) {
        trie_autocomplete trie = new trie_autocomplete();

        String dictPath = (args.length > 0) ? args[0] : "..\\data\\words.txt";
        String prefix   = (args.length > 1) ? args[1] : "ab";
        int limit       = (args.length > 2) ? parseIntOrDefault(args[2], 20) : 20;

        int loaded = loadDictionary(trie, dictPath);
        System.out.println("Loaded " + loaded + " words from " + dictPath);
        System.out.println();

        System.out.println("Autocomplete(\"" + prefix + "\") [limit=" + limit + "]");
        for (String w : trie.autocomplete(prefix, limit)) {
            System.out.println(w);
        }
    }
}
