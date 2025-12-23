/**
 * Trie Insert + Search (Dictionary-backed)
 *
 * Node.js script demonstrating a trie (prefix tree) that loads words
 * from a dictionary file and supports:
 * - Case-insensitive word insertion
 * - Exact word search
 * - Prefix search
 *
 * Dictionary path defaults to ..\data\words.txt (Windows-style relative path).
 *
 * Notes:
 * - The trie only accepts letters a–z (case-insensitive).
 *   If any character in a word or prefix is not a–z:
 *     - insert() rejects the entire word
 *     - search() and startsWith() return false
 * - Relative paths are resolved from the directory where `node` is executed.
 */

const fs = require("fs");     // File system access (reading dictionary file)
const path = require("path"); // Path resolution utilities

/*
 * Trie node definition.
 *
 * Each node contains:
 * - An array of 26 child references (one for each letter 'a'–'z')
 * - A flag indicating whether this node terminates a valid word
 */
class TrieNode {
  constructor() {
    this.children = new Array(26).fill(null); // Child nodes
    this.isEndOfWord = false;                 // End-of-word marker
  }
}

/*
 * Trie (prefix tree) implementation.
 *
 * Supports insertion, exact search, and prefix search.
 */
class Trie {
  constructor() {
    // Root node of the trie; always exists
    this.root = new TrieNode();
  }

  /*
   * Converts a lowercase character into a trie array index.
   *
   * Parameters:
   * - ch: character expected to be between 'a' and 'z'
   *
   * Returns:
   * - Index in range [0, 25] if valid
   * - -1 if the character is outside the supported range
   */
  static _index(ch) {
    if (ch < "a" || ch > "z") return -1;
    return ch.charCodeAt(0) - "a".charCodeAt(0);
  }

  /*
   * Inserts a word into the trie.
   *
   * Behavior:
   * - Converts characters to lowercase
   * - Rejects the entire word if any character is not 'a'–'z'
   * - Creates nodes lazily as needed
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

      // Abort insertion if an invalid character is encountered
      if (idx < 0) return; // reject entire word

      // Allocate a new child node if it does not exist
      if (!current.children[idx]) current.children[idx] = new TrieNode();

      // Move to the next node in the trie
      current = current.children[idx];
    }

    // Mark the final node as representing a complete word
    current.isEndOfWord = true;
  }

  /*
   * Traverses the trie following the given word or prefix.
   *
   * Used internally by search() and startsWith().
   *
   * Parameters:
   * - s: word or prefix to traverse
   *
   * Returns:
   * - The final TrieNode reached if traversal succeeds
   * - null if traversal fails due to invalid characters or missing nodes
   */
  _walk(s) {
    let current = this.root;

    // Traverse character by character
    for (let i = 0; i < s.length; i++) {
      const c = s[i].toLowerCase();
      const idx = Trie._index(c);

      // Fail if character is invalid
      if (idx < 0) return null;

      // Fail if required child node does not exist
      if (!current.children[idx]) return null;

      current = current.children[idx];
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
  search(word) {
    const node = this._walk(word);
    return !!(node && node.isEndOfWord);
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
  startsWith(prefix) {
    return this._walk(prefix) !== null;
  }
}

/*
 * Loads words from a dictionary file into the trie.
 *
 * Parameters:
 * - trie: Trie instance to populate
 * - dictPath: filesystem path to the dictionary file
 *
 * Returns:
 * - Number of words successfully loaded
 * - 0 if the file could not be opened or read
 */
function loadDictionary(trie, dictPath) {
  let text;

  try {
    // Read entire dictionary file as UTF-8 text
    text = fs.readFileSync(dictPath, "utf8");
  } catch (err) {
    // Report file access or I/O errors
    console.error(`Failed to open dictionary: ${dictPath}`);
    console.error(String(err.message || err));
    return 0;
  }

  // Split text into lines, handling both Unix and Windows line endings
  const lines = text.split(/\r?\n/);

  let count = 0;

  // Process each line as a potential word
  for (const line of lines) {
    const w = line.trim();

    // Skip empty lines
    if (!w) continue;

    // Insert word into the trie
    trie.insert(w);
    count++;
  }

  return count;
}

/*
 * Program entry point.
 *
 * Demonstrates:
 * - Trie construction
 * - Dictionary loading
 * - Exact word searching
 * - Prefix searching
 * - Helpful debugging output for path resolution
 */
function main() {
  // Create an empty trie
  const trie = new Trie();

  /*
   * Resolve dictionary path.
   * If a CLI argument is provided, use it; otherwise fall back to default path.
   */
  const dictPath = process.argv[2] || "..\\data\\words.txt";

  // Load dictionary words into the trie
  const loaded = loadDictionary(trie, dictPath);

  console.log(`Loaded ${loaded} words from ${dictPath}\n`);

  // Words to test for exact matches
  const tests = ["aardvark", "abandon", "zebra", "notaword"];
  for (const w of tests) {
    console.log(`search("${w}") = ${trie.search(w) ? "true" : "false"}`);
  }

  // Prefix tests
  console.log("\nPrefix tests:");
  const prefixes = ["ab", "alg", "aard", "nope"];
  for (const p of prefixes) {
    console.log(`startsWith("${p}") = ${trie.startsWith(p) ? "true" : "false"}`);
  }

  /*
   * Helpful debugging output:
   * Shows where relative paths are resolved from at runtime.
   */
  console.log(`\nWorking directory = ${process.cwd()}`);
  console.log(`Resolved dictionary path = ${path.resolve(dictPath)}`);
}

// Invoke the program
main();
