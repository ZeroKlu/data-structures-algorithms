#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

/*
 * Problem — Longest Common Subsequence (LCS)
 *
 * Given two strings text1 and text2, compute the length of their
 * longest common subsequence.
 *
 * A subsequence:
 * - preserves relative order of characters
 * - does NOT require contiguous characters
 *
 * Example:
 *   text1 = "abcde"
 *   text2 = "ace"
 *   LCS = "ace" -> length 3
 *
 * Approach:
 *   Dynamic Programming (2D table)
 *   Time:  O(m * n)
 *   Space: O(m * n)
 */
int longestCommonSubsequence(const string &text1, const string &text2) {
    /*
     * m = length of first string
     * n = length of second string
     */
    int m = (int)text1.size();
    int n = (int)text2.size();

    /*
     * dp[i][j] = length of the LCS between:
     *            text1[0..i-1] and text2[0..j-1]
     *
     * Why (m+1) x (n+1)?
     * - dp[0][*] and dp[*][0] represent comparisons with an empty prefix
     * - LCS with an empty string is always 0
     *
     * Initialization to 0 handles all base cases automatically.
     */
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    /*
     * Fill the DP table:
     * - i corresponds to prefix length in text1
     * - j corresponds to prefix length in text2
     *
     * Recurrence:
     * 1) If characters match:
     *      dp[i][j] = dp[i-1][j-1] + 1
     *    We extend the best LCS from the smaller prefixes.
     *
     * 2) If they do not match:
     *      dp[i][j] = max(dp[i-1][j], dp[i][j-1])
     *    We either:
     *      - skip the last character of text1's prefix, or
     *      - skip the last character of text2's prefix
     *    and choose the better result.
     */
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            // Compare the last characters of the current prefixes.
            if (text1[i - 1] == text2[j - 1]) {
                // Match: take diagonal + 1 (use this matching character).
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // No match: best of skipping one character from either string.
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    /*
     * dp[m][n] is the LCS length for the full strings.
     */
    return dp[m][n];
}

int main() {
    cout << "=== Test: longestCommonSubsequence ===\n\n";

    /*
     * Simple test-case struct:
     * - s1, s2: input strings
     * - expected: expected LCS length
     */
    struct TestCase {
        string s1;
        string s2;
        int expected;
    };

    /*
     * Tests include:
     * - typical case
     * - identical strings
     * - no overlap
     * - classic DP example (AGGTAB vs GXTXAYB -> 4)
     * - repeated characters
     * - empty-string edge cases
     */
    vector<TestCase> tests = {
        {"abcde", "ace", 3},
        {"abc", "abc", 3},
        {"abc", "def", 0},
        {"AGGTAB", "GXTXAYB", 4},
        {"bl", "yby", 1},
        {"aaaa", "aa", 2},
        {"", "", 0},
        {"abc", "", 0},
        {"", "abc", 0}
    };

    /*
     * Run all tests and print:
     * - the inputs
     * - the computed result
     * - the expected answer
     */
    for (const auto &t : tests) {
        int result = longestCommonSubsequence(t.s1, t.s2);

        cout << "LCS(\"" << t.s1 << "\", \"" << t.s2
             << "\") = " << result
             << " (expected " << t.expected << ")\n";
    }

    return 0;
}
