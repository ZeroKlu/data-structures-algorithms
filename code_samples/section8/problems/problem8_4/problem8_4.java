package code_samples.section8.problems.problem8_4;

public class problem8_4 {

    /**
     * Problem 8.4 — Unique Paths
     *
     * You are given an m x n grid.
     * - You start at the top-left corner (0, 0)
     * - You want to reach the bottom-right corner (m-1, n-1)
     * - You may only move:
     *      1) Down
     *      2) Right
     *
     * This method returns the total number of distinct paths
     * from start to finish.
     */
    int uniquePaths(int m, int n) {

        /*
         * dp[i][j] represents the number of unique paths
         * to reach cell (i, j).
         *
         * Dimensions:
         * - i: row index (0 to m-1)
         * - j: column index (0 to n-1)
         */
        int[][] dp = new int[m][n];

        /*
         * Base Case 1: First column (j = 0)
         *
         * For any cell in the first column, there is only
         * ONE way to reach it:
         * - Move DOWN from the start.
         */
        for (int i = 0; i < m; i++) {
            dp[i][0] = 1;
        }

        /*
         * Base Case 2: First row (i = 0)
         *
         * For any cell in the first row, there is only
         * ONE way to reach it:
         * - Move RIGHT from the start.
         */
        for (int j = 0; j < n; j++) {
            dp[0][j] = 1;
        }

        /*
         * Fill the rest of the DP table.
         *
         * For any cell (i, j), where i > 0 and j > 0:
         * - You can arrive either:
         *     1) From above    → dp[i - 1][j]
         *     2) From the left → dp[i][j - 1]
         *
         * Therefore:
         * dp[i][j] = dp[i - 1][j] + dp[i][j - 1]
         */
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }

        /*
         * The bottom-right cell contains the final answer:
         * the total number of unique paths from (0,0)
         * to (m-1,n-1).
         */
        return dp[m - 1][n - 1];
    }

    // ==============================
    // Test Harness
    // ==============================
    public static void main(String[] args) {

        problem8_4 solver = new problem8_4();

        /*
         * Each test case is of the form:
         * { m, n, expected }
         */
        int[][] tests = {
            {1, 1, 1},        // single cell
            {1, 5, 1},        // only right moves
            {3, 3, 6},        // small square grid
            {3, 7, 28},       // rectangular grid
            {3, 2, 3},
            {7, 3, 28},       // symmetry check
            {10, 10, 48620}   // larger grid
        };

        System.out.println("=== Test: uniquePaths ===\n");

        /*
         * Run each test and compare the result
         * with the expected value.
         */
        for (int[] t : tests) {
            int m = t[0];
            int n = t[1];
            int expected = t[2];

            int result = solver.uniquePaths(m, n);

            System.out.println(
                "uniquePaths(" + m + ", " + n + ") = " + result +
                " (expected " + expected + ")"
            );
        }
    }
}
