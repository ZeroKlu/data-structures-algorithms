/*
 * Problem 8.3 — Coin Change (Minimum Coins)
 *
 * Given:
 *   - An array of coin denominations (coins)
 *   - A target amount
 *
 * Goal:
 *   - Find the minimum number of coins required to make exactly `amount`
 *   - Return -1 if it is not possible
 *
 * This is a classic Dynamic Programming problem (unbounded knapsack),
 * where each coin can be used an unlimited number of times.
 */
function coinChange(coins, amount) {

    /*
     * Base case:
     * If the target amount is 0, no coins are needed.
     */
    if (amount === 0) return 0;

    /*
     * INF represents an unreachable state.
     *
     * The worst-case number of coins needed is `amount`
     * (using coin value 1 repeatedly),
     * so amount + 1 safely acts as "infinity".
     */
    const INF = amount + 1;

    /*
     * dp[x] will store the minimum number of coins
     * needed to make amount x.
     *
     * Initialize all values to INF (unreachable).
     */
    const dp = new Array(amount + 1).fill(INF);

    /*
     * Base case:
     * It takes 0 coins to make amount 0.
     */
    dp[0] = 0;

    /*
     * For each coin denomination, attempt to update
     * all amounts that can be formed using that coin.
     *
     * Outer loop: iterate over coins
     * Inner loop: iterate forward over amounts
     *
     * Forward iteration allows unlimited reuse of the same coin.
     */
    for (const coin of coins) {

        /*
         * Start from `coin` because amounts smaller than the coin
         * cannot include it.
         */
        for (let x = coin; x <= amount; x++) {

            /*
             * Transition:
             * If we can make (x - coin),
             * then we can make x by adding one more coin.
             *
             * dp[x] = min(
             *     current best dp[x],
             *     dp[x - coin] + 1
             * )
             */
            dp[x] = Math.min(dp[x], dp[x - coin] + 1);
        }
    }

    /*
     * If dp[amount] is still greater than `amount`,
     * then the target sum was never reachable.
     */
    return dp[amount] > amount ? -1 : dp[amount];
}

// ============================
// Tests
// ============================

console.log("=== Test: coinChange (Minimum Coins) ===\n");

/*
 * Each test includes:
 *   - coin denominations
 *   - target amount
 *   - expected minimum number of coins
 */
const tests = [
    { coins: [1, 2, 5], amount: 11, expected: 3 },        // 5 + 5 + 1
    { coins: [2], amount: 3, expected: -1 },              // impossible
    { coins: [1], amount: 0, expected: 0 },               // zero amount
    { coins: [1], amount: 2, expected: 2 },               // 1 + 1
    { coins: [1, 3, 4], amount: 6, expected: 2 },          // 3 + 3
    { coins: [2, 5, 10, 1], amount: 27, expected: 4 },     // 10 + 10 + 5 + 2
    { coins: [186, 419, 83, 408], amount: 6249, expected: 20 } // LeetCode heavy case
];

/*
 * Run each test and print results.
 */
tests.forEach((t, index) => {
    const result = coinChange(t.coins, t.amount);
    console.log(
        `Test ${index + 1}: coinChange([${t.coins.join(", ")}], ${t.amount}) = ${result} (expected ${t.expected})`
    );
});
