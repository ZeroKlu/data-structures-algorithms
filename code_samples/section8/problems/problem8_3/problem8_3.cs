/*
 * Problem: Coin Change (Minimum Coins)
 *
 * Given:
 *   - coins: an array of coin denominations (positive integers)
 *   - amount: the target sum
 *
 * Return:
 *   - the minimum number of coins needed to make exactly 'amount'
 *   - or -1 if it is not possible with the given coins
 *
 * This is a classic Dynamic Programming (DP) problem, often described
 * as an "unbounded knapsack" variant because each coin can be used
 * unlimited times.
 */
static int CoinChange(int[] coins, int amount)
{
    /*
     * Edge case:
     * If the target amount is 0, no coins are needed.
     */
    if (amount == 0) return 0;

    /*
     * INF is a sentinel value representing "unreachable".
     *
     * We use amount + 1 because:
     *   - The minimum number of coins can never exceed 'amount'
     *     (worst case: using coin value 1 repeatedly).
     *   - Therefore, amount + 1 is safely larger than any valid answer.
     */
    int INF = amount + 1;

    /*
     * dp[x] = minimum number of coins needed to make sum x.
     *
     * We allocate dp for all values from 0..amount.
     */
    int[] dp = new int[amount + 1];

    /*
     * Initialize all dp values as unreachable (INF).
     */
    for (int i = 0; i <= amount; i++)
    {
        dp[i] = INF;
    }

    /*
     * Base case:
     * It takes 0 coins to make sum 0.
     */
    dp[0] = 0;

    /*
     * Iterate over each coin denomination.
     *
     * Outer loop over coins + inner loop over amounts (in increasing order)
     * allows unlimited reuse of each coin.
     */
    foreach (int coin in coins)
    {
        /*
         * For each reachable sum x where this coin could be used:
         *   - If we can make (x - coin),
         *     then we can make x by adding one coin of value 'coin'.
         */
        for (int x = coin; x <= amount; x++)
        {
            /*
             * Transition:
             *   dp[x] = min(
             *       current best dp[x],
             *       dp[x - coin] + 1   // use this coin once
             *   )
             *
             * If dp[x - coin] was INF, adding 1 will not improve dp[x],
             * so no extra check is required.
             */
            dp[x] = Math.Min(dp[x], dp[x - coin] + 1);
        }
    }

    /*
     * If dp[amount] is still greater than 'amount',
     * then it was never updated to a valid value,
     * meaning the amount cannot be formed.
     */
    return dp[amount] > amount ? -1 : dp[amount];
}

// ============================
// Tests (top-level)
// ============================

/*
 * Each test case includes:
 *   - coins: available denominations
 *   - amount: target sum
 *   - expected: known correct result
 */
var tests = new (int[] coins, int amount, int expected)[]
{
    (new[] {1, 2, 5}, 11, 3),          // 5 + 5 + 1
    (new[] {2}, 3, -1),                // impossible
    (new[] {1}, 0, 0),                 // amount = 0
    (new[] {1}, 2, 2),                 // 1 + 1
    (new[] {1, 3, 4}, 6, 2),           // 3 + 3
    (new[] {2, 5, 10, 1}, 27, 4),      // 10 + 10 + 5 + 2
    (new[] {186, 419, 83, 408}, 6249, 20) // LeetCode large stress test
};

Console.WriteLine("=== Test: CoinChange (Minimum Coins) ===\n");

/*
 * Execute each test case and print results.
 */
int testNum = 1;
foreach (var (coins, amount, expected) in tests)
{
    int result = CoinChange(coins, amount);

    Console.WriteLine(
        $"Test {testNum++}: CoinChange([{string.Join(", ", coins)}], {amount}) = {result} (expected {expected})"
    );
}
