"""Problem 9.3 - Partition Equal Subset Sum."""

def can_partition(nums: list[int]) -> bool:
    """Return True if nums can be partitioned into two subsets with equal sum."""
    total = sum(nums)
    if total % 2 == 1:
        return False
    target = total // 2
    dp = [False] * (target + 1)
    dp[0] = True
    for x in nums:
        for s in range(target, x - 1, -1):
            if dp[s - x]:
                dp[s] = True
    return dp[target]

# ===========================
# Test Harness
# ===========================

def test(name, arr, expected):
    """Test can_partition."""
    result = can_partition(arr)
    print(name)
    print(f"Input: {arr}")
    print(f"can_partition = {result} (expected {expected})\n")


print("=== Testing can_partition ===\n")

test("Test 1: Classic example", [1, 5, 11, 5], True)
test("Test 2: Cannot partition", [1, 2, 3, 5], False)
test("Test 3: Single element", [7], False)
test("Test 4: Two equal numbers", [4, 4], True)
test("Test 5: Larger mixed set", [2, 2, 3, 5], False)
test("Test 6: Multiple valid subsets", [2, 2, 1, 1], True)
