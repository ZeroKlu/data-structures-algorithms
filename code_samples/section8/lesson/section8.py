"""
Section 8 - Dynamic Programming

This module demonstrates several classic dynamic programming (DP) problems:

  1) fib_dp        - Fibonacci number computation using O(1) space DP
  2) climb_stairs  - Counting ways to climb stairs (1 or 2 steps) using O(1) space DP
  3) knapsack_01   - 0/1 knapsack using a 1D DP array (space-optimized)
  4) lcs_length    - Longest Common Subsequence length using a 2D DP table

At the bottom, a simple test harness runs if the file is executed directly.
"""


def fib_dp(n: int) -> int:
    """
    Fibonacci using dynamic programming (iterative, O(1) extra space).

    Fibonacci definition:
      F(0) = 0
      F(1) = 1
      F(n) = F(n-1) + F(n-2)  for n >= 2

    Args:
        n: Which Fibonacci index to compute. Typically n >= 0.

    Returns:
        The nth Fibonacci number.

    Approach:
        Instead of storing a full dp array of size n+1, keep only:
          prev2 = F(i-2)
          prev1 = F(i-1)
        Then compute curr = prev1 + prev2 and slide the window.

    Complexity:
        Time:  O(n)
        Space: O(1)

    Notes:
        - For negative n, the current base-case logic returns n when n <= 1.
          In production code, you might validate input and raise ValueError.
        - Python integers are unbounded, so overflow is not a concern here
          (unlike fixed-width integer languages).
    """
    # Base cases: F(0)=0, F(1)=1
    if n <= 1:
        return n

    prev2 = 0  # F(0)
    prev1 = 1  # F(1)

    # Compute F(2) through F(n)
    for _ in range(2, n + 1):
        curr = prev1 + prev2  # F(i) = F(i-1) + F(i-2)
        prev2 = prev1         # shift window forward
        prev1 = curr

    return prev1  # F(n)


def climb_stairs(n: int) -> int:
    """
    Climb stairs (classic DP).

    Problem:
        You are at step 0 and want to reach step n.
        Each move you can climb either 1 step or 2 steps.
        Count how many distinct ways you can reach step n.

    Recurrence:
        ways(0) = 1  (one way: do nothing / already there)
        ways(1) = 1
        ways(n) = ways(n-1) + ways(n-2)  for n >= 2

    Args:
        n: Number of steps (typically n >= 0).

    Returns:
        Number of distinct ways to climb to step n.

    Approach:
        Use the same O(1) space pattern as Fibonacci:
          prev2 = ways(i-2)
          prev1 = ways(i-1)
        Then curr = prev1 + prev2.

    Complexity:
        Time:  O(n)
        Space: O(1)

    Notes:
        - This implementation returns 1 for n=0 and n=1, which matches the
          standard combinatorial interpretation.
        - Like Fibonacci, results grow quickly, but Python can represent them
          exactly with arbitrary-precision integers.
    """
    # Base cases: ways(0)=1, ways(1)=1
    if n <= 1:
        return 1

    prev2 = 1  # ways(0)
    prev1 = 1  # ways(1)

    # Compute ways(2) through ways(n)
    for _ in range(2, n + 1):
        curr = prev1 + prev2  # ways(i) = ways(i-1) + ways(i-2)
        prev2 = prev1
        prev1 = curr

    return prev1  # ways(n)


def knapsack_01(n: int, W: int, w: list[int], v: list[int]) -> int:
    """
    0/1 Knapsack using a 1D DP array (space-optimized).

    Problem:
        - There are n items.
        - Item i has weight w[i] and value v[i].
        - Knapsack capacity is W.
        - Each item can be selected at most once (0/1).

    Args:
        n: Number of items (should match len(w) and len(v)).
        W: Capacity (maximum total weight allowed).
        w: List of item weights.
        v: List of item values.

    Returns:
        Maximum total value achievable without exceeding capacity W.

    DP definition:
        dp[c] = best value achievable with capacity exactly/at most c
                after processing items up to some point.

    Transition (0/1 constraint):
        For each item i:
          for c from W down to w[i]:
              dp[c] = max(dp[c], dp[c - w[i]] + v[i])

    Why iterate capacity downward?
        - Downward iteration ensures each item is used at most once.
        - If you iterate upward, dp[c - w[i]] might already include item i,
          turning the solution into an unbounded knapsack.

    Complexity:
        Time:  O(n * W)
        Space: O(W)

    Notes:
        - Assumes W >= 0 and weights are non-negative.
        - If an item's weight is greater than W, it will never be considered
          by the inner loop (which is fine).
    """
    # dp[c] starts at 0, meaning: with capacity c and no chosen items, value=0
    dp = [0] * (W + 1)

    # Process each item once
    for i in range(n):
        # Traverse capacities backward to enforce the 0/1 (single-use) constraint
        for c in range(W, w[i] - 1, -1):
            with_item = dp[c - w[i]] + v[i]  # value if we take item i
            if with_item > dp[c]:
                dp[c] = with_item            # keep the better choice

    # Best value at capacity W
    return dp[W]


def lcs_length(s: str, t: str) -> int:
    """
    Longest Common Subsequence (LCS) length using a 2D DP table.

    Subsequence reminder:
        Characters must appear in order, but do not need to be contiguous.

    DP definition:
        dp[i][j] = LCS length between prefixes:
          s[:i] (first i characters of s)
          t[:j] (first j characters of t)

    Base:
        dp[0][*] = 0 and dp[*][0] = 0 (empty prefix vs anything)

    Transition:
        If s[i-1] == t[j-1]:
            dp[i][j] = dp[i-1][j-1] + 1
        Else:
            dp[i][j] = max(dp[i-1][j], dp[i][j-1])

    Args:
        s: First string.
        t: Second string.

    Returns:
        Length of the LCS between s and t.

    Complexity:
        Time:  O(len(s) * len(t))
        Space: O(len(s) * len(t))

    Notes:
        - Space can be optimized to O(min(n,m)) with a rolling array,
          but the full 2D table is easier to understand.
    """
    n, m = len(s), len(t)

    # dp table initialized to 0: (n+1) rows x (m+1) columns
    dp = [[0] * (m + 1) for _ in range(n + 1)]

    # Fill dp for non-empty prefixes
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            if s[i - 1] == t[j - 1]:
                # Matching characters extend the LCS by 1
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                # Otherwise, best LCS comes from skipping one char
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

    # Full strings correspond to dp[n][m]
    return dp[n][m]


# ============================
# Tests for Section 8 (DP)
# ============================

if __name__ == "__main__":
    # ---- fib_dp tests ----
    print("=== Test: fib_dp ===\n")

    tests = [0, 1, 2, 3, 4, 5, 10, 20, 30, 50]
    expected = [0, 1, 1, 2, 3, 5, 55, 6765, 832040, 12586269025]

    # Use zip to pair each test input with its expected output
    for num, exp in zip(tests, expected):
        result = fib_dp(num)
        print(f"fib_dp({num}) = {result} (expected {exp})")

    # ---- climb_stairs tests ----
    print("\n=== Test: climb_stairs ===\n")

    climb_tests = [0, 1, 2, 3, 4, 5, 10]
    climb_expected = [1, 1, 2, 3, 5, 8, 89]

    for num, exp in zip(climb_tests, climb_expected):
        result = climb_stairs(num)
        print(f"climb_stairs({num}) = {result} (expected {exp})")

    # ---- knapsack_01 tests ----
    print("\n=== Test: knapsack_01 ===\n")

    # Example weights & values
    weights = [2, 3, 4, 5]
    values = [3, 4, 5, 6]
    num_items = len(weights)

    # Capacity 5: best is weights 2+3 -> values 3+4 = 7
    print(
        f"knapsack_01(n, W=5) = {knapsack_01(num_items, 5, weights, values)} (expected 7)"
    )

    # Capacity 7: best is weights 2+5 -> values 3+6 = 9
    print(
        f"knapsack_01(n, W=7) = {knapsack_01(num_items, 7, weights, values)} (expected 9)"
    )

    # Capacity 3: best is weight 3 -> value 4
    print(
        f"knapsack_01(n, W=3) = {knapsack_01(num_items, 3, weights, values)} (expected 4)"
    )

    # ---- lcs_length tests ----
    print("\n=== Test: lcs_length ===\n")

    # Each test is: (s, t, expected_lcs_length)
    lcs_tests = [
        ("abcde", "ace", 3),
        ("abc", "abc", 3),
        ("abc", "def", 0),
        ("", "abc", 0),
        ("AGGTAB", "GXTXAYB", 4),
    ]

    for s_val, t_val, exp in lcs_tests:
        result = lcs_length(s_val, t_val)
        print(f'lcs_length("{s_val}", "{t_val}") = {result} (expected {exp})')
