"""Search a dictionary of words in a trie."""

# trie_insert_search.py
# Trie Insert + Search (Dictionary-backed)
# Loads words from ..\data\words.txt by default (Windows-style relative path)

from typing import Optional
import os
import sys

#pylint: disable=too-few-public-methods

ALPHABET_SIZE = 26

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

    # Insert a word; reject the entire word if any char is not a-z
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

    def _walk(self, s: str) -> Optional[TrieNode]:
        current = self.root
        for ch in s:
            c = ch.lower()
            idx = self._index(c)
            if idx < 0:
                return None
            nxt = current.children[idx]
            if nxt is None:
                return None
            current = nxt
        return current

    def search(self, word: str) -> bool:
        """Search word in trie."""
        node = self._walk(word)
        return bool(node and node.is_end)

    def starts_with(self, prefix: str) -> bool:
        """Search prefix in trie."""
        return self._walk(prefix) is not None


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


def main():
    """Main."""
    trie = Trie()

    # Default path matches the lesson convention; can be overridden by CLI arg
    dict_path = sys.argv[1] if len(sys.argv) > 1 else r"..\data\words.txt"
    loaded = load_dictionary(trie, dict_path)

    print(f"Loaded {loaded} words from {dict_path}\n")

    tests = ["aardvark", "abandon", "zebra", "notaword"]
    for w in tests:
        print(f'search("{w}") = {"true" if trie.search(w) else "false"}')

    print("\nPrefix tests:")
    prefixes = ["ab", "alg", "aard", "nope"]
    for p in prefixes:
        print(f'startsWith("{p}") = {"true" if trie.starts_with(p) else "false"}')

    # Helpful diagnostics for relative paths
    print("\nWorking directory =", os.getcwd())
    print("Resolved dictionary path =", os.path.abspath(dict_path))


if __name__ == "__main__":
    main()
