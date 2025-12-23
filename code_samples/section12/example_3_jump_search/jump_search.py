"""Lesson 3 - Jump Search (Sorted Data Only)"""

import math  # Provides mathematical functions such as sqrt
import os    # Provides operating system and file path utilities
import sys   # Provides access to interpreter exit functionality

# Disable pylint warning for catching broad exceptions
# (used here to provide detailed diagnostics during file loading)
# pylint: disable=broad-exception-caught

# ================================================================
# Jump Search (regular)
# ================================================================

def jump_search(arr: list[int], target: int) -> int:
    """
    Perform jump search on a sorted list of integers.

    Jump search works in two phases:
      1) Jump phase: advance in fixed-size blocks (sqrt(n)) to find
         a block where the target could exist.
      2) Linear search phase: scan linearly within that block.

    :param arr: Sorted list of integers to search
    :param target: Value to locate
    :return: Index of the target if found; otherwise -1
    """
    n = len(arr)          # Number of elements in the list
    if n == 0:
        return -1         # Empty list => not found

    # Determine jump (block) size
    step = int(math.sqrt(n))

    prev = 0              # Start index of the current block
    curr = step           # End index of the current block

    # ------------------------------------------------------------
    # Jump phase
    # ------------------------------------------------------------
    # Jump ahead block-by-block until:
    #  - curr is out of bounds, or
    #  - the value at arr[curr] is >= target
    while curr < n and arr[curr] < target:
        prev = curr       # Advance block start
        curr += step      # Jump to next block end

    # Clamp curr to the last valid index if we overshoot the list
    if curr >= n:
        curr = n - 1

    # ------------------------------------------------------------
    # Linear search within the identified block [prev, curr]
    # ------------------------------------------------------------
    for i in range(prev, curr + 1):
        if arr[i] == target:
            return i      # Target found

    # Target not found
    return -1

# ================================================================
# Jump Search with step counting
# ================================================================

def jump_search_steps(arr: list[int], target: int) -> tuple[int, int]:
    """
    Perform jump search while counting the number of comparisons.

    Comparisons are counted in both phases:
      - Jump phase comparisons against arr[curr]
      - Linear scan comparisons against arr[i]

    :param arr: Sorted list of integers to search
    :param target: Value to locate
    :return: Tuple containing:
             - index: position of the target, or -1 if not found
             - steps: number of comparisons performed
    """
    n = len(arr)          # Number of elements
    steps = 0             # Counts total number of comparisons

    # Handle empty list case
    if n == 0:
        return -1, steps

    # Determine jump (block) size
    step = int(math.sqrt(n))

    prev = 0              # Start index of the current block
    curr = step           # End index of the current block

    # ------------------------------------------------------------
    # Jump phase
    # ------------------------------------------------------------
    while curr < n:
        steps += 1        # Count comparison involving arr[curr]

        # If current block end is >= target, stop jumping
        if arr[curr] >= target:
            break

        # Otherwise, jump to the next block
        prev = curr
        curr += step

    # Clamp curr to the last valid index if we overshoot the list
    if curr >= n:
        curr = n - 1

    # ------------------------------------------------------------
    # Linear search phase within [prev, curr]
    # ------------------------------------------------------------
    for i in range(prev, curr + 1):
        steps += 1        # Count comparison arr[i] == target

        if arr[i] == target:
            return i, steps

    # Target not found
    return -1, steps

# ================================================================
# Load ordered.txt relative to this script
# ================================================================

def load_ordered_file() -> list[int]:
    """
    Load ordered.txt relative to this script's directory.

    Expected path:
        ../data/ordered.txt

    :return: List of integers read from ordered.txt,
             or an empty list on failure
    """
    # Determine the directory containing this script
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # Build absolute path to ordered.txt
    file_path = os.path.join(script_dir, "..", "data", "ordered.txt")
    file_path = os.path.abspath(file_path)

    # Print resolved path for diagnostic purposes
    print("Attempting to read:", file_path)

    try:
        # Open the file and read its contents
        with open(file_path, "r", encoding="utf-8") as f:
            return [int(x) for x in f.read().split()]

    except Exception as e:
        # Provide diagnostic output if file loading fails
        print("Error reading ordered.txt:", e)
        print("Working directory:", os.getcwd())
        return []

# ================================================================
# MAIN TESTS
# ================================================================

if __name__ == "__main__":
    # Load ordered dataset from disk
    array = load_ordered_file()

    # Validate successful file load
    if not array:
        print("Missing input file - aborting.")
        sys.exit()

    print("\n=== Jump Search Tests (sorted data only) ===")
    print(f"Loaded {len(array)} integers\n")

    # Test 1: Search for the first element
    idx, step_count = jump_search_steps(array, array[0])
    print(f"Search first  ({array[0]}): index={idx}, steps={step_count}")

    # Test 2: Search for the middle element
    mid_val = array[len(array) // 2]
    idx, step_count = jump_search_steps(array, mid_val)
    print(f"Search middle ({mid_val}): index={idx}, steps={step_count}")

    # Test 3: Search for the last element
    last_val = array[-1]
    idx, step_count = jump_search_steps(array, last_val)
    print(f"Search last   ({last_val}): index={idx}, steps={step_count}")

    # Test 4: Search for a value guaranteed not to exist
    idx, step_count = jump_search_steps(array, 999_999)
    print(f"Search missing (999999): index={idx}, steps={step_count}")
