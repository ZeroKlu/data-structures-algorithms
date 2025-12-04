"""Problem 9.5: Minimum Cost to Cut a Stick."""

def min_cost(n: int, cuts: list[int]) -> int:
    """Minimum Cost to Cut a Stick"""
    pos = [0] + sorted(cuts) + [n]
    m = len(pos)
    dp = [[0] * m for _ in range(m)]
    for length in range(2, m):
        for i in range(0, m - length):
            j = i + length
            best = float("inf")
            for k in range(i + 1, j):
                cand = dp[i][k] + dp[k][j] + (pos[j] - pos[i])
                best = min(best, cand)
            dp[i][j] = 0 if best == float("inf") else best
    return dp[0][m - 1]

# ===========================
# Test Harness
# ===========================

def test(name, n, cuts, expected):
    """Test min_cost."""
    result = min_cost(n, cuts)
    print(name)
    print(f"n = {n}, cuts = {cuts}")
    print(f"min_cost = {result} (expected {expected})\n")


print("=== Testing min_cost (Cutting Sticks DP) ===\n")

# Test 1: Classic example
test("Test 1: Classic example",
     7, [1, 3, 4, 5], 16)

# Test 2: Two cuts
test("Test 2: Two cuts",
     9, [5, 6], 13)

# Test 3: Single cut
test("Test 3: Single cut",
     10, [4], 10)

# Test 4: Even spacing (correct expected = 24)
test("Test 4: Even spacing",
     10, [2, 4, 6, 8], 24)

# Test 5: No cuts
test("Test 5: No cuts",
     100, [], 0)
