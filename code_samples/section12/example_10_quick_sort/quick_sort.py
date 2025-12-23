"""Example 10: Quick Sort with step counting (Python).

PURPOSE
-------
Demonstrate Quick Sort on integer data while counting steps to help
illustrate time complexity.

This script expects TWO files containing the SAME integers:
  - unordered.txt  (unsorted input)
  - ordered.txt    (the same integers sorted ascending)

Workflow:
  1) Load unordered + ordered using a robust multi-path search
  2) Quick-sort a COPY of unordered (do not mutate original input)
  3) Compare the sorted output against ordered.txt
  4) Print: element count, comparisons, writes, PASS/FAIL

INPUT FORMAT
------------
Files contain integers separated by whitespace (spaces/newlines/tabs).

STEP COUNTING MODEL
-------------------
We track two global counters:

  comparisons:
    - incremented each time we compare an array element to the pivot
      inside partition()

  writes:
    - incremented when we overwrite array positions during swaps

NOTE ABOUT SWAP "WRITE" COUNTING
--------------------------------
Python tuple-swap syntax:
    arr[i], arr[j] = arr[j], arr[i]

Conceptually, this overwrites TWO array slots (i and j), so we count it
as 2 writes. (Other languages may count 3 when using a temp variable.)

ALGORITHM NOTES
---------------
- Partition scheme: Lomuto partition
- Pivot choice: last element in the range (arr[right])
- Average time: O(n log n), worst case: O(n^2) with bad pivots
"""

from __future__ import annotations

import os
import sys

# pylint: disable=global-statement

# ------------------------------------------------------------
# GLOBAL STEP COUNTERS
# ------------------------------------------------------------
# We keep these global so partition() can update them without
# needing to pass counters through every recursive call.
comparisons = 0
writes = 0


# ------------------------------------------------------------
# Quick Sort implementation (in-place)
# ------------------------------------------------------------

def quick_sort(arr: list[int]) -> None:
    """Sort `arr` in-place using Quick Sort (Lomuto partition).

    Parameters
    ----------
    arr : list[int]
        The list of integers to be sorted in ascending order.

    Notes
    -----
    - This function sorts the list in-place (mutates arr).
    - Step counters are *not* reset here; the caller should reset
      `comparisons` and `writes` before running tests.
    """
    # If the array is empty, right becomes -1 and the recursive helper
    # immediately returns due to the base case.
    quick_sort_rec(arr, 0, len(arr) - 1)


def quick_sort_rec(arr: list[int], left: int, right: int) -> None:
    """Recursive Quick Sort helper that sorts arr[left..right].

    Base case:
      - If the subarray has 0 or 1 element (left >= right), it's sorted.

    Recursive case:
      - Partition the subarray, placing the pivot in its final position.
      - Recursively sort the left side and the right side.
    """
    if left >= right:
        return

    # Partition arr[left..right] and get pivot final index p
    p = partition(arr, left, right)

    # Recursively sort elements left of pivot
    quick_sort_rec(arr, left, p - 1)

    # Recursively sort elements right of pivot
    quick_sort_rec(arr, p + 1, right)


def partition(arr: list[int], left: int, right: int) -> int:
    """Lomuto partition scheme.

    Chooses the pivot as arr[right]. Reorders arr[left..right] so that:
      - all elements < pivot appear before pivot
      - pivot is placed at its final sorted position
      - all elements >= pivot appear after pivot

    Returns
    -------
    int
        The final index of the pivot after partitioning.

    Step counting
    -------------
    comparisons:
      - incremented once per check: arr[j] < pivot

    writes:
      - incremented by 2 for each swap (two array slots overwritten)
    """
    global comparisons, writes

    pivot = arr[right]   # pivot selection: last element
    i = left             # i marks the "next slot" for a < pivot value

    # Walk through the subarray excluding the pivot at `right`
    for j in range(left, right):
        comparisons += 1  # one comparison: arr[j] < pivot ?

        # Using "<" means values equal to pivot go to the right side.
        # Using "<=" would move equals left and can change behavior on duplicates.
        if arr[j] < pivot:
            # Swap arr[i] and arr[j] so the < pivot value moves left.
            # This overwrites two list positions -> 2 writes.
            arr[i], arr[j] = arr[j], arr[i]
            writes += 2
            i += 1

    # Finally, place pivot at index i by swapping arr[i] with arr[right].
    arr[i], arr[right] = arr[right], arr[i]
    writes += 2

    return i


# ------------------------------------------------------------
# File Loader with multiple fallback search paths
# ------------------------------------------------------------

def load_file(filename: str) -> list[int]:
    """Load an integer file using multiple fallback search paths.

    We try a variety of locations so this script works when executed from:
      - the lesson folder
      - section12 folder
      - project root
      - or other common working directories

    If the file is found:
      - prints the path used
      - returns a list of integers parsed from the file

    If not found:
      - prints diagnostics (cwd + all paths tried)
      - exits the program with error code 1
    """
    cwd = os.getcwd()

    # Determine this script's folder. This is useful because you might run:
    #   python example_10_quick_sort.py
    # from different working directories.
    script_dir = os.path.dirname(os.path.realpath(__file__))

    # Candidate paths (not exhaustive, but covers the layouts used so far)
    search_paths = [
        # 1) Working directory
        os.path.join(cwd, filename),
        os.path.join(cwd, "data", filename),

        # 2) Script directory
        os.path.join(script_dir, filename),
        os.path.join(script_dir, "data", filename),

        # 3) Typical lesson layout (lesson folder next to data/)
        os.path.join(script_dir, "..", "data", filename),
        os.path.join(script_dir, "..", "..", "data", filename),

        # 4) Sometimes you're in a nested folder and section12 is above
        os.path.join(script_dir, "..", "section12", "data", filename),
        os.path.join(script_dir, "..", "..", "section12", "data", filename),

        # 5) Running from project root
        os.path.join(cwd, "code_samples", "section12", "data", filename),
        os.path.join(cwd, "..", "code_samples", "section12", "data", filename),
    ]

    tried: list[str] = []

    for path in search_paths:
        tried.append(path)

        # Use exists() here because some paths may not be files (still fine).
        if os.path.exists(path):
            print(f"Loaded: {path}")
            with open(path, "r", encoding="utf-8") as f:
                # Split on any whitespace and parse as integers
                return [int(x) for x in f.read().split()]

    # If we get here, the file was not found in any candidate location.
    print(f"Error reading input file: {filename}")
    print(f"Working directory: {cwd}")
    print("Search paths attempted:")
    for p in tried:
        print("  " + p)
    print("Missing input file — aborting.")
    sys.exit(1)


# ------------------------------------------------------------
# Main execution / test harness
# ------------------------------------------------------------

if __name__ == "__main__":
    # Load the test arrays
    unordered = load_file("unordered.txt")
    expected = load_file("ordered.txt")

    # Reset global counters BEFORE sorting so results reflect only this run
    comparisons = 0
    writes = 0

    # Sort a COPY so we keep `unordered` intact for debugging if needed
    array = unordered.copy()
    quick_sort(array)

    # Verify sorted output matches the expected data exactly
    correct = (array == expected)

    # If not correct, print the first mismatch (helps pinpoint bugs quickly)
    if not correct:
        for idx, (a, b) in enumerate(zip(array, expected)):
            if a != b:
                print(f"Mismatch at index {idx}: got {a}, expected {b}")
                break

    # Print summary stats
    print("\nQuick Sort (Python)")
    print("-------------------")
    print(f"Elements:     {len(array)}")
    print(f"Comparisons:  {comparisons}")
    print(f"Writes:       {writes}")
    print(f"Correct?      {'YES ✔' if correct else 'NO ✘'}")

    # If correct, show a small sample for sanity
    if correct:
        print("\nFirst 10 sorted values:")
        print(" ".join(map(str, array[:10])))
