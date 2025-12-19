#include <stdio.h>

/*
 * climbStairs
 *
 * Problem:
 *   Given n steps, you can climb either 1 step or 2 steps at a time.
 *   Return the number of distinct ways to reach the top.
 *
 * Interpretation used in THIS implementation:
 *   - n = 0 → 0 ways   (explicit design choice for this version)
 *   - n = 1 → 1 way
 *   - n = 2 → 2 ways
 *
 * Recurrence:
 *   ways(n) = ways(n - 1) + ways(n - 2)
 *
 * This is equivalent to the Fibonacci sequence shifted by one index:
 *   ways(1) = 1
 *   ways(2) = 2
 *
 * Dynamic Programming Optimization:
 *   Instead of storing an entire dp[] array, we keep only:
 *     prev2 = ways(n - 2)
 *     prev1 = ways(n - 1)
 *
 * Complexity:
 *   Time:  O(n)
 *   Space: O(1)
 *
 * Notes:
 *   - The choice of returning 0 for n = 0 is a convention.
 *     Some definitions return 1 for n = 0 (one way: do nothing).
 *     As long as the definition is consistent, both are valid.
 */
int climbStairs(int n) {
    /*
     * Handle base cases directly.
     *
     * n <= 2:
     *   n = 0 → 0 ways (by design choice here)
     *   n = 1 → 1 way
     *   n = 2 → 2 ways
     */
    if (n <= 2) return n;

    /*
     * prev2 represents ways(1)
     * prev1 represents ways(2)
     *
     * Initial values:
     *   ways(1) = 1
     *   ways(2) = 2
     */
    int prev2 = 1; // dp[1]
    int prev1 = 2; // dp[2]

    /*
     * Iteratively compute ways(3) through ways(n).
     *
     * At each iteration:
     *   cur = ways(i) = ways(i-1) + ways(i-2)
     *   then shift the window forward:
     *     prev2 <- prev1
     *     prev1 <- cur
     */
    for (int i = 3; i <= n; i++) {
        int cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    /*
     * After the loop, prev1 holds ways(n).
     */
    return prev1;
}

/*
 * main
 *
 * Test harness for climbStairs().
 * Runs several test cases and prints the result alongside the expected value.
 */
int main(void) {
    printf("=== Test: climbStairs ===\n\n");

    /*
     * Each test case contains:
     *   n        → number of steps
     *   expected → expected number of ways (based on our chosen definition)
     */
    struct {
        int n;
        int expected;
    } tests[] = {
        {0, 0},  // By convention in this implementation
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

    /*
     * Execute each test and print the result.
     */
    for (int i = 0; i < numTests; i++) {
        int n = tests[i].n;
        int expected = tests[i].expected;
        int result = climbStairs(n);

        printf("climbStairs(%d) = %d (expected %d)\n",
               n, result, expected);
    }

    return 0;
}
