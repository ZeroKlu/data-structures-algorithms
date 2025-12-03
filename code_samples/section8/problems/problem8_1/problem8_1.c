#include <stdio.h>

int climbStairs(int n) {
    if (n <= 2) return n;
    int prev2 = 1; // dp[1]
    int prev1 = 2; // dp[2]
    for (int i = 3; i <= n; i++) {
        int cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

int main(void) {
    printf("=== Test: climbStairs ===\n\n");

    struct {
        int n;
        int expected;
    } tests[] = {
        {0, 0},  // You may define dp[0] as 0 or 1; using 0 here for consistency
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 5},
        {5, 8},
        {6, 13},
        {10, 89},
        {20, 10946}
    };

    int numTests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < numTests; i++) {
        int n = tests[i].n;
        int expected = tests[i].expected;
        int result = climbStairs(n);

        printf("climbStairs(%d) = %d (expected %d)\n",
               n, result, expected);
    }

    return 0;
}
