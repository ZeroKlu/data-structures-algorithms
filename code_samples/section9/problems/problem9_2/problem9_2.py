"""Problem 9.2: Knapsack Problem."""

def knap_sack(W: int, weight: list[int], value: list[int]) -> int:
    """Return the maximum value of a knapsack.

    This function solves the classic 0/1 Knapsack problem using a
    space-optimized dynamic programming approach.

    Problem definition:
        - You are given a knapsack with capacity W.
        - There are items, each with:
            weight[i] = weight of item i
            value[i]  = value of item i
        - Each item can be chosen at most once (0/1 constraint).
        - Goal: maximize the total value without exceeding capacity W.

    DP formulation:
        dp[w] = maximum value achievable with capacity w
                using items processed so far.

    Initialization:
        dp[w] = 0 for all w, because with no items selected,
        the maximum achievable value is 0 regardless of capacity.

    Transition:
        For each item (wt, val):
            for w from W down to wt:
                dp[w] = max(dp[w], dp[w - wt] + val)

    Why iterate w backwards?
        - Iterating from high to low capacities ensures each item
          is used at most once.
        - Iterating forward would allow the same item to be reused
          multiple times, turning this into the unbounded knapsack problem.

    Complexity:
        Time:  O(n * W), where n is the number of items
        Space: O(W), due to the 1D dp array

    Args:
        W: Knapsack capacity.
        weight: List of item weights.
        value: List of item values.

    Returns:
        Maximum total value achievable with capacity W.
    """
    # dp[w] stores the best value achievable with capacity w
    dp = [0] * (W + 1)

    # Process each item exactly once
    for wt, val in zip(weight, value):
        # Traverse capacities backwards to enforce the 0/1 constraint
        for w in range(W, wt - 1, -1):
            # Either skip the item (dp[w]) or take it
            # (dp[w - wt] + val), whichever yields a higher value
            dp[w] = max(dp[w], dp[w - wt] + val)

    # dp[W] contains the optimal value for full capacity
    return dp[W]

# ===========================
# Test Harness
# ===========================

def test(name, W, weight, value, expected):
    """Test knap_sack.

    Prints the test name, input parameters,
    computed result, and expected value.
    """
    result = knap_sack(W, weight, value)
    print(name)
    print(f"Capacity = {W}")
    print(f"Weights: {weight}")
    print(f"Values:  {value}")
    print(f"knap_sack = {result} (expected {expected})\n")


# Run a set of sanity checks covering common knapsack scenarios
print("=== Testing knap_sack ===\n")

# Test 1: Classic textbook example
# Optimal choice: weights 20 + 30 => value 100 + 120 = 220
test("Test 1: Classic example",
     50,
     [10, 20, 30],
     [60, 100, 120],
     220)

# Test 2: Small weights where all items fit
# Total weight = 1 + 2 + 3 = 6, total value = 65
test("Test 2: Small weights",
     6,
     [1, 2, 3],
     [10, 15, 40],
     65)

# Test 3: Capacity too small to take any item
# All weights exceed capacity => result = 0
test("Test 3: Too small capacity",
     4,
     [5, 6, 7],
     [10, 20, 30],
     0)

# Test 4: Zero capacity edge case
# Capacity = 0 => cannot take anything
test("Test 4: Zero capacity",
     0,
     [2, 3, 4],
     [20, 30, 40],
     0)

# Test 5: Larger mixed set
# Multiple optimal combinations yield total value = 100
test("Test 5: Larger mixed set",
     10,
     [2, 3, 5, 7],
     [20, 30, 50, 70],
     100)
