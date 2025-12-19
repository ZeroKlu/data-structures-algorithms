package code_samples.section8.problems.problem8_1;

/*
 * Problem 8.1 – Climbing Stairs
 *
 * Given a staircase with n steps, you can climb either 1 step or 2 steps
 * at a time. This class computes how many distinct ways there are to reach
 * the top.
 *
 * This is a classic dynamic programming problem whose solution follows
 * the Fibonacci sequence.
 */
public class problem8_1 {

    /*
     * Computes the number of distinct ways to climb a staircase with n steps.
     *
     * Recurrence relation:
     *   ways(n) = ways(n - 1) + ways(n - 2)
     *
     * Instead of storing all intermediate results in an array, this
     * implementation optimizes space by keeping only the previous two values.
     */
    int climbStairs(int n) {

        /*
         * Base cases:
         * n = 0 → 0 ways (by definition in this implementation)
         * n = 1 → 1 way  (single step)
         * n = 2 → 2 ways (1+1 or 2)
         */
        if (n <= 2) return n;

        /*
         * prev2 represents the number of ways to reach step (i - 2).
         * Initially corresponds to ways(1) = 1.
         */
        int prev2 = 1;

        /*
         * prev1 represents the number of ways to reach step (i - 1).
         * Initially corresponds to ways(2) = 2.
         */
        int prev1 = 2;

        /*
         * Iterate from step 3 up to step n, computing the number of ways
         * to reach each step using the recurrence relation.
         */
        for (int i = 3; i <= n; i++) {

            /*
             * The number of ways to reach the current step i
             * is the sum of the ways to reach the previous two steps.
             */
            int cur = prev1 + prev2;

            /*
             * Shift the window forward:
             * - prev2 becomes the old prev1
             * - prev1 becomes the newly computed value
             */
            prev2 = prev1;
            prev1 = cur;
        }

        /*
         * After the loop completes, prev1 holds the number of ways
         * to reach step n.
         */
        return prev1;
    }

    // ============================
    // Tests
    // ============================
    public static void main(String[] args) {

        // Create an instance of the solver
        problem8_1 solver = new problem8_1();

        /*
         * Test inputs representing different staircase sizes.
         * Includes small values, edge cases, and larger values
         * to demonstrate correctness and scalability.
         */
        int[] tests    = { 0, 1, 2, 3, 4, 5, 6, 10, 20 };

        /*
         * Expected results for each test input.
         * These values follow the Fibonacci-style progression
         * defined by the problem’s recurrence relation.
         */
        int[] expected = { 0, 1, 2, 3, 5, 8, 13, 89, 10946 };

        System.out.println("=== Test: climbStairs ===\n");

        /*
         * Run each test case, compute the result,
         * and compare it with the expected value.
         */
        for (int i = 0; i < tests.length; i++) {
            int n = tests[i];

            // Compute the number of ways to climb n steps
            int result = solver.climbStairs(n);

            // Output the result alongside the expected value
            System.out.println(
                "climbStairs(" + n + ") = " + result +
                " (expected " + expected[i] + ")"
            );
        }
    }
}
