using System;
using System.Collections.Generic;
using System.IO;

// Trie Autocomplete (top-level, dotnet-script friendly)

const int ALPHABET_SIZE = 26;

TrieNode root = new();

int Index(char c) => (c >= 'a' && c <= 'z') ? (c - 'a') : -1;

void Insert(string word)
{
    TrieNode current = root;
    foreach (char raw in word)
    {
        char c = char.ToLowerInvariant(raw);
        int idx = Index(c);
        if (idx < 0) return;
        if (current.Children[idx] == null) current.Children[idx] = new TrieNode();
        current = current.Children[idx];
    }
    current.IsEndOfWord = true;
}

TrieNode Walk(string prefix)
{
    TrieNode current = root;
    foreach (char raw in prefix)
    {
        char c = char.ToLowerInvariant(raw);
        int idx = Index(c);
        if (idx < 0) return null;
        if (current.Children[idx] == null) return null;
        current = current.Children[idx];
    }
    return current;
}

void DfsCollect(TrieNode node, List<string> outWords, char[] buffer, int depth, int limit)
{
    if (node == null || outWords.Count >= limit) return;

    if (node.IsEndOfWord)
    {
        outWords.Add(new string(buffer, 0, depth));
        if (outWords.Count >= limit) return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node.Children[i] != null)
        {
            buffer[depth] = (char)('a' + i);
            DfsCollect(node.Children[i], outWords, buffer, depth + 1, limit);
            if (outWords.Count >= limit) return;
        }
    }
}

List<string> Autocomplete(string prefix, int limit)
{
    TrieNode start = Walk(prefix);
    if (start == null) return [];

    var outWords = new List<string>(limit);
    char[] buffer = new char[256];

    int depth = 0;
    foreach (char raw in prefix)
    {
        if (depth >= buffer.Length) break;
        buffer[depth++] = char.ToLowerInvariant(raw);
    }

    DfsCollect(start, outWords, buffer, depth, limit);
    return outWords;
}

int LoadDictionary(string path)
{
    int count = 0;
    try
    {
        foreach (var line in File.ReadLines(path))
        {
            var w = line.Trim();
            if (w.Length == 0) continue;
            Insert(w);
            count++;
        }
    }
    catch (Exception ex)
    {
        Console.Error.WriteLine($"Failed to open dictionary: {path}");
        Console.Error.WriteLine(ex.Message);
        return 0;
    }
    return count;
}

int ParseIntOrDefault(string s, int def)
{
    if (string.IsNullOrWhiteSpace(s)) return def;
    return int.TryParse(s, out int v) && v > 0 ? v : def;
}

// -------------------- Main --------------------
// Usage: dotnet script .\trie_autocomplete.cs [dictPath] [prefix] [limit]

string dictPath = (Args.Count > 0) ? Args[0] : @"..\data\words.txt";
string prefix   = (Args.Count > 1) ? Args[1] : "ab";
int limit       = (Args.Count > 2) ? ParseIntOrDefault(Args[2], 20) : 20;

int loaded = LoadDictionary(dictPath);
Console.WriteLine($"Loaded {loaded} words from {dictPath}\n");

Console.WriteLine($"Autocomplete(\"{prefix}\") [limit={limit}]");
foreach (var w in Autocomplete(prefix, limit))
    Console.WriteLine(w);

Console.WriteLine();
Console.WriteLine($"Working directory = {Directory.GetCurrentDirectory()}");
Console.WriteLine($"Resolved dictionary path = {Path.GetFullPath(dictPath)}");

class TrieNode
{
    public TrieNode[] Children = new TrieNode[ALPHABET_SIZE];
    public bool IsEndOfWord = false;
}
