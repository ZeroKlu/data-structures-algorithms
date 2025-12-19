package code_samples.section8.problems.problem8_2;

/*
 * Problem 8.2 — House Robber
 *
 * You are given an array of non-negative integers where each element
 * represents the amount of money in a house along a street.
 *
 * Constraint:
 *   You cannot rob two adjacent houses (alarm system).
 *
 * Goal:
 *   Return the maximum amount of money you can rob without triggering the alarm.
 *
 * This is a classic Dynamic Programming (DP) problem.
 */
public class problem8_2 {

    /*
     * rob(nums)
     *
     * Dynamic Programming with space optimization.
     *
     * Core idea:
     *   At each house i, you have two choices:
     *     1) Skip this house → keep the best result so far.
     *     2) Rob this house → add its value to the best result from two houses back.
     *
     * We avoid building a full DP array by keeping only two variables:
     *   prev2 = dp[i - 2]
     *   prev1 = dp[i - 1]
     *
     * Time Complexity:
     *   O(n) — one pass through the array
     *
     * Space Complexity:
     *   O(1) — constant extra space
     */
    int rob(int[] nums) {

        /*
         * Edge case 1:
         *   No houses → no money to rob.
         */
        if (nums.length == 0) return 0;

        /*
         * Edge case 2:
         *   Only one house → rob it.
         */
        if (nums.length == 1) return nums[0];

        /*
         * Initialize DP base cases:
         *
         * prev2 represents dp[0]:
         *   Maximum money robbable from just the first house.
         */
        int prev2 = nums[0];

        /*
         * prev1 represents dp[1]:
         *   Maximum money robbable from the first two houses.
         *   You must choose the larger of the two.
         */
        int prev1 = Math.max(nums[0], nums[1]);

        /*
         * Iterate from the 3rd house onward (index 2).
         * At each step, decide whether to:
         *   - Take the current house
         *   - Skip the current house
         */
        for (int i = 2; i < nums.length; i++) {

            /*
             * Option 1: Take (rob) the current house.
             * If we rob house i, we must skip house i-1,
             * so we add nums[i] to prev2 (dp[i-2]).
             */
            int take = prev2 + nums[i];

            /*
             * Option 2: Skip the current house.
             * If we skip it, the best result remains prev1 (dp[i-1]).
             */
            int skip = prev1;

            /*
             * Choose the better of the two options.
             * This value becomes dp[i].
             */
            int cur = Math.max(take, skip);

            /*
             * Shift the DP window forward:
             *   prev2 ← dp[i-1]
             *   prev1 ← dp[i]
             */
            prev2 = prev1;
            prev1 = cur;
        }

        /*
         * prev1 now holds the maximum money robbable
         * considering all houses.
         */
        return prev1;
    }

    // ============================
    // Tests
    // ============================
    public static void main(String[] args) {

        problem8_2 solver = new problem8_2();

        /*
         * Test cases:
         * Each array represents house values along the street.
         */
        int[][] tests = {
            {},                  // empty → 0
            {5},                 // single house → 5
            {1, 2},              // choose max → 2
            {2, 7, 9, 3, 1},     // classic example → 12
            {1, 2, 3, 1},        // → 4
            {2, 1, 1, 2},        // → 4
            {10, 5, 20, 15, 30}, // → 60
            {1, 3, 1, 3, 100}    // → 103
        };

        /*
         * Expected outputs corresponding to each test case.
         */
        int[] expected = {
            0,
            5,
            2,
            12,
            4,
            4,
            60,
            103
        };

        System.out.println("=== Test: rob (House Robber) ===\n");

        /*
         * Run each test case and print results.
         */
        for (int i = 0; i < tests.length; i++) {
            int result = solver.rob(tests[i]);

            System.out.print("Test " + (i + 1) + ": rob([");

            // Print input array
            for (int j = 0; j < tests[i].length; j++) {
                System.out.print(tests[i][j]);
                if (j + 1 < tests[i].length) System.out.print(", ");
            }

            System.out.println("]) = " + result +
                               " (expected " + expected[i] + ")");
        }
    }
}
