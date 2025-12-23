"""Example 6 - Selection Sort with step counting and file tests."""

from pathlib import Path
import os

# pylint: disable=broad-exception-caught

# ============================================================
# Selection Sort with step counting
# ============================================================
#
# This implementation performs classic selection sort while
# counting:
#   - comparisons: how many element-to-element comparisons occur
#   - swaps: how many actual swaps are performed
#
# The input list is sorted IN PLACE.
#
# Returns:
#   (comparisons, swaps)
#
def selection_sort(arr: list[int]) -> tuple[int, int]:
    """Selection sort with comparison and swap counting."""
    comparisons = 0
    swaps = 0

    n = len(arr)

    # Outer loop selects the next position i to fill
    for i in range(n):
        min_idx = i

        # Inner loop finds the smallest element
        # in the unsorted suffix [i+1 .. n-1]
        for j in range(i + 1, n):
            comparisons += 1  # count every comparison
            if arr[j] < arr[min_idx]:
                min_idx = j

        # Swap only if a new minimum was found
        if min_idx != i:
            arr[i], arr[min_idx] = arr[min_idx], arr[i]
            swaps += 1  # count actual swap

    return comparisons, swaps


# ============================================================
# Script directory helper
# ============================================================
#
# Returns the absolute directory containing the currently
# running Python script (not the working directory).
#
def script_dir() -> Path:
    """Returns directory of the currently-running script."""
    return Path(os.path.abspath(os.path.dirname(__file__)))


# ============================================================
# File loader
# ============================================================
#
# Loads a whitespace-delimited list of integers from:
#
#   <script_dir>/../data/<filename>
#
# Returns:
#   - list[int] on success
#   - None on failure (missing file, parse error, etc.)
#
def load_file(filename: str) -> list[int] | None:
    """Load file relative to: <script_dir>/../data/<filename>."""
    try:
        # Base directory where this script lives
        base = script_dir()

        # Construct absolute path to data file
        full = (base / ".." / "data" / filename).resolve()

        # Diagnostic output for path debugging
        print(f"Attempting to read: {full}")

        # Read entire file and parse integers
        text = full.read_text()
        return [int(x) for x in text.split()]
    except Exception as ex:
        print(f"Error reading: {filename}")
        print(ex)
        return None


# ============================================================
# Main test harness
# ============================================================
#
# Workflow:
#   1) Load unordered.txt (unsorted input)
#   2) Load ordered.txt   (reference sorted output)
#   3) Run selection sort with step counting
#   4) Compare result against ordered.txt
#
def main():
    """Run tests."""

    # --------------------------------------------------
    # Load unordered input
    # --------------------------------------------------
    unordered = load_file("unordered.txt")
    if unordered is None:
        print("Missing unordered file - aborting.")
        return

    # --------------------------------------------------
    # Load ordered reference output
    # --------------------------------------------------
    ordered = load_file("ordered.txt")
    if ordered is None:
        print("Missing ordered file - aborting.")
        return

    # Sanity check: both files must have same length
    if len(unordered) != len(ordered):
        print("Length mismatch - aborting.")
        return

    print("\n--- Selection Sort (Step Counting) ---")

    # Work on a copy so original unordered data remains unchanged
    working = list(unordered)

    # Run selection sort and capture statistics
    comparisons, swaps = selection_sort(working)

    print(f"Comparisons: {comparisons}")
    print(f"Swaps:       {swaps}")

    # --------------------------------------------------
    # Verify sorted output against ordered.txt
    # --------------------------------------------------
    print("\nComparing to ordered.txt...")

    mismatches = 0
    for i, (a, b) in enumerate(zip(working, ordered)):
        if a != b:
            # Print only the first few mismatches
            if mismatches < 10:
                print(f"Mismatch @ index {i}: got {a}, expected {b}")
            mismatches += 1

    # Final verdict
    if mismatches == 0:
        print("SUCCESS: Output matches ordered.txt")
    else:
        print(f"FAIL: {mismatches} mismatches total.")


# ============================================================
# Script entry point
# ============================================================
if __name__ == "__main__":
    main()
