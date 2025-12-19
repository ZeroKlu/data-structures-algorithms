"""Problem 9.3 - Partition Equal Subset Sum."""

def can_partition(nums: list[int]) -> bool:
    """Return True if nums can be partitioned into two subsets with equal sum.

    This function solves the classic *Partition Equal Subset Sum* problem,
    which can be reduced to a 0/1 subset-sum decision problem.

    Key idea:
        - Let S be the sum of all elements.
        - If S is odd, it is impossible to split the array into two subsets
          with equal integer sums.
        - If S is even, the problem becomes:
              "Is there a subset of nums that sums to S // 2?"

    Dynamic Programming formulation (1D boolean DP):
        dp[s] = True if there exists a subset of the processed elements
                whose sum is exactly s.

    Initialization:
        dp[0] = True
        (A sum of 0 is always achievable by choosing no elements.)

    Transition:
        For each element x in nums:
            for s from target down to x:
                if dp[s - x] is True:
                    dp[s] = True

    Important detail:
        - The inner loop iterates *backwards* to ensure each element is used
          at most once (0/1 choice).
        - Iterating forwards would allow the same element to be reused,
          turning this into an unbounded subset-sum problem.

    Complexity:
        Time:  O(n * target), where target = sum(nums) // 2
        Space: O(target)

    Args:
        nums: List of positive integers.

    Returns:
        True if nums can be partitioned into two subsets with equal sum,
        False otherwise.
    """
    # Compute the total sum of all elements.
    total = sum(nums)

    # If the total sum is odd, it cannot be split into two equal halves.
    if total % 2 == 1:
        return False

    # Target sum for one subset.
    target = total // 2

    # dp[s] indicates whether sum s is achievable.
    dp = [False] * (target + 1)

    # Base case: sum 0 is always achievable.
    dp[0] = True

    # Process each number in the array.
    for x in nums:
        # Traverse sums backwards to enforce 0/1 usage of each element.
        for s in range(target, x - 1, -1):
            # If we could previously make (s - x),
            # then we can make s by including x.
            if dp[s - x]:
                dp[s] = True

    # If target sum is achievable, a valid partition exists.
    return dp[target]

# ===========================
# Test Harness
# ===========================

def test(name, arr, expected):
    """Test can_partition.

    Prints:
        - Test name
        - Input array
        - Computed result
        - Expected result
    """
    result = can_partition(arr)
    print(name)
    print(f"Input: {arr}")
    print(f"can_partition = {result} (expected {expected})\n")


# Run a series of sanity checks covering common cases.
print("=== Testing can_partition ===\n")

# Classic example: partition exists (sum = 22, target = 11).
test("Test 1: Classic example", [1, 5, 11, 5], True)

# Total sum is odd => cannot partition.
test("Test 2: Cannot partition", [1, 2, 3, 5], False)

# Single element cannot be split into two equal-sum subsets.
test("Test 3: Single element", [7], False)

# Two equal numbers can be partitioned trivially.
test("Test 4: Two equal numbers", [4, 4], True)

# Even total sum, but no subset achieves target.
test("Test 5: Larger mixed set", [2, 2, 3, 5], False)

# Multiple valid subsets exist (e.g., {2,1} and {2,1}).
test("Test 6: Multiple valid subsets", [2, 2, 1, 1], True)
