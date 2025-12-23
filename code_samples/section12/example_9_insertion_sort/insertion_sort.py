"""
Example 9 — Insertion Sort with Step Counting (Python)

PURPOSE
-------
This script demonstrates **Insertion Sort** on a list of integers while counting
two kinds of operations to illustrate time complexity:

  1) comparisons : how many times we compare array values to the current key
  2) writes      : how many times we write into the array (shifts + final insert)

It also verifies correctness by comparing the sorted output to a known-good
sorted file.

INPUT FILES
-----------
This program expects TWO files containing the SAME integers:

  unordered.txt  - unsorted integers
  ordered.txt    - the correctly sorted integers (ascending)

Integers may be separated by any whitespace (spaces, newlines, tabs).

RUNNING
-------
Run from your lesson folder (or anywhere, if the files are reachable via fallbacks):

  python example_9_insertion_sort.py

STEP COUNTING MODEL
-------------------
We count:
  - comparisons:
      each time we evaluate whether arr[j] > key
  - writes:
      each time we assign into arr[...] during sorting:
         * shifting:   arr[j + 1] = arr[j]
         * inserting:  arr[j + 1] = key

We do NOT count:
  - "key = arr[i]" as a write because it does not modify the array; it copies a
    value into a local variable.

COMPLEXITY (Big-O)
------------------
Let n be the number of elements.

Best case (already sorted):
  - comparisons ≈ n - 1   (one comparison per i, while-loop breaks immediately)
  - writes      ≈ n - 1   (we still write key back into position each pass)

Worst case (reverse sorted):
  - comparisons ≈ n(n - 1) / 2
  - writes      ≈ n(n - 1) / 2 + (n - 1)
    (lots of shifts + one final insert for each i)

NOTE ABOUT GLOBALS
------------------
This example uses global counters (comparisons, writes) so the test harness can
print them after sorting. In production code, you'd typically return them or
use a small Stats object instead of globals.
"""

import os
import sys

# Global counters (used only for demonstration / printing)
comparisons = 0
writes = 0

# pylint: disable=broad-exception-caught
# pylint: disable=global-statement

# ------------------------------------------------------------
# Insertion Sort (counts comparisons & writes)
# ------------------------------------------------------------
def insertion_sort(arr: list[int]) -> None:
    """
    Sorts `arr` IN PLACE using insertion sort.

    Side effects:
        - Modifies `arr` to be sorted ascending.
        - Updates global counters: `comparisons`, `writes`.

    Why insertion sort works:
        At iteration i, the subarray arr[0..i-1] is already sorted.
        We take arr[i] (the "key") and insert it into the correct position in
        that sorted region by shifting larger items one position to the right.
    """
    global comparisons, writes
    comparisons = 0
    writes = 0

    # Start at index 1 because a single element arr[0] is already "sorted".
    for i in range(1, len(arr)):
        # The element we want to insert into the sorted prefix.
        key = arr[i]

        # j walks backward through the sorted portion [0..i-1].
        j = i - 1

        # Shift elements that are greater than key to the right.
        #
        # Each loop iteration performs one comparison: arr[j] > key
        # If true, we shift arr[j] to arr[j+1], creating space for key.
        while j >= 0:
            comparisons += 1  # one comparison against key

            if arr[j] > key:
                # Shift arr[j] right by one position.
                arr[j + 1] = arr[j]
                writes += 1  # we wrote into the array

                # Move left and keep checking.
                j -= 1
            else:
                # arr[j] <= key => correct insertion point is j+1
                break

        # Insert key into the space we just opened.
        #
        # Even in the best case (already sorted), we still do this write.
        arr[j + 1] = key
        writes += 1


# ------------------------------------------------------------
# File loader: tries ../data/, ./data/, and current folder
# ------------------------------------------------------------
def load_file(filename: str) -> list[int]:
    """
    Load integers from a file using several likely paths.

    WHY MULTIPLE PATHS?
    -------------------
    Students often run scripts from different working directories, so relative
    paths can break. We try a few common layouts:

      1) ../data/<filename>  (typical if you run from a lesson folder)
      2) ./data/<filename>   (if you run from a parent folder)
      3) ./<filename>        (if you copied files into the run folder)

    If all attempts fail, we print diagnostics and exit.
    """
    wd = os.getcwd()

    attempts = [
        os.path.join(wd, "..", "data", filename),  # ../data/filename
        os.path.join(wd, "data", filename),        # ./data/filename
        os.path.join(wd, filename),                # ./filename
    ]

    for p in attempts:
        print(f"Trying: {p}")
        if os.path.exists(p):
            print(f"Loaded: {p}")

            try:
                # Read tokens as strings from the file, split on any whitespace,
                # then convert to ints.
                with open(p, "r", encoding="utf-8") as f:
                    # Collect all whitespace-separated tokens
                    tokens: list[str] = []
                    for line in f:
                        tokens.extend(line.split())

                return [int(tok) for tok in tokens]

            except Exception as ex:
                # Parsing / read error: show which file failed and why.
                print(f"Parse error in {p}: {ex}")
                sys.exit(1)

    # If none of the candidate paths worked, fail with helpful diagnostics.
    print(f"Error reading: {filename}")
    print(f"Working directory: {wd}")
    print("Missing input file — aborting.")
    sys.exit(1)


# ------------------------------------------------------------
# Test Harness
# ------------------------------------------------------------
if __name__ == "__main__":
    # Load test data
    unordered = load_file("unordered.txt")
    expected = load_file("ordered.txt")

    # Work on a copy so we preserve the original input list
    array = list(unordered)

    # Sort + count steps
    insertion_sort(array)

    # Verify correctness
    ok = array == expected

    # Print results
    print("\nInsertion Sort (Python)")
    print("-----------------------")
    print(f"Elements:     {len(array)}")
    print(f"Comparisons:  {comparisons}")
    print(f"Writes:       {writes}")
    print(f"Correct?      {'YES ✔' if ok else 'NO ✘'}")

    if ok:
        # Show a small sample of output for sanity.
        print("\nFirst 10 sorted values:")
        print(array[:10])
    else:
        # Print the first mismatch to help debugging quickly.
        print("First mismatch:")
        for idx, (a, b) in enumerate(zip(array, expected)):
            if a != b:
                print(f"Index {idx}: got {a}, expected {b}")
                break
