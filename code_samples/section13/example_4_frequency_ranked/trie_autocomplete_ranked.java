package code_samples.section13.example_4_frequency_ranked;

import java.io.*;
import java.util.*;

public class trie_autocomplete_ranked {

    private static class Node {
        Node[] children = new Node[26];
        int frequency = 0;
        boolean isEnd = false;
    }

    private final Node root = new Node();

    private int idx(char c) {
        return (c >= 'a' && c <= 'z') ? c - 'a' : -1;
    }

    public void insert(String word) {
        Node cur = root;
        for (char ch : word.toLowerCase().toCharArray()) {
            int i = idx(ch);
            if (i < 0) return;
            if (cur.children[i] == null) cur.children[i] = new Node();
            cur = cur.children[i];
        }
        cur.isEnd = true;
        cur.frequency++;
    }

    public List<Map.Entry<String,Integer>> autocomplete(String prefix, int limit) {
        Node start = root;
        for (char ch : prefix.toLowerCase().toCharArray()) {
            int i = idx(ch);
            if (i < 0 || start.children[i] == null) return List.of();
            start = start.children[i];
        }

        List<Map.Entry<String,Integer>> out = new ArrayList<>();
        dfs(start, new StringBuilder(prefix.toLowerCase()), out);

        out.sort((a,b) -> {
            if (!a.getValue().equals(b.getValue()))
                return b.getValue() - a.getValue();
            return a.getKey().compareTo(b.getKey());
        });

        return out.subList(0, Math.min(limit, out.size()));
    }

    private void dfs(Node node, StringBuilder buf,
                     List<Map.Entry<String,Integer>> out) {
        if (node.isEnd)
            out.add(Map.entry(buf.toString(), node.frequency));
        for (int i = 0; i < 26; i++) {
            if (node.children[i] != null) {
                buf.append((char)('a'+i));
                dfs(node.children[i], buf, out);
                buf.deleteCharAt(buf.length()-1);
            }
        }
    }

    private static void loadFile(trie_autocomplete_ranked t, String path)
            throws IOException {
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String w;
            while ((w = br.readLine()) != null)
                if (!w.isEmpty()) t.insert(w);
        }
    }

    public static void main(String[] args) throws Exception {
        trie_autocomplete_ranked trie = new trie_autocomplete_ranked();

        String dict = "..\\data\\words.txt";
        String freq = "..\\data\\frequency.txt";
        String prefix = "th";

        loadFile(trie, dict);
        loadFile(trie, freq);

        for (var e : trie.autocomplete(prefix, 20))
            System.out.println(e.getKey() + "\t(freq=" + e.getValue() + ")");
    }
}

