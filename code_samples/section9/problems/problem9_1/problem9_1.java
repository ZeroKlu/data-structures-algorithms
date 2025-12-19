package code_samples.section9.problems.problem9_1;

/**
 * problem9_1
 * ----------
 * Demonstrates an efficient O(n log n) solution for computing the length of the
 * Longest Increasing Subsequence (LIS) using the classic "tails" technique.
 *
 * This implementation focuses only on computing the length of the LIS,
 * not reconstructing the subsequence itself.
 */
public class problem9_1 {

    /**
     * Computes the length of the Longest Increasing Subsequence (LIS)
     * in the given integer array.
     *
     * Core idea (tails method):
     *   - tails[i] stores the smallest possible tail value of any strictly
     *     increasing subsequence of length (i + 1) seen so far.
     *   - The array tails[0..len-1] is always kept sorted.
     *
     * For each element x in nums:
     *   1) Binary search tails[0..len) to find the first index lo such that
     *      tails[lo] >= x (lower_bound).
     *   2) Replace tails[lo] with x.
     *   3) If lo == len, we have extended the LIS length by one.
     *
     * Important properties:
     *   - Replacing a value in tails does NOT reduce the LIS length.
     *   - Smaller tail values are always better because they allow more room
     *     for future extensions.
     *
     * Time Complexity:
     *   O(n log n), where n = nums.length
     *
     * Space Complexity:
     *   O(n), for the tails array
     *
     * @param nums input array of integers
     * @return length of the longest strictly increasing subsequence
     */
    int lengthOfLIS(int[] nums) {
        // tails[i] = minimum possible tail of an increasing subsequence
        // of length (i + 1)
        int[] tails = new int[nums.length];

        // Current length of the LIS found so far
        int len = 0;

        // Process each number in the input array
        for (int x : nums) {
            // Binary search range is [0, len)
            int lo = 0, hi = len;

            // Find the first index where tails[index] >= x
            while (lo < hi) {
                int mid = (lo + hi) / 2;

                // If current tail is smaller than x, x must go to the right
                if (tails[mid] < x) {
                    lo = mid + 1;
                } else {
                    // Otherwise, mid could be the replacement position
                    hi = mid;
                }
            }

            // Place x at its correct position in tails
            tails[lo] = x;

            // If x extends beyond the current LIS, increase length
            if (lo == len) {
                len++;
            }
        }

        // len equals the length of the LIS
        return len;
    }

    // ===========================
    // Test Harness (main)
    // ===========================

    /**
     * Runs a set of predefined tests to validate the lengthOfLIS method.
     *
     * Each test prints:
     *   - A descriptive test name
     *   - The input array
     *   - The computed LIS length
     *   - The expected LIS length
     */
    public static void main(String[] args) {
        problem9_1 solver = new problem9_1();

        System.out.println("=== Test 1: Basic Increasing Sequence ===");
        int[] arr1 = {1, 2, 3, 4, 5};
        System.out.println("Input: [1,2,3,4,5]");
        // Entire array is strictly increasing => LIS length = 5
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr1) + " (expected 5)\n");

        System.out.println("=== Test 2: Mixed Sequence ===");
        int[] arr2 = {10, 9, 2, 5, 3, 7, 101, 18};
        System.out.println("Input: [10,9,2,5,3,7,101,18]");
        // One LIS is [2,3,7,101] => length = 4
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr2) + " (expected 4)\n");

        System.out.println("=== Test 3: All Identical ===");
        int[] arr3 = {7, 7, 7, 7, 7};
        System.out.println("Input: [7,7,7,7,7]");
        // Strictly increasing means duplicates cannot extend the sequence
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr3) + " (expected 1)\n");

        System.out.println("=== Test 4: Empty Array ===");
        int[] arr4 = {};
        System.out.println("Input: []");
        // No elements => LIS length = 0
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr4) + " (expected 0)\n");

        System.out.println("=== Test 5: Decreasing Sequence ===");
        int[] arr5 = {9, 7, 5, 3, 1};
        System.out.println("Input: [9,7,5,3,1]");
        // Best increasing subsequence is any single element
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr5) + " (expected 1)\n");

        System.out.println("=== Test 6: Random Pattern ===");
        int[] arr6 = {4, 10, 4, 3, 8, 9};
        System.out.println("Input: [4,10,4,3,8,9]");
        // One LIS is [4,8,9] => length = 3
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr6) + " (expected 3)\n");
    }
}
