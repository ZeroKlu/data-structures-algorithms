"""Section 7 - Hash Tables and Sets"""

from collections import Counter
from typing import Iterable, Optional

def demo_hash() -> int:
    """Demo hash table."""
    freq: dict[str, int] = {}
    freq["apple"] = freq.get("apple", 0) + 1
    freq["banana"] = freq.get("banana", 0) + 2
    n = freq.get("apple", 0)
    return n

def count_words(words: Iterable[str]) -> dict[str, int]:
    """Count word frequencies."""
    return dict(Counter(words))

def first_duplicate(arr: Iterable[int]) -> Optional[int]:
    """Find the first duplicate in an array."""
    seen: set[int] = set()
    for x in arr:
        if x in seen:
            return x
        seen.add(x)
    return None

def two_sum(nums: list[int], target: int) -> tuple[int, int]:
    """Find two numbers in an array that sum to a target."""
    index_of: dict[int, int] = {}
    for i, x in enumerate(nums):
        need = target - x
        if need in index_of:
            return index_of[need], i
        index_of[x] = i
    return -1, -1

# ============================
# Tests (top-level statements)
# ============================

print("=== Test: demo_hash ===")
demo_hash()
print("(demo_hash ran — no visible output)\n")

# ------------------------------------------------------------

print("=== Test: count_words (word frequency) ===")

word_list = ["apple", "banana", "apple", "orange", "banana", "apple"]
frequency = count_words(word_list)

for key in ["apple", "banana", "orange", "pear"]:
    print(f"'{key}' -> {frequency.get(key, '(not found)')}")

print("Expected: apple=3, banana=2, orange=1, pear not found\n")

# ------------------------------------------------------------

print("=== Test: first_duplicate ===")

arr1 = [2, 5, 1, 2, 3, 5, 1]
print("Array 1:", arr1)
print("First duplicate:", first_duplicate(arr1), "(expected 2)\n")

arr2 = [1, 2, 3, 4, 5]
print("Array 2:", arr2)
print("First duplicate:", first_duplicate(arr2), "(expected None)\n")

# ------------------------------------------------------------

print("=== Test: two_sum ===")

numbers = [2, 7, 11, 15]
tgt = 9
print("Array:", numbers)
print("Target:", tgt)

pair1 = two_sum(numbers, tgt)
print("two_sum result indices:", pair1)
print("Expected: (0, 1) or (1, 0)\n")

# Negative test
print("--- Negative case (no pair) ---")
nums2 = [1, 2, 3]
target2 = 100
print("Array:", nums2)
print("Target:", target2)

pair2 = two_sum(nums2, target2)
print("two_sum result indices:", pair2)
print("Expected: (-1, -1)\n")
