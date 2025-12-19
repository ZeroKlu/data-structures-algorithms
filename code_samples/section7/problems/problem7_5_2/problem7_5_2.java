package code_samples.section7.problems.problem7_5_2;

import java.util.*;

/**
 * problem7_5_2
 *
 * This class demonstrates how to count the number of *unique value pairs*
 * in an integer array whose sum equals a given target.
 *
 * A “unique pair” is defined by its VALUES, not by indices.
 * That means duplicates in the array do not create additional pairs
 * if they represent the same two numbers.
 *
 * Examples:
 *   nums = [1, 1, 2, 2, 3, 3], target = 4
 *   Valid unique pairs are {1,3} and {2,2} → result = 2
 *
 *   nums = [1, 5, 1, 5], target = 6
 *   Valid unique pair is {1,5} → result = 1
 */
public class problem7_5_2 {

    /**
     * countUniquePairs
     *
     * Counts the number of distinct value pairs {a, b} such that:
     *     a + b == target
     *
     * Order does NOT matter:
     *     {a, b} is the same as {b, a}
     *
     * @param nums   Input array of integers (may contain duplicates).
     * @param target Target sum.
     *
     * @return Number of unique value pairs that sum to target.
     *
     * Algorithm (single pass with hash sets):
     *   - Use a HashSet<Integer> `seen` to track numbers encountered so far.
     *   - Use a HashSet<String> `pairs` to track unique pairs.
     *
     *   For each number x in nums:
     *     1) Compute y = target - x.
     *     2) If y has already been seen, then {x, y} is a valid pair.
     *     3) Normalize the pair as (a = min(x,y), b = max(x,y)).
     *     4) Store it as a string "a,b" in `pairs` to avoid duplicates.
     *     5) Add x to `seen`.
     *
     * Why normalization matters:
     *   - Without it, {1,3} and {3,1} would be treated as different pairs.
     *   - By forcing a <= b, each value pair has exactly one representation.
     *
     * Time Complexity (average):
     *   O(n)
     *   Each element is processed once, with O(1) average-time HashSet operations.
     *
     * Space Complexity:
     *   O(n)
     *   In the worst case, both `seen` and `pairs` grow linearly.
     *
     * Note:
     *   Using String keys ("a,b") is simple and readable but not the most efficient.
     *   A faster alternative would be:
     *     HashSet<Pair<Integer,Integer>> or a custom value object.
     */
    int countUniquePairs(int[] nums, int target) {
        // Tracks values that have already been processed
        Set<Integer> seen = new HashSet<>();

        // Tracks unique normalized value pairs ("a,b")
        Set<String> pairs = new HashSet<>();

        // Iterate through the array
        for (int x : nums) {
            // Compute the complementary value needed
            int y = target - x;

            // If we've seen y before, then x + y == target is possible
            if (seen.contains(y)) {
                // Normalize order so (x,y) and (y,x) map to the same key
                int a = Math.min(x, y);
                int b = Math.max(x, y);

                // Encode the pair as a unique string
                String key = a + "," + b;

                // Insert into the set (duplicates are ignored automatically)
                pairs.add(key);
            }

            // Record x as seen for future iterations
            seen.add(x);
        }

        // Number of unique value pairs found
        return pairs.size();
    }

    // ============================
    // Test Harness
    // ============================

    /**
     * main
     *
     * Runs several test cases to verify correctness of countUniquePairs().
     * Prints both actual and expected results.
     */
    public static void main(String[] args) {
        problem7_5_2 solver = new problem7_5_2();

        // Test input arrays
        int[][] testArrays = {
            {1, 1, 2, 2, 3, 3},  // {1,3}, {2,2}
            {2, 7, 11, 15},      // {2,7}
            {3, 3, 3},           // {3,3}
            {1, 2, 3, 4},        // no pairs
            {1, 5, 1, 5}         // {1,5}
        };

        // Corresponding targets
        int[] targets   = {4, 9, 6, 10, 6};

        // Expected results
        int[] expected  = {2, 1, 1, 0, 1};

        // Human-readable descriptions of expected pairs
        String[] notes = {
            "{1,3} and {2,2}",
            "{2,7}",
            "{3,3}",
            "no pairs",
            "{1,5}"
        };

        System.out.println("=== Test: countUniquePairs ===\n");

        // Run each test case
        for (int i = 0; i < testArrays.length; i++) {
            int[] nums = testArrays[i];
            int target = targets[i];

            // Execute function under test
            int result = solver.countUniquePairs(nums, target);

            // Print results
            System.out.println(
                "Input nums = " + Arrays.toString(nums) +
                ", target = " + target
            );
            System.out.println("Result:   " + result);
            System.out.println(
                "Expected: " + expected[i] +
                " (" + notes[i] + ")\n"
            );
        }
    }
}
