// Trie Prefix Counting (top-level, dotnet-script friendly)
//
// This script implements a trie (prefix tree) focused on prefix counting:
// given a prefix, return how many inserted words start with that prefix.
//
// Key behavior / constraints:
// - Only letters 'a'–'z' are accepted (case-insensitive).
// - If any character in a word is not a–z, Insert() rejects the entire word.
// - PrefixCount(prefix) returns 0 if the prefix is invalid or not present.
// - Dictionary loading reads words from a file (default: ..\data\words.txt).
// - Relative paths are resolved from the working directory where the script runs.

const int ALPHABET_SIZE = 26; // Number of supported lowercase letters ('a'–'z')

// Root node of the trie; acts as the entry point for all insert/query operations
TrieNode root = new();

/*
 * Convert a lowercase character into a child-array index.
 *
 * Parameters:
 * - c: expected to be 'a'–'z'
 *
 * Returns:
 * - 0..25 for valid 'a'–'z'
 * - -1 for any invalid character
 */
int Index(char c) => (c >= 'a' && c <= 'z') ? (c - 'a') : -1;

/*
 * Insert a word into the trie.
 *
 * Behavior:
 * - Converts each character to lowercase (case-insensitive handling)
 * - Rejects the entire word if any character is not 'a'–'z'
 * - Creates nodes lazily as needed
 * - Increments PrefixCount on each node visited along the path
 *
 * Note:
 * - PrefixCount is incremented after advancing into the child node, meaning
 *   it represents "how many inserted words share this prefix node".
 */
void Insert(string word)
{
    TrieNode current = root;

    // Traverse the trie character-by-character
    foreach (char raw in word)
    {
        // Normalize character to lowercase using invariant rules
        char c = char.ToLowerInvariant(raw);

        // Map character to a 0..25 index
        int idx = Index(c);

        // Reject entire word if an invalid character is encountered
        if (idx < 0) return; // reject word

        // Allocate a child node if it doesn't exist
        if (current.Children[idx] == null)
            current.Children[idx] = new TrieNode();

        // Move to the next node in the trie
        current = current.Children[idx];

        // Increment count of words that share this prefix (i.e., this node)
        current.PrefixCount++;
    }
}

/*
 * Return how many inserted words start with the given prefix.
 *
 * Behavior:
 * - Returns 0 if the prefix contains invalid characters (non a–z)
 * - Returns 0 if the prefix path does not exist in the trie
 * - Otherwise returns PrefixCount stored in the node representing the prefix
 *
 * Parameters:
 * - prefix: prefix string to query
 *
 * Returns:
 * - number of words sharing that prefix (0 if invalid / not found)
 */
int PrefixCount(string prefix)
{
    TrieNode current = root;

    // Walk the trie according to the prefix
    foreach (char raw in prefix)
    {
        char c = char.ToLowerInvariant(raw);
        int idx = Index(c);

        // Invalid character -> prefix cannot exist by design
        if (idx < 0) return 0;

        // Missing path -> no words share this prefix
        if (current.Children[idx] == null) return 0;

        current = current.Children[idx];
    }

    // The stored PrefixCount represents how many words share this prefix node
    return current.PrefixCount;
}

/*
 * Load a dictionary file (one word per line) into the trie.
 *
 * Parameters:
 * - path: filesystem path to the dictionary file
 *
 * Returns:
 * - number of non-empty lines processed (Insert() attempted for each)
 * - 0 if the file cannot be opened or read
 *
 * Notes:
 * - Insert() may reject words containing invalid characters, but this loader
 *   still increments "count" for every non-empty trimmed line (as written).
 */
int LoadDictionary(string path)
{
    int count = 0;

    try
    {
        // File.ReadLines streams lines lazily, suitable for large files
        foreach (var line in File.ReadLines(path))
        {
            // Trim whitespace and skip empty lines
            var w = line.Trim();
            if (w.Length == 0) continue;

            // Insert the word into the trie (may be rejected if invalid)
            Insert(w);
            count++;
        }
    }
    catch (Exception ex)
    {
        // Report file access / I/O errors
        Console.Error.WriteLine($"Failed to open dictionary: {path}");
        Console.Error.WriteLine(ex.Message);
        return 0;
    }

    return count;
}

// -------------------- Main --------------------

/*
 * Resolve dictionary path.
 * If a script argument is provided, use it; otherwise use the lesson default.
 */
string dictPath = (Args.Count > 0) ? Args[0] : @"..\data\words.txt";

// Load dictionary words into the trie
int loaded = LoadDictionary(dictPath);

Console.WriteLine($"Loaded {loaded} words from {dictPath}\n");

// Prefixes to query against the loaded trie
string[] prefixes = ["a", "ab", "alg", "aard", "z", "nope"];
foreach (var p in prefixes)
    Console.WriteLine($"prefixCount(\"{p}\") = {PrefixCount(p)}");

Console.WriteLine();

// Helpful diagnostics for debugging relative paths at runtime
Console.WriteLine($"Working directory = {Directory.GetCurrentDirectory()}");
Console.WriteLine($"Resolved dictionary path = {Path.GetFullPath(dictPath)}");

/*
 * Trie node definition.
 *
 * Each node contains:
 * - Children: 26 child references (for 'a'–'z')
 * - PrefixCount: number of inserted words that include this node in their path
 *
 * This script focuses on prefix counting, so there is no end-of-word flag here.
 */
class TrieNode
{
    public TrieNode[] Children = new TrieNode[ALPHABET_SIZE]; // Child nodes
    public int PrefixCount = 0; // number of words sharing this prefix
}
