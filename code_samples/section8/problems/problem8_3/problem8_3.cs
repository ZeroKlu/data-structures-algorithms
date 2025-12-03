static int CoinChange(int[] coins, int amount)
{
    if (amount == 0) return 0;
    int INF = amount + 1;
    int[] dp = new int[amount + 1];
    for (int i = 0; i <= amount; i++)
    {
        dp[i] = INF;
    }
    dp[0] = 0;

    foreach (int coin in coins)
    {
        for (int x = coin; x <= amount; x++)
        {
            dp[x] = Math.Min(dp[x], dp[x - coin] + 1);
        }
    }

    return dp[amount] > amount ? -1 : dp[amount];
}

// ============================
// Tests (top-level)
// ============================

var tests = new (int[] coins, int amount, int expected)[]
{
    (new[] {1, 2, 5}, 11, 3),
    (new[] {2}, 3, -1),
    (new[] {1}, 0, 0),
    (new[] {1}, 2, 2),
    (new[] {1, 3, 4}, 6, 2),
    (new[] {2, 5, 10, 1}, 27, 4),
    (new[] {186, 419, 83, 408}, 6249, 20) // LeetCode heavy test
};

Console.WriteLine("=== Test: CoinChange (Minimum Coins) ===\n");

int testNum = 1;
foreach (var t in tests)
{
    int result = CoinChange(t.coins, t.amount);
    Console.WriteLine(
        $"Test {testNum++}: CoinChange([{string.Join(", ", t.coins)}], {t.amount}) = {result} (expected {t.expected})"
    );
}
