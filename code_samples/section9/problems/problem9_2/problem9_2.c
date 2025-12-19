#include <stdio.h>
#include <stdlib.h>

/**
 * knapSack
 * --------
 * Solves the 0/1 Knapsack problem using a 1D dynamic programming array.
 *
 * Problem statement (0/1 knapsack):
 *   - You have n items, each with a weight[i] and value[i].
 *   - You have a knapsack with capacity W.
 *   - Each item can be taken at most once (0 or 1 times).
 *   - Goal: maximize total value while keeping total weight <= W.
 *
 * DP idea (1D optimization):
 *   Let dp[w] = maximum value achievable with capacity exactly (or up to) w
 *               using items considered so far.
 *
 * Transition for each item (wt, val):
 *   If we take this item, we move from capacity (w - wt) to w:
 *     dp[w] = max(dp[w], dp[w - wt] + val)
 *
 * Important detail (why we loop w from W down to wt):
 *   - Looping backwards ensures each item is used at most once.
 *   - If we looped forward (wt..W), we would allow reusing the same item
 *     multiple times in the same iteration, turning it into an *unbounded*
 *     knapsack variant.
 *
 * Complexity:
 *   Time:  O(n * W)
 *   Space: O(W)
 *
 * Parameters:
 *   W      - knapsack capacity
 *   weight - array of item weights (length n)
 *   value  - array of item values (length n)
 *   n      - number of items
 *
 * Returns:
 *   The maximum total value achievable with capacity W.
 */
int knapSack(int W, int *weight, int *value, int n) {
    // Allocate dp[0..W], initialized to 0.
    // dp[w] starts at 0 because with no items, best value is 0 for any capacity.
    int *dp = (int *)calloc(W + 1, sizeof(int));

    // Process items one by one.
    for (int i = 0; i < n; i++) {
        int wt = weight[i];
        int val = value[i];

        // Traverse capacity backwards to enforce 0/1 usage (each item used at most once).
        for (int w = W; w >= wt; w--) {
            // Candidate value if we include this item:
            // best value at remaining capacity (w - wt) plus this item's value.
            int cand = dp[w - wt] + val;

            // If taking the item improves the best known value at capacity w, update dp[w].
            if (cand > dp[w]) {
                dp[w] = cand;
            }
        }
    }

    // The answer for full capacity W is stored in dp[W].
    int ans = dp[W];

    // Free heap memory to avoid leaks.
    free(dp);

    return ans;
}

/**
 * test
 * ----
 * Helper function that runs a single knapsack test case and prints:
 *   - test name
 *   - capacity
 *   - weights array
 *   - values array
 *   - computed result and expected result
 *
 * Parameters:
 *   name     - test label
 *   W        - knapsack capacity
 *   weight   - weights array
 *   value    - values array
 *   n        - number of items
 *   expected - expected optimal knapsack value
 */
void test(const char *name, int W, int *weight, int *value, int n, int expected) {
    // Run the algorithm under test.
    int result = knapSack(W, weight, value, n);

    // Print a readable test report.
    printf("%s\n", name);
    printf("Capacity = %d\n", W);

    // Print weights in a compact {a,b,c} format.
    printf("Weights: {");
    for (int i = 0; i < n; i++) printf("%d%s", weight[i], i + 1 == n ? "" : ",");
    printf("}\n");

    // Print values in a compact {a,b,c} format.
    printf("Values:  {");
    for (int i = 0; i < n; i++) printf("%d%s", value[i], i + 1 == n ? "" : ",");
    printf("}\n");

    // Print result vs expected.
    printf("knapSack = %d (expected %d)\n\n", result, expected);
}

/**
 * main
 * ----
 * Test harness for the 0/1 knapsack implementation.
 *
 * Covers:
 *   - a classic textbook example
 *   - small items with capacity that fits multiple items
 *   - capacity too small to take any item
 *   - zero capacity
 *   - a case with multiple possible combinations (sanity check on maximizing)
 */
int main(void) {
    printf("=== Testing knapSack ===\n\n");

    // Test 1: Basic textbook example
    // Items: (10,60), (20,100), (30,120), capacity=50
    // Optimal: take weights 20+30 => value 100+120=220
    int wt1[] = {10, 20, 30};
    int val1[] = {60, 100, 120};
    test("Test 1: Classic example", 50, wt1, val1, 3, 220);

    // Test 2: Small items, capacity fits all items
    // Items: (1,10), (2,15), (3,40), capacity=6
    // Total weight = 1+2+3=6, total value = 10+15+40=65 (take all)
    int wt2[] = {1, 2, 3};
    int val2[] = {10, 15, 40};
    test("Test 2: Small weights", 6, wt2, val2, 3, 65);

    // Test 3: Capacity too small to take anything
    // All weights exceed capacity=4 => result must be 0
    int wt3[] = {5, 6, 7};
    int val3[] = {10, 20, 30};
    test("Test 3: Capacity too small", 4, wt3, val3, 3, 0);

    // Test 4: Zero capacity
    // Capacity=0 means nothing can be taken => result 0 regardless of items.
    int wt4[] = {2, 3, 4};
    int val4[] = {20, 30, 40};
    test("Test 4: Zero capacity", 0, wt4, val4, 3, 0);

    // Test 5: Larger mixed set
    // Items: (2,20), (3,30), (5,50), (7,70), capacity=10
    // One optimal: (3,30)+(7,70) => weight 10, value 100
    // Another: (2,20)+(3,30)+(5,50) => weight 10, value 100
    int wt5[] = {2, 3, 5, 7};
    int val5[] = {20, 30, 50, 70};
    test("Test 5: Larger mixed set", 10, wt5, val5, 4, 100);

    return 0;
}
