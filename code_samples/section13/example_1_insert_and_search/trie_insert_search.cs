// Trie Insert + Search (top-level, dotnet-script friendly)

const int ALPHABET_SIZE = 26;

TrieNode root = new();

int Index(char c) => (c >= 'a' && c <= 'z') ? (c - 'a') : -1;

// Insert a word; reject the entire word if any char is not a-z
void Insert(string word)
{
    TrieNode current = root;
    foreach (char raw in word)
    {
        char c = char.ToLowerInvariant(raw);
        int idx = Index(c);
        if (idx < 0) return;
        if (current.Children[idx] == null)
            current.Children[idx] = new TrieNode();
        current = current.Children[idx];
    }
    current.IsEndOfWord = true;
}

TrieNode Walk(string s)
{
    TrieNode current = root;
    foreach (char raw in s)
    {
        char c = char.ToLowerInvariant(raw);
        int idx = Index(c);
        if (idx < 0) return null;
        if (current.Children[idx] == null) return null;
        current = current.Children[idx];
    }
    return current;
}

bool Search(string word)
{
    var node = Walk(word);
    return node != null && node.IsEndOfWord;
}

bool StartsWith(string prefix) => Walk(prefix) != null;

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

// -------------------- Main --------------------

string dictPath = (Args.Count > 0) ? Args[0] : @"..\data\words.txt";
int loaded = LoadDictionary(dictPath);

Console.WriteLine($"Loaded {loaded} words from {dictPath}");
Console.WriteLine();

string[] tests = ["aardvark", "abandon", "zebra", "notaword"];
foreach (var w in tests)
    Console.WriteLine($"search(\"{w}\") = {(Search(w) ? "true" : "false")}");

Console.WriteLine();
Console.WriteLine("Prefix tests:");
string[] prefixes = ["ab", "alg", "aard", "nope"];
foreach (var p in prefixes)
    Console.WriteLine($"startsWith(\"{p}\") = {(StartsWith(p) ? "true" : "false")}");

Console.WriteLine();
Console.WriteLine($"Working directory = {Directory.GetCurrentDirectory()}");
Console.WriteLine($"Resolved dictionary path = {Path.GetFullPath(dictPath)}");

class TrieNode
{
    public TrieNode[] Children = new TrieNode[ALPHABET_SIZE];
    public bool IsEndOfWord = false;
}
