"""Problem 8.4. Unique Paths."""

def unique_paths(m: int, n: int) -> int:
    """Return the number of unique paths from (0, 0) to (m - 1, n - 1)."""
    dp = [[1] * n for _ in range(m)]
    for i in range(1, m):
        for j in range(1, n):
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1]
    return dp[m - 1][n - 1]

# ===============================
# Tests (top-level)
# ===============================

tests = [
    (1, 1, 1),
    (1, 5, 1),
    (3, 3, 6),
    (3, 7, 28),
    (3, 2, 3),
    (7, 3, 28),
    (10, 10, 48620),
]

print("=== Test: unique_paths ===\n")

for x, y, expected in tests:
    result = unique_paths(x, y)
    print(f"unique_paths({x}, {y}) = {result} (expected {expected})")
