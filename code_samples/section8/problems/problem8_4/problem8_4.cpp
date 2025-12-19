#include <iostream>
#include <vector>
using namespace std;

/*
 * Problem: Unique Paths
 *
 * You are given an m x n grid.
 * - You start at the top-left corner (0, 0)
 * - You want to reach the bottom-right corner (m-1, n-1)
 * - You may only move:
 *     1) Right
 *     2) Down
 *
 * This function calculates the total number of unique paths
 * using Dynamic Programming.
 */
int uniquePaths(int m, int n) {

    /*
     * Create a 2D DP table of size m x n.
     *
     * dp[i][j] represents the number of unique ways
     * to reach cell (i, j).
     *
     * Initialization:
     * - All cells are initialized to 1 because:
     *   * There is exactly ONE way to reach any cell in:
     *       - The first row (only move right)
     *       - The first column (only move down)
     */
    vector<vector<int>> dp(m, vector<int>(n, 1));

    /*
     * Fill the DP table starting from cell (1,1)
     * because the first row and first column are already correct.
     *
     * For each cell (i, j):
     * - You can arrive from:
     *     1) The cell above     -> dp[i - 1][j]
     *     2) The cell to the left -> dp[i][j - 1]
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
     * The bottom-right cell contains the total number
     * of unique paths from the start to the destination.
     */
    return dp[m - 1][n - 1];
}

int main() {
    cout << "=== Test: uniquePaths ===\n\n";

    /*
     * Each test case specifies:
     * - m: number of rows
     * - n: number of columns
     * - expected: correct number of unique paths
     */
    struct TestCase {
        int m, n;
        int expected;
    };

    vector<TestCase> tests = {
        {1, 1, 1},        // Single cell
        {1, 5, 1},        // Only right moves
        {3, 3, 6},        // Classic 3x3 grid
        {3, 7, 28},       // Rectangular grid
        {3, 2, 3},
        {7, 3, 28},       // Symmetry check
        {10, 10, 48620}   // Larger grid
    };

    /*
     * Run each test and compare the computed result
     * against the expected value.
     */
    for (const auto& t : tests) {
        int result = uniquePaths(t.m, t.n);
        cout << "uniquePaths(" << t.m << ", " << t.n << ") = "
             << result << " (expected " << t.expected << ")\n";
    }

    return 0;
}
