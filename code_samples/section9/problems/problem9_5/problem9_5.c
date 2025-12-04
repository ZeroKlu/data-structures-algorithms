#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static int int_cmp(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int minCost(int n, int *cuts, int cutsSize) {
    int m = cutsSize + 2;
    int *pos = (int *)malloc(m * sizeof(int));
    pos[0] = 0;
    for (int i = 0; i < cutsSize; i++) pos[i + 1] = cuts[i];
    pos[m - 1] = n;
    qsort(pos + 1, cutsSize, sizeof(int), int_cmp);

    int **dp = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        dp[i] = (int *)calloc(m, sizeof(int));
    }

    for (int len = 2; len < m; len++) {
        for (int i = 0; i + len < m; i++) {
            int j = i + len;
            int best = INT_MAX;
            for (int k = i + 1; k < j; k++) {
                int cand = dp[i][k] + dp[k][j] + (pos[j] - pos[i]);
                if (cand < best) best = cand;
            }
            dp[i][j] = (best == INT_MAX ? 0 : best);
        }
    }

    int ans = dp[0][m - 1];
    for (int i = 0; i < m; i++) free(dp[i]);
    free(dp);
    free(pos);
    return ans;
}

void test(const char *name, int n, int *cuts, int cutsSize, int expected) {
    int result = minCost(n, cuts, cutsSize);
    printf("%s\n", name);
    printf("n = %d, cuts = {", n);
    for (int i = 0; i < cutsSize; i++) {
        printf("%d%s", cuts[i], (i + 1 == cutsSize ? "" : ","));
    }
    printf("}\n");
    printf("minCost = %d (expected %d)\n\n", result, expected);
}

int main(void) {
    printf("=== Testing minCost (Cutting Sticks DP) ===\n\n");

    // ----------------------------------------------
    // Test 1: Classic example
    // n = 7, cuts = {1, 3, 4, 5}
    //
    // Expected answer: 16
    // (matches LeetCode 1547 example)
    // ----------------------------------------------
    int cuts1[] = {1, 3, 4, 5};
    test("Test 1: Classic LC example", 7, cuts1, 4, 16);

    // ----------------------------------------------
    // Test 2: Two cuts only
    // n = 9, cuts = {5, 6}
    //
    // Optimal: cut 5 (cost 9), then cut 6 (cost 4) => 13
    // ----------------------------------------------
    int cuts2[] = {5, 6};
    test("Test 2: Two cuts", 9, cuts2, 2, 13);

    // ----------------------------------------------
    // Test 3: Single cut
    // n = 10, cuts = {4}
    //
    // Only one cut exists: cost = 10
    // ----------------------------------------------
    int cuts3[] = {4};
    test("Test 3: Single cut", 10, cuts3, 1, 10);

    // ----------------------------------------------
    // Test 4: Cuts evenly spaced
    // n = 10, cuts = {2, 4, 6, 8}
    //
    // Known optimal cost: 34
    // ----------------------------------------------
    int cuts4[] = {2, 4, 6, 8};
    test("Test 4: Even spacing", 10, cuts4, 4, 24);

    // ----------------------------------------------
    // Test 5: No cuts
    // n = 100, cuts = {}
    //
    // Cost = 0
    // ----------------------------------------------
    int *cuts5 = NULL;
    test("Test 5: No cuts", 100, cuts5, 0, 0);

    return 0;
}
