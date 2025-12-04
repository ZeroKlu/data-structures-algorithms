package code_samples.section9.problems.problem9_4;

import java.util.Arrays;

public class problem9_4 {
    int tsp(int[][] cost) {
        int n = cost.length;
        int maxMask = 1 << n;
        int INF = Integer.MAX_VALUE / 2;
        int[][] dp = new int[maxMask][n];
        for (int[] row : dp) {
            Arrays.fill(row, INF);
        }
        dp[1][0] = 0; // only city 0 visited

        for (int mask = 1; mask < maxMask; mask++) {
            for (int i = 0; i < n; i++) {
                if ((mask & (1 << i)) == 0) continue;
                int curCost = dp[mask][i];
                if (curCost == INF) continue;
                for (int j = 0; j < n; j++) {
                    if ((mask & (1 << j)) != 0) continue;
                    int nextMask = mask | (1 << j);
                    int cand = curCost + cost[i][j];
                    if (cand < dp[nextMask][j]) {
                        dp[nextMask][j] = cand;
                    }
                }
            }
        }

        int fullMask = maxMask - 1;
        int ans = INF;
        for (int i = 0; i < n; i++) {
            int cand = dp[fullMask][i] + cost[i][0];
            if (cand < ans) ans = cand;
        }
        return ans;
    }

    // ===========================
    // Test Harness
    // ===========================
    
    public static void main(String[] args) {
        problem9_4 solver = new problem9_4();

        // -------------------------------
        // Test 1: Classic 4-city example
        // -------------------------------
        int[][] cost1 = {
            {0, 10, 15, 20},
            {10, 0, 35, 25},
            {15, 35, 0, 30},
            {20, 25, 30, 0}
        };
        runTest("Test 1: Classic 4-city example", cost1, 80, solver);

        // -------------------------------
        // Test 2: 3-city triangle
        // -------------------------------
        int[][] cost2 = {
            {0, 1, 4},
            {1, 0, 2},
            {4, 2, 0}
        };
        runTest("Test 2: 3-city triangle", cost2, 7, solver);

        // -------------------------------
        // Test 3: 4-city square
        // -------------------------------
        int[][] cost3 = {
            {0, 1, 2, 1},
            {1, 0, 1, 2},
            {2, 1, 0, 1},
            {1, 2, 1, 0}
        };
        runTest("Test 3: 4-city square", cost3, 4, solver);
    }


    private static void runTest(String name, int[][] cost, int expected, problem9_4 solver) {
        int result = solver.tsp(cost);

        System.out.println(name);
        System.out.println("Cost matrix:");
        for (int[] row : cost) {
            System.out.println(Arrays.toString(row));
        }

        System.out.println("tsp() = " + result + " (expected " + expected + ")\n");
    }
}
