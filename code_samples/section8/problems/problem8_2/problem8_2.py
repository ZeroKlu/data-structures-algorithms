"""
Problem 8.2 — House Robber

You are given a list of non-negative integers where nums[i] represents
the amount of money in the i-th house along a street.

Constraint:
    You cannot rob two adjacent houses (doing so triggers an alarm).

Goal:
    Return the maximum amount of money you can rob without triggering the alarm.

This is a classic Dynamic Programming (DP) problem.
"""

def rob(nums: list[int]) -> int:
    """
    Compute the maximum amount of money that can be robbed.

    Dynamic Programming Insight:
        Let dp[i] be the maximum money that can be robbed from houses 0..i.

        Recurrence:
            dp[i] = max(
                dp[i - 1],            # skip the current house
                dp[i - 2] + nums[i]   # rob the current house
            )

        Instead of storing the full dp array, we optimize space by keeping
        only the last two values:
            prev2 = dp[i - 2]
            prev1 = dp[i - 1]

    Time Complexity:
        O(n) — single pass through the list

    Space Complexity:
        O(1) — constant extra space
    """

    # Edge case: no houses → no money
    if not nums:
        return 0

    # Edge case: one house → rob it
    if len(nums) == 1:
        return nums[0]

    # Base case initialization:
    # prev2 corresponds to dp[0] → best we can do with only the first house
    prev2 = nums[0]

    # prev1 corresponds to dp[1] → best we can do with the first two houses
    prev1 = max(nums[0], nums[1])

    # Process houses starting from index 2
    for i in range(2, len(nums)):

        # Option 1: Rob the current house
        # If we rob house i, we must add its value to dp[i-2] (prev2)
        take = prev2 + nums[i]

        # Option 2: Skip the current house
        # If we skip it, the best total remains dp[i-1] (prev1)
        skip = prev1

        # The best result up to house i
        cur = max(take, skip)

        # Shift the rolling window forward:
        #   prev2 becomes old prev1
        #   prev1 becomes current result
        prev2, prev1 = prev1, cur

    # prev1 now holds dp[last], the final answer
    return prev1


# ============================
# Tests for rob (House Robber)
# ============================

if __name__ == "__main__":

    print("=== Test: rob (House Robber) ===\n")

    # Each test case represents a row of houses
    tests = [
        [],                  # empty input → 0
        [5],                 # single house → 5
        [1, 2],              # choose max → 2
        [2, 7, 9, 3, 1],     # classic example → 12
        [1, 2, 3, 1],        # → 4
        [2, 1, 1, 2],        # → 4
        [10, 5, 20, 15, 30], # → 60
        [1, 3, 1, 3, 100],   # → 103
    ]

    # Expected outputs corresponding to each test case
    expected = [0, 5, 2, 12, 4, 4, 60, 103]

    # Run all tests
    for numbers, exp in zip(tests, expected):
        result = rob(numbers)
        print(f"rob({numbers}) = {result} (expected {exp})")
