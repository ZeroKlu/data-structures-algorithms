// Trie Prefix Counting (top-level, dotnet-script friendly)

const int ALPHABET_SIZE = 26;

TrieNode root = new TrieNode();

int Index(char c) => (c >= 'a' && c <= 'z') ? (c - 'a') : -1;

// Insert a word; increments PrefixCount along the path.
// Rejects the entire word if any char is not a-z (case-insensitive).
void Insert(string word)
{
    TrieNode current = root;
    foreach (char raw in word)
    {
        char c = char.ToLowerInvariant(raw);
        int idx = Index(c);
        if (idx < 0) return; // reject word
        if (current.Children[idx] == null)
            current.Children[idx] = new TrieNode();
        current = current.Children[idx];
        current.PrefixCount++;
    }
}

// Return how many words start with prefix (0 if invalid or not found).
int PrefixCount(string prefix)
{
    TrieNode current = root;
    foreach (char raw in prefix)
    {
        char c = char.ToLowerInvariant(raw);
        int idx = Index(c);
        if (idx < 0) return 0;
        if (current.Children[idx] == null) return 0;
        current = current.Children[idx];
    }
    return current.PrefixCount;
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

// -------------------- Main --------------------

string dictPath = (Args.Count > 0) ? Args[0] : @"..\data\words.txt";
int loaded = LoadDictionary(dictPath);

Console.WriteLine($"Loaded {loaded} words from {dictPath}\n");

string[] prefixes = { "a", "ab", "alg", "aard", "z", "nope" };
foreach (var p in prefixes)
    Console.WriteLine($"prefixCount(\"{p}\") = {PrefixCount(p)}");

Console.WriteLine();
Console.WriteLine($"Working directory = {Directory.GetCurrentDirectory()}");
Console.WriteLine($"Resolved dictionary path = {Path.GetFullPath(dictPath)}");

class TrieNode
{
    public TrieNode[] Children = new TrieNode[ALPHABET_SIZE];
    public int PrefixCount = 0; // number of words sharing this prefix
}
