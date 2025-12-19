package code_samples.section7.problems.problem7_4;

import java.util.*;

/**
 * problem7_4
 *
 * Solves the "Subarray Sum Equals K" problem.
 *
 * Given an integer array `nums` and an integer `k`, this program counts
 * the number of contiguous subarrays whose elements sum exactly to `k`.
 *
 * This implementation uses a hash map and prefix sums to achieve
 * linear-time performance.
 */
public class problem7_4 {

    /**
     * subarraySum
     *
     * @param nums Input array of integers (may be empty, may contain negatives).
     * @param k    Target sum.
     *
     * @return Number of contiguous subarrays whose sum equals k.
     *
     * Core idea (prefix sums + frequency map):
     *
     *   Let prefix[i] be the sum of nums[0..i].
     *   The sum of a subarray (j+1 .. i) is:
     *
     *       prefix[i] - prefix[j]
     *
     *   We want:
     *
     *       prefix[i] - prefix[j] == k
     *       => prefix[j] == prefix[i] - k
     *
     *   While scanning the array:
     *     - Maintain a map that counts how many times each prefix sum
     *       has appeared so far.
     *     - For each new prefix sum P, add the count of (P - k)
     *       to the result.
     *
     * Why we initialize freq with (0 -> 1):
     *   - Represents an "empty prefix" before reading any elements.
     *   - Allows subarrays starting at index 0 (where prefix == k)
     *     to be counted correctly.
     *
     * Complexity (expected / average):
     *   - Time:  O(n), one pass through the array.
     *   - Space: O(n), in the worst case all prefix sums are distinct.
     *
     * Notes:
     *   - Works correctly with negative numbers (unlike sliding window methods).
     *   - For very large inputs, consider using long for prefix sums.
     */
    int subarraySum(int[] nums, int k) {
        // Map: prefixSum -> frequency count
        Map<Integer, Integer> freq = new HashMap<>();

        // Base case: prefix sum 0 appears once before processing any elements
        freq.put(0, 1);

        int prefix = 0; // running prefix sum
        int count = 0;  // number of valid subarrays found

        // Iterate through the array once
        for (int x : nums) {
            // Update the running prefix sum
            prefix += x;

            // We need previous prefix sums equal to (prefix - k)
            int need = prefix - k;

            // Add how many times that needed prefix sum has appeared
            count += freq.getOrDefault(need, 0);

            // Record that we've now seen this prefix sum
            freq.put(prefix, freq.getOrDefault(prefix, 0) + 1);
        }

        return count;
    }

    // ============================
    // Test harness
    // ============================

    /**
     * main
     *
     * Simple console-based test harness that exercises subarraySum()
     * with several representative test cases.
     */
    public static void main(String[] args) {
        problem7_4 sol = new problem7_4();

        // Test inputs
        int[][] tests = {
            {1, 1, 1},                    // expected 2
            {1, 2, 3},                    // expected 2: [1,2], [3]
            {3, 4, 7, 2, -3, 1, 4, 2},    // expected 4
            {},                           // expected 0 (empty input)
        };

        // Target k values aligned with tests[]
        int[] kVals = {2, 3, 7, 0};

        // Expected outputs aligned with tests[]
        int[] expected = {2, 2, 4, 0};

        System.out.println("=== Test: subarraySum ===\n");

        // Run all test cases
        for (int t = 0; t < tests.length; t++) {
            int[] nums = tests[t];
            int k = kVals[t];

            System.out.println("Input: " + Arrays.toString(nums) + ", k = " + k);

            int result = sol.subarraySum(nums, k);

            System.out.println(
                "Result: " + result +
                " (expected " + expected[t] + ")\n"
            );
        }
    }
}
