package code_samples.section8.problems.problem8_4;

public class problem8_4 {

    int uniquePaths(int m, int n) {
        int[][] dp = new int[m][n];
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
        return dp[m - 1][n - 1];
    }

    // ==============================
    // Test Harness
    // ==============================
    public static void main(String[] args) {

        problem8_4 solver = new problem8_4();

        // (m, n, expected)
        int[][] tests = {
            {1, 1, 1},
            {1, 5, 1},
            {3, 3, 6},
            {3, 7, 28},
            {3, 2, 3},
            {7, 3, 28},
            {10, 10, 48620}
        };

        System.out.println("=== Test: uniquePaths ===\n");

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
