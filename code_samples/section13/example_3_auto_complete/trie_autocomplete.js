/**
 * Trie Autocomplete (Dictionary-backed)
 *
 * This Node.js script demonstrates a trie (prefix tree) used for autocomplete.
 *
 * Defaults / usage:
 * - Default dictionary path: ..\data\words.txt (Windows-style relative path)
 * - Usage: node trie_autocomplete_dict.js [dictPath] [prefix] [limit]
 *
 * Key behavior / constraints:
 * - The trie only accepts letters 'a'–'z' (case-insensitive).
 * - If any character in a word is not a–z:
 *   - insert() rejects the entire word
 * - If any character in a prefix is not a–z:
 *   - _walk() returns null and autocomplete() returns an empty array
 *
 * Output ordering:
 * - Suggestions are returned in lexicographic order because DFS visits children
 *   in 'a'..'z' order.
 *
 * Path notes:
 * - Relative paths are resolved from the directory you run `node` in.
 */

const fs = require("fs");     // File system API (read dictionary file)
const path = require("path"); // Path utilities (resolve relative paths)

/*
 * Trie node definition.
 *
 * Each node contains:
 * - children: array of 26 child references (one for each letter 'a'–'z')
 * - isEndOfWord: indicates whether a complete word ends at this node
 */
class TrieNode {
  constructor() {
    this.children = new Array(26).fill(null); // Child pointers
    this.isEndOfWord = false;                 // End-of-word marker
  }
}

/*
 * Trie (prefix tree) implementation supporting insertion and autocomplete.
 */
class Trie {
  constructor() {
    // Root node (does not correspond to a character)
    this.root = new TrieNode();
  }

  /*
   * Convert a lowercase letter into a child index [0..25].
   *
   * Parameters:
   * - ch: expected to be a single character string between 'a' and 'z'
   *
   * Returns:
   * - 0..25 if valid
   * - -1 if invalid
   */
  static _index(ch) {
    if (ch < "a" || ch > "z") return -1;
    return ch.charCodeAt(0) - "a".charCodeAt(0);
  }

  /*
   * Insert a word into the trie.
   *
   * Behavior:
   * - Normalizes to lowercase (case-insensitive)
   * - Rejects entire word if any character is not 'a'–'z'
   * - Creates nodes lazily as needed
   * - Marks the final node as end-of-word
   *
   * Parameters:
   * - word: string to insert
   */
  insert(word) {
    let current = this.root;

    // Traverse each character in the word
    for (let i = 0; i < word.length; i++) {
      const c = word[i].toLowerCase();
      const idx = Trie._index(c);

      // Reject entire word if invalid character is encountered
      if (idx < 0) return;

      // Allocate child node if missing
      if (!current.children[idx]) current.children[idx] = new TrieNode();

      // Advance in the trie
      current = current.children[idx];
    }

    // Mark final node as representing a complete word
    current.isEndOfWord = true;
  }

  /*
   * Walk the trie according to a prefix and return the final node.
   *
   * Behavior:
   * - Normalizes to lowercase
   * - Returns null if prefix contains invalid characters or missing path
   *
   * Parameters:
   * - prefix: prefix to traverse
   *
   * Returns:
   * - TrieNode at the end of the prefix, or null if not found/invalid
   */
  _walk(prefix) {
    let current = this.root;

    // Traverse each character in the prefix
    for (let i = 0; i < prefix.length; i++) {
      const c = prefix[i].toLowerCase();
      const idx = Trie._index(c);

      // Invalid character -> prefix not supported
      if (idx < 0) return null;

      // Missing child -> prefix not present in trie
      if (!current.children[idx]) return null;

      // Advance in the trie
      current = current.children[idx];
    }

    return current;
  }

  /*
   * Return up to `limit` autocomplete suggestions for `prefix`.
   *
   * Steps:
   * 1) Find the trie node for the prefix via _walk().
   * 2) Seed a buffer with the normalized (lowercased) prefix.
   * 3) DFS over the subtree, appending letters to the buffer.
   * 4) Whenever an end-of-word node is found, emit the current buffer as a suggestion.
   *
   * Parameters:
   * - prefix: prefix to autocomplete
   * - limit: maximum number of suggestions to return
   *
   * Returns:
   * - Array of suggestion strings (possibly empty)
   */
  autocomplete(prefix, limit) {
    const start = this._walk(prefix);

    // If prefix is invalid or missing, return empty list
    if (!start) return [];

    // Seed buffer with the lowercased prefix
    const seed = prefix.toLowerCase();

    // Collected results
    const out = [];

    // Buffer represented as an array of characters for efficient push/pop
    const buf = seed.split("");

    /*
     * DFS helper that explores the trie subtree and collects words.
     *
     * Implementation notes:
     * - Uses closure over `out` and `buf` to avoid threading state everywhere.
     * - Stops early once `out.length` reaches `limit`.
     * - Visits children in 'a'..'z' order for lexicographic output.
     */
    const dfs = (node) => {
      if (!node || out.length >= limit) return;

      // If current node ends a word, add current buffer as a suggestion
      if (node.isEndOfWord) {
        out.push(buf.join(""));
        if (out.length >= limit) return;
      }

      // Explore children in lexicographic order
      for (let i = 0; i < 26; i++) {
        const child = node.children[i];
        if (child) {
          // Append next letter to buffer
          buf.push(String.fromCharCode("a".charCodeAt(0) + i));

          // Recurse into child subtree
          dfs(child);

          // Backtrack to restore buffer for next sibling
          buf.pop();

          // Stop early if we've reached the limit
          if (out.length >= limit) return;
        }
      }
    };

    // Start DFS from the prefix node
    dfs(start);
    return out;
  }
}

/*
 * Load a dictionary file into the trie.
 *
 * Parameters:
 * - trie: Trie instance to populate
 * - dictPath: filesystem path to dictionary file
 *
 * Returns:
 * - number of non-empty trimmed lines processed (insert attempted for each)
 * - 0 if file cannot be opened/read
 *
 * Notes:
 * - insert() may reject words with invalid characters, but this loader still
 *   increments count for each non-empty trimmed line (as written).
 */
function loadDictionary(trie, dictPath) {
  let text;

  try {
    // Read entire file as UTF-8 text
    text = fs.readFileSync(dictPath, "utf8");
  } catch (err) {
    // Report file access / I/O errors
    console.error(`Failed to open dictionary: ${dictPath}`);
    console.error(String(err.message || err));
    return 0;
  }

  // Split into lines (handles both Unix \n and Windows \r\n)
  const lines = text.split(/\r?\n/);

  let count = 0;

  // Insert each non-empty line into trie
  for (const line of lines) {
    const w = line.trim();
    if (!w) continue;

    trie.insert(w);
    count++;
  }

  return count;
}

/*
 * Parse a numeric limit or return a default.
 *
 * Parameters:
 * - s: string input (possibly undefined)
 * - def: default value if parsing fails or yields a non-positive number
 *
 * Returns:
 * - positive integer if valid
 * - def otherwise
 */
function parseLimit(s, def) {
  const v = Number.parseInt(s, 10);
  return Number.isFinite(v) && v > 0 ? v : def;
}

/*
 * Program entry point.
 *
 * CLI arguments:
 * - argv[2] (optional): dictPath
 * - argv[3] (optional): prefix
 * - argv[4] (optional): limit
 *
 * Demonstrates:
 * - Loading dictionary words into a trie
 * - Running autocomplete for a prefix
 * - Printing results and diagnostic path information
 */
function main() {
  const trie = new Trie();

  // Read CLI args with fallbacks to lesson defaults
  const dictPath = process.argv[2] || "..\\data\\words.txt";
  const prefix = process.argv[3] || "ab";
  const limit = process.argv[4] ? parseLimit(process.argv[4], 20) : 20;

  // Load dictionary
  const loaded = loadDictionary(trie, dictPath);
  console.log(`Loaded ${loaded} words from ${dictPath}\n`);

  // Autocomplete and print results
  console.log(`Autocomplete("${prefix}") [limit=${limit}]`);
  for (const w of trie.autocomplete(prefix, limit)) console.log(w);

  // Helpful diagnostics for debugging relative path resolution
  console.log(`\nWorking directory = ${process.cwd()}`);
  console.log(`Resolved dictionary path = ${path.resolve(dictPath)}`);
}

// Invoke the program
main();
