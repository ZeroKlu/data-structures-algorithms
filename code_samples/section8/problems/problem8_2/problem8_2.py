"""Problem 8.2. House Robber."""

def rob(nums: list[int]) -> int:
    """House robber."""
    if not nums:
        return 0
    if len(nums) == 1:
        return nums[0]
    prev2 = nums[0]
    prev1 = max(nums[0], nums[1])
    for i in range(2, len(nums)):
        take = prev2 + nums[i]
        skip = prev1
        cur = max(take, skip)
        prev2, prev1 = prev1, cur
    return prev1

# ============================
# Tests for rob (House Robber)
# ============================

if __name__ == "__main__":

    print("=== Test: rob (House Robber) ===\n")

    tests = [
        [],                 # 0
        [5],                # 5
        [1, 2],             # 2
        [2, 7, 9, 3, 1],    # 12
        [1, 2, 3, 1],       # 4
        [2, 1, 1, 2],       # 4
        [10, 5, 20, 15, 30],# 60
        [1, 3, 1, 3, 100],  # 103
    ]

    expected = [0, 5, 2, 12, 4, 4, 60, 103]

    for numbers, exp in zip(tests, expected):
        result = rob(numbers)
        print(f"rob({numbers}) = {result} (expected {exp})")
