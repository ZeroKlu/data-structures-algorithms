"""
Section 7 - Hash Tables and Sets

This module demonstrates common hash-based patterns in Python using:
  - dict for key/value mappings (hash tables)
  - set for fast membership checks
  - collections.Counter for concise frequency counting

The examples mirror classic interview / algorithm problems:
  - Counting word frequencies
  - Finding the first duplicate element
  - Solving the Two Sum problem
"""

from collections import Counter
from typing import Iterable, Optional


def demo_hash() -> int:
    """
    Demonstrates basic dictionary (hash table) usage in Python.

    Key ideas:
      - dict.get(key, default) safely retrieves a value without raising KeyError
      - Missing keys can be treated as having value 0 when counting

    Returns:
      - The count associated with "apple" (used only for demonstration)

    Note:
      - This function mutates a local dictionary only and prints nothing.
      - The return value is used solely to show how values can be retrieved.
    """
    # Initialize an empty dictionary: key -> count
    freq: dict[str, int] = {}

    # Increment counts using get(..., 0) to handle missing keys
    freq["apple"] = freq.get("apple", 0) + 1     # apple -> 1
    freq["banana"] = freq.get("banana", 0) + 2   # banana -> 2

    # Safely read a value; returns 0 if "apple" does not exist
    n = freq.get("apple", 0)

    return n


def count_words(words: Iterable[str]) -> dict[str, int]:
    """
    Counts how many times each word appears in the input iterable.

    Args:
      words:
        Any iterable of strings (list, tuple, generator, etc.)

    Returns:
      A dictionary mapping word -> frequency.

    Complexity:
      - Time:  O(n), where n is the number of input words
      - Space: O(k), where k is the number of distinct words

    Implementation note:
      - collections.Counter is a specialized dict subclass optimized
        for frequency counting.
      - Wrapping it with dict(...) returns a plain dictionary.
    """
    return dict(Counter(words))


def first_duplicate(arr: Iterable[int]) -> Optional[int]:
    """
    Finds the first duplicate value encountered when scanning left-to-right.

    Example:
      [2, 5, 1, 2, 3, 5, 1] -> 2

    Args:
      arr:
        Iterable of integers to scan.

    Returns:
      - The first duplicated integer if found
      - None if all values are unique

    Complexity:
      - Time:  O(n) average
      - Space: O(n) worst case (when all elements are unique)

    Notes:
      - Using a set provides average O(1) membership checks.
      - Returning Optional[int] avoids ambiguous sentinel values
        like 0 or -1.
    """
    # Tracks values that have already been seen
    seen: set[int] = set()

    for x in arr:
        # If x is already in the set, it is the first duplicate encountered
        if x in seen:
            return x

        # Otherwise, record that we've seen x
        seen.add(x)

    # No duplicates found
    return None


def two_sum(nums: list[int], target: int) -> tuple[int, int]:
    """
    Solves the classic Two Sum problem.

    Finds indices (i, j) such that:
      nums[i] + nums[j] == target

    Args:
      nums:
        List of integers.
      target:
        Desired sum.

    Returns:
      - A tuple (i, j) if a valid pair is found
      - (-1, -1) if no such pair exists

    Complexity:
      - Time:  O(n) average
      - Space: O(n)

    Approach:
      - Maintain a dictionary mapping value -> index seen so far.
      - For each element x at index i:
          need = target - x
        If need has already been seen, we have a solution.

    Notes:
      - If a value appears multiple times, the dictionary stores
        the most recent index.
      - Any valid pair is acceptable for this problem.
    """
    # Map from value -> index where it was last seen
    index_of: dict[int, int] = {}

    for i, x in enumerate(nums):
        # Compute the complement needed to reach target
        need = target - x

        # If we've already seen the needed value, return indices
        if need in index_of:
            return index_of[need], i

        # Record the current value and its index
        index_of[x] = i

    # No pair found
    return -1, -1


# ============================
# Tests (top-level statements)
# ============================
#
# These statements run immediately when the module is executed.
# No explicit main() function is required.
#

print("=== Test: demo_hash ===")
demo_hash()
print("(demo_hash ran - no visible output)\n")

# ------------------------------------------------------------

print("=== Test: count_words (word frequency) ===")

# Input words:
# apple x3, banana x2, orange x1
word_list = ["apple", "banana", "apple", "orange", "banana", "apple"]
frequency = count_words(word_list)

# Print selected keys, including one that does not exist ("pear")
for key in ["apple", "banana", "orange", "pear"]:
    print(f"'{key}' -> {frequency.get(key, '(not found)')}")

print("Expected: apple=3, banana=2, orange=1, pear not found\n")

# ------------------------------------------------------------

print("=== Test: first_duplicate ===")

# Case 1: duplicates exist
arr1 = [2, 5, 1, 2, 3, 5, 1]
print("Array 1:", arr1)
print("First duplicate:", first_duplicate(arr1), "(expected 2)\n")

# Case 2: no duplicates
arr2 = [1, 2, 3, 4, 5]
print("Array 2:", arr2)
print("First duplicate:", first_duplicate(arr2), "(expected None)\n")

# ------------------------------------------------------------

print("=== Test: two_sum ===")

# Positive case: 2 + 7 = 9
numbers = [2, 7, 11, 15]
tgt = 9
print("Array:", numbers)
print("Target:", tgt)

pair1 = two_sum(numbers, tgt)
print("two_sum result indices:", pair1)
print("Expected: (0, 1) or (1, 0)\n")

# Negative test: no valid pair
print("--- Negative case (no pair) ---")
nums2 = [1, 2, 3]
target2 = 100
print("Array:", nums2)
print("Target:", target2)

pair2 = two_sum(nums2, target2)
print("two_sum result indices:", pair2)
print("Expected: (-1, -1)\n")
