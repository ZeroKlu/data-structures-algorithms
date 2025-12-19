/*
 * Problem — Longest Common Subsequence (LCS)
 *
 * Given two strings text1 and text2, return the length of their
 * longest common subsequence.
 *
 * A subsequence:
 * - Maintains relative order
 * - Does NOT need to be contiguous
 *
 * Example:
 *   text1 = "abcde"
 *   text2 = "ace"
 *   LCS = "ace" → length = 3
 *
 * This implementation uses Dynamic Programming with a 2D table.
 *
 * Time Complexity:
 *   O(m * n), where:
 *     m = length of text1
 *     n = length of text2
 *
 * Space Complexity:
 *   O(m * n) for the DP table
 */
static int LongestCommonSubsequence(string text1, string text2)
{
    /*
     * m = length of the first string
     * n = length of the second string
     */
    int m = text1.Length;
    int n = text2.Length;

    /*
     * dp[i, j] represents the length of the LCS between:
     *   - the first i characters of text1 (text1[0..i-1])
     *   - the first j characters of text2 (text2[0..j-1])
     *
     * The table has size (m + 1) x (n + 1) to handle base cases:
     *   - dp[0, j] = 0 → LCS with an empty text1 prefix
     *   - dp[i, 0] = 0 → LCS with an empty text2 prefix
     *
     * C# initializes the array with zeros, so base cases
     * are already handled implicitly.
     */
    int[,] dp = new int[m + 1, n + 1];

    /*
     * Build the DP table bottom-up.
     *
     * We start from i = 1 and j = 1 because:
     *   - row 0 and column 0 represent empty prefixes
     */
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            /*
             * If the current characters match:
             *   - We can extend the LCS from the previous prefixes
             *   - Take the diagonal value and add 1
             */
            if (text1[i - 1] == text2[j - 1])
            {
                dp[i, j] = dp[i - 1, j - 1] + 1;
            }
            else
            {
                /*
                 * If the characters do NOT match:
                 *   - Either skip the current character from text1
                 *   - Or skip the current character from text2
                 *
                 * Take the maximum of those two possibilities.
                 */
                dp[i, j] = Math.Max(dp[i - 1, j], dp[i, j - 1]);
            }
        }
    }

    /*
     * dp[m, n] contains the LCS length for the full strings
     */
    return dp[m, n];
}

// ===============================
// Tests (top-level)
// ===============================

/*
 * Each test case contains:
 *   - s1: first input string
 *   - s2: second input string
 *   - expected: expected LCS length
 *
 * Test coverage includes:
 *   - Normal cases
 *   - Identical strings
 *   - No common subsequence
 *   - Repeated characters
 *   - Empty string edge cases
 */
var tests = new (string s1, string s2, int expected)[]
{
    ("abcde", "ace", 3),
    ("abc", "abc", 3),
    ("abc", "def", 0),
    ("AGGTAB", "GXTXAYB", 4),
    ("bl", "yby", 1),
    ("aaaa", "aa", 2),
    ("", "", 0),
    ("abc", "", 0),
    ("", "abc", 0)
};

Console.WriteLine("=== Test: LongestCommonSubsequence ===\n");

/*
 * Execute each test and print:
 *   - Input strings
 *   - Computed result
 *   - Expected result
 */
foreach (var t in tests)
{
    int result = LongestCommonSubsequence(t.s1, t.s2);
    Console.WriteLine(
        $"LCS(\"{t.s1}\", \"{t.s2}\") = {result} (expected {t.expected})"
    );
}
