package code_samples.section8.problems.problem8_5;

/*
 * Problem 8.5 — Longest Common Subsequence (LCS)
 *
 * Given two strings text1 and text2, return the length of their
 * longest common subsequence.
 *
 * A subsequence:
 *  - Preserves relative order
 *  - Does NOT need to be contiguous
 *
 * Example:
 *   text1 = "abcde"
 *   text2 = "ace"
 *   LCS = "ace" → length = 3
 *
 * This solution uses classic Dynamic Programming with a 2D table.
 *
 * Time Complexity:
 *   O(m * n), where:
 *     m = length of text1
 *     n = length of text2
 *
 * Space Complexity:
 *   O(m * n) for the DP table
 */
public class problem8_5 {

    /*
     * Computes the length of the longest common subsequence
     * between text1 and text2.
     */
    int longestCommonSubsequence(String text1, String text2) {

        /*
         * m = length of first string
         * n = length of second string
         */
        int m = text1.length();
        int n = text2.length();

        /*
         * dp[i][j] represents the length of the LCS between:
         *   - the first i characters of text1 (text1[0..i-1])
         *   - the first j characters of text2 (text2[0..j-1])
         *
         * The extra row and column (m+1, n+1) handle base cases:
         *   - dp[0][j] = 0 → empty prefix of text1
         *   - dp[i][0] = 0 → empty prefix of text2
         *
         * Java initializes arrays to 0, so base cases are implicit.
         */
        int[][] dp = new int[m + 1][n + 1];

        /*
         * Fill the DP table row by row (bottom-up).
         */
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {

                /*
                 * If the current characters match:
                 *   - Extend the LCS from the previous prefixes
                 *   - Take the diagonal value and add 1
                 */
                if (text1.charAt(i - 1) == text2.charAt(j - 1)) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                /*
                 * If the characters do NOT match:
                 *   - Either skip the current character in text1
                 *   - Or skip the current character in text2
                 *
                 * Take the maximum of those two choices.
                 */
                else {
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        /*
         * dp[m][n] contains the LCS length for the full strings
         */
        return dp[m][n];
    }

    // ==========================
    // Test Harness
    // ==========================
    public static void main(String[] args) {

        problem8_5 solver = new problem8_5();

        /*
         * Simple immutable test record:
         *   a        → first string
         *   b        → second string
         *   expected → expected LCS length
         */
        record Test(String a, String b, int expected) {}

        /*
         * Test cases cover:
         *  - Standard examples
         *  - Identical strings
         *  - No common subsequence
         *  - Repeated characters
         *  - Empty string edge cases
         */
        Test[] tests = {
            new Test("abcde", "ace", 3),
            new Test("abc", "abc", 3),
            new Test("abc", "def", 0),
            new Test("AGGTAB", "GXTXAYB", 4),
            new Test("bl", "yby", 1),
            new Test("aaaa", "aa", 2),
            new Test("", "", 0),
            new Test("abc", "", 0),
            new Test("", "abc", 0)
        };

        System.out.println("=== Test: longestCommonSubsequence ===\n");

        /*
         * Execute tests and print results
         */
        for (Test t : tests) {
            int result = solver.longestCommonSubsequence(t.a(), t.b());
            System.out.println(
                "LCS(\"" + t.a() + "\", \"" + t.b() + "\") = " + result +
                " (expected " + t.expected() + ")"
            );
        }
    }
}
