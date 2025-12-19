// Computes the number of distinct ways to climb a staircase of n steps,
// where at each move you can climb either 1 or 2 steps.
//
// This is a classic dynamic programming problem.
// The recurrence relation is:
//   ways(n) = ways(n - 1) + ways(n - 2)
//
// We optimize space by only keeping track of the previous two results
// instead of storing a full DP array.
static int ClimbStairs(int n) {

    // Base cases:
    // If there are 0 steps, we define the number of ways as 0
    // If there is 1 step, there is exactly 1 way to climb it
    // If there are 2 steps, there are exactly 2 ways: (1+1) or (2)
    if (n <= 2) return n;

    // prev2 represents the number of ways to reach step (i - 2)
    // Initially corresponds to ways(1) = 1
    int prev2 = 1;

    // prev1 represents the number of ways to reach step (i - 1)
    // Initially corresponds to ways(2) = 2
    int prev1 = 2;

    // Iterate from step 3 up to step n
    for (int i = 3; i <= n; i++) {

        // The number of ways to reach the current step i
        // is the sum of the ways to reach the previous two steps
        int cur = prev1 + prev2;

        // Shift the window forward:
        // prev2 becomes the old prev1 (ways to reach i - 1)
        prev2 = prev1;

        // prev1 becomes the current result (ways to reach i)
        prev1 = cur;
    }

    // At the end of the loop, prev1 holds the number of ways to reach step n
    return prev1;
}

// ============================
// Tests (top-level)
// ============================

// Test inputs representing different numbers of steps
int[] tests    = [0, 1, 2, 3, 4, 5, 6, 10, 20];

// Expected results corresponding to each test input
// These values follow the Fibonacci-style progression
// defined by the climbing-stairs recurrence
int[] expected = [0, 1, 2, 3, 5, 8, 13, 89, 10946];

Console.WriteLine("=== Test: ClimbStairs ===\n");

// Run each test case and compare the result to the expected value
for (int i = 0; i < tests.Length; i++)
{
    int n = tests[i];

    // Compute the number of ways to climb n steps
    int result = ClimbStairs(n);

    // Print the input, computed result, and expected result
    Console.WriteLine($"ClimbStairs({n}) = {result} (expected {expected[i]})");
}
