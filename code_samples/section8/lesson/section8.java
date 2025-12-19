package code_samples.section8.lesson;

public class section8 {

    /* ============================================================
       Fibonacci (DP with O(1) space)
       ============================================================ */

    /**
     * fibDp
     *
     * Purpose:
     *   Compute the nth Fibonacci number using an iterative dynamic-programming
     *   approach that uses constant extra space.
     *
     * Fibonacci definition:
     *   F(0) = 0
     *   F(1) = 1
     *   F(n) = F(n-1) + F(n-2) for n >= 2
     *
     * Parameters:
     *   n - Fibonacci index (assumes n >= 0)
     *
     * Returns:
     *   F(n) as a 64-bit signed integer (long).
     *
     * Approach:
     *   Instead of storing a full dp array, keep only the last two values:
     *     prev2 = F(i-2)
     *     prev1 = F(i-1)
     *   Then compute curr and shift forward.
     *
     * Complexity:
     *   Time:  O(n)
     *   Space: O(1)
     *
     * Notes:
     *   - Overflow: Fibonacci grows quickly; long will overflow for large n
     *     (beyond F(92) for signed 64-bit).
     *   - Negative n: current code returns n for n <= 1. In production,
     *     consider validating and throwing an IllegalArgumentException.
     */
    long fibDp(int n) {
        // Base cases: F(0)=0, F(1)=1
        if (n <= 1) return n;

        long prev2 = 0; // F(0)
        long prev1 = 1; // F(1)

        // Build from i=2 up to n
        for (int i = 2; i <= n; i++) {
            long curr = prev1 + prev2; // F(i) = F(i-1) + F(i-2)
            prev2 = prev1;             // shift window forward
            prev1 = curr;
        }

        // prev1 now holds F(n)
        return prev1;
    }

    /* ============================================================
       Climbing Stairs (DP with O(1) space)
       ============================================================ */

    /**
     * climbStairs
     *
     * Purpose:
     *   Count the number of distinct ways to reach step n when you can climb
     *   1 step or 2 steps at a time.
     *
     * Recurrence:
     *   ways(0) = 1  (one way: do nothing / already at the top)
     *   ways(1) = 1
     *   ways(n) = ways(n-1) + ways(n-2) for n >= 2
     *
     * Parameters:
     *   n - number of steps (assumes n >= 0)
     *
     * Returns:
     *   ways(n) as a long.
     *
     * Relationship:
     *   ways(n) corresponds to Fibonacci shifted by one index (ways(n) = F(n+1))
     *   under the F(0)=0,F(1)=1 Fibonacci definition.
     *
     * Complexity:
     *   Time:  O(n)
     *   Space: O(1)
     *
     * Notes:
     *   - This implementation returns 1 for n=0 and n=1, matching the
     *     combinatorial interpretation.
     *   - Overflow is possible for large n (same growth as Fibonacci).
     */
    long climbStairs(int n) {
        // Base cases: ways(0)=1, ways(1)=1
        if (n <= 1) return 1;

        long prev2 = 1; // ways(0)
        long prev1 = 1; // ways(1)

        // Compute ways(2) .. ways(n)
        for (int i = 2; i <= n; i++) {
            long curr = prev1 + prev2; // ways(i) = ways(i-1) + ways(i-2)
            prev2 = prev1;
            prev1 = curr;
        }

        return prev1; // ways(n)
    }

    /* ============================================================
       0/1 Knapsack (1D DP)
       ============================================================ */

    /**
     * knapSack01
     *
     * Purpose:
     *   Solve the 0/1 knapsack problem:
     *     - n items, each with weight w[i] and value v[i]
     *     - capacity W
     *     - each item can be used at most once (0/1)
     *
     * Parameters:
     *   n - number of items (should equal w.length and v.length)
     *   W - maximum capacity (weight limit)
     *   w - weights array
     *   v - values array
     *
     * Returns:
     *   Maximum achievable value without exceeding capacity W.
     *
     * DP definition:
     *   dp[c] = best value achievable with capacity c after processing some items.
     *
     * Transition (0/1 constraint):
     *   For each item i:
     *     for c from W down to w[i]:
     *       dp[c] = max(dp[c], dp[c - w[i]] + v[i])
     *
     * Why the backward loop:
     *   - Backward iteration prevents using the same item more than once.
     *   - Forward iteration would allow dp[c - w[i]] to already include item i,
     *     turning this into an unbounded knapsack.
     *
     * Complexity:
     *   Time:  O(n * W)
     *   Space: O(W)
     *
     * Notes:
     *   - Assumes W >= 0 and weights are non-negative.
     *   - Items heavier than W are naturally skipped by the inner loop condition.
     */
    int knapSack01(int n, int W, int[] w, int[] v) {
        // dp[c] = best value achievable with capacity c
        int[] dp = new int[W + 1];

        // Process items one by one
        for (int i = 0; i < n; i++) {
            // Traverse capacities backward to enforce 0/1 usage
            for (int c = W; c >= w[i]; c--) {
                int withItem = dp[c - w[i]] + v[i]; // value if we include item i
                if (withItem > dp[c]) {
                    dp[c] = withItem;               // keep the better option
                }
            }
        }

        return dp[W];
    }

    /* ============================================================
       Longest Common Subsequence (LCS) Length (2D DP)
       ============================================================ */

    /**
     * lcsLength
     *
     * Purpose:
     *   Compute the length of the Longest Common Subsequence (LCS) between
     *   strings s and t.
     *
     * Subsequence reminder:
     *   Characters must appear in order, but do NOT need to be contiguous.
     *
     * DP definition:
     *   dp[i][j] = LCS length between:
     *     s[0..i-1] (first i characters) and
     *     t[0..j-1] (first j characters)
     *
     * Base:
     *   dp[0][*] = 0 and dp[*][0] = 0 (empty prefix vs anything)
     *
     * Transition:
     *   If s[i-1] == t[j-1]:
     *     dp[i][j] = dp[i-1][j-1] + 1
     *   Else:
     *     dp[i][j] = max(dp[i-1][j], dp[i][j-1])
     *
     * Parameters:
     *   s - first string
     *   t - second string
     *
     * Returns:
     *   The LCS length.
     *
     * Complexity:
     *   Time:  O(n * m)
     *   Space: O(n * m)
     *
     * Notes:
     *   - Space can be optimized to O(min(n,m)) using a rolling array,
     *     but the 2D table is the most straightforward for learning.
     */
    int lcsLength(String s, String t) {
        int n = s.length();
        int m = t.length();

        // dp table initialized to 0 by default
        int[][] dp = new int[n + 1][m + 1];

        // Fill dp for non-empty prefixes only (start at 1)
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (s.charAt(i - 1) == t.charAt(j - 1)) {
                    // Matching characters extend the LCS by 1
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    // Otherwise, best LCS comes from skipping one char
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        // Full strings correspond to dp[n][m]
        return dp[n][m];
    }

    // ============================
    // Test Harness
    // ============================

    /**
     * main
     *
     * Runs table-driven tests for:
     *   - fibDp
     *   - climbStairs
     *   - knapSack01
     *   - lcsLength
     *
     * Prints actual results alongside expected values.
     */
    public static void main(String[] args) {

        section8 solver = new section8();

        /* ----------------------------
           fibDp tests
           ---------------------------- */

        // Inputs and expected outputs for Fibonacci
        int[] fibTests      = {0, 1, 2, 3, 4, 5, 10, 20, 30, 50};
        long[] fibExpected  = {0, 1, 1, 2, 3, 5, 55, 6765, 832040, 12586269025L};

        System.out.println("=== Test: fibDp ===\n");

        // Each fibTests[i] should match fibExpected[i]
        for (int i = 0; i < fibTests.length; i++) {
            int n = fibTests[i];
            long result = solver.fibDp(n);

            System.out.println("fibDp(" + n + ") = " + result +
                               " (expected " + fibExpected[i] + ")");
        }

        /* ----------------------------
           climbStairs tests
           ---------------------------- */

        System.out.println("\n=== Test: climbStairs ===\n");

        int[] climbTests       = {0, 1, 2, 3, 4, 5, 10};
        long[] climbExpected   = {1, 1, 2, 3, 5, 8, 89};

        // Each climbTests[i] should match climbExpected[i]
        for (int i = 0; i < climbTests.length; i++) {
            int n = climbTests[i];
            long result = solver.climbStairs(n);

            System.out.println("climbStairs(" + n + ") = " + result +
                               " (expected " + climbExpected[i] + ")");
        }

        /* ----------------------------
           knapSack01 tests
           ---------------------------- */

        System.out.println("\n=== Test: knapSack01 ===\n");

        {
            // Example items:
            // weights = [2,3,4,5]
            // values  = [3,4,5,6]
            int[] w = {2, 3, 4, 5};
            int[] v = {3, 4, 5, 6};
            int n = w.length;

            // Capacity 5: choose weights 2+3 => values 3+4 = 7
            int W1 = 5;
            int expected1 = 7;
            int result1 = solver.knapSack01(n, W1, w, v);
            System.out.println("knapSack01(W=" + W1 + ") = " + result1 +
                               " (expected " + expected1 + ")");

            // Capacity 7: choose weights 2+5 => values 3+6 = 9
            int W2 = 7;
            int expected2 = 9;
            int result2 = solver.knapSack01(n, W2, w, v);
            System.out.println("knapSack01(W=" + W2 + ") = " + result2 +
                               " (expected " + expected2 + ")");

            // Capacity 3: choose weight 3 => value 4
            int W3 = 3;
            int expected3 = 4;
            int result3 = solver.knapSack01(n, W3, w, v);
            System.out.println("knapSack01(W=" + W3 + ") = " + result3 +
                               " (expected " + expected3 + ")");
        }

        /* ----------------------------
           lcsLength tests
           ---------------------------- */

        System.out.println("\n=== Test: lcsLength ===\n");

        /**
         * LcsCase
         *
         * Small helper class to bundle one LCS test case:
         *   s        - first string
         *   t        - second string
         *   expected - expected LCS length
         */
        class LcsCase {
            String s, t;
            int expected;

            LcsCase(String s, String t, int expected) {
                this.s = s;
                this.t = t;
                this.expected = expected;
            }
        }

        // Test cases include: partial match, identical strings, no overlap, empty string, classic example
        LcsCase[] lcsTests = {
            new LcsCase("abcde", "ace", 3),        // "ace"
            new LcsCase("abc", "abc", 3),          // "abc"
            new LcsCase("abc", "def", 0),          // none
            new LcsCase("", "abc", 0),             // empty vs non-empty
            new LcsCase("AGGTAB", "GXTXAYB", 4)    // "GTAB"
        };

        // Run each LCS test case
        for (LcsCase c : lcsTests) {
            int result = solver.lcsLength(c.s, c.t);
            System.out.println("lcsLength(\"" + c.s + "\", \"" + c.t +
                               "\") = " + result +
                               " (expected " + c.expected + ")");
        }
    }
}
