"""
Problem 8.4 — Unique Paths

You are given an m x n grid.
- You start at the top-left corner (0, 0)
- You want to reach the bottom-right corner (m - 1, n - 1)
- You may only move:
    1) Right
    2) Down

This function computes the total number of unique paths
from start to finish using Dynamic Programming.
"""

def unique_paths(m: int, n: int) -> int:
    """
    Return the number of unique paths from (0, 0) to (m - 1, n - 1).

    Dynamic Programming definition:
    dp[i][j] = number of unique ways to reach cell (i, j)
    """

    # --------------------------------------------------
    # Create a DP table with m rows and n columns.
    #
    # Initialize every cell to 1 because:
    # - There is exactly ONE way to reach any cell
    #   in the first row (only move right).
    # - There is exactly ONE way to reach any cell
    #   in the first column (only move down).
    # --------------------------------------------------
    dp = [[1] * n for _ in range(m)]

    # --------------------------------------------------
    # Fill the DP table starting from cell (1, 1)
    # since row 0 and column 0 are already correct.
    #
    # For each cell (i, j):
    # - You can arrive from the cell above: dp[i - 1][j]
    # - Or from the cell to the left: dp[i][j - 1]
    #
    # Total paths to (i, j) is the sum of both.
    # --------------------------------------------------
    for i in range(1, m):
        for j in range(1, n):
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1]

    # --------------------------------------------------
    # The bottom-right cell contains the total number
    # of unique paths to reach the destination.
    # --------------------------------------------------
    return dp[m - 1][n - 1]


# ===============================
# Tests (top-level)
# ===============================

# Each test case is (m, n, expected_result)
tests = [
    (1, 1, 1),        # single cell
    (1, 5, 1),        # only right moves possible
    (3, 3, 6),        # small square grid
    (3, 7, 28),       # rectangular grid
    (3, 2, 3),
    (7, 3, 28),       # symmetry check
    (10, 10, 48620),  # larger grid
]

print("=== Test: unique_paths ===\n")

# Run all test cases and verify correctness
for m_val, n_val, expected in tests:
    result = unique_paths(m_val, n_val)
    print(f"unique_paths({m_val}, {n_val}) = {result} (expected {expected})")
