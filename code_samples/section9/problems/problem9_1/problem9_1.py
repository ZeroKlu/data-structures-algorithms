"""Problem 9.1: Longest Increasing Subsequence."""

from bisect import bisect_left

def length_of_lis(nums: list[int]) -> int:
    """Return the length of the longest increasing subsequence in nums."""
    tails: list[int] = []
    for x in nums:
        i = bisect_left(tails, x)
        if i == len(tails):
            tails.append(x)
        else:
            tails[i] = x
    return len(tails)

# ===========================
# Tests
# ===========================

def test(name, arr, expected):
    """Test length_of_lis."""
    result = length_of_lis(arr)
    print(name)
    print(f"Input: {arr}")
    print(f"length_of_lis = {result} (expected {expected})\n")


print("=== Testing length_of_lis ===\n")

test("Test 1: Increasing sequence", [1, 2, 3, 4, 5], 5)

test("Test 2: Mixed sequence", [10, 9, 2, 5, 3, 7, 101, 18], 4)

test("Test 3: All identical", [7, 7, 7, 7, 7], 1)

test("Test 4: Empty array", [], 0)

test("Test 5: Decreasing sequence", [9, 7, 5, 3, 1], 1)

test("Test 6: Random pattern", [4, 10, 4, 3, 8, 9], 3)
