// trie_autocomplete_ranked.js
//
// This Node.js script implements a trie (prefix tree) that supports frequency-ranked
// autocomplete. It loads words from two files:
// - ..\data\words.txt      (baseline dictionary words)
// - ..\data\frequency.txt  (additional occurrences / usage events)
// Each time a word is inserted, the terminal node's frequency counter is incremented.
//
// Ranking rules (as implemented):
// - Higher frequency first
// - If frequencies tie, lexicographically smaller word first
//
// Key constraints:
// - Only letters 'a'–'z' are accepted (case-insensitive).
// - If any character in a word is not a–z, insert() rejects the entire word.
// - If any character in the prefix is not a–z, autocomplete() returns [].
//
// Implementation approach:
// - DFS collects ALL completions under the prefix
// - Sort by frequency desc, then word asc
// - Slice to `limit` results
//
// Note:
// - Because this collects all completions before sorting, it can be memory/time heavy
//   for prefixes with many matches.

const fs = require("fs"); // File system API for reading dictionary/frequency files

/*
 * Node
 *
 * A trie node contains:
 * - children: array of 26 child references (a..z)
 * - freq: frequency count for the complete word ending at this node
 * - end: whether a complete word ends at this node
 *
 * Frequency is meaningful only when end === true.
 */
class Node {
  constructor() {
    this.children = Array(26).fill(null); // Child pointers (indices 0..25)
    this.freq = 0;                        // Word frequency counter (terminal nodes)
    this.end = false;                     // End-of-word marker
  }
}

/*
 * Trie
 *
 * Supports:
 * - insert(word): insert a word and bump its frequency at the terminal node
 * - autocomplete(prefix, limit): return top ranked completions for prefix
 */
class Trie {
  constructor() { this.root = new Node(); }

  /*
   * Convert a lowercase letter to child index [0..25].
   *
   * Parameters:
   * - c: single-character string
   *
   * Returns:
   * - 0..25 if valid 'a'..'z'
   * - -1 if invalid
   */
  idx(c) { return c >= "a" && c <= "z" ? c.charCodeAt(0) - 97 : -1; }

  /*
   * Insert a word into the trie and increment its terminal frequency.
   *
   * Behavior:
   * - Lowercases input (case-insensitive)
   * - Rejects the entire word if any char is not 'a'..'z'
   * - Creates missing nodes lazily
   * - Marks terminal node as end-of-word and increments freq
   *
   * Parameters:
   * - word: word to insert
   */
  insert(word) {
    let cur = this.root;

    // Iterate through lowercased characters
    for (const ch of word.toLowerCase()) {
      const i = this.idx(ch);

      // Reject entire word if invalid character encountered
      if (i < 0) return;

      // Allocate child if missing
      if (!cur.children[i]) cur.children[i] = new Node();

      // Advance to child node
      cur = cur.children[i];
    }

    // Mark complete word and bump frequency
    cur.end = true;
    cur.freq++;
  }

  /*
   * Autocomplete for a given prefix, ranked by frequency then word.
   *
   * Parameters:
   * - prefix: prefix to autocomplete
   * - limit: maximum number of suggestions to return
   *
   * Returns:
   * - Array of [word, freq] pairs, sorted best-first, truncated to `limit`
   *
   * Steps:
   * 1) Walk to the trie node for the prefix
   * 2) DFS the subtree collecting [word, freq] for terminal nodes
   * 3) Sort by frequency desc, then word asc
   * 4) Return first `limit` items
   *
   * Notes:
   * - This DFS does not short-circuit at `limit`; it collects everything, then sorts.
   */
  autocomplete(prefix, limit) {
    let cur = this.root;

    // Walk to the prefix node
    for (const ch of prefix.toLowerCase()) {
      const i = this.idx(ch);

      // Invalid prefix char or missing path => no suggestions
      if (i < 0 || !cur.children[i]) return [];

      cur = cur.children[i];
    }

    // Collected completions: [word, freq]
    const out = [];

    // Mutable character buffer seeded with lowercased prefix
    const buf = prefix.toLowerCase().split("");

    /*
     * DFS traversal collecting terminal words.
     *
     * Parameters:
     * - n: current node
     *
     * Behavior:
     * - If n.end is true, emit current buf as a candidate completion with n.freq
     * - Visit children in 'a'..'z' order (lexicographic traversal)
     * - Use buf push/pop to build words without allocating new strings each step
     */
    const dfs = (n) => {
      // If a complete word ends here, capture it
      if (n.end) out.push([buf.join(""), n.freq]);

      // Explore children in lexicographic order
      for (let i = 0; i < 26; i++) {
        if (n.children[i]) {
          // Append next letter
          buf.push(String.fromCharCode(97 + i));

          // Recurse into child subtree
          dfs(n.children[i]);

          // Backtrack: remove appended letter
          buf.pop();
        }
      }
    };

    // Traverse subtree beneath prefix
    dfs(cur);

    // Sort best-first:
    // - higher frequency first
    // - for ties, lexicographically smaller word first
    out.sort((a, b) => b[1] - a[1] || a[0].localeCompare(b[0]));

    // Return at most `limit` results (slice handles limit > out.length)
    return out.slice(0, limit);
  }
}

/* -------------------- Main script -------------------- */

// Build trie from dictionary + frequency inputs
const trie = new Trie();

/*
 * Load baseline dictionary words.
 *
 * Implementation:
 * - Read entire file as UTF-8 text
 * - Split by line endings (\n or \r\n)
 * - Insert each non-empty line
 *
 * Notes:
 * - Lines are not trimmed; a line with trailing spaces would include them and be rejected
 *   by insert() because spaces are not 'a'..'z'. (No changes requested—this is a note.)
 */
for (const w of fs.readFileSync("..\\data\\words.txt", "utf8").split(/\r?\n/))
  if (w) trie.insert(w);

/*
 * Load frequency/usage events.
 * Each non-empty line is treated as one occurrence of a word, bumping its freq.
 */
for (const w of fs.readFileSync("..\\data\\frequency.txt", "utf8").split(/\r?\n/))
  if (w) trie.insert(w);

/*
 * Query and print ranked autocomplete suggestions.
 *
 * Hard-coded example:
 * - prefix: "th"
 * - limit: 20
 *
 * Output format:
 * - word<TAB>(freq=N)
 */
for (const [w, f] of trie.autocomplete("th", 20))
  console.log(`${w}\t(freq=${f})`);
