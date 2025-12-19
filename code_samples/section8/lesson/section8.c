#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ============================================================
   Fibonacci (DP with O(1) space)
   ============================================================ */

/**
 * fib_dp
 *
 * Purpose:
 *   Computes the nth Fibonacci number using an iterative dynamic programming
 *   approach that uses O(1) extra space.
 *
 * Fibonacci definition (common):
 *   F(0) = 0
 *   F(1) = 1
 *   F(n) = F(n-1) + F(n-2) for n >= 2
 *
 * Parameters:
 *   n - index of Fibonacci number to compute (assumes n >= 0)
 *
 * Returns:
 *   F(n) as a long long.
 *
 * Approach:
 *   - Instead of building a full dp[] array, keep only the last two values:
 *       prev2 = F(i-2)
 *       prev1 = F(i-1)
 *     Then compute curr = prev1 + prev2, shift forward, repeat.
 *
 * Complexity:
 *   - Time:  O(n)
 *   - Space: O(1)
 *
 * Notes / caveats:
 *   - Overflow: Fibonacci grows quickly. long long will overflow eventually.
 *     For signed 64-bit, overflow occurs past F(92).
 *   - For negative n, this function returns n (due to the n <= 1 check),
 *     which is probably not desired; callers should enforce n >= 0.
 */
long long fib_dp(int n) {
    // Base cases: F(0) = 0, F(1) = 1
    if (n <= 1) return n;

    long long prev2 = 0; // F(0)
    long long prev1 = 1; // F(1)

    // Build up from i=2 to n
    for (int i = 2; i <= n; i++) {
        long long curr = prev1 + prev2; // F(i) = F(i-1) + F(i-2)
        prev2 = prev1;                  // shift window forward
        prev1 = curr;
    }

    // prev1 now holds F(n)
    return prev1;
}

/* ============================================================
   Climbing Stairs (DP with O(1) space)
   ============================================================ */

/**
 * climb_stairs
 *
 * Purpose:
 *   Computes the number of distinct ways to climb to the top of a staircase
 *   with n steps, when you can climb either 1 or 2 steps at a time.
 *
 * Classic recurrence:
 *   ways(0) = 1  (one way: do nothing / already at the top)
 *   ways(1) = 1  ([1])
 *   ways(n) = ways(n-1) + ways(n-2) for n >= 2
 *
 * Parameters:
 *   n - number of steps (assumes n >= 0)
 *
 * Returns:
 *   ways(n) as a long long.
 *
 * Relation to Fibonacci:
 *   ways(n) corresponds to F(n+1) if you use the F(0)=0,F(1)=1 definition.
 *
 * Complexity:
 *   - Time:  O(n)
 *   - Space: O(1)
 *
 * Notes:
 *   - This implementation defines climb_stairs(0) = 1, which matches the
 *     combinatorial interpretation (one way to stay at step 0).
 *   - Overflow can happen for large n (same growth rate as Fibonacci).
 */
long long climb_stairs(int n) {
    // For n=0 or n=1, there is exactly one way
    if (n <= 1) return 1;

    long long prev2 = 1; // ways(0)
    long long prev1 = 1; // ways(1)

    // Compute ways(i) for i=2..n
    for (int i = 2; i <= n; i++) {
        long long curr = prev1 + prev2; // ways(i) = ways(i-1) + ways(i-2)
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1; // ways(n)
}

/* ============================================================
   0/1 Knapsack (1D DP)
   ============================================================ */

/**
 * knapsack_01
 *
 * Purpose:
 *   Solves the 0/1 knapsack problem:
 *     - You have n items
 *     - Item i has weight w[i] and value v[i]
 *     - You have a maximum capacity W
 *     - You can take each item at most once (0/1)
 *   Return the maximum total value achievable without exceeding capacity.
 *
 * Parameters:
 *   n - number of items
 *   W - knapsack capacity (max allowed weight)
 *   w - array of item weights (length n)
 *   v - array of item values  (length n)
 *
 * Returns:
 *   The maximum value achievable with capacity W.
 *
 * DP idea:
 *   Let dp[c] = best value achievable with capacity exactly c (or up to c).
 *
 * Transition (0/1 constraint):
 *   For each item i:
 *     for c from W down to w[i]:
 *         dp[c] = max(dp[c], dp[c - w[i]] + v[i])
 *
 * Why iterate c downward:
 *   - Downward iteration ensures each item is used at most once.
 *   - If we iterated upward, dp[c - w[i]] might already include item i,
 *     effectively turning it into an unbounded knapsack.
 *
 * Complexity:
 *   - Time:  O(n * W)
 *   - Space: O(W)
 *
 * Notes / caveats:
 *   - If malloc fails, returns 0. That’s a simple fallback; in production code
 *     you may want to signal an error differently.
 *   - Assumes weights are non-negative and W >= 0.
 */
int knapsack_01(int n, int W, const int* w, const int* v) {
    // Allocate a 1D dp array of size W+1 initialized to 0
    int* dp = (int*)malloc((W + 1) * sizeof(int));
    if (!dp) return 0; // allocation failure fallback
    memset(dp, 0, (W + 1) * sizeof(int));

    // Process each item exactly once
    for (int i = 0; i < n; i++) {
        // Traverse capacities backward to enforce 0/1 constraint
        for (int c = W; c >= w[i]; c--) {
            int with_item = dp[c - w[i]] + v[i]; // value if we take item i
            if (with_item > dp[c]) {
                dp[c] = with_item;               // take the better option
            }
        }
    }

    // dp[W] is the best value with capacity up to W
    int result = dp[W];

    // Clean up heap memory
    free(dp);

    return result;
}

/* ============================================================
   Longest Common Subsequence (LCS) Length (2D DP)
   ============================================================ */

/**
 * lcs_length
 *
 * Purpose:
 *   Computes the length of the Longest Common Subsequence (LCS) between
 *   strings s and t.
 *
 * Subsequence vs substring:
 *   - Subsequence does NOT need to be contiguous.
 *   - Example: LCS("abcde", "ace") = "ace" (length 3)
 *
 * Parameters:
 *   s - first null-terminated string
 *   t - second null-terminated string
 *
 * Returns:
 *   Length of the LCS. Returns 0 on allocation failure.
 *
 * DP definition:
 *   dp[i][j] = LCS length between:
 *     s[0..i-1] (first i characters)
 *     t[0..j-1] (first j characters)
 *
 * Base:
 *   dp[0][j] = 0 for all j (empty string vs anything)
 *   dp[i][0] = 0 for all i
 *
 * Transition:
 *   If s[i-1] == t[j-1]:
 *       dp[i][j] = dp[i-1][j-1] + 1
 *   Else:
 *       dp[i][j] = max(dp[i-1][j], dp[i][j-1])
 *
 * Complexity:
 *   - Time:  O(n * m)
 *   - Space: O(n * m)
 *
 * Notes / caveats:
 *   - This uses a full 2D table. You can optimize space to O(min(n,m))
 *     using only two rows (rolling array) if needed.
 *   - Basic cleanup is implemented if allocation of a row fails.
 */
int lcs_length(const char* s, const char* t) {
    int n = (int)strlen(s);
    int m = (int)strlen(t);

    // Allocate an array of (n+1) row pointers
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    if (!dp) return 0;

    // Allocate each row of length (m+1)
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)malloc((m + 1) * sizeof(int));
        if (!dp[i]) {
            // Basic cleanup on failure (free rows already allocated)
            for (int k = 0; k < i; k++) free(dp[k]);
            free(dp);
            return 0;
        }
    }

    // Fill the DP table
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (i == 0 || j == 0) {
                // Empty prefix of either string => LCS length is 0
                dp[i][j] = 0;
            } else if (s[i - 1] == t[j - 1]) {
                // Matching characters extend the LCS by 1
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // Otherwise, best LCS comes from skipping one char from s or t
                int a = dp[i - 1][j];
                int b = dp[i][j - 1];
                dp[i][j] = (a > b) ? a : b;
            }
        }
    }

    // Result is for full strings
    int result = dp[n][m];

    // Free DP table memory
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
}

/* ============================================================
   Test Harness
   ============================================================ */

int main(void) {
    /* ============================
       Test fib_dp
       ============================ */
    printf("=== Test: fib_dp ===\n\n");

    /**
     * fib_tests
     *
     * A table-driven set of test cases for fib_dp.
     * Each entry contains:
     *   - n: input
     *   - expected: expected Fibonacci result
     *
     * Note:
     *   12586269025 fits in 64-bit signed, so fib_dp(50) is safe.
     */
    struct {
        int n;
        long long expected;
    } fib_tests[] = {
        {0, 0},
        {1, 1},
        {2, 1},
        {3, 2},
        {4, 3},
        {5, 5},
        {10, 55},
        {20, 6765},
        {30, 832040},
        {50, 12586269025ULL}
    };

    int numFibTests = (int)(sizeof(fib_tests) / sizeof(fib_tests[0]));

    for (int i = 0; i < numFibTests; i++) {
        int n = fib_tests[i].n;
        long long expected = fib_tests[i].expected;
        long long result = fib_dp(n);

        printf("fib_dp(%d) = %lld (expected %lld)\n",
               n, result, expected);
    }

    /* ============================
       Test climb_stairs
       ============================ */
    printf("\n=== Test: climb_stairs ===\n\n");

    /**
     * climb_tests
     *
     * ways(0) = 1 is included to confirm the chosen base case.
     */
    struct {
        int n;
        long long expected;
    } climb_tests[] = {
        {0, 1},  // ways to stay at step 0
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 5},
        {5, 8},
        {10, 89}
    };

    int numClimbTests = (int)(sizeof(climb_tests) / sizeof(climb_tests[0]));

    for (int i = 0; i < numClimbTests; i++) {
        int n = climb_tests[i].n;
        long long expected = climb_tests[i].expected;
        long long result = climb_stairs(n);

        printf("climb_stairs(%d) = %lld (expected %lld)\n",
               n, result, expected);
    }

    /* ============================
       Test knapsack_01
       ============================ */
    printf("\n=== Test: knapsack_01 ===\n\n");

    {
        // Example items:
        // weights: 2,3,4,5
        // values:  3,4,5,6
        int w[] = {2, 3, 4, 5};
        int v[] = {3, 4, 5, 6};
        int n = (int)(sizeof(w) / sizeof(w[0]));

        // Capacity W=5: best is weight 2+3 => value 3+4=7
        int W1 = 5;
        int expected1 = 7;
        int result1 = knapsack_01(n, W1, w, v);
        printf("knapsack_01(W=%d) = %d (expected %d)\n",
               W1, result1, expected1);

        // Capacity W=7: best is weight 2+5 => value 3+6=9
        int W2 = 7;
        int expected2 = 9;
        int result2 = knapsack_01(n, W2, w, v);
        printf("knapsack_01(W=%d) = %d (expected %d)\n",
               W2, result2, expected2);

        // Capacity W=3: best is take weight 3 => value 4
        int W3 = 3;
        int expected3 = 4;
        int result3 = knapsack_01(n, W3, w, v);
        printf("knapsack_01(W=%d) = %d (expected %d)\n",
               W3, result3, expected3);
    }

    /* ============================
       Test lcs_length
       ============================ */
    printf("\n=== Test: lcs_length ===\n\n");

    /**
     * lcs_tests
     *
     * Includes:
     *   - partial match
     *   - identical strings
     *   - no overlap
     *   - empty string case
     *   - classic textbook example (AGGTAB vs GXTXAYB => "GTAB")
     */
    struct {
        const char* s;
        const char* t;
        int expected;
    } lcs_tests[] = {
        {"abcde", "ace", 3},          // "ace"
        {"abc", "abc", 3},            // "abc"
        {"abc", "def", 0},            // no common subsequence
        {"", "abc", 0},               // empty vs non-empty
        {"AGGTAB", "GXTXAYB", 4}      // "GTAB"
    };

    int numLcsTests = (int)(sizeof(lcs_tests) / sizeof(lcs_tests[0]));

    for (int i = 0; i < numLcsTests; i++) {
        const char* s = lcs_tests[i].s;
        const char* t = lcs_tests[i].t;
        int expected = lcs_tests[i].expected;
        int result = lcs_length(s, t);

        printf("lcs_length(\"%s\", \"%s\") = %d (expected %d)\n",
               s, t, result, expected);
    }

    return 0;
}
