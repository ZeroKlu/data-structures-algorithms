/*
 * Problem 8.5 — Longest Common Subsequence (LCS)
 *
 * Given two strings text1 and text2, return the length of their
 * longest common subsequence.
 *
 * A subsequence:
 *  - Keeps the original character order
 *  - Does NOT need to be contiguous
 *
 * Example:
 *   text1 = "abcde"
 *   text2 = "ace"
 *   LCS = "ace" → length = 3
 *
 * This implementation uses Dynamic Programming with a 2D table.
 *
 * Time Complexity:
 *   O(m * n)
 *
 * Space Complexity:
 *   O(m * n)
 *
 * where:
 *   m = text1.length
 *   n = text2.length
 */
function longestCommonSubsequence(text1, text2) {

    /*
     * Lengths of the two input strings.
     */
    const m = text1.length;
    const n = text2.length;

    /*
     * dp[i][j] represents the length of the LCS between:
     *   - the first i characters of text1 (text1[0 .. i-1])
     *   - the first j characters of text2 (text2[0 .. j-1])
     *
     * We allocate (m + 1) x (n + 1) so that:
     *   - dp[0][*] = 0 → empty prefix of text1
     *   - dp[*][0] = 0 → empty prefix of text2
     *
     * JavaScript initializes the table with 0s, which naturally
     * covers the base cases.
     */
    const dp = Array.from(
        { length: m + 1 },
        () => new Array(n + 1).fill(0)
    );

    /*
     * Build the DP table bottom-up.
     *
     * i and j start from 1 because:
     *   - dp[0][*] and dp[*][0] are already base cases (0)
     */
    for (let i = 1; i <= m; i++) {
        for (let j = 1; j <= n; j++) {

            /*
             * If the current characters match:
             *   - We extend the LCS formed by the previous prefixes
             *   - Take the diagonal value dp[i-1][j-1] and add 1
             */
            if (text1[i - 1] === text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            /*
             * If the characters do NOT match:
             *   - Either skip the current character in text1
             *   - Or skip the current character in text2
             *
             * We take the maximum of those two options.
             */
            else {
                dp[i][j] = Math.max(
                    dp[i - 1][j],
                    dp[i][j - 1]
                );
            }
        }
    }

    /*
     * The bottom-right cell contains the LCS length
     * for the full strings text1 and text2.
     */
    return dp[m][n];
}

// =============================
// Tests (top-level)
// =============================

/*
 * Each test case contains:
 *   [string1, string2, expected LCS length]
 *
 * Tests cover:
 *  - Standard examples
 *  - Identical strings
 *  - No overlap
 *  - Repeated characters
 *  - Empty string edge cases
 */
const tests = [
    ["abcde", "ace", 3],
    ["abc", "abc", 3],
    ["abc", "def", 0],
    ["AGGTAB", "GXTXAYB", 4],
    ["bl", "yby", 1],
    ["aaaa", "aa", 2],
    ["", "", 0],
    ["abc", "", 0],
    ["", "abc", 0],
];

console.log("=== Test: longestCommonSubsequence ===\n");

/*
 * Execute tests and print results.
 */
for (const [s1, s2, expected] of tests) {
    const result = longestCommonSubsequence(s1, s2);
    console.log(
        `LCS("${s1}", "${s2}") = ${result} (expected ${expected})`
    );
}
