"""Example 7 - Bubble Sort."""

import os
import sys

# pylint: disable=broad-exception-caught
# pylint: disable=too-few-public-methods

# ============================================================
# Bubble Sort (baseline / no step counting)
# ============================================================
#
# This is a standard bubble sort that:
#   - Repeatedly passes through the list
#   - Swaps adjacent out-of-order items
#   - Stops early when a full pass does not perform any swaps
#
# Optimization used:
#   - After each pass, the largest remaining element is guaranteed to
#     be at the end of the current range, so we shrink `n` by 1.
#
# Side effects:
#   - Sorts `arr` in place (modifies the input list).
#
def bubble_sort(arr: list[int]) -> None:
    """Bubble sort."""
    n = len(arr)
    swapped = True

    # Continue passes until a pass completes with no swaps
    while swapped:
        swapped = False

        # Compare adjacent pairs within the current range [0..n-1]
        for i in range(1, n):
            if arr[i - 1] > arr[i]:
                # Swap adjacent elements
                arr[i - 1], arr[i] = arr[i], arr[i - 1]
                swapped = True

        # After each pass, the final element in this range is in correct position
        n -= 1


# ------------------------------------------------------------
# Step-counter structure
# ------------------------------------------------------------
#
# This class stores the "work counts" for bubble sort.
#
# Definitions:
#   - comparisons: number of adjacent comparisons performed
#       (the check: arr[i - 1] > arr[i])
#   - swaps: number of swaps actually executed
#
# Note:
#   - The script uses an instance of StepCounter as a mutable container
#     so bubble_sort_steps() can update counters without returning them.
#
class StepCounter:
    """Model for step counting."""

    def __init__(self):
        # Total number of adjacent comparisons performed
        self.comparisons = 0

        # Total number of swaps performed
        self.swaps = 0


# ------------------------------------------------------------
# Bubble Sort with step counting
# ------------------------------------------------------------
#
# Same bubble sort algorithm as bubble_sort(), but increments counters.
#
# Counts tracked:
#   - stats.comparisons: incremented once per adjacent comparison
#   - stats.swaps: incremented once per actual swap
#
# Important:
#   - Sorts `arr` in place.
#   - Does not reset stats; caller is responsible for creating a fresh
#     StepCounter (or manually clearing it) before calling if desired.
#
def bubble_sort_steps(arr: list[int], stats: StepCounter) -> None:
    """Bubble sort with step counting."""
    n = len(arr)
    swapped = True

    # Continue until a pass completes with no swaps
    while swapped:
        swapped = False

        # One full pass through the current unsorted range
        for i in range(1, n):
            # Count each adjacent comparison
            stats.comparisons += 1

            # Swap if out of order
            if arr[i - 1] > arr[i]:
                arr[i - 1], arr[i] = arr[i], arr[i - 1]
                stats.swaps += 1
                swapped = True

        # Largest element in this range is now in place
        n -= 1


# ------------------------------------------------------------
# Auto-detect project root (fixes your Python path issue)
# ------------------------------------------------------------
#
# This helper tries to locate a directory that contains "code_samples"
# by walking upward from the current working directory.
#
# Why:
#   - When you run scripts from different folders (VS Code, terminal, etc.),
#     os.getcwd() can vary, which breaks simple relative paths.
#
# Behavior:
#   - Starts at current working directory
#   - Moves up until it finds a folder containing "code_samples"
#   - If it reaches filesystem root with no match, falls back to os.getcwd()
#
def find_project_root() -> str:
    """Locate project root."""
    cur = os.getcwd()

    while True:
        # If the current directory contains "code_samples", treat it as root
        if "code_samples" in os.listdir(cur):
            return cur

        # Move up one level
        parent = os.path.dirname(cur)

        # If we can no longer move up, we've hit the filesystem root
        if parent == cur:
            return os.getcwd()

        cur = parent


# ------------------------------------------------------------
# Load a file from code_samples/section12/data
# ------------------------------------------------------------
#
# Loads whitespace-delimited integers from:
#   <project_root>/code_samples/section12/data/<filename>
#
# Returns:
#   - list[int] on success
#   - None on failure
#
# Prints:
#   - the exact path it tries, which helps with debugging.
#
def load_file(filename: str) -> list[int] | None:
    """Load integers from file relative to script location."""
    try:
        root = find_project_root()

        # Construct full path to the data file
        full = os.path.join(root, "code_samples", "section12", "data", filename)

        print(f"Attempting to read: {full}")

        # Read entire file and split on whitespace into integers
        with open(full, "r", encoding="utf-8") as f:
            return [int(x) for x in f.read().split()]

    except Exception as ex:
        # Handles missing file, permission issues, parse errors, etc.
        print(f"Error reading {filename}: {ex}")
        return None


# ------------------------------------------------------------
# Compare arrays for mismatches
# ------------------------------------------------------------
#
# Compares two lists element-by-element up to the shorter length.
# Prints only the first 10 mismatches so the output stays readable.
#
# Returns:
#   - total mismatch count
#
def compare_arrays(a: list[int], b: list[int]) -> int:
    """Compare arrays for mismatches."""
    mismatches = 0

    # Compare only within bounds of both arrays (defensive)
    for i in range(min(len(a), len(b))):
        if a[i] != b[i]:
            if mismatches < 10:
                print(f"Mismatch @ {i}: got {a[i]}, expected {b[i]}")
            mismatches += 1

    return mismatches


# ------------------------------------------------------------
# Test harness (top-level execution)
# ------------------------------------------------------------
#
# Workflow:
#   1) Load unordered.txt (input)
#   2) Load ordered.txt (expected output)
#   3) Validate both loaded and have equal lengths
#   4) Run bubble_sort_steps() on unordered data
#   5) Print stats and verify output against expected
#
unordered = load_file("unordered.txt")
expected = load_file("ordered.txt")

# Abort if either file could not be loaded
if unordered is None or expected is None:
    print("Missing input file — aborting.")
    sys.exit()

# Abort if lengths differ (comparison would be invalid)
if len(unordered) != len(expected):
    print("File lengths differ — aborting.")
    print(f"unordered: {len(unordered)}, expected: {len(expected)}")
    sys.exit()

# Create step counter for this run
counts = StepCounter()

print("\n--- Bubble Sort Step Count ---")

# Sort IN PLACE and count comparisons/swaps
bubble_sort_steps(unordered, counts)

# Report statistics
print(f"Comparisons: {counts.comparisons}")
print(f"Swaps:       {counts.swaps}")

# Verify correctness
print("\nChecking sorted output...")
mismatch_count = compare_arrays(unordered, expected)

if mismatch_count == 0:
    print("SUCCESS — output matches expected sorted list!")
else:
    print(f"FAIL — mismatches found: {mismatch_count}")
