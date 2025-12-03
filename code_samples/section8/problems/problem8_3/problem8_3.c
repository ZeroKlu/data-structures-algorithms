#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int coinChange(int *coins, int coinsSize, int amount) {
    if (amount == 0) return 0;
    int max = amount + 1;
    int *dp = (int *)malloc((amount + 1) * sizeof(int));
    for (int i = 0; i <= amount; i++) {
        dp[i] = max;
    }
    dp[0] = 0;
    for (int i = 0; i < coinsSize; i++) {
        int coin = coins[i];
        for (int x = coin; x <= amount; x++) {
            if (dp[x - coin] + 1 < dp[x]) {
                dp[x] = dp[x - coin] + 1;
            }
        }
    }
    int ans = dp[amount] > amount ? -1 : dp[amount];
    free(dp);
    return ans;
}

int main(void) {

    printf("=== Test: coinChange (Minimum Coins) ===\n\n");

    struct TestCase {
        int coins[10];
        int coinsSize;
        int amount;
        int expected;
    } tests[] = {
        {{1, 2, 5}, 3, 11, 3},   // 5 + 5 + 1
        {{2}, 1, 3, -1},         // impossible
        {{1}, 1, 0, 0},          // amount 0
        {{1}, 1, 2, 2},          // 1+1
        {{1, 3, 4}, 3, 6, 2},    // 3+3 or 4+1+1
        {{2, 5, 10, 1}, 4, 27, 4}, // 10+10+5+2
        {{186, 419, 83, 408}, 4, 6249, 20}, // leetcode large case
    };

    int numTests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < numTests; i++) {
        struct TestCase t = tests[i];

        int result = coinChange(t.coins, t.coinsSize, t.amount);

        printf("Test %d: coins=[", i + 1);
        for (int j = 0; j < t.coinsSize; j++) {
            printf("%d", t.coins[j]);
            if (j + 1 < t.coinsSize) printf(", ");
        }
        printf("], amount=%d -> %d (expected %d)\n",
               t.amount, result, t.expected);
    }

    return 0;
}
