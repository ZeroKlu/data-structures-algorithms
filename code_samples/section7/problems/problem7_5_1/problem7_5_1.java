package code_samples.section7.problems.problem7_5_1;

import java.util.*;

/**
 * problem7_5_1
 *
 * Demonstrates a hash-map-based solution to the classic
 * "Two Sum (return indices)" problem.
 *
 * Given an array of integers and a target value, the goal is to find
 * two distinct indices i and j such that:
 *
 *     nums[i] + nums[j] == target
 *
 * If no such pair exists, an empty array is returned.
 */
public class problem7_5_1 {

    /**
     * twoSumIndices
     *
     * Finds indices of two elements in the array whose values sum to `target`.
     *
     * @param nums   Input array of integers.
     * @param target Target sum.
     *
     * @return An array of two indices [i, j] such that
     *         nums[i] + nums[j] == target,
     *         or an empty array if no solution exists.
     *
     * Approach (single-pass hash map):
     *   - Use a HashMap to store values we have already seen along with
     *     their indices.
     *   - As we iterate through the array:
     *       1) Compute the value needed to reach the target.
     *       2) If that value is already in the map, we have found a valid pair.
     *       3) Otherwise, store the current value and its index in the map.
     *
     * Why this works:
     *   - At index i, the map contains only elements from indices < i,
     *     guaranteeing the two indices are distinct.
     *
     * Complexity (average / expected):
     *   - Time:  O(n)
     *       Each element is processed once, with O(1) average-time map operations.
     *   - Space: O(n)
     *       The map may store up to n elements.
     *
     * Notes:
     *   - If the same value appears multiple times, the map keeps the
     *     most recently stored index for that value.
     *   - This implementation returns the first valid pair it encounters.
     */
    int[] twoSumIndices(int[] nums, int target) {
        // Hash map: value -> index where that value appears
        Map<Integer, Integer> mp = new HashMap<>();

        // Iterate through the array once
        for (int i = 0; i < nums.length; i++) {
            // Compute the complement needed to reach the target
            int need = target - nums[i];

            // If we've already seen the needed value, we have a solution
            if (mp.containsKey(need)) {
                // Return indices of the matching pair
                return new int[]{mp.get(need), i};
            }

            // Otherwise, record the current value and its index
            mp.put(nums[i], i);
        }

        // No valid pair found
        return new int[0];
    }

    // ===============================
    // Test Harness
    // ===============================

    /**
     * main
     *
     * Simple console-based test harness for twoSumIndices().
     * Runs several test cases and prints both actual and expected results.
     */
    public static void main(String[] args) {
        problem7_5_1 sol = new problem7_5_1();

        // Test input arrays
        int[][] tests = {
            {2, 7, 11, 15},   // expected [0,1]
            {3, 2, 4},        // expected [1,2]
            {1, 2, 3, 4}      // expected [] (no solution)
        };

        // Corresponding target values
        int[] targets = {9, 6, 10};

        // Expected outputs for each test
        int[][] expected = {
            {0, 1},
            {1, 2},
            {}                 // no result
        };

        System.out.println("=== Test: twoSumIndices ===\n");

        // Run each test case
        for (int t = 0; t < tests.length; t++) {
            int[] nums = tests[t];
            int target = targets[t];

            // Call the function under test
            int[] result = sol.twoSumIndices(nums, target);

            // Print results
            System.out.println(
                "Input nums = " + Arrays.toString(nums) +
                ", target = " + target
            );
            System.out.println(
                "Result: " + Arrays.toString(result) +
                " (expected " + Arrays.toString(expected[t]) + ")\n"
            );
        }
    }
}
