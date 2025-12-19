/*
 * Problem: House Robber
 *
 * You are given an array of non-negative integers where nums[i]
 * represents the amount of money in the i-th house.
 *
 * Constraint:
 *   You cannot rob two adjacent houses.
 *
 * Goal:
 *   Return the maximum amount of money you can rob without triggering the alarm.
 *
 * This is a classic Dynamic Programming (DP) problem.
 */
function rob(nums) {

    /*
     * Edge case 1:
     *   If there are no houses, there is nothing to rob.
     */
    if (nums.length === 0) return 0;

    /*
     * Edge case 2:
     *   If there is only one house, rob it.
     */
    if (nums.length === 1) return nums[0];

    /*
     * prev2 represents dp[i - 2]:
     *   The maximum amount we can rob up to two houses back.
     *   Initially, this is the value of the first house.
     */
    let prev2 = nums[0];

    /*
     * prev1 represents dp[i - 1]:
     *   The maximum amount we can rob up to the previous house.
     *   For the first two houses, we choose the larger of the two.
     */
    let prev1 = Math.max(nums[0], nums[1]);

    /*
     * Iterate through the rest of the houses, starting from index 2.
     * For each house, decide whether to:
     *   - take it (and skip the previous one), or
     *   - skip it (keep the best result so far).
     */
    for (let i = 2; i < nums.length; i++) {

        /*
         * Option 1: Take (rob) the current house.
         * If we rob house i, we must skip house i - 1,
         * so we add nums[i] to prev2 (dp[i - 2]).
         */
        const take = prev2 + nums[i];

        /*
         * Option 2: Skip the current house.
         * The total remains the same as dp[i - 1], stored in prev1.
         */
        const skip = prev1;

        /*
         * The best result at this house is the maximum
         * of taking or skipping it.
         */
        const cur = Math.max(take, skip);

        /*
         * Move the rolling DP window forward:
         *   prev2 becomes dp[i - 1]
         *   prev1 becomes dp[i]
         */
        prev2 = prev1;
        prev1 = cur;
    }

    /*
     * prev1 now contains the maximum amount of money
     * that can be robbed from all houses.
     */
    return prev1;
}

// ============================
// Tests
// ============================

/*
 * Test cases:
 * Each array represents a row of houses with money values.
 */
const tests = [
    [],                      // empty input → 0
    [5],                     // single house → 5
    [1, 2],                  // choose max → 2
    [2, 7, 9, 3, 1],         // classic example → 12
    [1, 2, 3, 1],            // → 4
    [2, 1, 1, 2],            // → 4
    [10, 5, 20, 15, 30],     // → 60
    [1, 3, 1, 3, 100]        // → 103
];

/*
 * Expected results corresponding to each test case.
 */
const expected = [
    0,
    5,
    2,
    12,
    4,
    4,
    60,
    103
];

console.log("=== Test: rob (House Robber) ===\n");

/*
 * Run each test and print:
 *   - test number
 *   - input array
 *   - result
 *   - expected value
 */
for (let i = 0; i < tests.length; i++) {
    const nums = tests[i];
    const result = rob(nums);

    console.log(
        `Test ${i + 1}: rob([${nums.join(", ")}]) = ${result} (expected ${expected[i]})`
    );
}
