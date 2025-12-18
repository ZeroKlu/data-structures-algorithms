"""Prefix counting using a trie."""

# trie_prefix_count.py
# Trie Prefix Counting (Dictionary-backed)

import os
import sys

ALPHABET_SIZE = 26

#pylint: disable=too-few-public-methods

class TrieNode:
    """Trie node."""
    __slots__ = ("children", "prefix_count")

    def __init__(self):
        self.children = [None] * ALPHABET_SIZE
        self.prefix_count = 0


class Trie:
    """Trie."""
    def __init__(self):
        self.root = TrieNode()

    @staticmethod
    def _index(c: str) -> int:
        return ord(c) - ord("a") if "a" <= c <= "z" else -1

    # Insert a word; increments prefix_count along the path.
    # Rejects the entire word if any char is not a-z.
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
            current.prefix_count += 1

    # Return how many words start with prefix (0 if invalid or not found).
    def prefix_count(self, prefix: str) -> int:
        """Return how many words start with prefix."""
        current = self.root
        for ch in prefix:
            c = ch.lower()
            idx = self._index(c)
            if idx < 0:
                return 0
            nxt = current.children[idx]
            if nxt is None:
                return 0
            current = nxt
        return current.prefix_count


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

    prefixes = ["a", "ab", "alg", "aard", "z", "nope"]
    for p in prefixes:
        print(f'prefixCount("{p}") = {trie.prefix_count(p)}')

    print("\nWorking directory =", os.getcwd())
    print("Resolved dictionary path =", os.path.abspath(dict_path))


if __name__ == "__main__":
    main()
