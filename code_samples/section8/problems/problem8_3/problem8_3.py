"""
Problem 8.3 — Coin Change (Minimum Coins)

Goal:
- Given a list of coin denominations and a target amount,
  determine the minimum number of coins needed to make
  exactly that amount.
- If it is not possible, return -1.

This is a classic Dynamic Programming problem (unbounded knapsack),
where each coin may be used an unlimited number of times.
"""

def coin_change(coins: list[int], amount: int) -> int:
    """
    Returns the minimum number of coins required to make `amount`.

    Parameters:
    - coins: list[int]
        Available coin denominations (can be used multiple times).
    - amount: int
        Target amount to form.

    Returns:
    - int
        Minimum number of coins needed, or -1 if impossible.
    """

    # ---------------------------------------------------------
    # Base case:
    # If the target amount is 0, no coins are required.
    # ---------------------------------------------------------
    if amount == 0:
        return 0

    # ---------------------------------------------------------
    # INF represents an unreachable state.
    #
    # The maximum number of coins needed in the worst case
    # is `amount` (using coin value 1 repeatedly).
    # Using amount + 1 safely represents "infinity".
    # ---------------------------------------------------------
    INF = amount + 1

    # ---------------------------------------------------------
    # dp[x] will store the minimum number of coins needed
    # to make amount x.
    #
    # Initialize all values to INF (meaning unreachable).
    # ---------------------------------------------------------
    dp = [INF] * (amount + 1)

    # ---------------------------------------------------------
    # Base DP state:
    # It takes 0 coins to make amount 0.
    # ---------------------------------------------------------
    dp[0] = 0

    # ---------------------------------------------------------
    # Build the DP table.
    #
    # Outer loop:
    #   Iterate through each coin denomination.
    #
    # Inner loop:
    #   Try forming all amounts from `coin` to `amount`.
    #
    # The forward iteration allows unlimited reuse
    # of the same coin.
    # ---------------------------------------------------------
    for coin in coins:

        # Amounts smaller than `coin` cannot include this coin
        for x in range(coin, amount + 1):

            # -------------------------------------------------
            # Transition:
            # If we can make (x - coin),
            # then we can make x by adding one more coin.
            #
            # dp[x] = min(
            #     current best dp[x],
            #     dp[x - coin] + 1
            # )
            # -------------------------------------------------
            dp[x] = min(dp[x], dp[x - coin] + 1)

    # ---------------------------------------------------------
    # If dp[amount] is still greater than `amount`,
    # then the amount was never reachable.
    # ---------------------------------------------------------
    return -1 if dp[amount] > amount else dp[amount]


# ============================
# Test Cases (Top-Level)
# ============================

# Each test contains:
#   (coin denominations, target amount, expected result)
tests = [
    ([1, 2, 5], 11, 3),               # 5 + 5 + 1
    ([2], 3, -1),                     # impossible
    ([1], 0, 0),                      # zero amount
    ([1], 2, 2),                      # 1 + 1
    ([1, 3, 4], 6, 2),                # 3 + 3
    ([2, 5, 10, 1], 27, 4),            # 10 + 10 + 5 + 2
    ([186, 419, 83, 408], 6249, 20),   # LeetCode large test case
]

print("=== Test: coin_change (Minimum Coins) ===\n")

# Execute tests and compare results
for i, (coin_qty, total_amount, expected) in enumerate(tests, start=1):
    result = coin_change(coin_qty, total_amount)
    print(
        f"Test {i}: coin_change({coin_qty}, {total_amount}) = "
        f"{result} (expected {expected})"
    )
