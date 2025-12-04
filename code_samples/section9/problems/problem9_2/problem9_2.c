#include <stdio.h>
#include <stdlib.h>

int knapSack(int W, int *weight, int *value, int n) {
    int *dp = (int *)calloc(W + 1, sizeof(int));
    for (int i = 0; i < n; i++) {
        int wt = weight[i];
        int val = value[i];
        for (int w = W; w >= wt; w--) {
            int cand = dp[w - wt] + val;
            if (cand > dp[w]) {
                dp[w] = cand;
            }
        }
    }
    int ans = dp[W];
    free(dp);
    return ans;
}

void test(const char *name, int W, int *weight, int *value, int n, int expected) {
    int result = knapSack(W, weight, value, n);
    printf("%s\n", name);
    printf("Capacity = %d\n", W);
    printf("Weights: {");
    for (int i = 0; i < n; i++) printf("%d%s", weight[i], i + 1 == n ? "" : ",");
    printf("}\nValues:  {");
    for (int i = 0; i < n; i++) printf("%d%s", value[i], i + 1 == n ? "" : ",");
    printf("}\n");

    printf("knapSack = %d (expected %d)\n\n", result, expected);
}

int main(void) {
    printf("=== Testing knapSack ===\n\n");

    // Test 1: Basic textbook example
    int wt1[] = {10, 20, 30};
    int val1[] = {60, 100, 120};
    test("Test 1: Classic example", 50, wt1, val1, 3, 220);

    // Test 2: Small items, tight capacity
    int wt2[] = {1, 2, 3};
    int val2[] = {10, 15, 40};
    test("Test 2: Small weights", 6, wt2, val2, 3, 65);

    // Test 3: Capacity too small to take anything
    int wt3[] = {5, 6, 7};
    int val3[] = {10, 20, 30};
    test("Test 3: Capacity too small", 4, wt3, val3, 3, 0);

    // Test 4: Zero capacity
    int wt4[] = {2, 3, 4};
    int val4[] = {20, 30, 40};
    test("Test 4: Zero capacity", 0, wt4, val4, 3, 0);

    // Test 5: Multiple optimal combinations
    int wt5[] = {2, 3, 5, 7};
    int val5[] = {20, 30, 50, 70};
    test("Test 5: Larger mixed set", 10, wt5, val5, 4, 100);

    return 0;
}
