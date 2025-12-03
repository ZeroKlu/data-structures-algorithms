#include <stdio.h>
#include <stdlib.h>

int uniquePaths(int m, int n) {
    int **dp = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        dp[i] = (int *)malloc(n * sizeof(int));
    }
    for (int i = 0; i < m; i++) {
        dp[i][0] = 1;
    }
    for (int j = 0; j < n; j++) {
        dp[0][j] = 1;
    }
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }
    int ans = dp[m - 1][n - 1];
    for (int i = 0; i < m; i++) {
        free(dp[i]);
    }
    free(dp);
    return ans;
}


int main(void) {

    printf("=== Test: uniquePaths ===\n\n");

    struct {
        int m, n;
        int expected;
    } tests[] = {
        {1, 1, 1},
        {1, 5, 1},
        {3, 3, 6},
        {3, 7, 28},
        {3, 2, 3},
        {7, 3, 28},
        {10, 10, 48620}
    };

    int numTests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < numTests; i++) {
        int m = tests[i].m;
        int n = tests[i].n;
        int expected = tests[i].expected;
        int result = uniquePaths(m, n);

        printf("uniquePaths(%d, %d) = %d (expected %d)\n",
               m, n, result, expected);
    }

    return 0;
}
