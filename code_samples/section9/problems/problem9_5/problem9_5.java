package code_samples.section9.problems.problem9_5;

import java.util.Arrays;

/**
 * problem9_5
 * ----------
 * Solves the "Minimum Cost to Cut a Stick" problem using interval DP.
 *
 * Problem summary:
 *   - You have a stick spanning positions [0, n].
 *   - You must perform cuts at the positions in the `cuts` array.
 *   - Each time you cut a segment, the cost of that cut equals the length of
 *     the segment being cut at that moment.
 *   - After a cut, the segment splits into two smaller segments, and future
 *     cuts occur on those segments.
 *   - The order of cuts affects total cost.
 *   - Goal: minimize the total cost of performing all cuts.
 *
 * This is the classic "Cutting Sticks" / LeetCode 1547-style DP.
 */
public class problem9_5 {

    /**
     * minCost
     * -------
     * Computes the minimum possible total cost to perform all required cuts.
     *
     * Key idea (interval DP):
     *   Add the endpoints (0 and n) to the list of cut positions and sort them.
     *   Then solve subproblems over intervals between cut positions.
     *
     * Preprocessing:
     *   pos = [0, sorted(cuts...), n]
     *   m = pos.length = cuts.length + 2
     *
     * DP definition:
     *   dp[i][j] = minimum cost to fully cut the segment from pos[i] to pos[j],
     *             assuming all cuts strictly between i and j must be performed.
     *
     * Base case:
     *   If j == i + 1, there are no cuts inside the interval, so dp[i][j] = 0.
     *
     * Transition:
     *   If there is at least one cut between i and j, choose which cut k to do first:
     *     dp[i][j] = min over k in (i+1..j-1) of:
     *                 dp[i][k] + dp[k][j] + (pos[j] - pos[i])
     *
     * Explanation:
     *   - Performing the first cut within [pos[i], pos[j]] costs (pos[j] - pos[i]),
     *     the length of the segment at that time.
     *   - After cutting at pos[k], the remaining cuts split into two independent
     *     subproblems: [i, k] and [k, j].
     *
     * Complexity:
     *   Let m = cuts.length + 2
     *   Time:  O(m^3)  (len * i * k loops)
     *   Space: O(m^2)  (dp table)
     *
     * @param n    Length of the stick
     * @param cuts Cut positions
     * @return     Minimum total cost to perform all cuts
     */
    int minCost(int n, int[] cuts) {
        // Total number of cut positions after adding endpoints 0 and n
        int m = cuts.length + 2;

        // pos will contain [0, cuts..., n], with cuts sorted between endpoints
        int[] pos = new int[m];
        pos[0] = 0;       // left endpoint
        pos[m - 1] = n;   // right endpoint

        // Copy cuts into pos starting at index 1
        System.arraycopy(cuts, 0, pos, 1, cuts.length);

        // Sort only the interior positions (the cut positions), leaving endpoints in place
        // Arrays.sort(array, fromIndex, toIndex) sorts [fromIndex, toIndex) half-open.
        Arrays.sort(pos, 1, m - 1);

        // dp[i][j] = min cost to cut segment [pos[i], pos[j]]
        // Default 0 values naturally represent base cases with no interior cuts.
        int[][] dp = new int[m][m];

        // len is the distance between indices (j = i + len).
        // Start from 2 because:
        //   len = 1 => adjacent endpoints => no cuts inside => dp is 0
        for (int len = 2; len < m; len++) {
            for (int i = 0; i + len < m; i++) {
                int j = i + len;

                // best will track the minimum cost for interval [i, j]
                int best = Integer.MAX_VALUE;

                // Try each possible first cut k inside the interval (i, j)
                for (int k = i + 1; k < j; k++) {
                    // Cost = left interval + right interval + cost of cutting this segment once
                    int cand = dp[i][k] + dp[k][j] + (pos[j] - pos[i]);
                    best = Math.min(best, cand);
                }

                // If there were no k (happens only when j == i+1), the interval needs no cuts
                dp[i][j] = (best == Integer.MAX_VALUE ? 0 : best);
            }
        }

        // Final answer is the cost to cut the whole stick from 0 to n
        return dp[0][m - 1];
    }

    // ===========================
    // Test Harness
    // ===========================

    /**
     * main
     * ----
     * Runs predefined test cases for minCost().
     *
     * Each test prints:
     *   - test name
     *   - stick length n
     *   - cut positions
     *   - computed minCost result and expected result
     */
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

    /**
     * runTest
     * -------
     * Helper method to execute one test case and print results.
     *
     * @param name     Descriptive test name
     * @param n        Stick length
     * @param cuts     Cut positions array
     * @param expected Expected minimum cost
     * @param solver   Instance used to call minCost()
     */
    private static void runTest(String name, int n, int[] cuts,
                                int expected, problem9_5 solver) {

        // Run the algorithm
        int result = solver.minCost(n, cuts);

        // Print test details
        System.out.println(name);
        System.out.println("n = " + n + ", cuts = " + Arrays.toString(cuts));
        System.out.println("minCost = " + result + " (expected " + expected + ")\n");
    }
}
