"""Lesson 6 - Exponential Search (with step counting and file-based tests)."""

from __future__ import annotations
import os
import math

# pylint: disable=broad-exception-caught
# pylint: disable=line-too-long


def exponential_search(arr: list[int], target: int) -> tuple[int, int]:
    """
    Perform exponential search on a sorted array.

    Exponential search works in two main phases:
      1) Exponential range expansion:
         Repeatedly double an index (1, 2, 4, 8, ...) until the target
         is less than or equal to arr[bound], or the bound exceeds the array.
      2) Binary search:
         Once a candidate range is identified, perform binary search
         within that range.

    Returns:
        (index, step_count)
        - index: position of target in arr, or -1 if not found
        - step_count: approximate number of comparisons performed

    Assumes:
        - `arr` is sorted in ascending order
    """
    steps = 0                # Counts the number of comparison steps
    n = len(arr)             # Number of elements in the array

    # Handle empty array case
    if n == 0:
        return -1, steps

    # --------------------------------------------------
    # Step 0: Check the first element directly
    # --------------------------------------------------
    steps += 1
    if arr[0] == target:
        return 0, steps

    # --------------------------------------------------
    # Phase 1: Find a range [bound/2, bound] where target may lie
    # --------------------------------------------------
    bound = 1

    # Expand the bound exponentially while it is within array limits
    while bound < n:
        steps += 1  # compare arr[bound] == target
        if arr[bound] == target:
            return bound, steps

        steps += 1  # compare arr[bound] < target or > target
        if arr[bound] > target:
            break   # upper bound found; stop expanding

        bound *= 2  # double the bound and continue searching

    # --------------------------------------------------
    # Phase 2: Binary search within the identified range
    # --------------------------------------------------
    left = bound // 2
    right = min(bound, n - 1)

    # Standard binary search within [left, right]
    while left <= right:
        mid = left + (right - left) // 2

        steps += 1  # compare arr[mid] == target
        if arr[mid] == target:
            return mid, steps

        steps += 1  # compare arr[mid] < target
        if arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    # Target not found
    return -1, steps


def load_ordered_file() -> list[int] | None:
    """
    Attempt to load ordered.txt from several likely relative locations.

    This allows the script to be run from different directories, such as:
      - the lesson folder
      - section12/
      - the repository root

    Returns:
        A list of integers if the file is successfully loaded,
        or None if all attempts fail.
    """
    print(f"Working directory: {os.getcwd()}")

    # Candidate relative paths to ordered.txt
    candidates = [
        os.path.join("..", "data", "ordered.txt"),                # from lesson_6_exponential_search/
        os.path.join("..", "..", "data", "ordered.txt"),          # from section12/
        os.path.join("code_samples", "section12", "data", "ordered.txt"),
    ]

    # Try each candidate path
    for rel in candidates:
        full = os.path.abspath(rel)
        print(f"Attempting to read: {full}")

        if os.path.exists(full):
            try:
                with open(full, "r", encoding="utf-8") as f:
                    text = f.read()

                # Split on any whitespace and parse integers
                nums = [int(tok) for tok in text.split()]
                print(f"Loaded {len(nums)} integers from {full}")
                return nums

            except Exception as ex:  # noqa: BLE001
                # File exists but could not be read or parsed
                print(f"Error reading ordered.txt:\n{ex}")
        else:
            # File does not exist at this location
            print("  File does not exist here.")

    # All candidate paths failed
    print("Missing input file - aborting.")
    return None


def run_tests(arr: list[int]) -> None:
    """
    Run a set of exponential-search tests and print results.

    The output includes:
      - array size
      - approximate log2(n)
      - number of steps required for each search case
    """
    n = len(arr)
    approx_log_n = math.log2(n) if n > 0 else 0.0
    print(f"\nArray size n = {n}, log2(n) ≈ {approx_log_n:.2f}\n")

    # Test values:
    #   - first element
    #   - middle element
    #   - last element
    #   - value not present
    tests = [
        arr[0],               # first element
        arr[n // 2],          # middle element
        arr[-1],              # last element
        6001,                 # not present (assuming data is -5000..5000)
    ]

    # Run exponential search for each test value
    for t in tests:
        idx, steps = exponential_search(arr, t)
        if idx != -1:
            print(f"Target {t:6} found at index {idx:5}, steps = {steps:4}")
        else:
            print(f"Target {t:6} not found,            steps = {steps:4}")


if __name__ == "__main__":
    # Load ordered data and run tests if successful
    data = load_ordered_file()
    if data is not None:
        run_tests(data)
