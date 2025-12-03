"""Tests for Problem 8.3: Minimum Coins"""

def coin_change(coins: list[int], amount: int) -> int:
    """Returns the minimum number of coins to make change for amount."""
    if amount == 0:
        return 0
    INF = amount + 1
    dp = [INF] * (amount + 1)
    dp[0] = 0
    for coin in coins:
        for x in range(coin, amount + 1):
            dp[x] = min(dp[x], dp[x - coin] + 1)
    return -1 if dp[amount] > amount else dp[amount]


# ============================
# Test Cases (Top-Level)
# ============================

tests = [
    ([1, 2, 5], 11, 3),
    ([2], 3, -1),
    ([1], 0, 0),
    ([1], 2, 2),
    ([1, 3, 4], 6, 2),
    ([2, 5, 10, 1], 27, 4),
    ([186, 419, 83, 408], 6249, 20),  # LeetCode big test
]

print("=== Test: coin_change (Minimum Coins) ===\n")

for i, (coin_qty, total_amount, expected) in enumerate(tests, start=1):
    result = coin_change(coin_qty, total_amount)
    print(f"Test {i}: coin_change({coin_qty}, {total_amount}) = {result} (expected {expected})")
