package code_samples.section9.problems.problem9_3;

/**
 * problem9_3
 * ----------
 * Solves the "Partition Equal Subset Sum" problem using dynamic programming.
 *
 * Given an array of positive integers, this class determines whether the array
 * can be partitioned into two subsets such that the sum of elements in both
 * subsets is equal.
 */
public class problem9_3 {

    /**
     * Determines whether the given array can be partitioned into two subsets
     * with equal sums.
     *
     * Key observation:
     *   - Let S be the sum of all elements.
     *   - If S is odd, it is impossible to split it into two equal integer sums.
     *   - If S is even, the problem reduces to checking whether there exists
     *     a subset whose sum is exactly S / 2.
     *
     * Dynamic Programming formulation (1D boolean DP):
     *   dp[s] = true if a subset of the processed elements can form sum s.
     *
     * Initialization:
     *   dp[0] = true
     *   (A sum of 0 is always achievable by choosing no elements.)
     *
     * Transition:
     *   For each element x:
     *     for s from target down to x:
     *       if dp[s - x] is true, then dp[s] can be set to true.
     *
     * Important detail:
     *   - The inner loop iterates backward to ensure each element is used
     *     at most once (0/1 subset-sum).
     *   - Iterating forward would allow reusing the same element multiple times.
     *
     * Time Complexity:
     *   O(n * target), where target = totalSum / 2
     *
     * Space Complexity:
     *   O(target)
     *
     * @param nums Input array of positive integers
     * @return true if the array can be partitioned into two equal-sum subsets,
     *         false otherwise
     */
    boolean canPartition(int[] nums) {
        // Compute the total sum of all elements
        int total = 0;
        for (int x : nums) total += x;

        // If total sum is odd, equal partition is impossible
        if ((total & 1) == 1) return false;

        // Target sum for one subset
        int target = total / 2;

        // dp[s] indicates whether sum s is achievable
        boolean[] dp = new boolean[target + 1];

        // Base case: sum 0 is always achievable
        dp[0] = true;

        // Process each number in the array
        for (int x : nums) {
            // Traverse sums backwards to enforce 0/1 usage of each element
            for (int s = target; s >= x; s--) {
                // If we could previously make (s - x), then we can make s by adding x
                if (dp[s - x]) dp[s] = true;
            }
        }

        // If target sum is achievable, a valid partition exists
        return dp[target];
    }

    // ===========================
    // Test Harness
    // ===========================

    /**
     * Entry point for running predefined test cases.
     *
     * Each test prints:
     *   - Test name
     *   - Input array
     *   - Computed result
     *   - Expected result
     */
    public static void main(String[] args) {
        problem9_3 solver = new problem9_3();

        runTest("Test 1: Classic example", new int[]{1, 5, 11, 5}, true, solver);
        runTest("Test 2: Cannot partition", new int[]{1, 2, 3, 5}, false, solver);
        runTest("Test 3: Single element", new int[]{7}, false, solver);
        runTest("Test 4: Two equal numbers", new int[]{4, 4}, true, solver);
        runTest("Test 5: Larger mixed set", new int[]{2, 2, 3, 5}, false, solver);
        runTest("Test 6: Multiple valid subsets", new int[]{2, 2, 1, 1}, true, solver);
    }

    /**
     * Helper method to run and display a single test case.
     *
     * @param name      Descriptive test name
     * @param arr       Input array
     * @param expected  Expected boolean result
     * @param solver    Instance of problem9_3 used to execute the algorithm
     */
    private static void runTest(String name, int[] arr, boolean expected, problem9_3 solver) {
        // Run the partition check
        boolean result = solver.canPartition(arr);

        // Print test details
        System.out.println(name);
        System.out.print("Input: {");
        for (int i = 0; i < arr.length; i++) {
            System.out.print(arr[i]);
            if (i + 1 < arr.length) System.out.print(",");
        }
        System.out.println("}");

        // Print result versus expected outcome
        System.out.println("canPartition = " + result + " (expected " + expected + ")\n");
    }
}
