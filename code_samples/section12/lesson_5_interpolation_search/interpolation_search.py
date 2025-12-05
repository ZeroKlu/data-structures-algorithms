"""Lesson 5 - Interpolation Search (with step counting)."""

from typing import List, Tuple


def interpolation_search(arr: List[int], target: int) -> Tuple[int, int]:
    """
    Interpolation search on a sorted array of distinct integers.

    Returns:
        (index, steps)
        - index: position of target in arr, or -1 if not found
        - steps: number of probe iterations performed (illustrates complexity)
    """
    lo, hi = 0, len(arr) - 1
    steps = 0

    while lo <= hi and arr[lo] <= target <= arr[hi]:
        steps += 1  # one probe this iteration

        # Avoid division by zero: arr[hi] == arr[lo]
        if arr[hi] == arr[lo]:
            if arr[lo] == target:
                return lo, steps
            else:
                return -1, steps

        # Estimate position using interpolation formula
        pos = lo + (hi - lo) * (target - arr[lo]) // (arr[hi] - arr[lo])

        # Safety check: pos might fall slightly out of bounds due to integer math
        if pos < lo or pos > hi:
            return -1, steps

        if arr[pos] == target:
            return pos, steps
        elif arr[pos] < target:
            lo = pos + 1
        else:
            hi = pos - 1

    # Not found
    return -1, steps


if __name__ == "__main__":
    # Simple in-memory data: sorted integers -5000 .. 5000
    data = list(range(-5000, 5001))

    tests = [
        ("Best case (first element)", data[0]),
        ("Middle element", 0),
        ("Last element", data[-1]),
        ("Not present (beyond range)", 6000),
    ]

    print("Interpolation Search Demo (Python)")
    print(f"Array size: {len(data)}\n")

    for label, target_num in tests:
        idx, step_count = interpolation_search(data, target_num)
        print(f"{label}: target={target_num:5d}, index={idx:5d}, steps={step_count}")
