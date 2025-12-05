"""Lesson 3 - Jump Search (Sorted Data Only)"""

import math
import os
import sys

#pylint: disable=broad-exception-caught

# ================================================================
# Jump Search (regular)
# ================================================================
def jump_search(arr: list[int], target: int) -> int:
    """Jump Search (regular)"""
    n = len(arr)
    if n == 0:
        return -1

    step = int(math.sqrt(n))
    prev = 0
    curr = step

    # Jump phase
    while curr < n and arr[curr] < target:
        prev = curr
        curr += step

    if curr >= n:
        curr = n - 1

    # Linear search in block
    for i in range(prev, curr + 1):
        if arr[i] == target:
            return i

    return -1

# ================================================================
# Jump Search with step counting
# ================================================================
def jump_search_steps(arr: list[int], target: int) -> tuple[int, int]:
    """Jump Search with step counting."""
    n = len(arr)
    steps = 0

    if n == 0:
        return -1, steps

    step = int(math.sqrt(n))
    prev = 0
    curr = step

    # Jump phase
    while curr < n:
        steps += 1  # comparison arr[curr] < target or >=
        if arr[curr] >= target:
            break

        prev = curr
        curr += step

    if curr >= n:
        curr = n - 1

    # Linear search
    for i in range(prev, curr + 1):
        steps += 1  # comparison arr[i] == target
        if arr[i] == target:
            return i, steps

    return -1, steps

# ================================================================
# Load ordered.txt relative to this script
# ================================================================
def load_ordered_file() -> list[int]:
    """Load ordered.txt relative to this script."""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(script_dir, "..", "data", "ordered.txt")
    file_path = os.path.abspath(file_path)

    print("Attempting to read:", file_path)

    try:
        with open(file_path, "r", encoding="utf-8") as f:
            return [int(x) for x in f.read().split()]
    except Exception as e:
        print("Error reading ordered.txt:", e)
        print("Working directory:", os.getcwd())
        return []


# ================================================================
# MAIN TESTS
# ================================================================
if __name__ == "__main__":
    array = load_ordered_file()
    if not array:
        print("Missing input file — aborting.")
        sys.exit()

    print("\n=== Jump Search Tests (sorted data only) ===")
    print(f"Loaded {len(array)} integers\n")

    # First element
    idx, step_count = jump_search_steps(array, array[0])
    print(f"Search first  ({array[0]}): index={idx}, steps={step_count}")

    # Middle element
    mid_val = array[len(array) // 2]
    idx, step_count = jump_search_steps(array, mid_val)
    print(f"Search middle ({mid_val}): index={idx}, steps={step_count}")

    # Last element
    last_val = array[-1]
    idx, step_count = jump_search_steps(array, last_val)
    print(f"Search last   ({last_val}): index={idx}, steps={step_count}")

    # Missing element
    idx, step_count = jump_search_steps(array, 999_999)
    print(f"Search missing (999999): index={idx}, steps={step_count}")
