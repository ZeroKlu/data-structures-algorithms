/**
 * Trie Insert + Search (Dictionary-backed)
 * Node.js script that loads a dictionary from ..\data\words.txt (Windows-style relative path)
 *
 * Notes:
 * - The trie only accepts letters a-z (case-insensitive). If any character in a word/prefix
 *   is not a-z, insert() rejects that word, and search()/startsWith() return false.
 * - Relative paths are resolved from the directory you run `node` in.
 */

const fs = require("fs");
const path = require("path");

class TrieNode {
  constructor() {
    this.children = new Array(26).fill(null);
    this.isEndOfWord = false;
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

  insert(word) {
    let current = this.root;
    for (let i = 0; i < word.length; i++) {
      const c = word[i].toLowerCase();
      const idx = Trie._index(c);
      if (idx < 0) return; // reject entire word
      if (!current.children[idx]) current.children[idx] = new TrieNode();
      current = current.children[idx];
    }
    current.isEndOfWord = true;
  }

  _walk(s) {
    let current = this.root;
    for (let i = 0; i < s.length; i++) {
      const c = s[i].toLowerCase();
      const idx = Trie._index(c);
      if (idx < 0) return null;
      if (!current.children[idx]) return null;
      current = current.children[idx];
    }
    return current;
  }

  search(word) {
    const node = this._walk(word);
    return !!(node && node.isEndOfWord);
  }

  startsWith(prefix) {
    return this._walk(prefix) !== null;
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

  const tests = ["aardvark", "abandon", "zebra", "notaword"];
  for (const w of tests) {
    console.log(`search("${w}") = ${trie.search(w) ? "true" : "false"}`);
  }

  console.log("\nPrefix tests:");
  const prefixes = ["ab", "alg", "aard", "nope"];
  for (const p of prefixes) {
    console.log(`startsWith("${p}") = ${trie.startsWith(p) ? "true" : "false"}`);
  }

  // Helpful debugging: show where relative paths are resolved from.
  console.log(`\nWorking directory = ${process.cwd()}`);
  console.log(`Resolved dictionary path = ${path.resolve(dictPath)}`);
}

main();
