#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int tsp(int **cost, int n) {
    int maxMask = 1 << n;
    int **dp = (int **)malloc(maxMask * sizeof(int *));
    for (int mask = 0; mask < maxMask; mask++) {
        dp[mask] = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            dp[mask][i] = INT_MAX / 2;
        }
    }
    dp[1][0] = 0; // mask 1: only city 0 visited

    for (int mask = 1; mask < maxMask; mask++) {
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i))) continue;
            int curCost = dp[mask][i];
            if (curCost == INT_MAX / 2) continue;
            for (int j = 0; j < n; j++) {
                if (mask & (1 << j)) continue;
                int nextMask = mask | (1 << j);
                int cand = curCost + cost[i][j];
                if (cand < dp[nextMask][j]) {
                    dp[nextMask][j] = cand;
                }
            }
        }
    }

    int fullMask = maxMask - 1;
    int ans = INT_MAX;
    for (int i = 0; i < n; i++) {
        int cand = dp[fullMask][i] + cost[i][0];
        if (cand < ans) ans = cand;
    }

    for (int mask = 0; mask < maxMask; mask++) {
        free(dp[mask]);
    }
    free(dp);
    return ans;
}

// ===========================
// Tests
// ===========================

void test_tsp(const char *name, int **cost, int n, int expected) {
    int result = tsp(cost, n);
    printf("%s\n", name);
    printf("n = %d\n", n);
    printf("tsp tour cost (with return to start) = %d (expected %d)\n\n",
           result, expected);
}

int main(void) {
    printf("=== Testing tsp (bitmask DP) ===\n\n");

    // ---- Test 1: 4-node classic example ----
    // Cost matrix:
    //   0   10  15  20
    //   10  0   35  25
    //   15  35  0   30
    //   20  25  30  0
    //
    // Best tour cost including return to 0 is 80:
    //   0 -> 1 -> 3 -> 2 -> 0
    //   10 + 25 + 30 + 15 = 80
    int n1 = 4;
    int raw1[4][4] = {
        { 0, 10, 15, 20 },
        { 10, 0, 35, 25 },
        { 15, 35, 0, 30 },
        { 20, 25, 30, 0 }
    };
    int *cost1[4];
    for (int i = 0; i < n1; i++) {
        cost1[i] = raw1[i];
    }
    test_tsp("Test 1: 4-node classic example", cost1, n1, 80);

    // ---- Test 2: 3-node triangle ----
    // Cost matrix:
    //   0  1  4
    //   1  0  2
    //   4  2  0
    //
    // Possible tours:
    //   0 -> 1 -> 2 -> 0 = 1 + 2 + 4 = 7
    //   0 -> 2 -> 1 -> 0 = 4 + 2 + 1 = 7
    // So optimal TSP cost = 7
    int n2 = 3;
    int raw2[3][3] = {
        { 0, 1, 4 },
        { 1, 0, 2 },
        { 4, 2, 0 }
    };
    int *cost2[3];
    for (int i = 0; i < n2; i++) {
        cost2[i] = raw2[i];
    }
    test_tsp("Test 2: 3-node triangle", cost2, n2, 7);

    // ---- Test 3: 4-node square loop ----
    //   0 --1-- 1
    //   |       |
    //   1       1
    //   |       |
    //   3 --1-- 2
    //
    // Cost matrix (symmetric):
    //   0  1  2  1
    //   1  0  1  2
    //   2  1  0  1
    //   1  2  1  0
    //
    // One optimal tour: 0 -> 1 -> 2 -> 3 -> 0
    // cost = 1 + 1 + 1 + 1 = 4
    int n3 = 4;
    int raw3[4][4] = {
        { 0, 1, 2, 1 },
        { 1, 0, 1, 2 },
        { 2, 1, 0, 1 },
        { 1, 2, 1, 0 }
    };
    int *cost3[4];
    for (int i = 0; i < n3; i++) {
        cost3[i] = raw3[i];
    }
    test_tsp("Test 3: 4-node square", cost3, n3, 4);

    return 0;
}
