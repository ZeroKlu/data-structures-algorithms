function coinChange(coins, amount) {
    if (amount === 0) return 0;
    const INF = amount + 1;
    const dp = new Array(amount + 1).fill(INF);
    dp[0] = 0;
    for (const coin of coins) {
        for (let x = coin; x <= amount; x++) {
            dp[x] = Math.min(dp[x], dp[x - coin] + 1);
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}

// ============================
// Tests
// ============================

const tests = [
    { coins: [1, 2, 5], amount: 11, expected: 3 },
    { coins: [2], amount: 3, expected: -1 },
    { coins: [1], amount: 0, expected: 0 },
    { coins: [1], amount: 2, expected: 2 },
    { coins: [1, 3, 4], amount: 6, expected: 2 },
    { coins: [2, 5, 10, 1], amount: 27, expected: 4 },
    { coins: [186, 419, 83, 408], amount: 6249, expected: 20 } // LC heavy case
];

console.log("=== Test: coinChange (Minimum Coins) ===\n");

tests.forEach((t, index) => {
    const result = coinChange(t.coins, t.amount);
    console.log(
        `Test ${index + 1}: coinChange([${t.coins.join(", ")}], ${t.amount}) = ${result} (expected ${t.expected})`
    );
});
