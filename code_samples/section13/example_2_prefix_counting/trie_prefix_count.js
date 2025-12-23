/**
 * Trie Prefix Counting (Dictionary-backed)
 *
 * Node.js script demonstrating a trie (prefix tree) focused on prefix counting.
 * It loads a dictionary from "..\data\words.txt" by default (Windows-style relative path).
 *
 * Notes:
 * - The trie only accepts letters a–z (case-insensitive).
 *   If any character in a word or prefix is not a–z:
 *     - insert() rejects that word (no nodes/prefix counts are added)
 *     - prefixCount() returns 0 (treats the prefix as invalid / not found)
 * - Relative paths are resolved from the directory where `node` is executed.
 */

const fs = require("fs");     // File system access (read dictionary file)
const path = require("path"); // Path utilities (resolve relative dictionary paths)

/*
 * Trie node definition.
 *
 * Each node contains:
 * - children: array of 26 child references (one for each letter 'a'–'z')
 * - prefixCount: number of inserted words that share this prefix node
 *
 * In this prefix-counting variant, we do not store an end-of-word flag
 * because queries only require counts by prefix.
 */
class TrieNode {
  constructor() {
    // Child pointers; null indicates "no child for that character yet"
    this.children = new Array(26).fill(null);

    // Number of words whose insertion path includes this node
    this.prefixCount = 0; // number of words sharing this prefix
  }
}

/*
 * Trie (prefix tree) implementation for prefix counting.
 *
 * Supports:
 * - insert(word): adds a word and increments prefixCount along its path
 * - prefixCount(prefix): returns number of inserted words sharing that prefix
 */
class Trie {
  constructor() {
    // Root node (does not correspond to a character)
    this.root = new TrieNode();
  }

  /*
   * Convert a lowercase character to a child-array index [0, 25].
   *
   * Parameters:
   * - ch: expected to be a single character string between 'a' and 'z'
   *
   * Returns:
   * - 0..25 for valid 'a'–'z'
   * - -1 for invalid characters
   */
  static _index(ch) {
    if (ch < "a" || ch > "z") return -1;
    return ch.charCodeAt(0) - "a".charCodeAt(0);
  }

  // Insert a word; increments prefixCount along the path.
  // Rejects the entire word if any char is not a-z.
  insert(word) {
    let current = this.root;

    // Traverse each character in the word
    for (let i = 0; i < word.length; i++) {
      // Normalize to lowercase for case-insensitive behavior
      const c = word[i].toLowerCase();

      // Map character to 0..25 index
      const idx = Trie._index(c);

      // Reject entire word if any invalid character is encountered
      if (idx < 0) return; // reject word

      // Allocate child node if it doesn't exist yet
      if (!current.children[idx]) current.children[idx] = new TrieNode();

      // Move to the next node in the trie
      current = current.children[idx];

      // Increment count of words that share this prefix node
      current.prefixCount++;
    }
  }

  // Return how many words start with prefix (0 if invalid or not found)
  prefixCount(prefix) {
    let current = this.root;

    // Walk the trie according to the prefix
    for (let i = 0; i < prefix.length; i++) {
      const c = prefix[i].toLowerCase();
      const idx = Trie._index(c);

      // Invalid character -> prefix treated as invalid / not found
      if (idx < 0) return 0;

      // Missing path -> no inserted words share this prefix
      if (!current.children[idx]) return 0;

      // Advance to next node
      current = current.children[idx];
    }

    // prefixCount at this node equals number of words sharing this prefix
    return current.prefixCount;
  }
}

/*
 * Load words from a dictionary file into the trie.
 *
 * Parameters:
 * - trie: Trie instance to populate
 * - dictPath: filesystem path to the dictionary file
 *
 * Returns:
 * - number of non-empty trimmed lines processed (insert attempted for each)
 * - 0 if the file cannot be opened/read
 *
 * Notes:
 * - insert() may reject words containing invalid characters, but this loader
 *   still increments count for every non-empty trimmed line (as written).
 */
function loadDictionary(trie, dictPath) {
  let text;

  try {
    // Read entire file as UTF-8
    text = fs.readFileSync(dictPath, "utf8");
  } catch (err) {
    // Report file access / I/O errors
    console.error(`Failed to open dictionary: ${dictPath}`);
    console.error(String(err.message || err));
    return 0;
  }

  // Split into lines, handling both Unix and Windows line endings
  const lines = text.split(/\r?\n/);

  let count = 0;

  // Process each line as a candidate word
  for (const line of lines) {
    const w = line.trim();

    // Skip empty lines
    if (!w) continue;

    // Insert into trie (may be rejected if it contains invalid chars)
    trie.insert(w);
    count++;
  }

  return count;
}

/*
 * Program entry point.
 *
 * Demonstrates:
 * - Trie creation
 * - Dictionary loading
 * - prefixCount queries for sample prefixes
 * - Diagnostic output for working directory and resolved dictionary path
 */
function main() {
  const trie = new Trie();

  // Default path matches your lesson convention; can be overridden by CLI arg.
  const dictPath = process.argv[2] || "..\\data\\words.txt";

  // Load dictionary words into the trie
  const loaded = loadDictionary(trie, dictPath);

  console.log(`Loaded ${loaded} words from ${dictPath}\n`);

  // Prefixes to test
  const prefixes = ["a", "ab", "alg", "aard", "z", "nope"];
  for (const p of prefixes) {
    console.log(`prefixCount("${p}") = ${trie.prefixCount(p)}`);
  }

  // Helpful diagnostics for debugging relative-path resolution
  console.log(`\nWorking directory = ${process.cwd()}`);
  console.log(`Resolved dictionary path = ${path.resolve(dictPath)}`);
}

// Invoke the program
main();
