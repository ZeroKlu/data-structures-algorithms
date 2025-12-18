"""Frequency-ranked autocomplete using a trie."""

# trie_autocomplete_ranked.py
# Frequency-ranked autocomplete (Dictionary-backed)
# Usage: python trie_autocomplete_ranked.py [dictPath] [prefix] [limit] [usagePath]
#
# Ranking: frequency desc, then word asc

import os
import sys
from typing import List, Optional, Tuple

ALPHABET_SIZE = 26

#pylint: disable=too-few-public-methods
#pylint: disable=broad-exception-caught

class TrieNode:
    """Trie node."""
    __slots__ = ("children", "is_end", "frequency")

    def __init__(self):
        self.children = [None] * ALPHABET_SIZE
        self.is_end = False
        self.frequency = 0  # meaningful only if is_end


class Trie:
    """Trie."""
    def __init__(self):
        self.root = TrieNode()

    @staticmethod
    def _index(c: str) -> int:
        return ord(c) - ord("a") if "a" <= c <= "z" else -1

    def insert(self, word: str, bump_frequency: bool = True) -> None:
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
        if bump_frequency:
            current.frequency += 1

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

    def autocomplete_ranked(self, prefix: str, limit: int) -> List[Tuple[str, int]]:
        """Search prefix in trie and return ranked results."""
        start = self._walk(prefix)
        if start is None or limit <= 0:
            return []

        prefix_lc = prefix.lower()
        buf = list(prefix_lc)
        out: List[Tuple[str, int]] = []

        def dfs(node: TrieNode) -> None:
            if node is None:
                return
            if node.is_end:
                out.append(("".join(buf), node.frequency))
            for i in range(ALPHABET_SIZE):
                child = node.children[i]
                if child is not None:
                    buf.append(chr(ord("a") + i))
                    dfs(child)
                    buf.pop()

        dfs(start)

        out.sort(key=lambda t: (-t[1], t[0]))
        return out[:limit]


def load_file(trie: Trie, path: str) -> int:
    """Load file into trie."""
    count = 0
    try:
        with open(path, "r", encoding="utf-8") as f:
            for line in f:
                w = line.strip()
                if not w:
                    continue
                trie.insert(w, bump_frequency=True)
                count += 1
    except OSError as e:
        print(f"Failed to open file: {path}", file=sys.stderr)
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

    dict_path = sys.argv[1] if len(sys.argv) > 1 else r"..\data\frequency.txt"
    prefix = sys.argv[2] if len(sys.argv) > 2 else "th"
    limit = parse_int_or_default(sys.argv[3], 20) if len(sys.argv) > 3 else 20
    usage_path = sys.argv[4] if len(sys.argv) > 4 else None

    loaded = load_file(trie, dict_path)
    print(f"Loaded {loaded} words from {dict_path}")

    if usage_path:
        used = load_file(trie, usage_path)
        print(f"Loaded {used} usage events from {usage_path}")

    print(f'\nRanked Autocomplete("{prefix}") [limit={limit}]')
    for word, freq in trie.autocomplete_ranked(prefix, limit):
        print(f"{word}\t(freq={freq})")

    print("\nWorking directory =", os.getcwd())
    print("Resolved dictionary path =", os.path.abspath(dict_path))


if __name__ == "__main__":
    main()
