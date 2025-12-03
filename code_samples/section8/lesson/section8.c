#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long long fib_dp(int n) {
    if (n <= 1) return n;
    long long prev2 = 0; // F(0)
    long long prev1 = 1; // F(1)
    for (int i = 2; i <= n; i++) {
        long long curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

long long climb_stairs(int n) {
    if (n <= 1) return 1;
    long long prev2 = 1; // ways(0)
    long long prev1 = 1; // ways(1)
    for (int i = 2; i <= n; i++) {
        long long curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

int knapsack_01(int n, int W, const int* w, const int* v) {
    int* dp = (int*)malloc((W + 1) * sizeof(int));
    if (!dp) return 0;
    memset(dp, 0, (W + 1) * sizeof(int));

    for (int i = 0; i < n; i++) {
        for (int c = W; c >= w[i]; c--) {
            int with_item = dp[c - w[i]] + v[i];
            if (with_item > dp[c]) {
                dp[c] = with_item;
            }
        }
    }
    int result = dp[W];
    free(dp);
    return result;
}

int lcs_length(const char* s, const char* t) {
    int n = (int)strlen(s);
    int m = (int)strlen(t);
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    if (!dp) return 0;
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)malloc((m + 1) * sizeof(int));
        if (!dp[i]) {
            // basic cleanup on failure
            for (int k = 0; k < i; k++) free(dp[k]);
            free(dp);
            return 0;
        }
    }

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (s[i - 1] == t[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                int a = dp[i - 1][j];
                int b = dp[i][j - 1];
                dp[i][j] = (a > b) ? a : b;
            }
        }
    }
    int result = dp[n][m];
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);
    return result;
}

int main(void) {
    /* ============================
       Test fib_dp
       ============================ */
    printf("=== Test: fib_dp ===\n\n");

    struct {
        int n;
        long long expected;
    } fib_tests[] = {
        {0, 0},
        {1, 1},
        {2, 1},
        {3, 2},
        {4, 3},
        {5, 5},
        {10, 55},
        {20, 6765},
        {30, 832040},
        {50, 12586269025ULL}
    };

    int numFibTests = (int)(sizeof(fib_tests) / sizeof(fib_tests[0]));

    for (int i = 0; i < numFibTests; i++) {
        int n = fib_tests[i].n;
        long long expected = fib_tests[i].expected;
        long long result = fib_dp(n);

        printf("fib_dp(%d) = %lld (expected %lld)\n",
               n, result, expected);
    }

    /* ============================
       Test climb_stairs
       ============================ */
    printf("\n=== Test: climb_stairs ===\n\n");

    struct {
        int n;
        long long expected;
    } climb_tests[] = {
        {0, 1},  // ways to stay at step 0
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 5},
        {5, 8},
        {10, 89}
    };

    int numClimbTests = (int)(sizeof(climb_tests) / sizeof(climb_tests[0]));

    for (int i = 0; i < numClimbTests; i++) {
        int n = climb_tests[i].n;
        long long expected = climb_tests[i].expected;
        long long result = climb_stairs(n);

        printf("climb_stairs(%d) = %lld (expected %lld)\n",
               n, result, expected);
    }

    /* ============================
       Test knapsack_01
       ============================ */
    printf("\n=== Test: knapsack_01 ===\n\n");

    {
        int w[] = {2, 3, 4, 5};
        int v[] = {3, 4, 5, 6};
        int n = (int)(sizeof(w) / sizeof(w[0]));

        int W1 = 5;
        int expected1 = 7; // pick weights 2 and 3 -> values 3+4=7
        int result1 = knapsack_01(n, W1, w, v);
        printf("knapsack_01(W=%d) = %d (expected %d)\n",
               W1, result1, expected1);

        int W2 = 7;
        int expected2 = 9; // pick weights 2 and 5 -> values 3+6=9
        int result2 = knapsack_01(n, W2, w, v);
        printf("knapsack_01(W=%d) = %d (expected %d)\n",
               W2, result2, expected2);

        int W3 = 3;
        int expected3 = 4; // pick weight 3 -> value 4
        int result3 = knapsack_01(n, W3, w, v);
        printf("knapsack_01(W=%d) = %d (expected %d)\n",
               W3, result3, expected3);
    }

    /* ============================
       Test lcs_length
       ============================ */
    printf("\n=== Test: lcs_length ===\n\n");

    struct {
        const char* s;
        const char* t;
        int expected;
    } lcs_tests[] = {
        {"abcde", "ace", 3},          // "ace"
        {"abc", "abc", 3},            // "abc"
        {"abc", "def", 0},            // no common subsequence
        {"", "abc", 0},               // empty vs non-empty
        {"AGGTAB", "GXTXAYB", 4}      // "GTAB"
    };

    int numLcsTests = (int)(sizeof(lcs_tests) / sizeof(lcs_tests[0]));

    for (int i = 0; i < numLcsTests; i++) {
        const char* s = lcs_tests[i].s;
        const char* t = lcs_tests[i].t;
        int expected = lcs_tests[i].expected;
        int result = lcs_length(s, t);

        printf("lcs_length(\"%s\", \"%s\") = %d (expected %d)\n",
               s, t, result, expected);
    }

    return 0;
}
