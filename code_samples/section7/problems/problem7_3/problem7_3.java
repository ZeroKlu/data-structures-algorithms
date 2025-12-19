package code_samples.section7.problems.problem7_3;

import java.util.*;

/**
 * problem7_3
 *
 * Solves the classic "Longest Consecutive Sequence" problem.
 *
 * Given an unsorted array of integers, find the length of the longest sequence
 * of consecutive integers (numbers that differ by exactly 1).
 *
 * Example:
 *   Input:  [100, 4, 200, 1, 3, 2]
 *   Output: 4   (sequence: 1, 2, 3, 4)
 *
 * Key idea:
 *   Use a hash-based set to achieve O(1) average-time lookups, allowing an
 *   overall O(n) expected-time solution.
 */
public class problem7_3 {

    /**
     * longestConsecutive
     *
     * @param nums Input array of integers (may be empty and may contain duplicates).
     * @return Length of the longest run of consecutive integers.
     *
     * Approach:
     *   1) Insert all values into a HashSet to:
     *        - Remove duplicates automatically
     *        - Enable O(1) average membership checks
     *
     *   2) Iterate over each unique value x in the set.
     *        - Only attempt to build a sequence starting at x if (x - 1)
     *          is NOT present in the set.
     *        - This guarantees x is the *start* (smallest value) of a sequence.
     *
     *   3) From each valid start, count upward (x, x+1, x+2, ...)
     *      until the sequence breaks.
     *
     * Why the “start of sequence” check matters:
     *   Without checking !set.contains(x - 1), each sequence would be counted
     *   repeatedly (once from every element in the sequence), leading to
     *   O(n²) behavior in dense cases.
     *
     * Complexity (expected / average):
     *   - Time:  O(n)
     *       Each element is inserted once, and each sequence is traversed once.
     *   - Space: O(n)
     *       HashSet stores all unique elements.
     *
     * Notes:
     *   - HashSet iteration order is arbitrary.
     *   - Works correctly with negative numbers.
     *   - Duplicates in the input do not affect correctness.
     */
    int longestConsecutive(int[] nums) {
        // Store all numbers in a HashSet to remove duplicates and allow fast lookups
        Set<Integer> set = new HashSet<>();
        for (int x : nums) set.add(x);

        // Tracks the maximum length found so far
        int best = 0;

        // Iterate over each unique number in the set
        for (int x : set) {
            // Only start counting if x is the beginning of a sequence
            // (i.e., there is no x-1 in the set)
            if (!set.contains(x - 1)) { // start of sequence
                int cur = x;
                int length = 1;

                // Count consecutive numbers upward
                while (set.contains(cur + 1)) {
                    cur++;
                    length++;
                }

                // Update the best (maximum) sequence length
                best = Math.max(best, length);
            }
        }

        return best;
    }

    // ============================
    // Test harness
    // ============================

    /**
     * main
     *
     * Simple console-based test harness (not a unit test framework).
     * Exercises longestConsecutive() with:
     *   - typical cases
     *   - duplicates
     *   - empty input
     *   - negative values
     */
    public static void main(String[] args) {
        problem7_3 sol = new problem7_3();

        // Test inputs
        int[][] tests = {
            {100, 4, 200, 1, 3, 2},          // longest: 1,2,3,4
            {0, 3, 7, 2, 5, 8, 4, 6, 0, 1},  // longest: 0..8
            {1, 2, 0, 1},                    // longest: 0,1,2
            {},                              // empty input
            {-2, -1, 0, 2, 3, 4, 5}          // longest: 2,3,4,5
        };

        // Expected outputs aligned with the test cases
        int[] expected = {
            4,   // 1,2,3,4
            9,   // 0..8
            3,   // 0,1,2
            0,   // empty
            4    // 2,3,4,5
        };

        System.out.println("=== Test: longestConsecutive ===\n");

        // Run all test cases
        for (int t = 0; t < tests.length; t++) {
            int[] nums = tests[t];

            System.out.print("Input: " + Arrays.toString(nums) + "\n");
            int result = sol.longestConsecutive(nums);
            System.out.println(
                "Longest consecutive length: " + result +
                " (expected " + expected[t] + ")\n"
            );
        }
    }
}
