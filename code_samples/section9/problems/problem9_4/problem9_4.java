package code_samples.section9.problems.problem9_4;

import java.util.Arrays;

/**
 * problem9_4
 * ----------
 * Solves the Traveling Salesman Problem (TSP) using bitmask dynamic programming
 * (Held–Karp algorithm).
 *
 * Problem variant solved:
 *   - Start at city 0
 *   - Visit every city exactly once
 *   - Return to city 0
 *   - Minimize total tour cost
 *
 * This implementation returns the minimum cost of a complete tour (cycle).
 * It does not reconstruct the actual path; it computes only the optimal cost.
 */
public class problem9_4 {

    /**
     * tsp
     * ---
     * Computes the minimum tour cost for the Traveling Salesman Problem (TSP)
     * using a bitmask DP approach.
     *
     * Input:
     *   cost[i][j] is the travel cost from city i to city j.
     *
     * DP definition:
     *   dp[mask][i] = minimum cost to start at city 0, visit exactly the cities
     *                whose bits are set in `mask`, and end at city i.
     *
     * Bitmask representation:
     *   - mask uses n bits (0..n-1), where bit k = 1 means city k is visited.
     *   - maxMask = 1 << n is the number of subsets of cities.
     *
     * Initialization:
     *   dp[1][0] = 0
     *   (mask=1 means only city 0 visited; we are at city 0 with cost 0)
     *
     * Transition:
     *   For each state (mask, i):
     *     try to go to an unvisited city j:
     *       nextMask = mask | (1 << j)
     *       dp[nextMask][j] = min(dp[nextMask][j], dp[mask][i] + cost[i][j])
     *
     * Finalization:
     *   Once all cities are visited (fullMask), close the tour by returning to 0:
     *     ans = min_i (dp[fullMask][i] + cost[i][0])
     *
     * Complexity:
     *   Time:  O(2^n * n^2)
     *   Space: O(2^n * n)
     *
     * Notes on INF:
     *   Using Integer.MAX_VALUE / 2 reduces overflow risk during:
     *     dp + cost (so it stays well below MAX_VALUE).
     *
     * @param cost Square cost matrix (n x n)
     * @return Minimum tour cost starting/ending at city 0
     */
    int tsp(int[][] cost) {
        // Number of cities
        int n = cost.length;

        // Total number of subsets of cities
        int maxMask = 1 << n;

        // Sentinel for "unreachable" DP states
        int INF = Integer.MAX_VALUE / 2;

        // dp[mask][i] = best cost to start at 0, visit 'mask', and end at i
        int[][] dp = new int[maxMask][n];

        // Initialize all dp states to INF
        for (int[] row : dp) {
            Arrays.fill(row, INF);
        }

        // Base case: only city 0 visited, cost is 0, ending at city 0
        dp[1][0] = 0; // only city 0 visited

        // Iterate over all visited-city subsets
        for (int mask = 1; mask < maxMask; mask++) {
            // Try each city i as the current endpoint of this visited set
            for (int i = 0; i < n; i++) {
                // If city i isn't in mask, we can't end at i for this subset
                if ((mask & (1 << i)) == 0) continue;

                int curCost = dp[mask][i];

                // Skip unreachable states
                if (curCost == INF) continue;

                // Attempt to extend path to an unvisited city j
                for (int j = 0; j < n; j++) {
                    // If city j already visited, skip
                    if ((mask & (1 << j)) != 0) continue;

                    int nextMask = mask | (1 << j);
                    int cand = curCost + cost[i][j];

                    // Relaxation: keep the best cost to reach (nextMask, j)
                    if (cand < dp[nextMask][j]) {
                        dp[nextMask][j] = cand;
                    }
                }
            }
        }

        // Mask where all cities are visited
        int fullMask = maxMask - 1;

        // Close the tour: return to city 0 from whichever city i we end at
        int ans = INF;
        for (int i = 0; i < n; i++) {
            // dp[fullMask][i] is best cost to visit all cities and end at i
            // Add cost to return to start and complete the cycle
            int cand = dp[fullMask][i] + cost[i][0];
            if (cand < ans) ans = cand;
        }

        return ans;
    }

    // ===========================
    // Test Harness
    // ===========================

    /**
     * main
     * ----
     * Runs a set of predefined tests for the bitmask DP TSP solution.
     *
     * Each test prints:
     *   - Test name
     *   - Cost matrix
     *   - Computed tsp() cost
     *   - Expected tsp() cost
     */
    public static void main(String[] args) {
        problem9_4 solver = new problem9_4();

        // -------------------------------
        // Test 1: Classic 4-city example
        // -------------------------------
        // Best tour cost including return to 0 is 80:
        // 0 -> 1 -> 3 -> 2 -> 0
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
        // Both tours cost 7:
        // 0 -> 1 -> 2 -> 0 or 0 -> 2 -> 1 -> 0
        int[][] cost2 = {
            {0, 1, 4},
            {1, 0, 2},
            {4, 2, 0}
        };
        runTest("Test 2: 3-city triangle", cost2, 7, solver);

        // -------------------------------
        // Test 3: 4-city square
        // -------------------------------
        // One optimal tour: 0 -> 1 -> 2 -> 3 -> 0
        // Total cost = 4
        int[][] cost3 = {
            {0, 1, 2, 1},
            {1, 0, 1, 2},
            {2, 1, 0, 1},
            {1, 2, 1, 0}
        };
        runTest("Test 3: 4-city square", cost3, 4, solver);
    }

    /**
     * runTest
     * -------
     * Helper method to execute a test case and print the results.
     *
     * @param name      Descriptive test name
     * @param cost      Cost matrix to pass into tsp()
     * @param expected  Expected optimal tour cost
     * @param solver    Instance of problem9_4 used to run tsp()
     */
    private static void runTest(String name, int[][] cost, int expected, problem9_4 solver) {
        // Run the TSP solver
        int result = solver.tsp(cost);

        // Print test details
        System.out.println(name);
        System.out.println("Cost matrix:");
        for (int[] row : cost) {
            System.out.println(Arrays.toString(row));
        }

        // Print result vs expected
        System.out.println("tsp() = " + result + " (expected " + expected + ")\n");
    }
}
