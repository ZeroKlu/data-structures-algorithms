"""Frequency-ranked autocomplete using a trie."""

# trie_autocomplete_ranked.py
# Frequency-ranked autocomplete (Dictionary-backed)
# Usage: python trie_autocomplete_ranked.py [dictPath] [prefix] [limit] [usagePath]
#
# Ranking: frequency desc, then word asc
#
# This script builds a trie (prefix tree) that supports "ranked" autocomplete:
# - Each time a word is inserted, its terminal node's frequency can be incremented.
# - Autocomplete gathers all words under a prefix and ranks them by:
#   1) frequency (descending)
#   2) word (ascending) for ties
#
# Key behavior / constraints:
# - Only letters 'a'–'z' are accepted (case-insensitive).
# - If any character in a word/prefix is not a–z:
#   - insert() rejects the entire word
#   - _walk() returns None
#   - autocomplete_ranked() returns []
#
# Path notes:
# - Default dict_path points at ..\data\frequency.txt (Windows-style relative path).
# - Relative paths are resolved from the directory where the script is executed.

#pylint: disable=anomalous-backslash-in-string

import os   # Diagnostics for working directory and absolute path resolution
import sys  # CLI arguments and stderr output
from typing import List, Optional, Tuple  # Type hints for return values and optional nodes

# Number of supported lowercase English letters
ALPHABET_SIZE = 26

# pylint: disable=too-few-public-methods
# pylint: disable=broad-exception-caught
# - too-few-public-methods: small data-holder classes are intentional here
# - broad-exception-caught: used for "parse int" fallback logic


class TrieNode:
    """
    Trie node.

    Fields:
    - children: fixed-size list (length 26) of child references for 'a'..'z'
    - is_end: True if a complete word ends at this node
    - frequency: frequency/score for the word ending at this node
                 (meaningful only when is_end is True)
    """

    # __slots__ saves memory by preventing creation of __dict__ per node
    __slots__ = ("children", "is_end", "frequency")

    def __init__(self):
        # Child pointers; None indicates missing child
        self.children = [None] * ALPHABET_SIZE

        # End-of-word marker
        self.is_end = False

        # Frequency counter (only meaningful for terminal nodes)
        self.frequency = 0


class Trie:
    """
    Trie (prefix tree) supporting frequency-ranked autocomplete.

    Supports:
    - insert(word, bump_frequency): insert a word, optionally incrementing frequency
    - autocomplete_ranked(prefix, limit): return ranked suggestions under prefix
    """

    def __init__(self):
        # Root node does not correspond to any character
        self.root = TrieNode()

    @staticmethod
    def _index(c: str) -> int:
        """
        Convert a lowercase character into an index [0..25].

        Parameters:
        - c: expected to be a single-character string between 'a' and 'z'

        Returns:
        - 0..25 if valid
        - -1 if invalid
        """
        return ord(c) - ord("a") if "a" <= c <= "z" else -1

    def insert(self, word: str, bump_frequency: bool = True) -> None:
        """
        Insert word into trie.

        Parameters:
        - word: word to insert
        - bump_frequency: if True, increment terminal node's frequency

        Behavior:
        - Case-insensitive: lowercases each character
        - Rejects entire word if any character is not 'a'..'z'
        - Creates nodes lazily as needed
        - Marks terminal node as end-of-word
        - Optionally increments terminal frequency

        Frequency meaning:
        - If you insert the same word multiple times with bump_frequency=True,
          its frequency increases accordingly.
        """
        current = self.root

        # Walk/create nodes for each character
        for ch in word:
            c = ch.lower()
            idx = self._index(c)

            # Reject entire word if invalid character encountered
            if idx < 0:
                return

            # Create child node if missing
            if current.children[idx] is None:
                current.children[idx] = TrieNode()

            # Advance in the trie
            current = current.children[idx]

        # Mark as a complete word
        current.is_end = True

        # Optionally increase frequency count
        if bump_frequency:
            current.frequency += 1

    def _walk(self, prefix: str) -> Optional[TrieNode]:
        """
        Walk the trie for a prefix and return the final node.

        Parameters:
        - prefix: prefix to traverse

        Returns:
        - TrieNode at the end of the prefix
        - None if invalid character encountered or path missing

        Behavior:
        - Lowercases each character (case-insensitive)
        - Rejects prefixes with non a–z characters
        """
        current = self.root

        for ch in prefix:
            c = ch.lower()
            idx = self._index(c)

            # Invalid prefix character
            if idx < 0:
                return None

            nxt = current.children[idx]

            # Missing path -> prefix not present
            if nxt is None:
                return None

            current = nxt

        return current

    def autocomplete_ranked(self, prefix: str, limit: int) -> List[Tuple[str, int]]:
        """
        Return ranked autocomplete suggestions for `prefix`.

        Parameters:
        - prefix: prefix to autocomplete
        - limit: maximum number of results to return

        Returns:
        - List of (word, frequency) tuples, ranked best-first

        Steps:
        1) Find the node corresponding to the prefix via _walk().
        2) DFS the subtree collecting every terminal word and its frequency.
        3) Sort collected items by:
           - frequency descending
           - word ascending for ties
        4) Return the first `limit` items.

        Note:
        - This implementation collects *all* completions under the prefix before sorting,
          which is straightforward but can be expensive for large subtrees.
        """
        start = self._walk(prefix)

        # No matches if prefix invalid/missing or limit is non-positive
        if start is None or limit <= 0:
            return []

        # Seed buffer with the lowercased prefix to build candidate words during DFS
        prefix_lc = prefix.lower()
        buf = list(prefix_lc)

        # Output list of (word, freq)
        out: List[Tuple[str, int]] = []

        def dfs(node: TrieNode) -> None:
            """
            Depth-first traversal collecting all terminal words under `node`.

            Uses `buf` as a mutable character buffer:
            - append next letter -> recurse -> pop back (backtracking)
            """
            if node is None:
                return

            # If this node ends a word, record the current buffer and frequency
            if node.is_end:
                out.append(("".join(buf), node.frequency))

            # Explore children in lexicographic order a..z
            for i in range(ALPHABET_SIZE):
                child = node.children[i]
                if child is not None:
                    buf.append(chr(ord("a") + i))
                    dfs(child)
                    buf.pop()

        # DFS from the prefix node
        dfs(start)

        # Sort best-first: frequency descending, word ascending
        out.sort(key=lambda t: (-t[1], t[0]))

        # Return only the top `limit` results
        return out[:limit]


def load_file(trie: Trie, path: str) -> int:
    """
    Load a file into the trie, one word per line.

    Parameters:
    - trie: Trie instance to populate
    - path: file path to read

    Returns:
    - number of non-empty trimmed lines processed (insert attempted for each)
    - 0 if file cannot be opened/read

    Behavior:
    - Strips whitespace from each line
    - Skips empty lines
    - Inserts each word with bump_frequency=True (so each line bumps frequency)
    """
    count = 0
    try:
        with open(path, "r", encoding="utf-8") as f:
            for line in f:
                w = line.strip()
                if not w:
                    continue

                # Each occurrence bumps frequency for that word
                trie.insert(w, bump_frequency=True)
                count += 1

    except OSError as e:
        # Print I/O errors to stderr and signal failure via 0
        print(f"Failed to open file: {path}", file=sys.stderr)
        print(str(e), file=sys.stderr)
        return 0

    return count


def parse_int_or_default(s: str, default: int) -> int:
    """
    Parse an integer or return a default.

    Parameters:
    - s: input string to parse
    - default: fallback if parsing fails or value is non-positive

    Returns:
    - parsed integer if > 0
    - default otherwise
    """
    try:
        v = int(s)
        return v if v > 0 else default
    except Exception:
        return default


def main() -> None:
    """
    Program entry point.

    CLI arguments:
    - argv[1] (optional): dictPath   (default: r"..\data\frequency.txt")
    - argv[2] (optional): prefix     (default: "th")
    - argv[3] (optional): limit      (default: 20)
    - argv[4] (optional): usagePath  (default: None)

    Behavior:
    - Loads dictPath into the trie (each line bumps frequency)
    - Optionally loads a usagePath where each line represents one usage event
      (each line further bumps frequency)
    - Prints ranked autocomplete results for the given prefix

    Diagnostics:
    - Prints the working directory and resolved dictionary path
    """
    trie = Trie()

    # Defaults match the lesson convention; allow override via CLI args
    dict_path = sys.argv[1] if len(sys.argv) > 1 else r"..\data\frequency.txt"
    prefix = sys.argv[2] if len(sys.argv) > 2 else "th"
    limit = parse_int_or_default(sys.argv[3], 20) if len(sys.argv) > 3 else 20
    usage_path = sys.argv[4] if len(sys.argv) > 4 else None

    # Load baseline dictionary/frequency file
    loaded = load_file(trie, dict_path)
    print(f"Loaded {loaded} words from {dict_path}")

    # Optionally load usage events file to further bump frequencies
    if usage_path:
        used = load_file(trie, usage_path)
        print(f"Loaded {used} usage events from {usage_path}")

    # Run ranked autocomplete and print results
    print(f'\nRanked Autocomplete("{prefix}") [limit={limit}]')
    for word, freq in trie.autocomplete_ranked(prefix, limit):
        print(f"{word}\t(freq={freq})")

    # Helpful diagnostics for relative-path resolution
    print("\nWorking directory =", os.getcwd())
    print("Resolved dictionary path =", os.path.abspath(dict_path))


if __name__ == "__main__":
    main()
