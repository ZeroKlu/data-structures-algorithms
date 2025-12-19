#include <stdio.h>
#include <stdlib.h>

/*
 * Problem: Unique Paths
 *
 * You are given an m x n grid.
 * - You start at the top-left corner (0,0)
 * - You want to reach the bottom-right corner (m-1,n-1)
 * - You may only move:
 *     - Down
 *     - Right
 *
 * This function computes the number of unique paths
 * using Dynamic Programming.
 */
int uniquePaths(int m, int n) {

    /*
     * Allocate a 2D DP table:
     *
     * dp[i][j] will store the number of unique paths
     * to reach cell (i, j).
     *
     * Memory layout:
     * - dp is an array of pointers (rows)
     * - each dp[i] is an array of n integers (columns)
     */
    int **dp = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        dp[i] = (int *)malloc(n * sizeof(int));
    }

    /*
     * Base case initialization:
     *
     * First column (j = 0):
     * - Only one way to reach any cell in the first column:
     *   move DOWN repeatedly.
     */
    for (int i = 0; i < m; i++) {
        dp[i][0] = 1;
    }

    /*
     * First row (i = 0):
     * - Only one way to reach any cell in the first row:
     *   move RIGHT repeatedly.
     */
    for (int j = 0; j < n; j++) {
        dp[0][j] = 1;
    }

    /*
     * Fill the DP table:
     *
     * For any interior cell (i, j):
     * - You can arrive from:
     *     1) Above:    dp[i - 1][j]
     *     2) Left:     dp[i][j - 1]
     *
     * So the recurrence relation is:
     *
     * dp[i][j] = dp[i - 1][j] + dp[i][j - 1]
     */
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }

    /*
     * The answer is located in the bottom-right cell
     * of the grid.
     */
    int ans = dp[m - 1][n - 1];

    /*
     * Free dynamically allocated memory
     * to prevent memory leaks.
     */
    for (int i = 0; i < m; i++) {
        free(dp[i]);
    }
    free(dp);

    return ans;
}


int main(void) {

    printf("=== Test: uniquePaths ===\n\n");

    /*
     * Test cases:
     * Each case specifies:
     *   - m rows
     *   - n columns
     *   - expected number of unique paths
     */
    struct {
        int m, n;
        int expected;
    } tests[] = {
        {1, 1, 1},        // single cell
        {1, 5, 1},        // only right moves
        {3, 3, 6},        // classic 3x3 grid
        {3, 7, 28},       // rectangular grid
        {3, 2, 3},
        {7, 3, 28},       // symmetry check
        {10, 10, 48620}   // larger grid
    };

    int numTests = sizeof(tests) / sizeof(tests[0]);

    /*
     * Execute each test and compare with expected output
     */
    for (int i = 0; i < numTests; i++) {
        int m = tests[i].m;
        int n = tests[i].n;
        int expected = tests[i].expected;
        int result = uniquePaths(m, n);

        printf(
            "uniquePaths(%d, %d) = %d (expected %d)\n",
            m, n, result, expected
        );
    }

    return 0;
}
