"""Problem 9.5: Minimum Cost to Cut a Stick."""

def min_cost(n: int, cuts: list[int]) -> int:
    """Minimum Cost to Cut a Stick

    Solves the classic "Minimum Cost to Cut a Stick" / "Cutting Sticks" problem
    using interval dynamic programming (e.g., LeetCode 1547).

    Problem statement:
        - You have a stick spanning positions [0, n].
        - You must perform cuts at each position listed in `cuts`.
        - When you cut a segment, the cost of that cut equals the *length of the
          current segment* being cut.
        - After cutting, the segment splits into two subsegments.
        - The order of cuts affects the total cost.
        - Goal: minimize the total cost.

    Key idea (interval DP):
        If we consider an interval [pos[i], pos[j]] and choose the first cut at
        some pos[k] (i < k < j), then:
            - We pay (pos[j] - pos[i]) once for that first cut (current segment length).
            - Remaining cuts split into two independent subproblems:
                  [pos[i], pos[k]] and [pos[k], pos[j]].

    Preprocessing:
        Build a sorted list of cut positions including endpoints:
            pos = [0] + sorted(cuts) + [n]
        Let m = len(pos).

    DP definition:
        dp[i][j] = minimum cost to fully cut the segment from pos[i] to pos[j],
                   assuming we must perform all cuts strictly between i and j
                   (i.e., at indices i+1..j-1).

    Base case:
        If j == i + 1, there are no cut positions inside the interval, so:
            dp[i][j] = 0

    Transition:
        For intervals that contain at least one cut, try each possible first cut k:
            dp[i][j] = min_{k in (i+1..j-1)} (
                           dp[i][k] + dp[k][j] + (pos[j] - pos[i])
                       )

    Complexity:
        Let m = len(pos) = len(cuts) + 2
        Time:  O(m^3)  (length * i * k loops)
        Space: O(m^2)  (dp table)

    Args:
        n: Length of the stick.
        cuts: List of cut positions.

    Returns:
        Minimum total cost to perform all cuts.
    """
    # Include endpoints and sort cut positions so intervals are in increasing order.
    pos = [0] + sorted(cuts) + [n]
    m = len(pos)

    # dp[i][j] = min cost to cut segment [pos[i], pos[j]] fully.
    # Initialized to 0, which naturally covers base intervals with no interior cuts.
    dp = [[0] * m for _ in range(m)]

    # length is the index distance between i and j (j = i + length).
    # Start at 2 because:
    #   length = 1 => adjacent endpoints => no cuts inside => dp stays 0
    for length in range(2, m):
        for i in range(0, m - length):
            j = i + length

            # Find best cost for this interval by trying all first cuts k.
            best = float("inf")
            for k in range(i + 1, j):
                # Cost = left subinterval + right subinterval + current segment length.
                cand = dp[i][k] + dp[k][j] + (pos[j] - pos[i])
                best = min(best, cand)

            # If there were no valid k (happens only when j == i+1),
            # best remains inf and the correct value is 0.
            dp[i][j] = 0 if best == float("inf") else best

    # Answer is cost to cut the entire stick [0, n].
    return dp[0][m - 1]

# ===========================
# Test Harness
# ===========================

def test(name, n, cuts, expected):
    """Test min_cost.

    Prints:
        - Test name
        - Input stick length and cut positions
        - Computed result vs expected result
    """
    result = min_cost(n, cuts)
    print(name)
    print(f"n = {n}, cuts = {cuts}")
    print(f"min_cost = {result} (expected {expected})\n")


print("=== Testing min_cost (Cutting Sticks DP) ===\n")

# Test 1: Classic example
# n = 7, cuts = {1,3,4,5} => expected 16
test("Test 1: Classic example",
     7, [1, 3, 4, 5], 16)

# Test 2: Two cuts
# n = 9, cuts = {5,6} => expected 13
test("Test 2: Two cuts",
     9, [5, 6], 13)

# Test 3: Single cut
# n = 10, cuts = {4} => expected 10
test("Test 3: Single cut",
     10, [4], 10)

# Test 4: Even spacing
# n = 10, cuts = {2,4,6,8} => expected 24
test("Test 4: Even spacing",
     10, [2, 4, 6, 8], 24)

# Test 5: No cuts
# n = 100, cuts = {} => expected 0
test("Test 5: No cuts",
     100, [], 0)
