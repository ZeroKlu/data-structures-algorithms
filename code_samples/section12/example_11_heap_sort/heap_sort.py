"""Example 11: Heap Sort (with comparisons & writes, plus file testing)."""

import os
import sys

# ------------------------------------------------------------
# Global counters
# ------------------------------------------------------------
comparisons = 0
writes = 0

#pylint: disable=global-statement
#pylint: disable=consider-using-enumerate

# ------------------------------------------------------------
# Heap sort implementation
# ------------------------------------------------------------

def heapify(arr, n, i):
    """Max-heapify at index i."""
    global comparisons, writes

    largest = i
    left = 2 * i + 1
    right = 2 * i + 2

    if left < n:
        comparisons += 1
        if arr[left] > arr[largest]:
            largest = left

    if right < n:
        comparisons += 1
        if arr[right] > arr[largest]:
            largest = right

    if largest != i:
        arr[i], arr[largest] = arr[largest], arr[i]
        writes += 3
        heapify(arr, n, largest)


def heap_sort(arr):
    """Heap sort."""
    global writes
    n = len(arr)

    # Build max heap
    for i in range(n // 2 - 1, -1, -1):
        heapify(arr, n, i)

    # Extract elements
    for end in range(n - 1, 0, -1):
        arr[0], arr[end] = arr[end], arr[0]
        writes += 3
        heapify(arr, end, 0)


# ------------------------------------------------------------
# File loader (tries multiple common paths)
# ------------------------------------------------------------

def load_file(filename):
    """Load file with multiple fallback search paths."""
    cwd = os.getcwd()

    attempts = [
        os.path.join(cwd, filename),
        os.path.join(cwd, "code_samples", "section12", "data", filename),
        os.path.join(cwd, "data", filename),
        os.path.join(cwd, "..", "data", filename),
        os.path.join(cwd, "..", "..", "data", filename),
        os.path.join(cwd, "section12", "data", filename),
        os.path.join(cwd, "..", "section12", "data", filename),
        os.path.join(cwd, "..", "..", "section12", "data", filename),
    ]

    for path in attempts:
        if os.path.exists(path):
            print(f"Loaded: {path}")
            with open(path, "r", encoding="utf-8") as f:
                return list(map(int, f.read().split()))

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
    unordered = load_file("unordered.txt")
    expected = load_file("ordered.txt")

    if len(unordered) != len(expected):
        print("Length mismatch between unordered and ordered files.")
        print(f"unordered length = {len(unordered)}")
        print(f"expected length = {len(expected)}")
        sys.exit(1)

    array = unordered[:]  # copy
    comparisons = 0
    writes = 0

    heap_sort(array)

    # Validate
    ok = True
    for x in range(len(array)):
        if array[x] != expected[x]:
            print(f"Mismatch at index {x}: got {array[x]}, expected {expected[x]}")
            ok = False
            break

    print()
    print("Heap Sort (Python)")
    print("------------------")
    print(f"Elements:     {len(array)}")
    print(f"Comparisons:  {comparisons}")
    print(f"Writes:       {writes}")
    print(f"Correct?      {'YES ✓' if ok else 'NO ✗'}")
