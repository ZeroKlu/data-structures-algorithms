/**
 * Trie Autocomplete (Dictionary-backed)
 * Default dictionary path: ..\data\words.txt (override via CLI arg 1)
 * Usage: node trie_autocomplete_dict.js [dictPath] [prefix] [limit]
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
      if (idx < 0) return;
      if (!current.children[idx]) current.children[idx] = new TrieNode();
      current = current.children[idx];
    }
    current.isEndOfWord = true;
  }

  _walk(prefix) {
    let current = this.root;
    for (let i = 0; i < prefix.length; i++) {
      const c = prefix[i].toLowerCase();
      const idx = Trie._index(c);
      if (idx < 0) return null;
      if (!current.children[idx]) return null;
      current = current.children[idx];
    }
    return current;
  }

  autocomplete(prefix, limit) {
    const start = this._walk(prefix);
    if (!start) return [];

    const seed = prefix.toLowerCase();
    const out = [];
    const buf = seed.split("");

    const dfs = (node) => {
      if (!node || out.length >= limit) return;
      if (node.isEndOfWord) {
        out.push(buf.join(""));
        if (out.length >= limit) return;
      }
      for (let i = 0; i < 26; i++) {
        const child = node.children[i];
        if (child) {
          buf.push(String.fromCharCode("a".charCodeAt(0) + i));
          dfs(child);
          buf.pop();
          if (out.length >= limit) return;
        }
      }
    };

    dfs(start);
    return out;
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

function parseLimit(s, def) {
  const v = Number.parseInt(s, 10);
  return Number.isFinite(v) && v > 0 ? v : def;
}

function main() {
  const trie = new Trie();

  const dictPath = process.argv[2] || "..\\data\\words.txt";
  const prefix = process.argv[3] || "ab";
  const limit = process.argv[4] ? parseLimit(process.argv[4], 20) : 20;

  const loaded = loadDictionary(trie, dictPath);
  console.log(`Loaded ${loaded} words from ${dictPath}\n`);

  console.log(`Autocomplete("${prefix}") [limit=${limit}]`);
  for (const w of trie.autocomplete(prefix, limit)) console.log(w);

  console.log(`\nWorking directory = ${process.cwd()}`);
  console.log(`Resolved dictionary path = ${path.resolve(dictPath)}`);
}

main();
