#include <iostream>
#include <vector>
using namespace std;

/**
 * knapSack
 * --------
 * Solves the classic 0/1 Knapsack problem using a space-optimized
 * one-dimensional dynamic programming approach.
 *
 * Problem definition:
 *   - You are given a knapsack with capacity W.
 *   - There are n items, each with:
 *       weight[i] = weight of item i
 *       value[i]  = value of item i
 *   - Each item can be taken at most once (0/1 constraint).
 *   - Goal: maximize total value without exceeding capacity W.
 *
 * DP formulation:
 *   dp[w] = maximum value achievable with capacity w
 *           using items processed so far.
 *
 * Initialization:
 *   dp[w] = 0 for all w, because with no items the value is 0.
 *
 * Transition:
 *   For each item (wt, val):
 *     for w from W down to wt:
 *       dp[w] = max(dp[w], dp[w - wt] + val)
 *
 * Important detail:
 *   - The loop over w goes *backwards* (from W down to wt).
 *   - This ensures each item is used at most once.
 *   - Iterating forward would allow reusing the same item multiple times
 *     (which would turn this into an unbounded knapsack).
 *
 * Complexity:
 *   Time:  O(n * W)
 *   Space: O(W)
 *
 * @param W       Knapsack capacity
 * @param weight  Vector of item weights
 * @param value   Vector of item values
 * @return        Maximum total value achievable with capacity W
 */
int knapSack(int W, const vector<int> &weight, const vector<int> &value) {
    // dp[w] stores the best value achievable with capacity w
    vector<int> dp(W + 1, 0);

    // Number of items
    int n = (int)weight.size();

    // Process each item exactly once
    for (int i = 0; i < n; i++) {
        int wt = weight[i];
        int val = value[i];

        // Traverse capacities backwards to enforce 0/1 usage
        for (int w = W; w >= wt; w--) {
            // Either skip the item, or take it if it improves the value
            dp[w] = max(dp[w], dp[w - wt] + val);
        }
    }

    // dp[W] holds the optimal value for full capacity
    return dp[W];
}

/**
 * test
 * ----
 * Helper function to run and print a single knapsack test case.
 *
 * Prints:
 *   - Test name
 *   - Knapsack capacity
 *   - List of item weights
 *   - List of item values
 *   - Computed result and expected result
 *
 * @param name      Descriptive name of the test
 * @param W         Knapsack capacity
 * @param weight    Vector of item weights
 * @param value     Vector of item values
 * @param expected  Expected optimal knapsack value
 */
void test(const string &name, int W, const vector<int> &weight,
          const vector<int> &value, int expected) 
{
    // Run the knapsack solver
    int result = knapSack(W, weight, value);

    // Print formatted test output
    cout << name << "\n";
    cout << "Capacity = " << W << "\n";

    cout << "Weights: {";
    for (size_t i = 0; i < weight.size(); i++) {
        cout << weight[i] << (i + 1 == weight.size() ? "" : ",");
    }
    cout << "}\n";

    cout << "Values:  {";
    for (size_t i = 0; i < value.size(); i++) {
        cout << value[i] << (i + 1 == value.size() ? "" : ",");
    }
    cout << "}\n";

    cout << "knapSack = " << result 
         << " (expected " << expected << ")\n\n";
}

/**
 * main
 * ----
 * Test harness for the 0/1 knapsack implementation.
 *
 * Covers several common scenarios:
 *   - Classic textbook example
 *   - Small items where all fit
 *   - Capacity too small to take any item
 *   - Zero capacity edge case
 *   - Larger mixed set with multiple optimal solutions
 */
int main() {
    cout << "=== Testing knapSack ===\n\n";

    // Test 1: Classic example
    // Items: (10,60), (20,100), (30,120), capacity = 50
    // Optimal solution: take items with weights 20 and 30 => value 220
    test("Test 1: Classic example",
         50,
         {10, 20, 30},
         {60, 100, 120},
         220);

    // Test 2: Small weights
    // All items fit exactly: total value = 10 + 15 + 40 = 65
    test("Test 2: Small weights",
         6,
         {1, 2, 3},
         {10, 15, 40},
         65);

    // Test 3: Capacity too small
    // All items exceed capacity => result = 0
    test("Test 3: Too small capacity",
         4,
         {5, 6, 7},
         {10, 20, 30},
         0);

    // Test 4: Zero capacity
    // Capacity 0 means nothing can be taken
    test("Test 4: Zero capacity",
         0,
         {2, 3, 4},
         {20, 30, 40},
         0);

    // Test 5: Larger mixed set
    // Multiple optimal combinations give value 100
    test("Test 5: Larger mixed set",
         10,
         {2, 3, 5, 7},
         {20, 30, 50, 70},
         100);

    return 0;
}
