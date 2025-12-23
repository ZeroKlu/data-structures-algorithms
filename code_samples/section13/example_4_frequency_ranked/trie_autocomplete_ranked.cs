// trie_autocomplete_ranked.cs
//
// Run: dotnet script .\trie_autocomplete_ranked.cs
// Optional args: [dictPath] [freqPath] [prefix] [limit]
//
// This script implements a trie (prefix tree) that supports ranked autocomplete.
// It loads words from two files:
// - dictPath: baseline dictionary words
// - freqPath: additional "usage" or frequency events (each occurrence bumps frequency)
// Then it prints the top ranked completions for a given prefix.
//
// Ranking rules (as implemented):
// - Higher frequency first (descending)
// - If frequencies tie, lexicographically smaller word first (ascending, ordinal)
//
// Key constraints:
// - Only letters 'a'–'z' are accepted (case-insensitive).
// - If any character in a word is not a–z, Insert() rejects the entire word.
// - If any character in the prefix is not a–z, AutocompleteRanked() returns empty.
//
// Notes about this implementation:
// - It collects *all* completions under the prefix via DFS, then sorts and truncates.
//   For very large subtrees, a top-K heap could reduce memory/time, but this is simpler.

const int ALPHABET_SIZE = 26; // Number of supported lowercase English letters

// Root node of the trie; does not correspond to any letter
Node root = new();

/*
 * Convert a lowercase character into an index [0..25].
 *
 * Parameters:
 * - c: expected to be 'a'..'z'
 *
 * Returns:
 * - 0..25 if valid
 * - -1 if invalid
 */
int Idx(char c) => (c >= 'a' && c <= 'z') ? (c - 'a') : -1;

/*
 * Insert a word into the trie (and bump frequency at the terminal node).
 *
 * Behavior:
 * - Normalizes characters to lowercase (case-insensitive)
 * - Rejects the entire word if any character is not 'a'–'z'
 * - Creates missing nodes lazily
 * - Marks the terminal node as end-of-word and increments its Frequency
 *
 * Parameters:
 * - word: string to insert
 *
 * Frequency meaning:
 * - Frequency counts how many times this exact word has been inserted (across both files).
 */
void Insert(string word)
{
    var cur = root;

    // Traverse each character in the word
    foreach (char raw in word)
    {
        // Normalize to lowercase for consistent indexing
        char ch = char.ToLowerInvariant(raw);

        // Map to child index
        int i = Idx(ch);

        // Reject entire word if invalid character encountered
        if (i < 0) return;

        // Create child node if missing (null-coalescing assignment)
        cur.Children[i] ??= new Node();

        // Advance in the trie
        cur = cur.Children[i];
    }

    // Mark terminal node and bump frequency
    cur.IsEnd = true;
    cur.Frequency++;
}

/*
 * Load a file of words, inserting each non-empty trimmed line into the trie.
 *
 * Parameters:
 * - path: filesystem path to read
 *
 * Notes:
 * - File.ReadLines() streams lines lazily (better for large files).
 * - This function does not catch exceptions; if the file doesn't exist or is unreadable,
 *   the script will throw.
 * - Insert() will reject lines containing invalid characters.
 */
void LoadFile(string path)
{
    foreach (var line in File.ReadLines(path))
    {
        var w = line.Trim();
        if (w.Length == 0) continue;
        Insert(w);
    }
}

/*
 * Return ranked autocomplete suggestions for a prefix.
 *
 * Parameters:
 * - prefix: prefix to autocomplete
 * - limit: maximum number of suggestions to return
 *
 * Returns:
 * - List of (word, freq) tuples, sorted best-first
 *
 * Steps:
 * 1) Walk from root to the node corresponding to `prefix`.
 * 2) DFS from that node collecting all terminal words and their frequencies.
 * 3) Sort results by frequency descending, then word ascending (ordinal).
 * 4) Truncate list to `limit`.
 *
 * Important nuance:
 * - This is "collect all then sort"; it is simple but can be heavy if the prefix
 *   matches many words.
 */
List<(string word, int freq)> AutocompleteRanked(string prefix, int limit)
{
    // Treat non-positive limits as "no results"
    if (limit <= 0) return [];

    // ---- Walk to prefix node ----
    var cur = root;
    foreach (char raw in prefix)
    {
        char ch = char.ToLowerInvariant(raw);
        int i = Idx(ch);

        // Invalid prefix char or missing path -> no suggestions
        if (i < 0 || cur.Children[i] == null) return new();

        cur = cur.Children[i];
    }

    // Collect all completions beneath the prefix node
    var results = new List<(string word, int freq)>();

    // Dynamic buffer used during DFS to build words.
    // Using a List<char> avoids fixed-size buffer overflow concerns.
    var buf = new List<char>(prefix.Length + 32);

    // Seed the buffer with the lowercased prefix
    foreach (char raw in prefix) buf.Add(char.ToLowerInvariant(raw));

    /*
     * DFS traversal that collects every complete word under `node`.
     *
     * Implementation details:
     * - When a node is terminal (IsEnd), convert buffer to a string and store with Frequency.
     * - For each child, append the corresponding letter, recurse, then remove it (backtrack).
     *
     * Note:
     * - This DFS does NOT enforce the `limit` during traversal; it collects everything,
     *   then sorting/truncation happens afterward.
     */
    void Dfs(Node node)
    {
        if (node == null) return;

        // If node ends a word, emit current buffer as a candidate suggestion
        if (node.IsEnd)
            results.Add((new string(buf.ToArray()), node.Frequency));

        // Explore children in lexicographic order ('a'..'z')
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            var child = node.Children[i];
            if (child != null)
            {
                // Append next character
                buf.Add((char)('a' + i));

                // Recurse into child subtree
                Dfs(child);

                // Backtrack: remove appended character
                buf.RemoveAt(buf.Count - 1);
            }
        }
    }

    // Start DFS from the prefix node we walked to
    Dfs(cur);

    // ---- Sort best-first ----
    results.Sort((a, b) =>
    {
        int freqCmp = b.freq.CompareTo(a.freq); // frequency desc
        if (freqCmp != 0) return freqCmp;
        return string.CompareOrdinal(a.word, b.word); // word asc
    });

    // ---- Truncate to `limit` ----
    if (results.Count > limit) results.RemoveRange(limit, results.Count - limit);
    return results;
}

// -------------------- Main --------------------
// Defaults:
//
// Args[0] = dictPath (baseline dictionary words)
// Args[1] = freqPath (frequency/usage events; each line bumps Frequency)
// Args[2] = prefix
// Args[3] = limit
string dictPath = (Args.Count > 0) ? Args[0] : @"..\data\words.txt";
string freqPath = (Args.Count > 1) ? Args[1] : @"..\data\frequency.txt";
string prefix   = (Args.Count > 2) ? Args[2] : "th";
int limit        = (Args.Count > 3 && int.TryParse(Args[3], out int v) && v > 0) ? v : 20;

// Load baseline dictionary then apply frequency/usage events
LoadFile(dictPath);
LoadFile(freqPath);

// Print ranked autocomplete results
Console.WriteLine($"Ranked Autocomplete(\"{prefix}\") [limit={limit}]");
foreach (var (word, freq) in AutocompleteRanked(prefix, limit))
    Console.WriteLine($"{word}\t(freq={freq})");

Console.WriteLine();

// Helpful diagnostics for debugging relative-path resolution
Console.WriteLine($"Working directory = {Directory.GetCurrentDirectory()}");
Console.WriteLine($"Resolved dictionary path = {Path.GetFullPath(dictPath)}");
Console.WriteLine($"Resolved frequency path  = {Path.GetFullPath(freqPath)}");

/*
 * Trie node definition used by the ranked autocomplete trie.
 *
 * Each node contains:
 * - Children: 26 child references (a–z)
 * - Frequency: how many times a complete word ending here has been inserted
 * - IsEnd: whether a complete word ends at this node
 *
 * Notes:
 * - Frequency is only meaningful when IsEnd is true.
 * - Non-terminal nodes may have Frequency==0.
 */
class Node
{
    public Node[] Children = new Node[ALPHABET_SIZE]; // Child pointers
    public int Frequency = 0;                         // Word frequency counter (terminal nodes)
    public bool IsEnd = false;                        // End-of-word marker
}
