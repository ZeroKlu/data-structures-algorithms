// trie_autocomplete_ranked.cs
// Run: dotnet script .\trie_autocomplete_ranked.cs
// Optional args: [dictPath] [freqPath] [prefix] [limit]

const int ALPHABET_SIZE = 26;

Node root = new();

int Idx(char c) => (c >= 'a' && c <= 'z') ? (c - 'a') : -1;

// Insert (bump frequency at terminal). Reject word if any char not a-z.
void Insert(string word)
{
    var cur = root;
    foreach (char raw in word)
    {
        char ch = char.ToLowerInvariant(raw);
        int i = Idx(ch);
        if (i < 0) return;
        cur.Children[i] ??= new Node();
        cur = cur.Children[i];
    }
    cur.IsEnd = true;
    cur.Frequency++;
}

void LoadFile(string path)
{
    foreach (var line in File.ReadLines(path))
    {
        var w = line.Trim();
        if (w.Length == 0) continue;
        Insert(w);
    }
}

List<(string word, int freq)> AutocompleteRanked(string prefix, int limit)
{
    if (limit <= 0) return new();

    // Walk to prefix node
    var cur = root;
    foreach (char raw in prefix)
    {
        char ch = char.ToLowerInvariant(raw);
        int i = Idx(ch);
        if (i < 0 || cur.Children[i] == null) return new();
        cur = cur.Children[i];
    }

    var results = new List<(string word, int freq)>();

    // Dynamic buffer avoids out-of-bounds during DFS
    var buf = new List<char>(prefix.Length + 32);
    foreach (char raw in prefix) buf.Add(char.ToLowerInvariant(raw));

    void Dfs(Node node)
    {
        if (node == null) return;

        if (node.IsEnd)
            results.Add((new string(buf.ToArray()), node.Frequency));

        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            var child = node.Children[i];
            if (child != null)
            {
                buf.Add((char)('a' + i));
                Dfs(child);
                buf.RemoveAt(buf.Count - 1);
            }
        }
    }

    Dfs(cur);

    results.Sort((a, b) =>
    {
        int freqCmp = b.freq.CompareTo(a.freq); // frequency desc
        if (freqCmp != 0) return freqCmp;
        return string.CompareOrdinal(a.word, b.word); // word asc
    });

    if (results.Count > limit) results.RemoveRange(limit, results.Count - limit);
    return results;
}

// -------------------- Main --------------------
// Defaults:
string dictPath = (Args.Count > 0) ? Args[0] : @"..\data\words.txt";
string freqPath = (Args.Count > 1) ? Args[1] : @"..\data\frequency.txt";
string prefix   = (Args.Count > 2) ? Args[2] : "th";
int limit        = (Args.Count > 3 && int.TryParse(Args[3], out int v) && v > 0) ? v : 20;

LoadFile(dictPath);
LoadFile(freqPath);

Console.WriteLine($"Ranked Autocomplete(\"{prefix}\") [limit={limit}]");
foreach (var (word, freq) in AutocompleteRanked(prefix, limit))
    Console.WriteLine($"{word}\t(freq={freq})");

Console.WriteLine();
Console.WriteLine($"Working directory = {Directory.GetCurrentDirectory()}");
Console.WriteLine($"Resolved dictionary path = {Path.GetFullPath(dictPath)}");
Console.WriteLine($"Resolved frequency path  = {Path.GetFullPath(freqPath)}");

class Node
{
    public Node[] Children = new Node[ALPHABET_SIZE];
    public int Frequency = 0;
    public bool IsEnd = false;
}
