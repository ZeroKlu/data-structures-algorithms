"""Example 9 — Insertion Sort with Step Counting"""

import os
import sys

comparisons = 0
writes = 0

#pylint: disable=broad-exception-caught
#pylint: disable=global-statement

# ------------------------------------------------------------
# Insertion Sort (counts comparisons & writes)
# ------------------------------------------------------------
def insertion_sort(arr):
    """Insertion sort."""
    global comparisons, writes
    comparisons = 0
    writes = 0

    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1

        # Compare & shift
        while j >= 0:
            comparisons += 1
            if arr[j] > key:
                arr[j + 1] = arr[j]
                writes += 1
                j -= 1
            else:
                break

        # Insert key at correct position
        arr[j + 1] = key
        writes += 1

    return arr

# ------------------------------------------------------------
# File loader: tries ../data/, ./data/, and current folder
# ------------------------------------------------------------
def load_file(filename):
    """Load integers from file relative to script location."""
    wd = os.getcwd()

    attempts = [
        os.path.join(wd, "..", "data", filename),   # ../data/filename
        os.path.join(wd, "data", filename),         # ./data/filename
        os.path.join(wd, filename),                 # ./filename
    ]

    for path in attempts:
        print(f"Trying: {path}")
        if os.path.exists(path):
            print(f"Loaded: {path}")
            try:
                with open(path, "r", encoding="utf-8") as f:
                    tokens = []
                    for line in f:
                        tokens.extend(line.split())
                    return list(map(int, tokens))
            except Exception as e:
                print(f"Parse error in {path}: {e}")
                sys.exit(1)

    print(f"Error reading: {filename}")
    print(f"Working directory: {wd}")
    print("Missing input file — aborting.")
    sys.exit(1)

# ------------------------------------------------------------
# Test Harness
# ------------------------------------------------------------
if __name__ == "__main__":
    unordered = load_file("unordered.txt")
    expected = load_file("ordered.txt")

    array = list(unordered)
    insertion_sort(array)

    ok = array == expected
    print("\nInsertion Sort (Python)")
    print("-----------------------")
    print(f"Elements:     {len(array)}")
    print(f"Comparisons:  {comparisons}")
    print(f"Writes:       {writes}")
    print(f"Correct?      {'YES ✔' if ok else 'NO ✘'}")

    if ok:
        print("\nFirst 10 sorted values:")
        print(array[:10])
    else:
        print("First mismatch:")
        for x, (a, b) in enumerate(zip(array, expected)):
            if a != b:
                print(f"Index {x}: got {a}, expected {b}")
                break
