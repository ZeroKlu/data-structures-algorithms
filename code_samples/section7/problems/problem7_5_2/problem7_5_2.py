"""
Problem 7.5 (Part 2) – Count Unique Pairs That Sum to a Target.

This module counts how many *distinct value pairs* {a, b} exist in an array
such that a + b equals a given target. Uniqueness is based on VALUES, not indices.
"""


def count_unique_pairs(nums: list[int], target: int) -> int:
    """
    Count the number of unique value pairs that sum to a target.

    Definition:
        Given an integer array `nums` and an integer `target`, count how many
        *distinct value pairs* {a, b} satisfy:
            a + b == target

        The order of values does not matter:
            {a, b} is the same as {b, a}

        Duplicate values in the array do NOT create additional pairs if they
        represent the same value pair.

    Examples:
        nums = [1, 1, 2, 2, 3, 3], target = 4
        Valid unique pairs are:
            {1,3} and {2,2}
        Result = 2

        nums = [1, 5, 1, 5], target = 6
        Valid unique pair:
            {1,5}
        Result = 1

    Parameters:
        nums:
            List of integers (may contain duplicates).
        target:
            Target sum.

    Returns:
        The number of unique value pairs that sum to the target.

    Algorithm (single pass with sets):
        - Maintain a set `seen` of values encountered so far.
        - Maintain a set `pairs` of normalized pairs (a, b) where a <= b.

        For each value x in nums:
            1) Compute y = target - x.
            2) If y is already in `seen`, then (x, y) forms a valid pair.
            3) Normalize the pair so order does not matter:
                   a = min(x, y), b = max(x, y)
            4) Add the tuple (a, b) to `pairs`.
            5) Add x to `seen`.

    Why normalization matters:
        - Without normalization, (1,3) and (3,1) would be treated as different.
        - Enforcing a <= b ensures each value pair has a single canonical form.

    Why tuples instead of strings:
        - Tuples are immutable and hashable.
        - Faster and cleaner than building and hashing strings like "a,b".

    Complexity (expected / average):
        - Time:  O(n)
            Each element is processed once with O(1) average-time set operations.
        - Space: O(n)
            The `seen` set and `pairs` set may grow linearly in the worst case.
    """
    # Stores values that have already been encountered
    seen: set[int] = set()

    # Stores unique normalized value pairs (a, b) with a <= b
    pairs: set[tuple[int, int]] = set()

    # Iterate through each number in the array
    for x in nums:
        # Compute the complementary value needed
        y = target - x

        # If y has been seen before, x and y form a valid pair
        if y in seen:
            # Normalize the pair so order does not matter
            a = min(x, y)
            b = max(x, y)

            # Add the normalized pair to the set
            # Duplicate pairs are automatically ignored
            pairs.add((a, b))

        # Mark x as seen for future iterations
        seen.add(x)

    # The number of unique pairs found
    return len(pairs)


# ============================
# Tests (top-level statements)
# ============================
#
# These tests execute immediately when the script is run.
#

tests = [
    ([1, 1, 2, 2, 3, 3], 4, 2, "{1,3} and {2,2}"),
    ([2, 7, 11, 15],     9, 1, "{2,7}"),
    ([3, 3, 3],          6, 1, "{3,3}"),
    ([1, 2, 3, 4],      10, 0, "none"),
    ([1, 5, 1, 5],       6, 1, "{1,5}")
]

print("=== Test: count_unique_pairs ===\n")

# Run each test case and compare actual vs expected output
for numbers, tgt, expected, note in tests:
    result = count_unique_pairs(numbers, tgt)
    print(f"Input nums = {numbers}, target = {tgt}")
    print(f"Result:   {result}")
    print(f"Expected: {expected} ({note})\n")
