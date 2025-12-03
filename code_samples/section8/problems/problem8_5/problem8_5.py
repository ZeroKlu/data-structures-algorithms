"""Problem 8.5: Longest Common Subsequence."""

def lcs_length(text1: str, text2: str) -> int:
    """Longest common subsequence."""
    m, n = len(text1), len(text2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if text1[i - 1] == text2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])
    return dp[m][n]

# ============================
# Tests (top-level)
# ============================

tests = [
    ("abcde", "ace", 3),
    ("abc", "abc", 3),
    ("abc", "def", 0),
    ("AGGTAB", "GXTXAYB", 4),
    ("bl", "yby", 1),
    ("aaaa", "aa", 2),
    ("", "", 0),
    ("abc", "", 0),
    ("", "abc", 0),
]

print("=== Test: lcs_length ===\n")

for s1, s2, expected in tests:
    result = lcs_length(s1, s2)
    print(f'LCS("{s1}", "{s2}") = {result} (expected {expected})')
