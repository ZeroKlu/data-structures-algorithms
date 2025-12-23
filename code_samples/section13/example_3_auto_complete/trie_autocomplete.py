"""Autocomplete using a trie."""

# trie_autocomplete.py
# Trie Autocomplete (Dictionary-backed)
# Usage: python trie_autocomplete.py [dictPath] [prefix] [limit]
#
# This script demonstrates a trie (prefix tree) used for autocomplete.
#
# Key behavior / constraints:
# - Only letters 'a'–'z' are accepted (case-insensitive).
# - If any character in a word is not a–z, insert() rejects the entire word.
# - If any character in a prefix is not a–z (or the path is missing),
#   autocomplete() returns an empty list.
#
# Output ordering:
# - Suggestions are produced in lexicographic order because DFS visits children
#   in 'a'..'z' order.
#
# Path notes:
# - The default dictionary path is a Windows-style relative path: ..\data\words.txt
# - Relative paths are resolved from the directory where the script is executed.

#pylint: disable=anomalous-backslash-in-string

import os   # Working directory + path utilities (os.getcwd, os.path.abspath)
import sys  # CLI args (sys.argv) + stderr output
from typing import List, Optional  # Type hints for return types and optional nodes

# Number of supported lowercase English letters ('a'–'z')
ALPHABET_SIZE = 26

# pylint: disable=too-few-public-methods
# pylint: disable=broad-exception-caught
# These suppressions reflect the intentionally small class design and the
# decision to catch broad exceptions in parsing logic.


class TrieNode:
    """
    Trie node.

    Each node contains:
    - children: fixed-size list of child references (one per letter 'a'–'z')
    - is_end: flag indicating whether a complete word terminates at this node
    """

    # __slots__ reduces per-instance memory usage by preventing __dict__ creation
    __slots__ = ("children", "is_end")

    def __init__(self):
        # Child pointers, indexed 0..25; None indicates the child does not exist
        self.children = [None] * ALPHABET_SIZE

        # True if this node represents the end of a valid inserted word
        self.is_end = False


class Trie:
    """
    Trie (prefix tree) implementation supporting autocomplete.

    Supports:
    - insert(word): insert a word (case-insensitive)
    - autocomplete(prefix, limit): return up to N words matching a prefix
    """

    def __init__(self):
        # Root node (does not correspond to a character)
        self.root = TrieNode()

    @staticmethod
    def _index(c: str) -> int:
        """
        Convert a lowercase character into a trie index.

        Parameters:
        - c: expected to be a single-character string between 'a' and 'z'

        Returns:
        - 0..25 if valid
        - -1 if invalid
        """
        return ord(c) - ord("a") if "a" <= c <= "z" else -1

    def insert(self, word: str) -> None:
        """
        Insert a word into the trie.

        Behavior:
        - Normalizes characters to lowercase (case-insensitive)
        - Rejects the entire word if any character is not 'a'–'z'
        - Creates nodes lazily as needed
        - Marks the final node as end-of-word

        Parameters:
        - word: string to insert
        """
        current = self.root

        # Traverse each character in the word
        for ch in word:
            c = ch.lower()
            idx = self._index(c)

            # Abort insertion if invalid character encountered
            if idx < 0:
                return

            # Allocate child node if missing
            if current.children[idx] is None:
                current.children[idx] = TrieNode()

            # Advance in the trie
            current = current.children[idx]

        # Mark end-of-word
        current.is_end = True

    def _walk(self, prefix: str) -> Optional[TrieNode]:
        """
        Walk the trie according to a prefix and return the final node.

        Behavior:
        - Normalizes to lowercase
        - Returns None if prefix contains invalid characters (non a–z)
        - Returns None if any required child node is missing (prefix not found)

        Parameters:
        - prefix: prefix to traverse

        Returns:
        - TrieNode at the end of the prefix, or None if not found/invalid
        """
        current = self.root

        # Traverse each character in the prefix
        for ch in prefix:
            c = ch.lower()
            idx = self._index(c)

            # Invalid character -> prefix not supported by this trie
            if idx < 0:
                return None

            nxt = current.children[idx]

            # Missing node -> prefix not present
            if nxt is None:
                return None

            current = nxt

        return current

    def autocomplete(self, prefix: str, limit: int) -> List[str]:
        """
        Return up to `limit` autocomplete suggestions for `prefix`.

        Steps:
        1) Find the node corresponding to `prefix` via _walk().
        2) Seed a mutable buffer with the lowercased prefix characters.
        3) DFS over the subtree:
           - When an end-of-word node is encountered, emit the buffer as a word.
           - Visit children in 'a'..'z' order for lexicographic results.
           - Stop once `limit` results are collected.

        Parameters:
        - prefix: prefix to autocomplete
        - limit: maximum number of suggestions to return

        Returns:
        - list of suggestion strings (possibly empty)
        """
        # Locate the trie node that corresponds to the prefix
        start = self._walk(prefix)

        # Invalid/missing prefix -> no suggestions
        if start is None:
            return []

        # Normalize prefix for output and to seed the DFS buffer
        prefix_lc = prefix.lower()

        # Buffer holds the current word under construction as a list of characters
        # (list push/pop is efficient for DFS backtracking)
        buf = list(prefix_lc)

        # Output list of completed words found
        out: List[str] = []

        def dfs(node: TrieNode) -> None:
            """
            Depth-first traversal collecting suggestions.

            Parameters:
            - node: current trie node in DFS

            Behavior:
            - Appends completed words to `out`
            - Uses `buf` as the mutable current-word buffer
            - Stops early when out reaches `limit`
            """
            if node is None or len(out) >= limit:
                return

            # If node marks end-of-word, emit current buffer as a suggestion
            if node.is_end:
                out.append("".join(buf))
                if len(out) >= limit:
                    return

            # Explore children in lexicographic order for deterministic output
            for i in range(ALPHABET_SIZE):
                child = node.children[i]
                if child is not None:
                    # Append next letter to current word
                    buf.append(chr(ord("a") + i))

                    # Recurse into child subtree
                    dfs(child)

                    # Backtrack: remove appended letter
                    buf.pop()

                    # Stop early once limit is reached
                    if len(out) >= limit:
                        return

        # Start DFS from the node representing the prefix
        dfs(start)
        return out


def load_dictionary(trie: Trie, path: str) -> int:
    """
    Load words from a dictionary file into the trie.

    Parameters:
    - trie: Trie instance to populate
    - path: filesystem path to dictionary file

    Returns:
    - number of non-empty lines processed (insert attempted for each)
    - 0 if file cannot be opened/read

    Notes:
    - insert() may reject words containing invalid characters, but this function
      still increments count for each non-empty trimmed line (as written).
    """
    count = 0

    try:
        # Read dictionary line-by-line using UTF-8 encoding
        with open(path, "r", encoding="utf-8") as f:
            for line in f:
                # Trim whitespace and skip empty lines
                w = line.strip()
                if not w:
                    continue

                # Insert word into trie (may be rejected if invalid)
                trie.insert(w)
                count += 1

    except OSError as e:
        # Report file access / I/O errors to stderr
        print(f"Failed to open dictionary: {path}", file=sys.stderr)
        print(str(e), file=sys.stderr)
        return 0

    return count


def parse_int_or_default(s: str, default: int) -> int:
    """
    Parse an integer or return a default value.

    Parameters:
    - s: input string to parse
    - default: value to return if parsing fails or value is non-positive

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
    - argv[1] (optional): dictPath (default: r"..\data\words.txt")
    - argv[2] (optional): prefix   (default: "ab")
    - argv[3] (optional): limit    (default: 20)

    Demonstrates:
    - Loading a dictionary into a trie
    - Autocomplete query for a prefix
    - Printing results and diagnostic path information
    """
    trie = Trie()

    # Read CLI args with fallbacks to lesson defaults
    dict_path = sys.argv[1] if len(sys.argv) > 1 else r"..\data\words.txt"
    prefix = sys.argv[2] if len(sys.argv) > 2 else "ab"
    limit = parse_int_or_default(sys.argv[3], 20) if len(sys.argv) > 3 else 20

    # Load dictionary into the trie
    loaded = load_dictionary(trie, dict_path)
    print(f"Loaded {loaded} words from {dict_path}\n")

    # Run autocomplete and print results
    print(f'Autocomplete("{prefix}") [limit={limit}]')
    for w in trie.autocomplete(prefix, limit):
        print(w)

    # Helpful diagnostics for debugging relative-path resolution
    print("\nWorking directory =", os.getcwd())
    print("Resolved dictionary path =", os.path.abspath(dict_path))


if __name__ == "__main__":
    main()
