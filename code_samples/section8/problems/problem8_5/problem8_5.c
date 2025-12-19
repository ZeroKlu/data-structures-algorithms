#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Problem — Longest Common Subsequence (LCS)
 *
 * Given two strings text1 and text2, find the length of their
 * longest common subsequence.
 *
 * A subsequence:
 * - preserves relative order
 * - does NOT require characters to be contiguous
 *
 * Example:
 *   text1 = "abcde"
 *   text2 = "ace"
 *   LCS = "ace"  -> length 3
 */
int longestCommonSubsequence(const char *text1, const char *text2) {

    /*
     * m = length of first string
     * n = length of second string
     */
    int m = (int)strlen(text1);
    int n = (int)strlen(text2);

    /*
     * dp is a (m+1) x (n+1) 2D table where:
     *   dp[i][j] = length of LCS between:
     *             text1[0 .. i-1] and text2[0 .. j-1]
     *
     * We use m+1 and n+1 so that dp[0][*] and dp[*][0] represent
     * LCS with an empty prefix, which is always 0.
     */
    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    if (!dp) {
        /* If allocation fails, we can't compute; return 0 as a safe fallback. */
        return 0;
    }

    /*
     * Allocate each row as an array of (n+1) integers.
     * Using calloc initializes all values to 0, which automatically sets
     * base cases:
     *   dp[0][j] = 0 for all j
     *   dp[i][0] = 0 for all i
     */
    for (int i = 0; i <= m; i++) {
        dp[i] = (int *)calloc((size_t)(n + 1), sizeof(int));
        if (!dp[i]) {
            /*
             * If a row allocation fails, clean up what we've already allocated
             * to avoid memory leaks, then return 0.
             */
            for (int k = 0; k < i; k++) {
                free(dp[k]);
            }
            free(dp);
            return 0;
        }
    }

    /*
     * Fill the DP table from smallest prefixes to full strings.
     *
     * Recurrence:
     * - If the last characters match:
     *       dp[i][j] = dp[i-1][j-1] + 1
     *   because we can extend the LCS of the smaller prefixes by this match.
     *
     * - Otherwise:
     *       dp[i][j] = max(dp[i-1][j], dp[i][j-1])
     *   meaning we either skip the last char of text1 or skip the last char
     *   of text2, choosing the better result.
     */
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {

            /* Compare the last character of each current prefix */
            if (text1[i - 1] == text2[j - 1]) {
                /* Characters match: extend the previous best */
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                /* Characters differ: take the best by skipping one side */
                int a = dp[i - 1][j]; /* skip text1[i-1] */
                int b = dp[i][j - 1]; /* skip text2[j-1] */
                dp[i][j] = (a > b) ? a : b;
            }
        }
    }

    /*
     * dp[m][n] is the LCS length for the full strings text1 and text2.
     */
    int ans = dp[m][n];

    /*
     * Free all allocated memory to avoid leaks.
     * - First free each row
     * - Then free the array of row pointers
     */
    for (int i = 0; i <= m; i++) {
        free(dp[i]);
    }
    free(dp);

    return ans;
}

int main(void) {
    printf("=== Test: longestCommonSubsequence ===\n\n");

    /*
     * Test cases:
     * - typical examples
     * - identical strings
     * - no overlap
     * - classic DP example (AGGTAB vs GXTXAYB => 4)
     * - repeated characters
     * - empty string edge cases
     */
    struct TestCase {
        const char *s1;
        const char *s2;
        int expected;
    } tests[] = {
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

    int numTests = (int)(sizeof(tests) / sizeof(tests[0]));

    /* Run all tests and print actual vs expected results */
    for (int i = 0; i < numTests; i++) {
        int result = longestCommonSubsequence(tests[i].s1, tests[i].s2);
        printf("LCS(\"%s\", \"%s\") = %d (expected %d)\n",
               tests[i].s1, tests[i].s2, result, tests[i].expected);
    }

    return 0;
}
