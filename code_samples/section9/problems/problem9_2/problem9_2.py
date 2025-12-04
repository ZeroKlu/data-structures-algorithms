"""Problem 9.2: Knapsack Problem."""

def knap_sack(W: int, weight: list[int], value: list[int]) -> int:
    """Return the maximum value of a knapsack."""
    dp = [0] * (W + 1)
    for wt, val in zip(weight, value):
        for w in range(W, wt - 1, -1):
            dp[w] = max(dp[w], dp[w - wt] + val)
    return dp[W]

# ===========================
# Test Harness
# ===========================

def test(name, W, weight, value, expected):
    """Test knap_sack."""
    result = knap_sack(W, weight, value)
    print(name)
    print(f"Capacity = {W}")
    print(f"Weights: {weight}")
    print(f"Values:  {value}")
    print(f"knap_sack = {result} (expected {expected})\n")


print("=== Testing knap_sack ===\n")

test("Test 1: Classic example",
     50,
     [10, 20, 30],
     [60, 100, 120],
     220)

test("Test 2: Small weights",
     6,
     [1, 2, 3],
     [10, 15, 40],
     65)

test("Test 3: Too small capacity",
     4,
     [5, 6, 7],
     [10, 20, 30],
     0)

test("Test 4: Zero capacity",
     0,
     [2, 3, 4],
     [20, 30, 40],
     0)

test("Test 5: Larger mixed set",
     10,
     [2, 3, 5, 7],
     [20, 30, 50, 70],
     100)
