package code_samples.section9.problems.problem9_2;

/**
 * problem9_2
 * ----------
 * Implements the classic 0/1 Knapsack problem using a space-optimized
 * dynamic programming approach.
 *
 * This version computes only the maximum achievable value for a given
 * capacity and set of items; it does not reconstruct which items were chosen.
 */
public class problem9_2 {

    /**
     * Solves the 0/1 Knapsack problem.
     *
     * Problem definition:
     *   - You are given a knapsack with capacity W.
     *   - There are items, each with a weight and a value.
     *   - Each item can be chosen at most once (0/1 constraint).
     *   - Goal: maximize total value without exceeding capacity W.
     *
     * Dynamic Programming formulation (1D optimization):
     *   dp[w] = maximum value achievable with capacity w
     *           using items processed so far.
     *
     * Initialization:
     *   dp[w] = 0 for all w, since with no items the best value is 0.
     *
     * Transition:
     *   For each item (wt, val):
     *     for w from W down to wt:
     *       dp[w] = max(dp[w], dp[w - wt] + val)
     *
     * Critical detail:
     *   The loop over w goes backwards (from W down to wt) to ensure that
     *   each item is only used once. Iterating forwards would allow the same
     *   item to be reused multiple times, turning this into an unbounded
     *   knapsack problem.
     *
     * Time Complexity:
     *   O(n * W), where n is the number of items.
     *
     * Space Complexity:
     *   O(W), due to the 1D dp array.
     *
     * @param W       Knapsack capacity
     * @param weight  Array of item weights
     * @param value   Array of item values
     * @return        Maximum total value achievable with capacity W
     */
    int knapSack(int W, int[] weight, int[] value) {
        // dp[w] stores the best value achievable with capacity w
        int[] dp = new int[W + 1];

        // Process each item exactly once
        for (int i = 0; i < weight.length; i++) {
            int wt = weight[i];
            int val = value[i];

            // Traverse capacities backwards to enforce 0/1 constraint
            for (int w = W; w >= wt; w--) {
                // Either skip the item (dp[w]) or take it (dp[w - wt] + val)
                dp[w] = Math.max(dp[w], dp[w - wt] + val);
            }
        }

        // dp[W] contains the optimal value for full capacity
        return dp[W];
    }

    // ===========================
    // Test Harness
    // ===========================

    /**
     * Entry point for running predefined test cases.
     *
     * Each test prints:
     *   - Test name
     *   - Knapsack capacity
     *   - Weights array
     *   - Values array
     *   - Computed result and expected result
     */
    public static void main(String[] args) {
        problem9_2 solver = new problem9_2();

        runTest("Test 1: Classic example",
                50,
                new int[]{10, 20, 30},
                new int[]{60, 100, 120},
                220,
                solver);

        runTest("Test 2: Small weights",
                6,
                new int[]{1, 2, 3},
                new int[]{10, 15, 40},
                65,
                solver);

        runTest("Test 3: Too small capacity",
                4,
                new int[]{5, 6, 7},
                new int[]{10, 20, 30},
                0,
                solver);

        runTest("Test 4: Zero capacity",
                0,
                new int[]{2, 3, 4},
                new int[]{20, 30, 40},
                0,
                solver);

        runTest("Test 5: Larger mixed set",
                10,
                new int[]{2, 3, 5, 7},
                new int[]{20, 30, 50, 70},
                100,
                solver);
    }

    /**
     * Helper method to execute and display a single knapsack test case.
     *
     * @param name      Descriptive test name
     * @param W         Knapsack capacity
     * @param weight    Array of item weights
     * @param value     Array of item values
     * @param expected  Expected optimal knapsack value
     * @param solver    Instance of problem9_2 used to run the algorithm
     */
    private static void runTest(String name, int W, int[] weight, int[] value,
                                int expected, problem9_2 solver) {
        // Run the knapsack algorithm
        int result = solver.knapSack(W, weight, value);

        // Print formatted test output
        System.out.println(name);
        System.out.println("Capacity = " + W);

        System.out.print("Weights: {");
        for (int i = 0; i < weight.length; i++)
            System.out.print(weight[i] + (i + 1 == weight.length ? "" : ","));
        System.out.println("}");

        System.out.print("Values:  {");
        for (int i = 0; i < value.length; i++)
            System.out.print(value[i] + (i + 1 == value.length ? "" : ","));
        System.out.println("}");

        System.out.println("knapSack = " + result + " (expected " + expected + ")\n");
    }
}
