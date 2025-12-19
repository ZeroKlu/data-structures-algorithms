"""Problem 9.1: Longest Increasing Subsequence."""

from bisect import bisect_left

def length_of_lis(nums: list[int]) -> int:
    """Return the length of the longest increasing subsequence in nums.

    This function computes only the *length* of the LIS, not the subsequence
    itself.

    It uses the classic O(n log n) "tails" technique (often described via
    patience sorting):

    Invariant:
        tails[i] stores the smallest possible tail value of any strictly
        increasing subsequence of length (i + 1) encountered so far.

    Key idea:
        - Smaller tail values are always better, because they leave more room
          to extend the subsequence with future numbers.
        - The `tails` list is always kept sorted, allowing binary search.

    Algorithm:
        For each element x in nums:
            1) Find the leftmost index i where tails[i] >= x using bisect_left.
            2) If i == len(tails), x extends the longest subsequence found so far.
            3) Otherwise, replace tails[i] with x to improve the tail value for
               subsequences of length i + 1.

    Important notes:
        - The subsequence must be *strictly* increasing, so duplicates do not
          extend the LIS.
        - The contents of `tails` do not necessarily form an actual subsequence
          from the input; they are only used to compute the length.

    Complexity:
        Time:  O(n log n), where n = len(nums)
        Space: O(n), for the tails list

    Args:
        nums: List of integers.

    Returns:
        Length of the longest strictly increasing subsequence.
    """
    # tails[i] = minimum possible tail value of an increasing subsequence
    # of length (i + 1)
    tails: list[int] = []

    # Process each number in the input sequence
    for x in nums:
        # Find the insertion/replacement position for x:
        # the first index where tails[i] >= x
        i = bisect_left(tails, x)

        if i == len(tails):
            # x is larger than all existing tails, so it extends the LIS
            tails.append(x)
        else:
            # Replace the existing tail value to keep tails minimal
            tails[i] = x

    # The length of tails equals the length of the LIS
    return len(tails)

# ===========================
# Tests
# ===========================

def test(name, arr, expected):
    """Test length_of_lis.

    Prints the test name, input array, computed result,
    and expected value for easy verification.
    """
    result = length_of_lis(arr)
    print(name)
    print(f"Input: {arr}")
    print(f"length_of_lis = {result} (expected {expected})\n")


# Run a series of sanity checks covering common LIS patterns
print("=== Testing length_of_lis ===\n")

# Strictly increasing sequence => LIS uses all elements
test("Test 1: Increasing sequence", [1, 2, 3, 4, 5], 5)

# Classic mixed example => one LIS is [2, 3, 7, 101]
test("Test 2: Mixed sequence", [10, 9, 2, 5, 3, 7, 101, 18], 4)

# All identical values => strictly increasing disallows duplicates
test("Test 3: All identical", [7, 7, 7, 7, 7], 1)

# Empty input => LIS length is 0
test("Test 4: Empty array", [], 0)

# Strictly decreasing => best LIS is any single element
test("Test 5: Decreasing sequence", [9, 7, 5, 3, 1], 1)

# Random pattern => one LIS is [4, 8, 9]
test("Test 6: Random pattern", [4, 10, 4, 3, 8, 9], 3)
