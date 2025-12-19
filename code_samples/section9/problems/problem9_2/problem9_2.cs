/**
 * KnapSack
 * --------
 * Solves the classic 0/1 Knapsack problem using a space-optimized
 * one-dimensional dynamic programming approach.
 *
 * Problem summary:
 *   - You are given a knapsack with capacity W.
 *   - Each item has a weight[i] and a value[i].
 *   - Each item may be taken at most once (0/1 constraint).
 *   - Goal: maximize total value without exceeding capacity W.
 *
 * DP formulation:
 *   dp[w] = maximum value achievable with capacity w
 *           using items processed so far.
 *
 * Initialization:
 *   dp[w] = 0 for all w, because with no items selected,
 *   the maximum value is 0 regardless of capacity.
 *
 * Transition:
 *   For each item (wt, val):
 *     for w from W down to wt:
 *       dp[w] = max(dp[w], dp[w - wt] + val)
 *
 * Why iterate w backwards?
 *   - Iterating from W down to wt ensures each item is used at most once.
 *   - Iterating forward would allow reusing the same item multiple times,
 *     which would turn this into the *unbounded* knapsack problem.
 *
 * Complexity:
 *   Time:  O(n * W), where n = number of items
 *   Space: O(W)
 *
 * @param W       Knapsack capacity
 * @param weight  Array of item weights
 * @param value   Array of item values
 * @return        Maximum total value achievable with capacity W
 */
static int KnapSack(int W, int[] weight, int[] value) {
    // dp[w] stores the best value achievable with capacity w
    int[] dp = new int[W + 1];

    // Process each item exactly once
    for (int i = 0; i < weight.Length; i++) {
        int wt = weight[i];
        int val = value[i];

        // Traverse capacities backwards to enforce the 0/1 constraint
        for (int w = W; w >= wt; w--) {
            // Either skip the item (dp[w]) or take it
            // (dp[w - wt] + val), whichever is better
            dp[w] = Math.Max(dp[w], dp[w - wt] + val);
        }
    }

    // dp[W] contains the optimal value for full capacity
    return dp[W];
}

/**
 * Test
 * ----
 * Helper method that runs a single knapsack test case and prints:
 *   - Test name
 *   - Knapsack capacity
 *   - Weights array
 *   - Values array
 *   - Computed result and expected result
 *
 * @param name      Descriptive test name
 * @param W         Knapsack capacity
 * @param weight    Item weights
 * @param value     Item values
 * @param expected  Expected optimal knapsack value
 */
static void Test(string name, int W, int[] weight, int[] value, int expected)
{
    // Run the knapsack solver
    int result = KnapSack(W, weight, value);

    // Print formatted test output
    Console.WriteLine(name);
    Console.WriteLine($"Capacity = {W}");
    Console.WriteLine($"Weights: [{string.Join(",", weight)}]");
    Console.WriteLine($"Values:  [{string.Join(",", value)}]");
    Console.WriteLine($"KnapSack = {result} (expected {expected})\n");
}

// ===========================
// Test Harness
// ===========================

// Header for all tests
Console.WriteLine("=== Testing KnapSack ===\n");

// Test 1: Classic textbook example
// Optimal choice: weights 20 + 30 => value 100 + 120 = 220
Test("Test 1: Classic example",
    50,
    [10, 20, 30],
    [60, 100, 120],
    220);

// Test 2: Small weights, capacity fits all items
// Total weight = 1 + 2 + 3 = 6, total value = 65
Test("Test 2: Small weights",
    6,
    [1, 2, 3],
    [10, 15, 40],
    65);

// Test 3: Capacity too small to take any item
// All weights exceed capacity => result = 0
Test("Test 3: Too small capacity",
    4,
    [5, 6, 7],
    [10, 20, 30],
    0);

// Test 4: Zero capacity edge case
// Capacity = 0 => cannot take anything
Test("Test 4: Zero capacity",
    0,
    [2, 3, 4],
    [20, 30, 40],
    0);

// Test 5: Larger mixed set
// Multiple optimal combinations yield total value = 100
Test("Test 5: Larger mixed set",
    10,
    [2, 3, 5, 7],
    [20, 30, 50, 70],
    100);
