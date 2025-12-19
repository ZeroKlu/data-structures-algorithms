/*
 * Computes the number of distinct ways to climb a staircase with n steps.
 *
 * At each step, you can climb either 1 step or 2 steps.
 * This creates a recurrence relation similar to the Fibonacci sequence:
 *
 *   ways(n) = ways(n - 1) + ways(n - 2)
 *
 * This implementation uses dynamic programming with constant space,
 * keeping only the two previous results instead of an entire array.
 */
function climbStairs(n) {

    /*
     * Base cases:
     * n = 0 → 0 ways (by convention in this implementation)
     * n = 1 → 1 way
     * n = 2 → 2 ways (1+1, 2)
     */
    if (n <= 2) return n;

    /*
     * prev2 represents the number of ways to reach step (i - 2).
     * Initialized to ways(1) = 1.
     */
    let prev2 = 1;

    /*
     * prev1 represents the number of ways to reach step (i - 1).
     * Initialized to ways(2) = 2.
     */
    let prev1 = 2;

    /*
     * Iterate from step 3 up to step n.
     * At each step, compute the total number of ways using the
     * sum of the previous two step counts.
     */
    for (let i = 3; i <= n; i++) {

        /*
         * The number of ways to reach step i is the sum of the
         * ways to reach steps (i - 1) and (i - 2).
         */
        const cur = prev1 + prev2;

        /*
         * Shift the rolling window forward:
         * - prev2 takes the old prev1 value
         * - prev1 takes the newly computed value
         */
        prev2 = prev1;
        prev1 = cur;
    }

    /*
     * After the loop completes, prev1 contains the number of ways
     * to reach the nth step.
     */
    return prev1;
}

// ============================
// Tests (top-level)
// ============================

/*
 * Test inputs covering:
 * - edge cases (0, 1)
 * - small staircases
 * - larger values to demonstrate scalability
 */
const tests    = [0, 1, 2, 3, 4, 5, 6, 10, 20];

/*
 * Expected results for each test input.
 * These follow the Fibonacci-style progression of the problem.
 */
const expected = [0, 1, 2, 3, 5, 8, 13, 89, 10946];

console.log("=== Test: climbStairs ===\n");

/*
 * Execute each test case and print the computed result
 * alongside the expected value for easy verification.
 */
for (let i = 0; i < tests.length; i++) {
    const n = tests[i];

    // Compute the number of ways to climb n steps
    const result = climbStairs(n);

    // Output result and expected value
    console.log(`climbStairs(${n}) = ${result} (expected ${expected[i]})`);
}
