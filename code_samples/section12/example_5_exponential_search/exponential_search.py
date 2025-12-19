"""Lesson 6 - Exponential Search (with step counting and file-based tests)."""

from __future__ import annotations
import os
import math

#pylint: disable=broad-exception-caught
#pylint: disable=line-too-long

def exponential_search(arr: list[int], target: int) -> tuple[int, int]:
    """
    Exponential search in a sorted array.
    Returns (index, step_count), where step_count ~ number of comparisons.

    Assumes `arr` is sorted in ascending order.
    """
    steps = 0
    n = len(arr)
    if n == 0:
        return -1, steps

    # Check first element
    steps += 1
    if arr[0] == target:
        return 0, steps

    # Find range [bound/2, bound] where target may lie
    bound = 1
    while bound < n:
        steps += 1  # compare arr[bound] with target (==)
        if arr[bound] == target:
            return bound, steps
        steps += 1  # compare arr[bound] with target (< or >)
        if arr[bound] > target:
            break
        bound *= 2

    # Binary search in that range
    left = bound // 2
    right = min(bound, n - 1)

    while left <= right:
        mid = left + (right - left) // 2
        steps += 1  # arr[mid] == target?
        if arr[mid] == target:
            return mid, steps
        steps += 1  # arr[mid] < target?
        if arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return -1, steps


def load_ordered_file() -> list[int] | None:
    """Try to load ordered.txt from a few likely relative locations."""
    print(f"Working directory: {os.getcwd()}")

    candidates = [
        os.path.join("..", "data", "ordered.txt"),                # from lesson_6_exponential_search/
        os.path.join("..", "..", "data", "ordered.txt"),          # from section12/
        os.path.join("code_samples", "section12", "data", "ordered.txt"),
    ]

    for rel in candidates:
        full = os.path.abspath(rel)
        print(f"Attempting to read: {full}")
        if os.path.exists(full):
            try:
                with open(full, "r", encoding="utf-8") as f:
                    text = f.read()
                # Split on any whitespace and parse ints
                nums = [int(tok) for tok in text.split()]
                print(f"Loaded {len(nums)} integers from {full}")
                return nums
            except Exception as ex:  # noqa: BLE001
                print(f"Error reading ordered.txt:\n{ex}")
        else:
            print("  File does not exist here.")

    print("Missing input file - aborting.")
    return None


def run_tests(arr: list[int]) -> None:
    """Run a few exponential-search tests and print comparisons."""
    n = len(arr)
    approx_log_n = math.log2(n) if n > 0 else 0.0
    print(f"\nArray size n = {n}, log2(n) ≈ {approx_log_n:.2f}\n")

    # Test values: first, middle, last, and a value not present
    tests = [
        arr[0],               # first element
        arr[n // 2],          # middle element
        arr[-1],              # last element
        6001,                 # not present (assuming data is -5000..5000)
    ]

    for t in tests:
        idx, steps = exponential_search(arr, t)
        if idx != -1:
            print(f"Target {t:6} found at index {idx:5}, steps = {steps:4}")
        else:
            print(f"Target {t:6} not found,            steps = {steps:4}")


if __name__ == "__main__":
    data = load_ordered_file()
    if data is not None:
        run_tests(data)
