"""
Example 11: Heap Sort (with comparisons & writes, plus file testing).

PURPOSE
-------
Demonstrate Heap Sort on a list of integers while counting:
  - comparisons: value comparisons used to decide heap ordering
  - writes:      array writes caused by swaps (counted as 3 writes per swap)

INPUT FILES
-----------
This script expects TWO files containing the SAME integers:
  - unordered.txt : integers in arbitrary order
  - ordered.txt   : the same integers sorted ascending

Integers may be separated by any whitespace (spaces, newlines, tabs).

WHAT WE COUNT (Step Counting Rules)
-----------------------------------
comparisons:
  - We count only comparisons of DATA values that determine ordering:
      arr[left]  > arr[largest]
      arr[right] > arr[largest]
  - We do NOT count loop condition checks, index comparisons like (left < n),
    or recursion overhead, since those are not directly comparing data values.

writes:
  - We count only WRITES to the array caused by swapping two elements.
  - We treat a swap as 3 writes (temp, arr[a], arr[b]) to match the convention
    used in your other languages (C/C++/Java/C# examples).
  - Python tuple-swap is atomic at the language level, but conceptually it still
    “moves” values; we count it the same way for consistent analysis.

HEAP SORT OVERVIEW (MAX-HEAP)
-----------------------------
Heap Sort works in two phases:

1) Build a MAX-HEAP
   - Reorganize the array so the largest element is at index 0
   - Every parent is >= its children

2) Repeatedly extract the maximum
   - Swap index 0 (max) with the last element in the heap
   - Reduce the heap size
   - Restore heap property via heapify()

Time complexity:  O(n log n)
Space complexity: O(1) extra (in-place), aside from file loading
"""

import os
import sys

# ------------------------------------------------------------
# Global counters (reset before sorting)
# ------------------------------------------------------------
comparisons = 0
writes = 0

#pylint: disable=global-statement
#pylint: disable=consider-using-enumerate

# ------------------------------------------------------------
# Heap sort implementation
# ------------------------------------------------------------

def heapify(arr, n, i):
    """
    Restore the MAX-HEAP property for the subtree rooted at index i.

    Parameters
    ----------
    arr : list[int]
        The array being heap-sorted (treated as a binary heap).
    n : int
        Current heap size. Only indices [0 .. n-1] are considered in the heap.
        (The suffix arr[n:] is already sorted during the extraction phase.)
    i : int
        Root index of the subtree we want to fix.

    How it works
    ------------
    A node i in a binary heap has:
      left child  at 2*i + 1
      right child at 2*i + 2

    We:
      1) Find the largest among {i, left, right} (within heap bounds).
      2) If a child is larger than the parent, swap parent with that child.
      3) Recursively heapify the affected child subtree, because the swap may
         have violated heap order further down.

    Step counting
    -------------
    - comparisons is incremented once each time we compare heap values:
        arr[left] > arr[largest]
        arr[right] > arr[largest]
    - writes is incremented by 3 each time we perform a swap.
    """
    global comparisons, writes

    largest = i
    left = 2 * i + 1
    right = 2 * i + 2

    # Compare left child to current largest (if it exists)
    if left < n:
        comparisons += 1
        if arr[left] > arr[largest]:
            largest = left

    # Compare right child to current largest (if it exists)
    if right < n:
        comparisons += 1
        if arr[right] > arr[largest]:
            largest = right

    # If the parent wasn't the largest, swap and keep fixing downward
    if largest != i:
        # Swap values in the array (conceptually 3 writes)
        arr[i], arr[largest] = arr[largest], arr[i]
        writes += 3

        # Recursively heapify the subtree where we moved the old parent
        heapify(arr, n, largest)


def heap_sort(arr):
    """
    Heap sort in-place using a MAX-HEAP.

    Parameters
    ----------
    arr : list[int]
        Array to sort in place.

    Algorithm phases
    ----------------
    1) Build max heap:
       - Start from the last internal node (n//2 - 1) and heapify down to 0.
       - Leaves (indices >= n//2) are already heaps of size 1.

    2) Extract max repeatedly:
       - Swap arr[0] (current max) with arr[end] (end of heap)
       - Decrease heap size to `end`
       - heapify the new root to restore heap property

    Step counting
    -------------
    - heapify() handles comparisons and swap writes.
    - We also count writes for the root/end swap during extraction (3 writes).
    """
    # global comparisons, writes
    global writes

    n = len(arr)

    # (Optional safety) Reset counters here if you want heap_sort() to be self-contained.
    # Your main already resets counters, but this prevents accidental reuse.
    # comparisons = 0
    # writes = 0

    # -----------------------------
    # Phase 1: Build max heap
    # -----------------------------
    # The last index that can have children is (n//2 - 1).
    for i in range(n // 2 - 1, -1, -1):
        heapify(arr, n, i)

    # -----------------------------
    # Phase 2: Extract max elements
    # -----------------------------
    # Move current max to the end, shrink heap, and fix root again.
    for end in range(n - 1, 0, -1):
        # Swap max element at root into its final sorted position at `end`
        arr[0], arr[end] = arr[end], arr[0]
        writes += 3  # count swap as 3 writes for consistency

        # Restore heap property on the reduced heap [0..end-1]
        heapify(arr, end, 0)


# ------------------------------------------------------------
# File loader (tries multiple common paths)
# ------------------------------------------------------------

def load_file(filename):
    """
    Load integers from a text file using multiple fallback search paths.

    Why multiple paths?
    -------------------
    Depending on how you run Python (VS Code, terminal, from repo root, from
    lesson folder, etc.), the working directory can be different. These fallbacks
    make the script resilient.

    Search order
    ------------
    We try each path in `attempts` and return the first one found.

    Returns
    -------
    list[int]
        The integers from the file.

    Exits
    -----
    If the file cannot be found in any candidate location, prints diagnostics and
    exits the program with status code 1.
    """
    cwd = os.getcwd()

    attempts = [
        # Common: file in current working directory
        os.path.join(cwd, filename),

        # Common when run from repo root
        os.path.join(cwd, "code_samples", "section12", "data", filename),

        # Alternative layouts
        os.path.join(cwd, "data", filename),
        os.path.join(cwd, "..", "data", filename),
        os.path.join(cwd, "..", "..", "data", filename),

        # section12 folder variants
        os.path.join(cwd, "section12", "data", filename),
        os.path.join(cwd, "..", "section12", "data", filename),
        os.path.join(cwd, "..", "..", "section12", "data", filename),
    ]

    for path in attempts:
        if os.path.exists(path):
            print(f"Loaded: {path}")
            with open(path, "r", encoding="utf-8") as f:
                # Split on whitespace and parse as ints
                return list(map(int, f.read().split()))

    # If none worked, print debugging info and exit.
    print(f"Error reading input file: {filename}")
    print(f"Working directory: {cwd}")
    print("Search paths attempted:")
    for p in attempts:
        print("  " + p)
    print("Missing input file — aborting.")
    sys.exit(1)


# ------------------------------------------------------------
# Main test harness
# ------------------------------------------------------------

if __name__ == "__main__":
    # Load test data
    unordered = load_file("unordered.txt")
    expected = load_file("ordered.txt")

    # Sanity check: the files should contain the same number of integers
    if len(unordered) != len(expected):
        print("Length mismatch between unordered and ordered files.")
        print(f"unordered length = {len(unordered)}")
        print(f"expected length  = {len(expected)}")
        sys.exit(1)

    # Copy unordered so we don't mutate the original loaded list
    array = unordered[:]

    # Reset counters for this run
    comparisons = 0
    writes = 0

    # Sort
    heap_sort(array)

    # Validate: compare sorted output to expected output
    ok = True
    bad_index = -1

    for x in range(len(array)):
        if array[x] != expected[x]:
            ok = False
            bad_index = x
            print(f"Mismatch at index {x}: got {array[x]}, expected {expected[x]}")
            break

    # Report results
    print()
    print("Heap Sort (Python)")
    print("------------------")
    print(f"Elements:     {len(array)}")
    print(f"Comparisons:  {comparisons}")
    print(f"Writes:       {writes}")
    print(f"Correct?      {'YES ✓' if ok else 'NO ✗'}")

    # Optional: show a small sample when correct (helps confirm output quickly)
    if ok:
        print("\nFirst 10 sorted values:")
        print(array[:10])
