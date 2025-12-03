#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int longestCommonSubsequence(const char *text1, const char *text2) {
    int m = (int)strlen(text1);
    int n = (int)strlen(text2);
    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) {
        dp[i] = (int *)calloc(n + 1, sizeof(int));
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                int a = dp[i - 1][j];
                int b = dp[i][j - 1];
                dp[i][j] = a > b ? a : b;
            }
        }
    }

    int ans = dp[m][n];

    for (int i = 0; i <= m; i++) {
        free(dp[i]);
    }
    free(dp);

    return ans;
}

int main(void) {
    printf("=== Test: longestCommonSubsequence ===\n\n");

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

    int numTests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < numTests; i++) {
        int result = longestCommonSubsequence(tests[i].s1, tests[i].s2);
        printf("LCS(\"%s\", \"%s\") = %d (expected %d)\n",
               tests[i].s1, tests[i].s2, result, tests[i].expected);
    }

    return 0;
}
