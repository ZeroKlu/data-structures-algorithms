#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

/*
 * Problem: Coin Change (Minimum Coins)
 *
 * Given:
 *   - coins[]: available coin denominations (positive integers)
 *   - amount: a target sum
 *
 * Return:
 *   - the minimum number of coins needed to make up 'amount'
 *   - or -1 if it's not possible
 *
 * Dynamic Programming (DP) Approach (Unbounded Knapsack / Min-Coin):
 *
 * Let dp[x] = minimum number of coins needed to make sum x.
 *
 * Base:
 *   dp[0] = 0  (0 coins to make sum 0)
 *
 * Transition:
 *   For each coin value 'coin', for each sum x from coin..amount:
 *       dp[x] = min(dp[x], dp[x - coin] + 1)
 *
 * Why this works:
 *   If we can make (x - coin), then we can make x by adding one 'coin'.
 *   We try all coins and keep the best (minimum coins).
 *
 * Complexity:
 *   Time:  O(coinsSize * amount)
 *   Space: O(amount)
 */
int coinChange(int *coins, int coinsSize, int amount) {

    /*
     * Edge case: amount == 0 always requires 0 coins.
     */
    if (amount == 0) return 0;

    /*
     * We'll use a "sentinel" value that represents "impossible / not computed yet".
     *
     * A common trick:
     *   max = amount + 1
     *
     * Reason:
     *   The minimum number of coins to make 'amount' can never exceed 'amount'
     *   if a coin of denomination 1 exists (worst case: 1+1+...+1).
     *   So amount+1 is safely "larger than any valid answer".
     */
    int max = amount + 1;

    /*
     * Allocate dp array where:
     *   dp[x] = minimum coins to make sum x
     * for x in [0..amount].
     */
    int *dp = (int *)malloc((amount + 1) * sizeof(int));

    /*
     * In production code, you’d typically check dp != NULL here.
     * For these examples, we assume allocation succeeds.
     */

    /*
     * Initialize all dp values to "impossible" (max sentinel).
     * This means: "we currently don't know how to make this sum".
     */
    for (int i = 0; i <= amount; i++) {
        dp[i] = max;
    }

    /*
     * Base case: 0 coins needed to make sum 0.
     */
    dp[0] = 0;

    /*
     * Outer loop over coin denominations.
     *
     * Note:
     *   This loop order (coins outer, sums inner increasing) corresponds to
     *   an "unbounded" coin system (you can reuse coins unlimited times).
     *
     * When x increases from coin..amount, dp[x - coin] may have already been
     * improved using the same coin earlier in this iteration, which correctly
     * allows taking the coin multiple times.
     */
    for (int i = 0; i < coinsSize; i++) {
        int coin = coins[i];

        /*
         * For every target sum x where using this coin is possible (x >= coin):
         * try improving dp[x] by taking one 'coin' plus whatever it took
         * to make (x - coin).
         */
        for (int x = coin; x <= amount; x++) {

            /*
             * Candidate solution:
             *   dp[x - coin] + 1
             *
             * If dp[x - coin] is still 'max' (impossible), then
             * dp[x - coin] + 1 would be max+1, which won't beat dp[x] anyway.
             * So this still behaves safely without needing a separate check.
             */
            if (dp[x - coin] + 1 < dp[x]) {
                dp[x] = dp[x - coin] + 1;
            }
        }
    }

    /*
     * If dp[amount] is still larger than 'amount', then we consider it impossible.
     *
     * Why compare to amount?
     *   Valid solutions must be <= amount (again, because amount+1 was our sentinel).
     */
    int ans = dp[amount] > amount ? -1 : dp[amount];

    /*
     * Free heap memory to avoid leaks.
     */
    free(dp);

    return ans;
}

int main(void) {

    printf("=== Test: coinChange (Minimum Coins) ===\n\n");

    /*
     * Each test case includes:
     *   - coins[]: coin denominations (stored in a fixed-size array for convenience)
     *   - coinsSize: how many of those entries are actually used
     *   - amount: target sum
     *   - expected: known correct answer
     */
    struct TestCase {
        int coins[10];
        int coinsSize;
        int amount;
        int expected;
    } tests[] = {
        {{1, 2, 5}, 3, 11, 3},          // 5 + 5 + 1
        {{2}, 1, 3, -1},                // impossible to make 3 using only 2s
        {{1}, 1, 0, 0},                 // amount 0 → 0 coins
        {{1}, 1, 2, 2},                 // 1 + 1
        {{1, 3, 4}, 3, 6, 2},           // 3 + 3 is best (2 coins)
        {{2, 5, 10, 1}, 4, 27, 4},      // 10 + 10 + 5 + 2
        {{186, 419, 83, 408}, 4, 6249, 20}, // known large LeetCode case
    };

    /*
     * Compute number of test cases at compile time.
     */
    int numTests = sizeof(tests) / sizeof(tests[0]);

    /*
     * Run each test case, print the input and output.
     */
    for (int i = 0; i < numTests; i++) {
        struct TestCase t = tests[i];

        int result = coinChange(t.coins, t.coinsSize, t.amount);

        /*
         * Print coins array in a readable format.
         */
        printf("Test %d: coins=[", i + 1);
        for (int j = 0; j < t.coinsSize; j++) {
            printf("%d", t.coins[j]);
            if (j + 1 < t.coinsSize) printf(", ");
        }

        /*
         * Print amount, computed result, and expected result.
         */
        printf("], amount=%d -> %d (expected %d)\n",
               t.amount, result, t.expected);
    }

    return 0;
}
