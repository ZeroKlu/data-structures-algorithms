"""Autocomplete using a trie."""

# trie_autocomplete.py
# Trie Autocomplete (Dictionary-backed)
# Usage: python trie_autocomplete.py [dictPath] [prefix] [limit]

import os
import sys
from typing import List, Optional

ALPHABET_SIZE = 26

#pylint: disable=too-few-public-methods
#pylint: disable=broad-exception-caught

class TrieNode:
    """Trie node."""
    __slots__ = ("children", "is_end")

    def __init__(self):
        self.children = [None] * ALPHABET_SIZE
        self.is_end = False


class Trie:
    """Trie."""
    def __init__(self):
        self.root = TrieNode()

    @staticmethod
    def _index(c: str) -> int:
        return ord(c) - ord("a") if "a" <= c <= "z" else -1

    def insert(self, word: str) -> None:
        """Insert word into trie."""
        current = self.root
        for ch in word:
            c = ch.lower()
            idx = self._index(c)
            if idx < 0:
                return
            if current.children[idx] is None:
                current.children[idx] = TrieNode()
            current = current.children[idx]
        current.is_end = True

    def _walk(self, prefix: str) -> Optional[TrieNode]:
        current = self.root
        for ch in prefix:
            c = ch.lower()
            idx = self._index(c)
            if idx < 0:
                return None
            nxt = current.children[idx]
            if nxt is None:
                return None
            current = nxt
        return current

    def autocomplete(self, prefix: str, limit: int) -> List[str]:
        """Search prefix in trie."""
        start = self._walk(prefix)
        if start is None:
            return []

        prefix_lc = prefix.lower()
        buf = list(prefix_lc)
        out: List[str] = []

        def dfs(node: TrieNode) -> None:
            if node is None or len(out) >= limit:
                return
            if node.is_end:
                out.append("".join(buf))
                if len(out) >= limit:
                    return
            for i in range(ALPHABET_SIZE):
                child = node.children[i]
                if child is not None:
                    buf.append(chr(ord("a") + i))
                    dfs(child)
                    buf.pop()
                    if len(out) >= limit:
                        return

        dfs(start)
        return out


def load_dictionary(trie: Trie, path: str) -> int:
    """Load dictionary into trie."""
    count = 0
    try:
        with open(path, "r", encoding="utf-8") as f:
            for line in f:
                w = line.strip()
                if not w:
                    continue
                trie.insert(w)
                count += 1
    except OSError as e:
        print(f"Failed to open dictionary: {path}", file=sys.stderr)
        print(str(e), file=sys.stderr)
        return 0
    return count


def parse_int_or_default(s: str, default: int) -> int:
    """Parse int or return default."""
    try:
        v = int(s)
        return v if v > 0 else default
    except Exception:
        return default


def main() -> None:
    """Main."""
    trie = Trie()

    dict_path = sys.argv[1] if len(sys.argv) > 1 else r"..\data\words.txt"
    prefix = sys.argv[2] if len(sys.argv) > 2 else "ab"
    limit = parse_int_or_default(sys.argv[3], 20) if len(sys.argv) > 3 else 20

    loaded = load_dictionary(trie, dict_path)
    print(f"Loaded {loaded} words from {dict_path}\n")

    print(f'Autocomplete("{prefix}") [limit={limit}]')
    for w in trie.autocomplete(prefix, limit):
        print(w)

    print("\nWorking directory =", os.getcwd())
    print("Resolved dictionary path =", os.path.abspath(dict_path))


if __name__ == "__main__":
    main()
