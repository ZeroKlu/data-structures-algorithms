/*
 * Problem: House Robber
 *
 * You are given an array of non-negative integers where nums[i] represents
 * how much money is in the i-th house. You cannot rob two adjacent houses,
 * because that would trigger the alarm.
 *
 * Goal:
 *   Return the maximum amount of money you can rob.
 *
 * Dynamic Programming (DP) Insight:
 *   Let dp[i] be the maximum money you can rob from houses 0..i.
 *
 * Recurrence:
 *   dp[i] = max(
 *              dp[i - 1],            // skip house i
 *              dp[i - 2] + nums[i]   // take house i (must skip i-1)
 *           )
 *
 * Space Optimization:
 *   We only ever need dp[i-1] and dp[i-2], so we keep two rolling values:
 *     prev1 = dp[i-1]
 *     prev2 = dp[i-2]
 *
 * Complexity:
 *   Time:  O(n)  (single pass)
 *   Space: O(1)  (constant extra space)
 */
static int Rob(int[] nums) {

    /*
     * Edge case: empty array (no houses) → no money.
     */
    if (nums.Length == 0) return 0;

    /*
     * Edge case: one house → best is to rob it.
     */
    if (nums.Length == 1) return nums[0];

    /*
     * Initialize rolling DP values:
     *
     * prev2 corresponds to dp[0]:
     *   With only house 0 available, best is nums[0].
     */
    int prev2 = nums[0];

    /*
     * prev1 corresponds to dp[1]:
     *   With houses 0 and 1 available, best is max(nums[0], nums[1]).
     */
    int prev1 = Math.Max(nums[0], nums[1]);

    /*
     * Process houses from index 2 through the end.
     * For each house i, decide whether to:
     *   - take it: prev2 + nums[i]
     *   - skip it: prev1
     * Then store the maximum as the new best.
     */
    for (int i = 2; i < nums.Length; i++) {

        /*
         * Option 1: Take (rob) the current house i.
         * If we take house i, we cannot take house i-1,
         * so we add nums[i] to prev2 (which represents dp[i-2]).
         */
        int take = prev2 + nums[i];

        /*
         * Option 2: Skip the current house i.
         * If we skip it, the best remains dp[i-1], which is prev1.
         */
        int skip = prev1;

        /*
         * The best up through house i is whichever option yields more.
         * This is dp[i].
         */
        int cur = Math.Max(take, skip);

        /*
         * Slide the rolling window forward for the next iteration:
         * - prev2 becomes old prev1 (dp[i-1] becomes dp[i-2] next step)
         * - prev1 becomes cur      (dp[i] becomes dp[i-1] next step)
         */
        prev2 = prev1;
        prev1 = cur;
    }

    /*
     * After the loop, prev1 holds dp[last] —
     * the maximum amount we can rob without triggering the alarm.
     */
    return prev1;
}

// ============================
// Tests (top-level)
// ============================

/*
 * Test inputs for Rob().
 *
 * Notes:
 * - Uses Array.Empty<int>() to represent an empty input cleanly in C#.
 * - The other arrays use collection-expression syntax (C# 12): [ ... ].
 */
int[][] tests =
[
    [],       // empty input → expected 0
    [5],                      // single house → rob it
    [1, 2],                   // choose max(1,2) → 2
    [2, 7, 9, 3, 1],          // classic example → 12 (2 + 9 + 1)
    [1, 2, 3, 1],             // 1 + 3 = 4
    [2, 1, 1, 2],             // 2 + 2 = 4
    [10, 5, 20, 15, 30],      // 10 + 20 + 30 = 60
    [1, 3, 1, 3, 100]         // 3 + 100 = 103
];

/*
 * Expected outputs aligned by index with the 'tests' array.
 */
int[] expected =
[
    0,
    5,
    2,
    12,
    4,
    4,
    60,
    103
];

Console.WriteLine("=== Test: Rob (House Robber) ===\n");

/*
 * Run each test case, compute Rob(nums), and print:
 * - the test number
 * - the input array
 * - the computed result
 * - the expected result
 */
for (int i = 0; i < tests.Length; i++)
{
    int result = Rob(tests[i]);

    // string.Join produces a readable comma-separated list of array values.
    // For the empty array case, this prints "Rob([])" with nothing between.
    Console.WriteLine($"Test {i + 1}: Rob([{string.Join(", ", tests[i])}]) = {result} (expected {expected[i]})");
}
