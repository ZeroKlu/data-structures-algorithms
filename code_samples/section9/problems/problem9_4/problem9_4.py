"""Problem 9.4: Bitmask DP. (TSP)"""

def tsp(cost: list[list[int]]) -> int:
    """Return the minimum cost of TSP."""
    n = len(cost)
    max_mask = 1 << n
    INF = 10**9
    dp = [[INF] * n for _ in range(max_mask)]
    dp[1][0] = 0

    for mask in range(1, max_mask):
        for i in range(n):
            if not mask & (1 << i):
                continue
            cur = dp[mask][i]
            if cur == INF:
                continue
            for j in range(n):
                if mask & (1 << j):
                    continue
                next_mask = mask | (1 << j)
                cand = cur + cost[i][j]
                if cand < dp[next_mask][j]:
                    dp[next_mask][j] = cand

    full_mask = max_mask - 1
    ans = INF
    for i in range(n):
        cand = dp[full_mask][i] + cost[i][0]
        ans = min(ans, cand)
    return ans

# ===========================
# Test Harness
# ===========================

def test(name, cost, expected):
    """Test tsp."""
    result = tsp(cost)
    print(name)
    print("Cost matrix:")
    for row in cost:
        print(" ", row)
    print(f"tsp() = {result} (expected {expected})\n")


print("=== Testing tsp (Bitmask DP) ===\n")

# ---------------------------------------
# Test 1: Classic 4-city example
# ---------------------------------------
test(
    "Test 1: Classic 4-city example",
    [
        [0, 10, 15, 20],
        [10, 0, 35, 25],
        [15, 35, 0, 30],
        [20, 25, 30, 0]
    ],
    80
)

# ---------------------------------------
# Test 2: 3-city triangle
# ---------------------------------------
test(
    "Test 2: 3-city triangle",
    [
        [0, 1, 4],
        [1, 0, 2],
        [4, 2, 0]
    ],
    7
)

# ---------------------------------------
# Test 3: 4-city square loop
# ---------------------------------------
test(
    "Test 3: 4-city square",
    [
        [0, 1, 2, 1],
        [1, 0, 1, 2],
        [2, 1, 0, 1],
        [1, 2, 1, 0]
    ],
    4
)
