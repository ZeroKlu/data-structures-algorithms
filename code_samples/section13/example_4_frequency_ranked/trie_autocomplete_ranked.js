// trie_autocomplete_ranked.js
const fs = require("fs");

class Node {
  constructor() {
    this.children = Array(26).fill(null);
    this.freq = 0;
    this.end = false;
  }
}

class Trie {
  constructor() { this.root = new Node(); }

  idx(c) { return c >= "a" && c <= "z" ? c.charCodeAt(0) - 97 : -1; }

  insert(word) {
    let cur = this.root;
    for (const ch of word.toLowerCase()) {
      const i = this.idx(ch);
      if (i < 0) return;
      if (!cur.children[i]) cur.children[i] = new Node();
      cur = cur.children[i];
    }
    cur.end = true;
    cur.freq++;
  }

  autocomplete(prefix, limit) {
    let cur = this.root;
    for (const ch of prefix.toLowerCase()) {
      const i = this.idx(ch);
      if (i < 0 || !cur.children[i]) return [];
      cur = cur.children[i];
    }

    const out = [];
    const buf = prefix.toLowerCase().split("");

    const dfs = (n) => {
      if (n.end) out.push([buf.join(""), n.freq]);
      for (let i = 0; i < 26; i++) {
        if (n.children[i]) {
          buf.push(String.fromCharCode(97+i));
          dfs(n.children[i]);
          buf.pop();
        }
      }
    };

    dfs(cur);
    out.sort((a,b) => b[1]-a[1] || a[0].localeCompare(b[0]));
    return out.slice(0, limit);
  }
}

const trie = new Trie();
for (const w of fs.readFileSync("..\\data\\words.txt","utf8").split(/\r?\n/))
  if (w) trie.insert(w);
for (const w of fs.readFileSync("..\\data\\frequency.txt","utf8").split(/\r?\n/))
  if (w) trie.insert(w);

for (const [w,f] of trie.autocomplete("th",20))
  console.log(`${w}\t(freq=${f})`);
