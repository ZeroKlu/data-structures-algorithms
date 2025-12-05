"""Example 7 - Bubble Sort."""

import os
import sys

#pylint: disable=broad-exception-caught
#pylint: disable=too-few-public-methods

def bubble_sort(arr: list[int]) -> None:
    """Bubble sort."""
    n = len(arr)
    swapped = True
    while swapped:
        swapped = False
        for i in range(1, n):
            if arr[i - 1] > arr[i]:
                arr[i - 1], arr[i] = arr[i], arr[i - 1]
                swapped = True
        n -= 1

# ------------------------------------------------------------
# Step-counter structure
# ------------------------------------------------------------
class StepCounter:
    """Model for step counting."""
    def __init__(self):
        self.comparisons = 0
        self.swaps = 0


# ------------------------------------------------------------
# Bubble Sort with step counting
# ------------------------------------------------------------
def bubble_sort_steps(arr: list[int], stats: StepCounter) -> None:
    """Bubble sort with step counting."""
    n = len(arr)
    swapped = True

    while swapped:
        swapped = False
        for i in range(1, n):
            stats.comparisons += 1
            if arr[i - 1] > arr[i]:
                arr[i - 1], arr[i] = arr[i], arr[i - 1]
                stats.swaps += 1
                swapped = True
        n -= 1


# ------------------------------------------------------------
# Auto-detect project root (fixes your Python path issue)
# ------------------------------------------------------------
def find_project_root() -> str:
    """Locate project root."""
    cur = os.getcwd()
    while True:
        if "code_samples" in os.listdir(cur):
            return cur
        parent = os.path.dirname(cur)
        if parent == cur:  # reached FS root
            return os.getcwd()
        cur = parent


# ------------------------------------------------------------
# Load a file from code_samples/section12/data
# ------------------------------------------------------------
def load_file(filename: str) -> list[int] | None:
    """Load integers from file relative to script location."""
    try:
        root = find_project_root()
        full = os.path.join(root, "code_samples", "section12", "data", filename)

        print(f"Attempting to read: {full}")

        with open(full, "r", encoding="utf-8") as f:
            return [int(x) for x in f.read().split()]
    except Exception as ex:
        print(f"Error reading {filename}: {ex}")
        return None


# ------------------------------------------------------------
# Compare arrays for mismatches
# ------------------------------------------------------------
def compare_arrays(a: list[int], b: list[int]) -> int:
    """Compare arrays for mismatches."""
    mismatches = 0
    for i in range(min(len(a), len(b))):
        if a[i] != b[i]:
            if mismatches < 10:
                print(f"Mismatch @ {i}: got {a[i]}, expected {b[i]}")
            mismatches += 1
    return mismatches


# ------------------------------------------------------------
# Test harness (top-level execution)
# ------------------------------------------------------------
unordered = load_file("unordered.txt")
expected = load_file("ordered.txt")

if unordered is None or expected is None:
    print("Missing input file — aborting.")
    sys.exit()

if len(unordered) != len(expected):
    print("File lengths differ — aborting.")
    print(f"unordered: {len(unordered)}, expected: {len(expected)}")
    sys.exit()

counts = StepCounter()
print("\n--- Bubble Sort Step Count ---")
bubble_sort_steps(unordered, counts)

print(f"Comparisons: {counts.comparisons}")
print(f"Swaps:       {counts.swaps}")

print("\nChecking sorted output...")
mismatch_count = compare_arrays(unordered, expected)

if mismatch_count == 0:
    print("SUCCESS — output matches expected sorted list!")
else:
    print(f"FAIL — mismatches found: {mismatch_count}")
