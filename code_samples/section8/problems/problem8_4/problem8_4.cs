static int UniquePaths(int m, int n)
{
    /*
     * Problem: Unique Paths
     *
     * You are given an m x n grid.
     * - Start position: top-left corner (0, 0)
     * - End position:   bottom-right corner (m-1, n-1)
     * - Allowed moves: only RIGHT or DOWN
     *
     * Goal:
     * Return the total number of distinct paths from start to finish.
     */

    /*
     * dp[i, j] will store the number of unique ways
     * to reach cell (i, j).
     *
     * Grid coordinates:
     * - i represents the row (0 .. m-1)
     * - j represents the column (0 .. n-1)
     */
    int[,] dp = new int[m, n];

    /*
     * Base Case 1: First column
     *
     * For any cell in the first column (j = 0),
     * there is exactly ONE way to reach it:
     * - move DOWN all the way
     */
    for (int i = 0; i < m; i++) {
        dp[i, 0] = 1;
    }

    /*
     * Base Case 2: First row
     *
     * For any cell in the first row (i = 0),
     * there is exactly ONE way to reach it:
     * - move RIGHT all the way
     */
    for (int j = 0; j < n; j++) {
        dp[0, j] = 1;
    }

    /*
     * Fill the rest of the DP table.
     *
     * For any cell (i, j), where i > 0 and j > 0:
     * - You can arrive from:
     *     1) The cell above:     dp[i - 1, j]
     *     2) The cell to the left: dp[i, j - 1]
     *
     * Therefore, the recurrence relation is:
     *
     * dp[i, j] = dp[i - 1, j] + dp[i, j - 1]
     */
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[i, j] = dp[i - 1, j] + dp[i, j - 1];
        }
    }

    /*
     * The bottom-right cell contains the total number
     * of unique paths from start to finish.
     */
    return dp[m - 1, n - 1];
}

// ===============================
// Tests (top-level)
// ===============================

/*
 * Each test case contains:
 * - m: number of rows
 * - n: number of columns
 * - expected: known correct number of unique paths
 */
var tests = new (int m, int n, int expected)[]
{
    (1, 1, 1),        // Single cell
    (1, 5, 1),        // Only right moves
    (3, 3, 6),        // Small square grid
    (3, 7, 28),       // Rectangular grid
    (3, 2, 3),
    (7, 3, 28),       // Symmetry check
    (10, 10, 48620)   // Larger grid
};

Console.WriteLine("=== Test: UniquePaths ===\n");

/*
 * Execute each test and compare the computed result
 * with the expected value.
 */
foreach (var t in tests)
{
    int result = UniquePaths(t.m, t.n);
    Console.WriteLine(
        $"UniquePaths({t.m}, {t.n}) = {result} (expected {t.expected})"
    );
}
