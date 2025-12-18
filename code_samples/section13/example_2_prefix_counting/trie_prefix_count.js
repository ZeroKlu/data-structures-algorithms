/**
 * Trie Prefix Counting (Dictionary-backed)
 * Node.js script that loads a dictionary from ..\data\words.txt by default.
 *
 * Notes:
 * - The trie only accepts letters a-z (case-insensitive). If any character in a word/prefix
 *   is not a-z, insert() rejects that word, and prefixCount() returns 0.
 * - Relative paths are resolved from the directory you run `node` in.
 */

const fs = require("fs");
const path = require("path");

class TrieNode {
  constructor() {
    this.children = new Array(26).fill(null);
    this.prefixCount = 0; // number of words sharing this prefix
  }
}

class Trie {
  constructor() {
    this.root = new TrieNode();
  }

  static _index(ch) {
    if (ch < "a" || ch > "z") return -1;
    return ch.charCodeAt(0) - "a".charCodeAt(0);
  }

  // Insert a word; increments prefixCount along the path.
  // Rejects the entire word if any char is not a-z.
  insert(word) {
    let current = this.root;
    for (let i = 0; i < word.length; i++) {
      const c = word[i].toLowerCase();
      const idx = Trie._index(c);
      if (idx < 0) return; // reject word
      if (!current.children[idx]) current.children[idx] = new TrieNode();
      current = current.children[idx];
      current.prefixCount++;
    }
  }

  // Return how many words start with prefix (0 if invalid or not found)
  prefixCount(prefix) {
    let current = this.root;
    for (let i = 0; i < prefix.length; i++) {
      const c = prefix[i].toLowerCase();
      const idx = Trie._index(c);
      if (idx < 0) return 0;
      if (!current.children[idx]) return 0;
      current = current.children[idx];
    }
    return current.prefixCount;
  }
}

function loadDictionary(trie, dictPath) {
  let text;
  try {
    text = fs.readFileSync(dictPath, "utf8");
  } catch (err) {
    console.error(`Failed to open dictionary: ${dictPath}`);
    console.error(String(err.message || err));
    return 0;
  }

  const lines = text.split(/\r?\n/);
  let count = 0;
  for (const line of lines) {
    const w = line.trim();
    if (!w) continue;
    trie.insert(w);
    count++;
  }
  return count;
}

function main() {
  const trie = new Trie();

  // Default path matches your lesson convention; can be overridden by CLI arg.
  const dictPath = process.argv[2] || "..\\data\\words.txt";
  const loaded = loadDictionary(trie, dictPath);

  console.log(`Loaded ${loaded} words from ${dictPath}\n`);

  const prefixes = ["a", "ab", "alg", "aard", "z", "nope"];
  for (const p of prefixes) {
    console.log(`prefixCount("${p}") = ${trie.prefixCount(p)}`);
  }

  console.log(`\nWorking directory = ${process.cwd()}`);
  console.log(`Resolved dictionary path = ${path.resolve(dictPath)}`);
}

main();
