"""Lesson 3 - Binary Search."""

import os

# pylint: disable=broad-exception-caught

def binary_search(arr: list[int], target: int) -> int:
    """Binary search."""
    lo, hi = 0, len(arr) - 1
    while lo <= hi:
        mid = lo + (hi - lo) // 2
        if arr[mid] == target:
            return mid
        if arr[mid] < target:
            lo = mid + 1
        else:
            hi = mid - 1
    return -1


def binary_search_steps(arr: list[int], target: int) -> tuple[int, int]:
    """Binary search that also returns number of steps."""
    lo, hi = 0, len(arr) - 1
    steps = 0
    while lo <= hi:
        steps += 1
        mid = lo + (hi - lo) // 2
        if arr[mid] == target:
            return mid, steps
        if arr[mid] < target:
            lo = mid + 1
        else:
            hi = mid - 1
    return -1, steps


def load_ordered_file() -> list[int]:
    """
    Load ordered.txt located at:
        code_samples/section12/data/ordered.txt
    relative to this script's directory.
    """
    # path to this .py file
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # go up one level into section12/, then into data/
    file_path = os.path.join(script_dir, "..", "data", "ordered.txt")
    file_path = os.path.abspath(file_path)

    print(f"Attempting to read: {file_path}")

    try:
        with open(file_path, "r", encoding="utf-8") as f:
            text = f.read()
        return [int(x) for x in text.split()]
    except Exception as e:
        print("Error reading ordered.txt")
        print("Working directory:", os.getcwd())
        print("Exception:", e)
        return []


def run_tests(arr: list[int]) -> None:
    """Run tests."""
    print("\n=== Binary Search Tests ===")
    print(f"Loaded {len(arr)} integers\n")

    # First element
    idx, steps = binary_search_steps(arr, arr[0])
    print(f"Search first ({arr[0]}): index={idx}, steps={steps}")

    # Middle element
    mid_val = arr[len(arr) // 2]
    idx, steps = binary_search_steps(arr, mid_val)
    print(f"Search middle ({mid_val}): index={idx}, steps={steps}")

    # Last element
    idx, steps = binary_search_steps(arr, arr[-1])
    print(f"Search last ({arr[-1]}): index={idx}, steps={steps}")

    # Missing element
    idx, steps = binary_search_steps(arr, 999_999)
    print(f"Search missing (999999): index={idx}, steps={steps}")

def main() -> None:
    """Main."""
    arr = load_ordered_file()
    if not arr:
        print("Missing input file - aborting.")
        return

    run_tests(arr)

if __name__ == "__main__":
    main()
