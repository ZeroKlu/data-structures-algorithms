"""Lesson 5 - Interpolation Search (with step counting)."""

from typing import List, Tuple


def interpolation_search(arr: List[int], target: int) -> Tuple[int, int]:
    """
    Perform interpolation search on a sorted array of distinct integers.

    Interpolation search estimates where the target value is likely to be
    located based on its numeric value relative to the values at the
    current search bounds.

    This algorithm performs best when:
      - the data is sorted in ascending order, and
      - the data is uniformly distributed.

    Returns:
        (index, steps)
        - index: position of target in arr, or -1 if not found
        - steps: number of probe iterations performed
                 (used here to illustrate algorithmic complexity)
    """

    lo, hi = 0, len(arr) - 1   # Search bounds (inclusive)
    steps = 0                 # Counts the number of probe iterations

    # Continue searching while:
    #  - bounds are valid, and
    #  - target lies within the value range [arr[lo], arr[hi]]
    while lo <= hi and arr[lo] <= target <= arr[hi]:
        steps += 1  # Count this probe / iteration

        # --------------------------------------------------
        # Special case: all values in [lo, hi] are identical
        # --------------------------------------------------
        # This avoids division by zero in the interpolation formula.
        if arr[hi] == arr[lo]:
            if arr[lo] == target:
                return lo, steps
            return -1, steps

        # --------------------------------------------------
        # Estimate the likely position using interpolation:
        #
        # pos = lo + (hi - lo) * (target - arr[lo]) / (arr[hi] - arr[lo])
        #
        # Integer division (//) is used to produce a valid index.
        # --------------------------------------------------
        pos = lo + (hi - lo) * (target - arr[lo]) // (arr[hi] - arr[lo])

        # --------------------------------------------------
        # Safety check:
        # Due to integer arithmetic or skewed distributions,
        # the estimated position might fall outside [lo, hi].
        # --------------------------------------------------
        if pos < lo or pos > hi:
            return -1, steps

        # Compare against the estimated position
        if arr[pos] == target:
            return pos, steps
        if arr[pos] < target:
            lo = pos + 1      # Target lies to the right
        else:
            hi = pos - 1      # Target lies to the left

    # Target was not found
    return -1, steps


if __name__ == "__main__":
    # --------------------------------------------------
    # Demo / Test Harness
    # --------------------------------------------------

    # Simple in-memory dataset:
    # Sorted integers from -5000 to +5000 (inclusive)
    data = list(range(-5000, 5001))

    # Test cases covering common scenarios
    tests = [
        ("Best case (first element)", data[0]),          # Near-minimum value
        ("Middle element", 0),                           # Central value
        ("Last element", data[-1]),                      # Near-maximum value
        ("Not present (beyond range)", 6000),            # Outside array range
    ]

    print("Interpolation Search Demo (Python)")
    print(f"Array size: {len(data)}\n")

    # Execute each test and display results
    for label, target_num in tests:
        idx, step_count = interpolation_search(data, target_num)
        print(
            f"{label}: "
            f"target={target_num:5d}, "
            f"index={idx:5d}, "
            f"steps={step_count}"
        )
