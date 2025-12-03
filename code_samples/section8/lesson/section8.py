"""Section 8 - Dynamic Programming"""

def fib_dp(n: int) -> int:
    """Fibonacci using dynamic programming."""
    if n <= 1:
        return n
    prev2 = 0  # F(0)
    prev1 = 1  # F(1)
    for _ in range(2, n + 1):
        curr = prev1 + prev2
        prev2 = prev1
        prev1 = curr
    return prev1

def climb_stairs(n: int) -> int:
    """Climb stairs."""
    if n <= 1:
        return 1
    prev2 = 1  # ways(0)
    prev1 = 1  # ways(1)
    for _ in range(2, n + 1):
        curr = prev1 + prev2
        prev2 = prev1
        prev1 = curr
    return prev1

def knapsack_01(n: int, W: int, w: list[int], v: list[int]) -> int:
    """Knapsack"""
    dp = [0] * (W + 1)
    for i in range(n):
        for c in range(W, w[i] - 1, -1):
            with_item = dp[c - w[i]] + v[i]
            if with_item > dp[c]:
                dp[c] = with_item
    return dp[W]

def lcs_length(s: str, t: str) -> int:
    """Longest common subsequence."""
    n, m = len(s), len(t)
    dp = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            if s[i - 1] == t[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])
    return dp[n][m]

# ============================
# Tests for Section 8 (DP)
# ============================

if __name__ == "__main__":

    print("=== Test: fib_dp ===\n")

    tests =    [0, 1, 2, 3, 4, 5, 10, 20, 30, 50]
    expected = [0, 1, 1, 2, 3, 5, 55, 6765, 832040, 12586269025]

    for num, exp in zip(tests, expected):
        result = fib_dp(num)
        print(f"fib_dp({num}) = {result} (expected {exp})")

    print("\n=== Test: climb_stairs ===\n")

    climb_tests    = [0, 1, 2, 3, 4, 5, 10]
    climb_expected = [1, 1, 2, 3, 5, 8, 89]

    for num, exp in zip(climb_tests, climb_expected):
        result = climb_stairs(num)
        print(f"climb_stairs({num}) = {result} (expected {exp})")

    print("\n=== Test: knapsack_01 ===\n")

    # Example weights & values
    weights = [2, 3, 4, 5]
    values = [3, 4, 5, 6]
    num = len(weights)

    print(f"knapsack_01(n, W=5) = {knapsack_01(num, 5, weights, values)} (expected 7)")
    print(f"knapsack_01(n, W=7) = {knapsack_01(num, 7, weights, values)} (expected 9)")
    print(f"knapsack_01(n, W=3) = {knapsack_01(num, 3, weights, values)} (expected 4)")

    print("\n=== Test: lcs_length ===\n")

    lcs_tests = [
        ("abcde", "ace", 3),
        ("abc", "abc", 3),
        ("abc", "def", 0),
        ("", "abc", 0),
        ("AGGTAB", "GXTXAYB", 4)
    ]

    for s_val, t_val, exp in lcs_tests:
        result = lcs_length(s_val, t_val)
        print(f'lcs_length("{s_val}", "{t_val}") = {result} (expected {exp})')
