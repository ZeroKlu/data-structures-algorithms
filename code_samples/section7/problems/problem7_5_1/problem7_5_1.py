"""
Problem 7.5 (Part 1) - Two Sum (Return Indices).

This module implements a hash-table-based solution to find two distinct
indices in an array such that the corresponding values sum to a given target.
"""


def two_sum_indices(nums: list[int], target: int) -> list[int]:
    """
    Finds indices of two numbers in the array that add up to the target.

    Definition:
        Given an array of integers `nums` and an integer `target`,
        return the indices [i, j] such that:
            nums[i] + nums[j] == target

        Each index must be used at most once.

    Args:
        nums:
            List of integers to search through.
        target:
            Desired sum of two elements.

    Returns:
        A list containing two indices [i, j] if a valid pair is found.
        Returns an empty list [] if no such pair exists.

    Core idea (single-pass hash table):
        - As we iterate through the array, we keep a dictionary that maps:
              value -> index where that value was last seen
        - For each element x at index i:
              need = target - x
          If `need` is already in the dictionary, then we have found a previous
          element that pairs with x to reach the target.
        - Otherwise, store x and its index for future lookups.

    Why this works:
        - At iteration i, the dictionary only contains elements from indices < i,
          so we never reuse the same element twice.
        - Dictionary lookups are O(1) on average, giving an efficient solution.

    Complexity:
        - Time:  O(n)
            Each element is processed once.
        - Space: O(n)
            The dictionary may store up to n elements.

    Notes:
        - If the same value appears multiple times, the dictionary keeps the
          most recent index for that value.
        - This implementation returns the first valid pair it encounters.
    """
    # Dictionary mapping value -> index where it appears in nums
    index_of: dict[int, int] = {}

    # Iterate through the array with both index and value
    for i, x in enumerate(nums):
        # Compute the complementary value needed to reach the target
        need = target - x

        # If we've already seen the needed value, return the matching indices
        if need in index_of:
            return [index_of[need], i]

        # Otherwise, record the current value and its index
        index_of[x] = i

    # If we finish the loop without finding a pair, return empty list
    return []


# ============================
# Tests (top-level statements)
# ============================
#
# These statements execute immediately when the module is run.
#

tests = [
    ([2, 7, 11, 15], 9,  [0, 1]),   # 2 + 7 = 9
    ([3, 2, 4],      6,  [1, 2]),   # 2 + 4 = 6
    ([1, 2, 3, 4],   10, []),       # no solution
]

print("=== Test: two_sum_indices ===\n")

# Run each test case and compare actual vs expected output
for numbers, tgt, expected in tests:
    result = two_sum_indices(numbers, tgt)
    print(f"Input: nums={numbers}, target={tgt}")
    print(f"Result: {result} (expected {expected})\n")
