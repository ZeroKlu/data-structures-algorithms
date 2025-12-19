package code_samples.section8.problems.problem8_3;

/*
 * Problem 8.3 — Coin Change (Minimum Coins)
 *
 * Given:
 *   - An array of coin denominations (coins[])
 *   - A target amount
 *
 * Goal:
 *   - Compute the minimum number of coins needed to make up the given amount
 *   - Return -1 if it is not possible
 *
 * This is a classic Dynamic Programming problem (unbounded knapsack),
 * where each coin can be used an unlimited number of times.
 */
public class problem8_3 {

    /*
     * coinChange(coins, amount)
     *
     * dp[x] = minimum number of coins needed to make amount x
     *
     * We build the solution bottom-up from 0 to amount.
     */
    int coinChange(int[] coins, int amount) {

        /*
         * Base case:
         * If the target amount is 0, no coins are required.
         */
        if (amount == 0) return 0;

        /*
         * INF is a sentinel value representing "unreachable".
         *
         * The maximum number of coins needed can never exceed 'amount'
         * (worst case: using coin value 1 repeatedly),
         * so amount + 1 safely represents infinity.
         */
        int INF = amount + 1;

        /*
         * dp[x] will store the minimum number of coins needed
         * to make sum x.
         *
         * We allocate dp for all sums from 0 through amount.
         */
        int[] dp = new int[amount + 1];

        /*
         * Initialize all dp values as unreachable.
         */
        for (int i = 0; i <= amount; i++) {
            dp[i] = INF;
        }

        /*
         * Base case:
         * It takes 0 coins to make amount 0.
         */
        dp[0] = 0;

        /*
         * For each coin denomination, try to update all reachable sums.
         *
         * The outer loop iterates over coins,
         * the inner loop iterates forward over amounts,
         * which allows unlimited reuse of each coin.
         */
        for (int coin : coins) {

            /*
             * For every amount x that can include this coin:
             *   - If we can make (x - coin),
             *     then we can make x by adding one more coin.
             */
            for (int x = coin; x <= amount; x++) {

                /*
                 * Transition:
                 * dp[x] = min(
                 *     current best dp[x],
                 *     dp[x - coin] + 1
                 * )
                 *
                 * If dp[x - coin] was unreachable (INF),
                 * dp[x - coin] + 1 will not improve dp[x].
                 */
                dp[x] = Math.min(dp[x], dp[x - coin] + 1);
            }
        }

        /*
         * If dp[amount] is still larger than 'amount',
         * then the target sum was never reachable.
         */
        return dp[amount] > amount ? -1 : dp[amount];
    }

    // ============================
    // Test Harness
    // ============================
    public static void main(String[] args) {

        problem8_3 solver = new problem8_3();

        /*
         * Different coin sets for testing.
         */
        int[][] coinSets = {
            {1, 2, 5},
            {2},
            {1},
            {1, 3, 4},
            {2, 5, 10, 1},
            {186, 419, 83, 408}
        };

        /*
         * Target amounts corresponding to each coin set.
         */
        int[] amounts = {
            11,     // -> 3 (5 + 5 + 1)
            3,      // -> -1 (impossible)
            0,      // -> 0 (no coins needed)
            6,      // -> 2 (3 + 3)
            27,     // -> 4 (10 + 10 + 5 + 2)
            6249    // -> 20 (LeetCode stress test)
        };

        /*
         * Expected outputs for verification.
         */
        int[] expected = {
            3,
            -1,
            0,
            2,
            4,
            20
        };

        System.out.println("=== Test: coinChange (Minimum Coins) ===\n");

        /*
         * Run each test case and print results.
         */
        for (int i = 0; i < amounts.length; i++) {

            int result = solver.coinChange(coinSets[i], amounts[i]);

            System.out.print("Test " + (i + 1) + ": coins = [");
            for (int j = 0; j < coinSets[i].length; j++) {
                System.out.print(coinSets[i][j]);
                if (j + 1 < coinSets[i].length) System.out.print(", ");
            }

            System.out.println("], amount = " + amounts[i] +
                               " -> " + result +
                               " (expected " + expected[i] + ")");
        }
    }
}
