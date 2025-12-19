"""
Problem 8.5 — Longest Common Subsequence (LCS)

Given two strings, text1 and text2, compute the length of their
Longest Common Subsequence (LCS).

A subsequence:
- Preserves character order
- Does NOT require characters to be contiguous

Example:
    text1 = "abcde"
    text2 = "ace"
    LCS = "ace" → length = 3

This solution uses Dynamic Programming (DP).
"""

#pylint: disable=pointless-string-statement

def lcs_length(text1: str, text2: str) -> int:
    """
    Compute the length of the Longest Common Subsequence (LCS)
    between two strings.

    Parameters:
        text1 (str): First input string
        text2 (str): Second input string

    Returns:
        int: Length of the longest common subsequence
    """

    # Lengths of the input strings
    m, n = len(text1), len(text2)

    """
    dp[i][j] represents the LCS length between:
      - the first i characters of text1  (text1[0 : i])
      - the first j characters of text2  (text2[0 : j])

    The DP table has dimensions (m + 1) x (n + 1) to naturally
    handle empty-prefix base cases.

    Base cases:
      dp[0][j] = 0  → empty text1
      dp[i][0] = 0  → empty text2
    """
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    """
    Fill the DP table row by row.

    We start from i = 1 and j = 1 because:
    - dp[0][*] and dp[*][0] are already initialized to 0
    """
    for i in range(1, m + 1):
        for j in range(1, n + 1):

            # If the current characters match,
            # extend the LCS formed by the previous prefixes.
            if text1[i - 1] == text2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1

            # Otherwise, we skip one character:
            # - either from text1 (dp[i-1][j])
            # - or from text2 (dp[i][j-1])
            # and take the better of the two.
            else:
                dp[i][j] = max(
                    dp[i - 1][j],
                    dp[i][j - 1]
                )

    """
    The bottom-right cell contains the LCS length
    for the full strings text1 and text2.
    """
    return dp[m][n]


# ============================
# Tests (top-level)
# ============================

"""
Each test case contains:
  (string1, string2, expected LCS length)

These tests cover:
- Partial overlap
- Identical strings
- No common characters
- Repeated characters
- Empty-string edge cases
"""
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

# Execute and verify each test case
for s1, s2, expected in tests:
    result = lcs_length(s1, s2)
    print(f'LCS("{s1}", "{s2}") = {result} (expected {expected})')
