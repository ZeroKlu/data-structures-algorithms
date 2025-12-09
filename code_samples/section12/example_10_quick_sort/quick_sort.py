"""Example 10: Quick Sort with step counting."""

import os
import sys

#pylint: disable=global-statement

# Global counters
comparisons = 0
writes = 0

# ------------------------------------------------------------
# Quick Sort implementation
# ------------------------------------------------------------

def quick_sort(arr: list[int]) -> None:
    """Quick sort."""
    quick_sort_rec(arr, 0, len(arr) - 1)

def quick_sort_rec(arr: list[int], left: int, right: int) -> None:
    """Quick sort recursive helper."""
    if left >= right:
        return
    p = partition(arr, left, right)
    quick_sort_rec(arr, left, p - 1)
    quick_sort_rec(arr, p + 1, right)

def partition(arr: list[int], left: int, right: int) -> int:
    """Lomuto partition."""
    global comparisons, writes

    pivot = arr[right]
    i = left

    for j in range(left, right):
        comparisons += 1
        if arr[j] < pivot:
            arr[i], arr[j] = arr[j], arr[i]
            writes += 2
            i += 1

    arr[i], arr[right] = arr[right], arr[i]
    writes += 2
    return i

# ------------------------------------------------------------
# File Loader with multiple fallback search paths
# ------------------------------------------------------------

def load_file(filename: str) -> list[int]:
    """Load file with multiple fallback search paths."""
    cwd = os.getcwd()

    # Determine this script's folder
    script_dir = os.path.dirname(os.path.realpath(__file__))

    search_paths = [
        os.path.join(cwd, filename),
        os.path.join(cwd, "data", filename),
        os.path.join(script_dir, filename),
        os.path.join(script_dir, "data", filename),
        os.path.join(script_dir, "..", "data", filename),
        os.path.join(script_dir, "..", "..", "data", filename),
        os.path.join(script_dir, "..", "section12", "data", filename),
        os.path.join(script_dir, "..", "..", "section12", "data", filename),
        os.path.join(cwd, "code_samples", "section12", "data", filename),
        os.path.join(cwd, "..", "code_samples", "section12", "data", filename),
    ]

    tried = []

    for path in search_paths:
        tried.append(path)
        if os.path.exists(path):
            print(f"Loaded: {path}")
            with open(path, "r", encoding="utf-8") as f:
                return [int(x) for x in f.read().split()]

    print(f"Error reading input file: {filename}")
    print(f"Working directory: {cwd}")
    print("Search paths attempted:")
    for p in tried:
        print("  " + p)
    print("Missing input file — aborting.")
    sys.exit(1)

# ------------------------------------------------------------
# Main execution
# ------------------------------------------------------------

if __name__ == "__main__":
    unordered = load_file("unordered.txt")
    expected = load_file("ordered.txt")

    # Reset counters
    comparisons = 0
    writes = 0

    array = unordered.copy()
    quick_sort(array)

    # Verify result
    correct = array == expected
    if not correct:
        for x, (a, b) in enumerate(zip(array, expected)):
            if a != b:
                print(f"Mismatch at index {x}: got {a}, expected {b}")
                break

    print("\nQuick Sort (Python)")
    print("-------------------")
    print(f"Elements:     {len(array)}")
    print(f"Comparisons:  {comparisons}")
    print(f"Writes:       {writes}")
    print(f"Correct?      {'YES ✔' if correct else 'NO ✘'}")
