"""Problem 9.4: Bitmask DP. (TSP)"""

def tsp(cost: list[list[int]]) -> int:
    """Return the minimum cost of TSP.

    This function solves the Traveling Salesman Problem (TSP) using
    bitmask dynamic programming (Held–Karp algorithm).

    Problem variant solved:
        - Start at city 0
        - Visit every city exactly once
        - Return to city 0
        - Minimize total tour cost

    Input:
        cost[i][j] is the travel cost from city i to city j.
        The matrix is assumed to be n x n (square).

    DP definition:
        dp[mask][i] = minimum cost to start at city 0, visit exactly the set
                      of cities represented by `mask`, and end at city i.

    Bitmask representation:
        - mask is an integer with n bits (0..n-1).
        - If bit k is set, city k has been visited.
        - max_mask = 1 << n is the total number of subsets of cities.

    Initialization:
        dp[1][0] = 0
        (mask=1 means only city 0 is visited, and we are currently at city 0.)
        All other dp states start as INF ("unreachable").

    Transition:
        For each reachable state (mask, i):
            try to visit any unvisited city j:
                next_mask = mask | (1 << j)
                dp[next_mask][j] = min(dp[next_mask][j],
                                       dp[mask][i] + cost[i][j])

    Finalization:
        After all cities are visited (full_mask), close the tour by returning
        to city 0:
            ans = min_i (dp[full_mask][i] + cost[i][0])

    Complexity:
        Time:  O(2^n * n^2)
        Space: O(2^n * n)

    Notes about INF:
        INF is chosen as 1e9, which is large enough for typical small n test cases.
        In production, you might choose something larger (or use a very large int)
        depending on maximum edge weights and n.

    Args:
        cost: Square cost matrix (n x n).

    Returns:
        Minimum tour cost starting and ending at city 0, visiting all cities once.
    """
    # Number of cities
    n = len(cost)

    # Total number of subsets of cities
    max_mask = 1 << n

    # Sentinel for unreachable/unknown best costs
    INF = 10**9

    # dp[mask][i] = best cost to start at 0, visit cities in mask, and end at i
    dp = [[INF] * n for _ in range(max_mask)]

    # Base case: only city 0 visited (mask=1), cost is 0, ending at city 0
    dp[1][0] = 0

    # Iterate over all subsets of visited cities
    for mask in range(1, max_mask):
        # Consider each city i as the endpoint
        for i in range(n):
            # If i is not in mask, we cannot be ending at i for this subset
            if not mask & (1 << i):
                continue

            cur = dp[mask][i]

            # Skip unreachable states
            if cur == INF:
                continue

            # Try extending the path to any unvisited city j
            for j in range(n):
                # If j is already visited, skip
                if mask & (1 << j):
                    continue

                next_mask = mask | (1 << j)
                cand = cur + cost[i][j]

                # Relaxation: keep the minimum cost for reaching (next_mask, j)
                if cand < dp[next_mask][j]:
                    dp[next_mask][j] = cand

    # Mask where all cities are visited
    full_mask = max_mask - 1

    # Close the tour by returning to city 0
    ans = INF
    for i in range(n):
        # Cost to end at i after visiting all cities + cost to return to start
        cand = dp[full_mask][i] + cost[i][0]
        ans = min(ans, cand)

    return ans

# ===========================
# Test Harness
# ===========================

def test(name, cost, expected):
    """Test tsp.

    Prints:
        - Test name
        - Cost matrix
        - Computed tsp() result
        - Expected result
    """
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
# Best tour cost including return to 0 is 80:
# 0 -> 1 -> 3 -> 2 -> 0
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
# Both tours cost 7:
# 0 -> 1 -> 2 -> 0 or 0 -> 2 -> 1 -> 0
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
# One optimal tour: 0 -> 1 -> 2 -> 3 -> 0, cost = 4
test(
    "Test 3: 4-city square loop",
    [
        [0, 1, 2, 1],
        [1, 0, 1, 2],
        [2, 1, 0, 1],
        [1, 2, 1, 0]
    ],
    4
)
