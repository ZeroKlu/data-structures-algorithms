/**
 * Problem 8.4 — Unique Paths
 *
 * You are given an m x n grid.
 * - Start position: top-left corner (0, 0)
 * - End position: bottom-right corner (m-1, n-1)
 * - Allowed moves:
 *      1) Move RIGHT
 *      2) Move DOWN
 *
 * This function returns the total number of unique paths
 * from the start to the end.
 */
function uniquePaths(m, n) {

    /*
     * dp[i][j] represents the number of unique paths
     * to reach cell (i, j).
     *
     * We initialize the entire grid with 1s because:
     * - There is exactly ONE way to reach any cell
     *   in the first row (only move right)
     * - There is exactly ONE way to reach any cell
     *   in the first column (only move down)
     */
    const dp = Array.from({ length: m }, () => new Array(n).fill(1));

    /*
     * Fill the DP table starting from cell (1, 1),
     * since row 0 and column 0 are already correct.
     *
     * For each cell (i, j):
     * - You can come from the cell above: dp[i - 1][j]
     * - Or from the cell to the left: dp[i][j - 1]
     *
     * Total paths to (i, j) is the sum of both.
     */
    for (let i = 1; i < m; i++) {
        for (let j = 1; j < n; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }

    /*
     * The bottom-right cell contains the total number
     * of unique paths from (0,0) to (m-1,n-1).
     */
    return dp[m - 1][n - 1];
}

// ====================================
// Tests (top-level)
// ====================================

/*
 * Each test case is of the form:
 * [m, n, expectedResult]
 */
const tests = [
    [1, 1, 1],        // single cell
    [1, 5, 1],        // only right moves possible
    [3, 3, 6],        // small square grid
    [3, 7, 28],       // rectangular grid
    [3, 2, 3],
    [7, 3, 28],       // symmetry check
    [10, 10, 48620]   // larger grid
];

console.log("=== Test: uniquePaths ===\n");

/*
 * Run each test and compare the result
 * with the expected output.
 */
for (const [m, n, expected] of tests) {
    const result = uniquePaths(m, n);
    console.log(
        `uniquePaths(${m}, ${n}) = ${result} (expected ${expected})`
    );
}
