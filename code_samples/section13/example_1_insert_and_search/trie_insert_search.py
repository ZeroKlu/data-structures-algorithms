"""Search a dictionary of words in a trie."""

# trie_insert_search.py
# Trie Insert + Search (Dictionary-backed)
#
# Loads words from ..\data\words.txt by default (Windows-style relative path).
# Relative paths are resolved from the directory where the script is executed.

from typing import Optional
import os
import sys

# pylint: disable=too-few-public-methods
# This file intentionally defines small, simple classes without many methods.

# Number of supported lowercase English letters ('a'–'z')
ALPHABET_SIZE = 26


class TrieNode:
    """
    Trie node.

    Each node contains:
    - A fixed-size list of child references (one per letter 'a'–'z')
    - A flag indicating whether this node terminates a valid word
    """

    # __slots__ is used to reduce per-instance memory overhead
    __slots__ = ("children", "is_end")

    def __init__(self):
        # Child nodes, indexed by character position (0–25)
        self.children = [None] * ALPHABET_SIZE

        # True if this node marks the end of a valid word
        self.is_end = False


class Trie:
    """
    Trie (prefix tree) implementation.

    Supports:
    - Case-insensitive word insertion
    - Exact word search
    - Prefix search
    """

    def __init__(self):
        # Root node of the trie; always exists
        self.root = TrieNode()

    @staticmethod
    def _index(c: str) -> int:
        """
        Convert a lowercase character into a trie array index.

        Parameters:
        - c: character expected to be between 'a' and 'z'

        Returns:
        - Index in range [0, 25] if valid
        - -1 if the character is outside the supported range
        """
        return ord(c) - ord("a") if "a" <= c <= "z" else -1

    # Insert a word; reject the entire word if any char is not a-z
    def insert(self, word: str) -> None:
        """
        Insert a word into the trie.

        Behavior:
        - Converts characters to lowercase
        - Rejects the entire word if any character is not 'a'–'z'
        - Creates nodes lazily as needed

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

            # Allocate a new child node if needed
            if current.children[idx] is None:
                current.children[idx] = TrieNode()

            # Move to the next node in the trie
            current = current.children[idx]

        # Mark the final node as representing a complete word
        current.is_end = True

    def _walk(self, s: str) -> Optional[TrieNode]:
        """
        Traverse the trie following the given word or prefix.

        Used internally by search() and starts_with().

        Parameters:
        - s: word or prefix to traverse

        Returns:
        - The final TrieNode reached if traversal succeeds
        - None if traversal fails due to invalid characters or missing nodes
        """
        current = self.root

        # Traverse character by character
        for ch in s:
            c = ch.lower()
            idx = self._index(c)

            # Fail if character is invalid
            if idx < 0:
                return None

            nxt = current.children[idx]

            # Fail if required child node does not exist
            if nxt is None:
                return None

            current = nxt

        return current

    def search(self, word: str) -> bool:
        """
        Search for a complete word in the trie.

        Parameters:
        - word: word to search for

        Returns:
        - True if the word exists and is marked as complete
        - False otherwise
        """
        node = self._walk(word)
        return bool(node and node.is_end)

    def starts_with(self, prefix: str) -> bool:
        """
        Check whether any word in the trie starts with the given prefix.

        Parameters:
        - prefix: prefix string to test

        Returns:
        - True if the prefix exists in the trie
        - False otherwise
        """
        return self._walk(prefix) is not None


def load_dictionary(trie: Trie, path: str) -> int:
    """
    Load words from a dictionary file into the trie.

    Parameters:
    - trie: Trie instance to populate
    - path: filesystem path to the dictionary file

    Returns:
    - Number of words successfully loaded
    - 0 if the file could not be opened or read
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

                # Insert word into the trie
                trie.insert(w)
                count += 1

    except OSError as e:
        # Report file access or I/O errors
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
    - Exact word searching
    - Prefix searching
    - Helpful diagnostics for path resolution
    """
    trie = Trie()

    # Default path matches the lesson convention; can be overridden by CLI arg
    dict_path = sys.argv[1] if len(sys.argv) > 1 else r"..\data\words.txt"

    # Load dictionary words into the trie
    loaded = load_dictionary(trie, dict_path)

    print(f"Loaded {loaded} words from {dict_path}\n")

    # Words to test for exact matches
    tests = ["aardvark", "abandon", "zebra", "notaword"]
    for w in tests:
        print(f'search("{w}") = {"true" if trie.search(w) else "false"}')

    # Prefix tests
    print("\nPrefix tests:")
    prefixes = ["ab", "alg", "aard", "nope"]
    for p in prefixes:
        print(f'startsWith("{p}") = {"true" if trie.starts_with(p) else "false"}')

    # Helpful diagnostics for relative paths
    print("\nWorking directory =", os.getcwd())
    print("Resolved dictionary path =", os.path.abspath(dict_path))


if __name__ == "__main__":
    main()
