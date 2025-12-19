/**
 * knapSack
 * --------
 * Solves the classic 0/1 Knapsack problem using a space-optimized
 * one-dimensional dynamic programming approach.
 *
 * Problem definition:
 *   - You are given a knapsack with capacity W.
 *   - There are items, each with a weight and a value.
 *   - Each item can be taken at most once (0/1 constraint).
 *   - Goal: maximize total value without exceeding capacity W.
 *
 * DP formulation:
 *   dp[w] = maximum value achievable with capacity w
 *           using items processed so far.
 *
 * Initialization:
 *   dp[w] = 0 for all w, because with no items selected the value is 0.
 *
 * Transition:
 *   For each item (wt, val):
 *     for w from W down to wt:
 *       dp[w] = max(dp[w], dp[w - wt] + val)
 *
 * Why iterate w backwards?
 *   - Iterating from W down to wt ensures each item is used at most once.
 *   - Iterating forward would allow the same item to be reused multiple times,
 *     turning this into the unbounded knapsack problem.
 *
 * Complexity:
 *   Time:  O(n * W), where n = number of items
 *   Space: O(W)
 *
 * @param {number} W       Knapsack capacity
 * @param {number[]} weight  Array of item weights
 * @param {number[]} value   Array of item values
 * @returns {number} Maximum total value achievable with capacity W
 */
function knapSack(W, weight, value) {
    // dp[w] stores the best value achievable with capacity w
    const dp = new Array(W + 1).fill(0);

    // Process each item exactly once
    for (let i = 0; i < weight.length; i++) {
        const wt = weight[i];
        const val = value[i];

        // Traverse capacities backwards to enforce the 0/1 constraint
        for (let w = W; w >= wt; w--) {
            // Either skip the item (dp[w]) or take it (dp[w - wt] + val),
            // whichever yields a larger total value
            dp[w] = Math.max(dp[w], dp[w - wt] + val);
        }
    }

    // dp[W] contains the optimal value for full capacity
    return dp[W];
}

// ===========================
// Test Harness
// ===========================

/**
 * Runs a single knapsack test case and prints:
 *   - test name
 *   - knapsack capacity
 *   - weights array
 *   - values array
 *   - computed result and expected result
 *
 * @param {string} name      Descriptive test name
 * @param {number} W         Knapsack capacity
 * @param {number[]} weight  Item weights
 * @param {number[]} value   Item values
 * @param {number} expected  Expected optimal knapsack value
 */
function test(name, W, weight, value, expected) {
    // Execute the knapsack solver
    const result = knapSack(W, weight, value);

    // Print formatted test output
    console.log(name);
    console.log(`Capacity = ${W}`);
    console.log(`Weights: [${weight.join(",")}]`);
    console.log(`Values:  [${value.join(",")}]`);
    console.log(`knapSack = ${result} (expected ${expected})\n`);
}

// Header for all tests
console.log("=== Testing knapSack ===\n");

// Test 1: Classic textbook example
// Optimal choice: weights 20 + 30 => value 100 + 120 = 220
test("Test 1: Classic example",
     50,
     [10, 20, 30],
     [60, 100, 120],
     220);

// Test 2: Small weights where all items fit
// Total weight = 1 + 2 + 3 = 6, total value = 65
test("Test 2: Small weights",
     6,
     [1, 2, 3],
     [10, 15, 40],
     65);

// Test 3: Capacity too small to take any item
// All weights exceed capacity => result = 0
test("Test 3: Too small capacity",
     4,
     [5, 6, 7],
     [10, 20, 30],
     0);

// Test 4: Zero capacity edge case
// Capacity = 0 => cannot take anything
test("Test 4: Zero capacity",
     0,
     [2, 3, 4],
     [20, 30, 40],
     0);

// Test 5: Larger mixed set
// Multiple optimal combinations yield total value = 100
test("Test 5: Larger mixed set",
     10,
     [2, 3, 5, 7],
     [20, 30, 50, 70],
     100);
