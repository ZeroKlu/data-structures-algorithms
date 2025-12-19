"""
Problem 7.3 - Longest Consecutive Sequence.

This module implements an efficient, hash-set-based solution to find the
length of the longest sequence of consecutive integers in an unsorted array.
"""


def longest_consecutive(nums: list[int]) -> int:
    """
    Finds the length of the longest run of consecutive integers in the input list.

    Definition:
        A consecutive sequence consists of numbers that differ by exactly 1,
        such as [1, 2, 3, 4]. The numbers do NOT need to appear in order
        in the original array.

    Args:
        nums:
            List of integers. May be empty and may contain duplicates.

    Returns:
        Length of the longest consecutive sequence.
        Returns 0 if the input list is empty.

    Core idea:
        - Convert the list to a set for O(1) average-time membership checks.
        - Only start counting a sequence from numbers that are the *start*
          of a run (i.e., x - 1 is not present in the set).
        - Extend the sequence forward (x, x+1, x+2, ...) until it breaks.

    Why the "start of sequence" check matters:
        Without checking that (x - 1) is not in the set, each sequence would be
        counted multiple times—once from every element in the run—leading to
        O(n²) behavior in dense cases.

    Complexity (average / expected):
        - Time:  O(n)
            Each number is inserted into the set once, and each consecutive
            sequence is traversed once.
        - Space: O(n)
            The set stores all unique values from the input.

    Notes:
        - Duplicate values in the input do not affect correctness because sets
          automatically remove duplicates.
        - This algorithm works correctly with negative numbers.
    """
    # Convert the list to a set:
    #  - Removes duplicates
    #  - Enables fast membership checks
    s = set(nums)

    # Tracks the maximum sequence length found so far
    best = 0

    # Iterate over each unique value in the set
    for x in s:
        # Only attempt to build a sequence starting at x if x-1 is NOT present.
        # This ensures x is the smallest value in its consecutive run.
        if x - 1 not in s:
            cur = x
            length = 1

            # Extend the sequence forward as long as consecutive numbers exist
            while cur + 1 in s:
                cur += 1
                length += 1

            # Update the best (maximum) length if this run is longer
            best = max(best, length)

    return best


# ============================
# Tests (top-level statements)
# ============================
#
# These statements execute immediately when the module is run.
# No explicit main() function is required.
#

tests = [
    [100, 4, 200, 1, 3, 2],             # expected 4: 1,2,3,4
    [0, 3, 7, 2, 5, 8, 4, 6, 0, 1],      # expected 9: 0..8 (duplicate 0 ignored)
    [1, 2, 0, 1],                       # expected 3: 0,1,2
    [],                                 # expected 0: empty input
    [-2, -1, 0, 2, 3, 4, 5]             # expected 4: 2,3,4,5
]

expected = [4, 9, 3, 0, 4]

print("=== Test: longest_consecutive ===\n")

# Run each test case and compare the result with the expected value
for numbers, exp in zip(tests, expected):
    result = longest_consecutive(numbers)
    print(f"Input: {numbers}")
    print(f"Longest consecutive length: {result} (expected {exp})\n")
