"""Prefix counting using a trie."""

# trie_prefix_count.py
# Trie Prefix Counting (Dictionary-backed)
#
# This script implements a trie (prefix tree) that supports:
# - Inserting words (case-insensitive)
# - Counting how many inserted words start with a given prefix
#
# Dictionary loading:
# - By default, loads from ..\data\words.txt (Windows-style relative path)
# - Relative paths are resolved from the working directory where the script runs
#
# Key constraints:
# - Only letters 'a'–'z' are accepted.
# - If any character in a word is not a–z, insert() rejects the entire word.
# - If any character in a prefix is not a–z (or the path is missing),
#   prefix_count() returns 0.

import os   # Provides access to current working directory and path utilities
import sys  # Provides access to command-line arguments and stderr output

# Number of supported lowercase English letters ('a'–'z')
ALPHABET_SIZE = 26

# pylint: disable=too-few-public-methods
# These classes are intentionally small/simple, so this lint warning is suppressed.


class TrieNode:
    """
    Trie node.

    Each node contains:
    - children: a fixed-size list of child references (one per letter 'a'–'z')
    - prefix_count: number of inserted words that share this prefix node
    """

    # __slots__ reduces per-instance memory usage by preventing __dict__ creation
    __slots__ = ("children", "prefix_count")

    def __init__(self):
        # Child pointers indexed 0..25; None indicates the child does not exist
        self.children = [None] * ALPHABET_SIZE

        # Count of words whose insertion path includes this node
        self.prefix_count = 0


class Trie:
    """
    Trie (prefix tree) implementation for prefix counting.

    Supports:
    - insert(word): inserts a word and increments prefix_count along the path
    - prefix_count(prefix): returns how many words start with the given prefix
    """

    def __init__(self):
        # Root node (does not correspond to a character)
        self.root = TrieNode()

    @staticmethod
    def _index(c: str) -> int:
        """
        Convert a lowercase character into a trie index.

        Parameters:
        - c: a single-character string expected to be between 'a' and 'z'

        Returns:
        - 0..25 if valid
        - -1 if invalid
        """
        return ord(c) - ord("a") if "a" <= c <= "z" else -1

    # Insert a word; increments prefix_count along the path.
    # Rejects the entire word if any char is not a-z.
    def insert(self, word: str) -> None:
        """
        Insert a word into the trie.

        Behavior:
        - Normalizes characters to lowercase (case-insensitive)
        - Rejects the entire word if any character is not 'a'–'z'
        - Creates nodes lazily as needed
        - Increments prefix_count for each node visited along the word path

        Parameters:
        - word: string to insert into the trie
        """
        current = self.root

        # Traverse each character in the word
        for ch in word:
            c = ch.lower()
            idx = self._index(c)

            # Abort insertion if an invalid character is encountered
            if idx < 0:
                return

            # Allocate child node if it doesn't exist yet
            if current.children[idx] is None:
                current.children[idx] = TrieNode()

            # Move to the next node
            current = current.children[idx]

            # Increment count for this prefix node
            current.prefix_count += 1

    # Return how many words start with prefix (0 if invalid or not found).
    def prefix_count(self, prefix: str) -> int:
        """
        Return how many inserted words start with the given prefix.

        Behavior:
        - Returns 0 if prefix contains invalid characters (non a–z)
        - Returns 0 if any required trie node is missing (prefix not present)
        - Otherwise returns prefix_count stored in the prefix node

        Parameters:
        - prefix: prefix string to query

        Returns:
        - integer count of words sharing this prefix
        """
        current = self.root

        # Walk the trie according to the prefix
        for ch in prefix:
            c = ch.lower()
            idx = self._index(c)

            # Invalid character means the prefix is not supported by this trie
            if idx < 0:
                return 0

            nxt = current.children[idx]

            # Missing node means no inserted words share this prefix
            if nxt is None:
                return 0

            current = nxt

        # Node's prefix_count represents number of words sharing this prefix
        return current.prefix_count


def load_dictionary(trie: Trie, path: str) -> int:
    """
    Load words from a dictionary file into the trie.

    Parameters:
    - trie: Trie instance to populate
    - path: filesystem path to dictionary file

    Returns:
    - number of non-empty lines processed (insert attempted for each)
    - 0 if the file could not be opened/read

    Notes:
    - insert() may reject words with invalid characters, but this function
      still increments "count" for each non-empty trimmed line (as written).
    """
    count = 0

    try:
        # Open dictionary file using UTF-8 encoding
        with open(path, "r", encoding="utf-8") as f:
            for line in f:
                # Trim whitespace and skip empty lines
                w = line.strip()
                if not w:
                    continue

                # Insert into trie (may be rejected if invalid)
                trie.insert(w)
                count += 1

    except OSError as e:
        # Report file access / I/O errors to stderr
        print(f"Failed to open dictionary: {path}", file=sys.stderr)
        print(str(e), file=sys.stderr)
        return 0

    return count


def main():
    """
    Program entry point.

    Demonstrates:
    - Trie construction
    - Dictionary loading
    - Prefix-count queries
    - Helpful diagnostics for working directory and path resolution
    """
    trie = Trie()

    # Default path matches the lesson convention; can be overridden by CLI arg
    dict_path = sys.argv[1] if len(sys.argv) > 1 else r"..\data\words.txt"

    # Load dictionary words into the trie
    loaded = load_dictionary(trie, dict_path)

    print(f"Loaded {loaded} words from {dict_path}\n")

    # Prefixes to query against the loaded trie
    prefixes = ["a", "ab", "alg", "aard", "z", "nope"]
    for p in prefixes:
        print(f'prefixCount("{p}") = {trie.prefix_count(p)}')

    # Helpful diagnostics for debugging relative paths at runtime
    print("\nWorking directory =", os.getcwd())
    print("Resolved dictionary path =", os.path.abspath(dict_path))


if __name__ == "__main__":
    main()
