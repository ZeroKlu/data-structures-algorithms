"""Example 6 - Selection Sort with step counting and file tests."""

from pathlib import Path
import os

#pylint: disable=broad-exception-caught

def selection_sort(arr: list[int]) -> tuple[int, int]:
    """Selection sort with comparison and swap counting."""
    comparisons = 0
    swaps = 0

    n = len(arr)
    for i in range(n):
        min_idx = i
        for j in range(i + 1, n):
            comparisons += 1
            if arr[j] < arr[min_idx]:
                min_idx = j

        if min_idx != i:
            arr[i], arr[min_idx] = arr[min_idx], arr[i]
            swaps += 1

    return comparisons, swaps


def script_dir() -> Path:
    """Returns directory of the currently-running script."""
    return Path(os.path.abspath(os.path.dirname(__file__)))


def load_file(filename: str) -> list[int] | None:
    """Load file relative to: <script_dir>/../data/<filename>."""
    try:
        base = script_dir()
        full = (base / ".." / "data" / filename).resolve()

        print(f"Attempting to read: {full}")

        text = full.read_text()
        return [int(x) for x in text.split()]
    except Exception as ex:
        print(f"Error reading: {filename}")
        print(ex)
        return None


def main():
    """Run tests."""
    unordered = load_file("unordered.txt")
    if unordered is None:
        print("Missing unordered file - aborting.")
        return

    ordered = load_file("ordered.txt")
    if ordered is None:
        print("Missing ordered file - aborting.")
        return

    if len(unordered) != len(ordered):
        print("Length mismatch - aborting.")
        return

    print("\n--- Selection Sort (Step Counting) ---")

    working = list(unordered)
    comparisons, swaps = selection_sort(working)

    print(f"Comparisons: {comparisons}")
    print(f"Swaps:       {swaps}")

    print("\nComparing to ordered.txt...")

    mismatches = 0
    for i, (a, b) in enumerate(zip(working, ordered)):
        if a != b:
            if mismatches < 10:
                print(f"Mismatch @ index {i}: got {a}, expected {b}")
            mismatches += 1

    if mismatches == 0:
        print("SUCCESS: Output matches ordered.txt")
    else:
        print(f"FAIL: {mismatches} mismatches total.")


if __name__ == "__main__":
    main()
