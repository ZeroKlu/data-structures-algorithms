#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*
 * Problem: House Robber
 *
 * You are given a list of non-negative integers, where nums[i] is the amount
 * of money in the i-th house. You cannot rob two adjacent houses (alarm triggers).
 *
 * Goal:
 *   Return the maximum amount of money you can rob.
 *
 * Dynamic Programming Idea:
 *   Let dp[i] be the maximum money you can rob from houses [0..i].
 *
 * Recurrence:
 *   dp[i] = max(
 *              dp[i - 1],            // skip house i
 *              dp[i - 2] + nums[i]   // take house i (must skip i-1)
 *           )
 *
 * Space Optimization:
 *   We don't need the whole dp[] array—only dp[i-1] and dp[i-2].
 *   Use two rolling variables:
 *     prev1 = dp[i-1]
 *     prev2 = dp[i-2]
 */
int rob(const vector<int> &nums) {

    /*
     * Edge case: no houses → no money to rob.
     */
    if (nums.empty()) return 0;

    /*
     * Edge case: one house → the best we can do is rob it.
     */
    if (nums.size() == 1) return nums[0];

    /*
     * Initialize rolling DP values:
     *
     * prev2 corresponds to dp[0] (best up to the first house).
     * With only house 0 available, best is nums[0].
     */
    int prev2 = nums[0];

    /*
     * prev1 corresponds to dp[1] (best up to the second house).
     * With houses 0 and 1 available, best is max(nums[0], nums[1]).
     */
    int prev1 = max(nums[0], nums[1]);

    /*
     * Process houses from index 2 to the end.
     * Each iteration computes dp[i] using the recurrence:
     *   dp[i] = max(dp[i-1], dp[i-2] + nums[i])
     */
    for (size_t i = 2; i < nums.size(); i++) {

        /*
         * Option 1: Take house i.
         * If we rob house i, we must skip house i-1.
         * So we add nums[i] to prev2 (which represents dp[i-2]).
         */
        int take = prev2 + nums[i];

        /*
         * Option 2: Skip house i.
         * If we skip it, the best remains dp[i-1], which is prev1.
         */
        int skip = prev1;

        /*
         * Choose whichever option yields more money.
         * This becomes the new dp[i].
         */
        int cur = max(take, skip);

        /*
         * Slide the rolling window forward:
         * - prev2 becomes old prev1 (dp[i-1] becomes dp[i-2] next iteration)
         * - prev1 becomes cur      (dp[i] becomes dp[i-1] next iteration)
         */
        prev2 = prev1;
        prev1 = cur;
    }

    /*
     * After processing all houses, prev1 holds dp[last],
     * i.e., the maximum amount we can rob overall.
     */
    return prev1;
}

int main() {
    cout << "=== Test: rob (House Robber) ===\n\n";

    /*
     * Each TestCase stores:
     * - nums: the list of house values
     * - expected: the known correct result for that input
     */
    struct TestCase {
        vector<int> nums;
        int expected;
    };

    /*
     * Test cases include:
     * - empty input
     * - single-element input
     * - small arrays where results are easy to verify by hand
     * - classic examples commonly used for this problem
     */
    vector<TestCase> tests = {
        {{}, 0},                          // empty → 0
        {{5}, 5},                         // single house → take it
        {{1, 2}, 2},                      // best is max(1,2)
        {{2, 7, 9, 3, 1}, 12},             // classic example: 2 + 9 + 1 = 12
        {{1, 2, 3, 1}, 4},                 // 1 + 3 = 4
        {{2, 1, 1, 2}, 4},                 // 2 + 2 = 4
        {{10, 5, 20, 15, 30}, 60},         // 10 + 20 + 30 = 60
        {{1, 3, 1, 3, 100}, 103}            // 3 + 100 = 103
    };

    /*
     * Run all tests and print:
     * - the input array
     * - the computed result
     * - the expected result
     */
    for (size_t i = 0; i < tests.size(); i++) {
        int result = rob(tests[i].nums);

        cout << "Test " << (i + 1) << ": rob([";
        for (size_t j = 0; j < tests[i].nums.size(); j++) {
            cout << tests[i].nums[j];
            if (j + 1 < tests[i].nums.size()) cout << ", ";
        }
        cout << "]) = " << result
             << " (expected " << tests[i].expected << ")\n";
    }

    return 0;
}
