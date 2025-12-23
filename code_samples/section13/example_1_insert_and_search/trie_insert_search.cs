// Trie Insert + Search (top-level, dotnet-script friendly)
//
// This file implements a simple trie (prefix tree) for lowercase
// English words using top-level statements compatible with dotnet-script.

const int ALPHABET_SIZE = 26; // Number of supported lowercase letters ('a'–'z')

// Root node of the trie
TrieNode root = new();

/*
 * Maps a lowercase character to an array index.
 *
 * Parameters:
 * - c: character expected to be between 'a' and 'z'
 *
 * Returns:
 * - Index in range [0, 25] if valid
 * - -1 if the character is outside the supported range
 */
int Index(char c) => (c >= 'a' && c <= 'z') ? (c - 'a') : -1;

/*
 * Inserts a word into the trie.
 *
 * Behavior:
 * - Converts all characters to lowercase
 * - Rejects the entire word if any character is not 'a'–'z'
 * - Creates trie nodes lazily as needed
 *
 * Parameters:
 * - word: string to insert into the trie
 */
void Insert(string word)
{
    TrieNode current = root;

    // Traverse each character in the word
    foreach (char raw in word)
    {
        // Normalize character to lowercase using invariant culture
        char c = char.ToLowerInvariant(raw);

        // Convert character to child index
        int idx = Index(c);

        // Abort insertion if an invalid character is encountered
        if (idx < 0) return;

        // Create a new child node if it does not exist
        if (current.Children[idx] == null)
            current.Children[idx] = new TrieNode();

        // Move to the next node in the trie
        current = current.Children[idx];
    }

    // Mark the final node as representing a complete word
    current.IsEndOfWord = true;
}

/*
 * Traverses the trie following the given string.
 *
 * Used internally by Search() and StartsWith().
 *
 * Parameters:
 * - s: word or prefix to traverse
 *
 * Returns:
 * - The final TrieNode reached if traversal succeeds
 * - null if traversal fails due to invalid characters or missing nodes
 */
TrieNode Walk(string s)
{
    TrieNode current = root;

    // Traverse character by character
    foreach (char raw in s)
    {
        char c = char.ToLowerInvariant(raw);
        int idx = Index(c);

        // Fail if character is invalid
        if (idx < 0) return null;

        // Fail if required child node does not exist
        if (current.Children[idx] == null) return null;

        current = current.Children[idx];
    }

    return current;
}

/*
 * Searches for a complete word in the trie.
 *
 * Parameters:
 * - word: word to search for
 *
 * Returns:
 * - true if the word exists and is marked as complete
 * - false otherwise
 */
bool Search(string word)
{
    var node = Walk(word);
    return node != null && node.IsEndOfWord;
}

/*
 * Checks whether any word in the trie starts with the given prefix.
 *
 * Parameters:
 * - prefix: prefix string to test
 *
 * Returns:
 * - true if the prefix exists in the trie
 * - false otherwise
 */
bool StartsWith(string prefix) => Walk(prefix) != null;

/*
 * Loads words from a dictionary file into the trie.
 *
 * Parameters:
 * - path: filesystem path to the dictionary file
 *
 * Returns:
 * - Number of words successfully loaded
 * - 0 if the file could not be opened or read
 */
int LoadDictionary(string path)
{
    int count = 0;

    try
    {
        // Read file lazily line by line
        foreach (var line in File.ReadLines(path))
        {
            // Trim whitespace and skip empty lines
            var w = line.Trim();
            if (w.Length == 0) continue;

            // Insert word into the trie
            Insert(w);
            count++;
        }
    }
    catch (Exception ex)
    {
        // Report file access or I/O errors
        Console.Error.WriteLine($"Failed to open dictionary: {path}");
        Console.Error.WriteLine(ex.Message);
        return 0;
    }

    return count;
}

// -------------------- Main --------------------

/*
 * Resolve dictionary path.
 * If an argument is provided, use it; otherwise fall back to default path.
 */
string dictPath = (Args.Count > 0) ? Args[0] : @"..\data\words.txt";

// Load dictionary words into the trie
int loaded = LoadDictionary(dictPath);

Console.WriteLine($"Loaded {loaded} words from {dictPath}");
Console.WriteLine();

// Words to test for exact search matches
string[] tests = ["aardvark", "abandon", "zebra", "notaword"];
foreach (var w in tests)
    Console.WriteLine($"search(\"{w}\") = {(Search(w) ? "true" : "false")}");

Console.WriteLine();
Console.WriteLine("Prefix tests:");

// Prefixes to test
string[] prefixes = ["ab", "alg", "aard", "nope"];
foreach (var p in prefixes)
    Console.WriteLine($"startsWith(\"{p}\") = {(StartsWith(p) ? "true" : "false")}");

Console.WriteLine();

// Output environment information useful for debugging file paths
Console.WriteLine($"Working directory = {Directory.GetCurrentDirectory()}");
Console.WriteLine($"Resolved dictionary path = {Path.GetFullPath(dictPath)}");

/*
 * Trie node definition.
 *
 * Each node contains:
 * - An array of child references (one per letter 'a'–'z')
 * - A flag indicating whether this node terminates a valid word
 */
class TrieNode
{
    public TrieNode[] Children = new TrieNode[ALPHABET_SIZE]; // Child nodes
    public bool IsEndOfWord = false;                           // End-of-word marker
}
