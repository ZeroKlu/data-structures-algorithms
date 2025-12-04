package code_samples.section9.problems.problem9_5;

import java.util.Arrays;

public class problem9_5 {
    int minCost(int n, int[] cuts) {
        int m = cuts.length + 2;
        int[] pos = new int[m];
        pos[0] = 0;
        pos[m - 1] = n;
        System.arraycopy(cuts, 0, pos, 1, cuts.length);
        Arrays.sort(pos, 1, m - 1);

        int[][] dp = new int[m][m];

        for (int len = 2; len < m; len++) {
            for (int i = 0; i + len < m; i++) {
                int j = i + len;
                int best = Integer.MAX_VALUE;
                for (int k = i + 1; k < j; k++) {
                    int cand = dp[i][k] + dp[k][j] + (pos[j] - pos[i]);
                    best = Math.min(best, cand);
                }
                dp[i][j] = (best == Integer.MAX_VALUE ? 0 : best);
            }
        }
        return dp[0][m - 1];
    }

    // ===========================
    // Test Harness
    // ===========================
    public static void main(String[] args) {
        problem9_5 solver = new problem9_5();

        runTest("Test 1: Classic example",
                7, new int[]{1, 3, 4, 5}, 16, solver);

        runTest("Test 2: Two cuts",
                9, new int[]{5, 6}, 13, solver);

        runTest("Test 3: Single cut",
                10, new int[]{4}, 10, solver);

        runTest("Test 4: Even spacing (correct = 24)",
                10, new int[]{2, 4, 6, 8}, 24, solver);

        runTest("Test 5: No cuts",
                100, new int[]{}, 0, solver);
    }

    private static void runTest(String name, int n, int[] cuts,
                                int expected, problem9_5 solver) {

        int result = solver.minCost(n, cuts);

        System.out.println(name);
        System.out.println("n = " + n + ", cuts = " + Arrays.toString(cuts));
        System.out.println("minCost = " + result + " (expected " + expected + ")\n");
    }
}
