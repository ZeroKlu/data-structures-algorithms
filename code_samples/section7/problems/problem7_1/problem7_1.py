"""
Problem 7.1 - First Unique Character in a String.

This module provides a clean, hash-table-based solution to find the index
of the first non-repeating character in a string, along with a simple
console-driven test harness.
"""

def first_uniq_char(s: str) -> int:
    """
    Finds the index of the first character in the string that appears exactly once.

    Args:
        s:
            Input string to examine (may be empty).

    Returns:
        - The 0-based index of the first unique character.
        - -1 if no such character exists or the string is empty.

    Approach:
        Two-pass algorithm:
          1) Count character frequencies using a dictionary.
          2) Scan the string again to find the first character with frequency == 1.

    Complexity:
        - Time:  O(n), where n = len(s)
        - Space: O(k), where k = number of distinct characters

    Notes:
        - Python strings are Unicode; this function counts Unicode code points,
          not grapheme clusters. Most algorithm problems expect this behavior.
        - Using dict.get(key, 0) avoids KeyError for missing characters.
    """
    # Frequency table: character -> count
    freq: dict[str, int] = {}

    # First pass: count occurrences of each character
    for ch in s:
        freq[ch] = freq.get(ch, 0) + 1

    # Second pass: find the first index with frequency exactly 1
    for i, ch in enumerate(s):
        if freq[ch] == 1:
            return i

    # No unique character found
    return -1


# ============================
# Tests (top-level statements)
# ============================
#
# These statements execute immediately when the module is run.
# No explicit main() function is required.
#

tests = [
    "leetcode",       # expected 0 ('l')
    "loveleetcode",   # expected 2 ('v')
    "aabbcc",         # expected -1
    "",               # expected -1 (empty string)
    "x",              # expected 0
    "aabbc",          # expected 4 ('c')
    "swiss"           # expected 1 ('w')
]

expected = [
    0,
    2,
    -1,
    -1,
    0,
    4,
    1
]

print("=== Testing first_uniq_char ===\n")

# Run each test case and compare with expected output
for val, exp in zip(tests, expected):
    result = first_uniq_char(val)

    print(f"Input: \"{val}\"")
    print(f"first_uniq_char -> {result} (expected {exp})\n")
