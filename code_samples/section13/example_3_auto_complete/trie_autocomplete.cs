// Trie Autocomplete (top-level, dotnet-script friendly)
//
// This script implements a trie (prefix tree) that supports autocomplete.
// It is designed to run as a single-file, top-level C# script using dotnet-script.
//
// Key behavior / constraints:
// - Only letters 'a'–'z' are accepted (case-insensitive).
// - If any character in a word is not a–z, Insert() rejects the entire word.
// - If any character in a prefix is not a–z, Walk() returns null and
//   Autocomplete() returns an empty list.
// - Autocomplete results are produced in lexicographic order because DFS
//   visits children in 'a'..'z' order.
//
// Dictionary loading:
// - Loads from a file path (default: ..\data\words.txt)
// - Relative paths resolve from the working directory where the script runs.

const int ALPHABET_SIZE = 26; // Number of supported lowercase English letters

// Root node of the trie; the entry point for all insert/search/autocomplete operations
TrieNode root = new();

/*
 * Convert a lowercase character into an index [0, 25].
 *
 * Parameters:
 * - c: expected to be 'a'..'z'
 *
 * Returns:
 * - 0..25 if valid
 * - -1 if invalid
 */
int Index(char c) => (c >= 'a' && c <= 'z') ? (c - 'a') : -1;

/*
 * Insert a word into the trie.
 *
 * Behavior:
 * - Normalizes characters to lowercase (case-insensitive)
 * - Rejects the entire word if any character is not 'a'–'z'
 * - Creates missing nodes as needed
 * - Marks the final node as an end-of-word
 *
 * Parameters:
 * - word: string to insert
 */
void Insert(string word)
{
    TrieNode current = root;

    // Traverse each character in the word
    foreach (char raw in word)
    {
        // Normalize character to lowercase (invariant) for consistent indexing
        char c = char.ToLowerInvariant(raw);

        // Convert to index
        int idx = Index(c);

        // Reject whole word if any invalid character appears
        if (idx < 0) return;

        // Allocate child node if missing
        if (current.Children[idx] == null) current.Children[idx] = new TrieNode();

        // Move to the next trie node
        current = current.Children[idx];
    }

    // Mark the final node as a complete word
    current.IsEndOfWord = true;
}

/*
 * Walk the trie for a given prefix and return the node at the end of the prefix.
 *
 * Behavior:
 * - Lowercases each character
 * - Returns null if any character is invalid (non a–z)
 * - Returns null if any required child node is missing (prefix not found)
 *
 * Parameters:
 * - prefix: prefix to walk
 *
 * Returns:
 * - TrieNode representing the final character of prefix, or null if not found/invalid
 */
TrieNode Walk(string prefix)
{
    TrieNode current = root;

    foreach (char raw in prefix)
    {
        char c = char.ToLowerInvariant(raw);
        int idx = Index(c);

        // Invalid character -> prefix not supported
        if (idx < 0) return null;

        // Missing path -> prefix not present in trie
        if (current.Children[idx] == null) return null;

        current = current.Children[idx];
    }

    return current;
}

/*
 * Depth-first traversal to collect autocomplete suggestions.
 *
 * Parameters:
 * - node: subtree root to explore
 * - outWords: output list of collected words
 * - buffer: character buffer containing the current word under construction
 * - depth: current length (number of valid chars in buffer)
 * - limit: maximum number of suggestions to collect
 *
 * Behavior:
 * - When node.IsEndOfWord is true, the current buffer[0..depth) is a complete word.
 * - Visits children in 'a'..'z' order to produce lexicographic suggestions.
 * - Stops early once outWords.Count reaches limit.
 *
 * Implementation detail:
 * - This DFS writes the next letter into buffer[depth] before recursing.
 * - The buffer is reused across recursion to avoid repeated allocations.
 */
void DfsCollect(TrieNode node, List<string> outWords, char[] buffer, int depth, int limit)
{
    // Base cases: no subtree, or we've reached the desired limit
    if (node == null || outWords.Count >= limit) return;

    // If this node represents the end of a word, add the current buffer as a suggestion
    if (node.IsEndOfWord)
    {
        outWords.Add(new string(buffer, 0, depth));
        if (outWords.Count >= limit) return;
    }

    // Explore children in lexicographic order
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node.Children[i] != null)
        {
            // Append the next character to the buffer
            buffer[depth] = (char)('a' + i);

            // Recurse deeper to extend the word
            DfsCollect(node.Children[i], outWords, buffer, depth + 1, limit);

            // Stop immediately once we reach the limit (avoid extra traversal)
            if (outWords.Count >= limit) return;
        }
    }
}

/*
 * Return autocomplete suggestions for a prefix, up to a given limit.
 *
 * Steps:
 * 1) Find the trie node corresponding to the prefix via Walk().
 * 2) Seed a character buffer with the (lowercased) prefix.
 * 3) DFS from the prefix node to enumerate suffixes and collect full words.
 *
 * Parameters:
 * - prefix: prefix to autocomplete
 * - limit: maximum number of suggestions to return
 *
 * Returns:
 * - List of suggestions (possibly empty)
 *
 * Notes:
 * - The returned suggestions include the prefix itself only if the prefix
 *   is also a complete word (i.e., start.IsEndOfWord is true).
 */
List<string> Autocomplete(string prefix, int limit)
{
    TrieNode start = Walk(prefix);

    // If prefix is invalid or not found, return empty result
    if (start == null) return [];

    // Pre-size list capacity to avoid repeated growth allocations
    var outWords = new List<string>(limit);

    // Buffer used to build candidate words (prefix + explored suffix letters)
    // Size 256 is a fixed cap for suggestions; longer words will be truncated by seeding logic.
    char[] buffer = new char[256];

    // Seed the buffer with the prefix (lowercased) and track initial depth
    int depth = 0;
    foreach (char raw in prefix)
    {
        if (depth >= buffer.Length) break;         // Prevent buffer overflow
        buffer[depth++] = char.ToLowerInvariant(raw);
    }

    // Collect suggestions under the prefix node
    DfsCollect(start, outWords, buffer, depth, limit);
    return outWords;
}

/*
 * Load a dictionary file into the trie.
 *
 * Parameters:
 * - path: filesystem path to the dictionary file
 *
 * Returns:
 * - number of non-empty trimmed lines processed (Insert attempted for each)
 * - 0 if the file cannot be opened/read
 *
 * Notes:
 * - Insert() may reject words containing invalid characters, but this function
 *   still increments "count" for each non-empty trimmed line (as written).
 */
int LoadDictionary(string path)
{
    int count = 0;

    try
    {
        // Streams lines lazily (better for large files than ReadAllLines)
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
        // Report file access / I/O errors
        Console.Error.WriteLine($"Failed to open dictionary: {path}");
        Console.Error.WriteLine(ex.Message);
        return 0;
    }

    return count;
}

/*
 * Parse an integer from a string or return a default.
 *
 * Parameters:
 * - s: input string (possibly null/empty/whitespace)
 * - def: fallback default value
 *
 * Returns:
 * - parsed positive integer if valid (> 0)
 * - def otherwise
 */
int ParseIntOrDefault(string s, int def)
{
    if (string.IsNullOrWhiteSpace(s)) return def;
    return int.TryParse(s, out int v) && v > 0 ? v : def;
}

// -------------------- Main --------------------
// Usage: dotnet script .\trie_autocomplete.cs [dictPath] [prefix] [limit]

/*
 * Resolve dictionary path and optional arguments:
 * - Args[0]: dictionary path
 * - Args[1]: prefix
 * - Args[2]: limit
 */
string dictPath = (Args.Count > 0) ? Args[0] : @"..\data\words.txt";
string prefix   = (Args.Count > 1) ? Args[1] : "ab";
int limit       = (Args.Count > 2) ? ParseIntOrDefault(Args[2], 20) : 20;

// Load dictionary words into the trie
int loaded = LoadDictionary(dictPath);
Console.WriteLine($"Loaded {loaded} words from {dictPath}\n");

// Run autocomplete and print the results
Console.WriteLine($"Autocomplete(\"{prefix}\") [limit={limit}]");
foreach (var w in Autocomplete(prefix, limit))
    Console.WriteLine(w);

Console.WriteLine();

// Helpful diagnostics for debugging relative path resolution
Console.WriteLine($"Working directory = {Directory.GetCurrentDirectory()}");
Console.WriteLine($"Resolved dictionary path = {Path.GetFullPath(dictPath)}");

/*
 * Trie node definition.
 *
 * Each node contains:
 * - Children: 26 child references for 'a'–'z'
 * - IsEndOfWord: true if a complete word ends at this node
 *
 * This variant does not store prefix counts because autocomplete enumerates
 * words via DFS instead.
 */
class TrieNode
{
    public TrieNode[] Children = new TrieNode[ALPHABET_SIZE]; // Child nodes
    public bool IsEndOfWord = false;                          // End-of-word marker
}
