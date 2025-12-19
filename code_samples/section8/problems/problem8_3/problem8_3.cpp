#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*
 * Problem: Coin Change (Minimum Coins)
 *
 * Given:
 *   - coins: a list of coin denominations (positive integers)
 *   - amount: target sum
 *
 * Return:
 *   - the minimum number of coins needed to make exactly 'amount'
 *   - or -1 if it's not possible
 *
 * Dynamic Programming (DP) Idea (Unbounded Knapsack / Min-Coin):
 *
 * Let dp[x] = minimum number of coins needed to make sum x.
 *
 * Base:
 *   dp[0] = 0  (0 coins to make 0)
 *
 * Transition:
 *   For each coin 'c' and for each x from c..amount:
 *       dp[x] = min(dp[x], dp[x - c] + 1)
 *
 * Explanation:
 *   If you can make (x - c), then you can make x by adding one coin of value c.
 *   We take the best (minimum) across all possible last coins used.
 *
 * Loop Order Note:
 *   - Outer loop over coins
 *   - Inner loop over x increasing from coin..amount
 *
 * This loop order supports "unbounded" usage of coins (you may reuse a coin
 * multiple times), because dp[x - coin] may have been improved earlier in the
 * same coin iteration.
 *
 * Complexity:
 *   Time:  O(coins.size() * amount)
 *   Space: O(amount)
 */
int coinChange(const vector<int> &coins, int amount) {

    /*
     * Edge case: amount == 0 → no coins needed.
     */
    if (amount == 0) return 0;

    /*
     * Use a sentinel value (INF) to mean "currently impossible".
     *
     * Common trick:
     *   INF = amount + 1
     *
     * Why this is safe:
     *   Any valid solution (minimum number of coins) can never exceed amount
     *   if a 1-coin exists; otherwise it may be impossible.
     *   So amount+1 is definitely larger than any feasible minimum.
     */
    const int INF = amount + 1;

    /*
     * dp[x] = minimum coins needed to make sum x.
     * Initialize all dp values to INF (unreachable), then set dp[0] = 0.
     */
    vector<int> dp(amount + 1, INF);
    dp[0] = 0;

    /*
     * Try each coin denomination.
     */
    for (int coin : coins) {

        /*
         * For each reachable sum x where using this coin is possible (x >= coin),
         * see if we can improve dp[x] by taking this coin once.
         */
        for (int x = coin; x <= amount; x++) {

            /*
             * Candidate if we take 'coin' as the last coin:
             *   dp[x - coin] + 1
             *
             * If dp[x - coin] is INF (unreachable), then INF + 1 won't improve
             * dp[x], so we don't need a separate guard check here.
             */
            dp[x] = min(dp[x], dp[x - coin] + 1);
        }
    }

    /*
     * If dp[amount] is still > amount (i.e., still INF-ish),
     * then no combination of coins can form 'amount'.
     */
    return dp[amount] > amount ? -1 : dp[amount];
}

int main() {

    cout << "=== Test: coinChange (Minimum Coins) ===\n\n";

    /*
     * TestCase groups together:
     *   - coins: denominations
     *   - amount: target sum
     *   - expected: known correct output
     */
    struct TestCase {
        vector<int> coins;
        int amount;
        int expected;
    };

    /*
     * A small suite of representative tests:
     * - normal cases
     * - impossible case
     * - zero amount case
     * - a larger known example
     */
    vector<TestCase> tests = {
        {{1, 2, 5}, 11, 3},              // 5 + 5 + 1
        {{2}, 3, -1},                    // cannot make 3 from only 2s
        {{1}, 0, 0},                     // amount 0 → 0 coins
        {{1}, 2, 2},                     // 1 + 1
        {{1, 3, 4}, 6, 2},               // best is 3 + 3 (2 coins)
        {{2, 5, 10, 1}, 27, 4},          // 10 + 10 + 5 + 2 (4 coins)
        {{186, 419, 83, 408}, 6249, 20}  // known LeetCode large case
    };

    /*
     * Run each test, compute result, and print a readable report.
     */
    for (size_t i = 0; i < tests.size(); i++) {
        int result = coinChange(tests[i].coins, tests[i].amount);

        cout << "Test " << i + 1 << ": coins=[";
        for (size_t j = 0; j < tests[i].coins.size(); j++) {
            cout << tests[i].coins[j];
            if (j + 1 < tests[i].coins.size()) cout << ", ";
        }

        cout << "], amount=" << tests[i].amount
             << " -> " << result
             << " (expected " << tests[i].expected << ")\n";
    }

    return 0;
}
