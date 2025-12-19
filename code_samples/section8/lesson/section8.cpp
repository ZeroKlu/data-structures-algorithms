#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

/* ============================================================
   Fibonacci (DP with O(1) space)
   ============================================================ */

/**
 * fib_dp
 *
 * Purpose:
 *   Computes the nth Fibonacci number using an iterative dynamic-programming
 *   approach with O(1) extra space.
 *
 * Fibonacci definition:
 *   F(0) = 0
 *   F(1) = 1
 *   F(n) = F(n-1) + F(n-2)  for n >= 2
 *
 * Parameters:
 *   n - index of Fibonacci number to compute (assumes n >= 0)
 *
 * Returns:
 *   F(n) as a 64-bit signed integer (long long).
 *
 * Approach:
 *   - Keep only the previous two Fibonacci values:
 *       prev2 = F(i-2)
 *       prev1 = F(i-1)
 *     Compute current = prev1 + prev2, then shift the window.
 *
 * Complexity:
 *   - Time:  O(n)
 *   - Space: O(1)
 *
 * Notes:
 *   - Overflow: Fibonacci grows quickly; long long overflows beyond F(92).
 *   - For negative n, this function returns n due to the n <= 1 check;
 *     callers should ensure n is non-negative.
 */
long long fib_dp(int n) {
    // Base cases: F(0)=0, F(1)=1
    if (n <= 1) return n;

    long long prev2 = 0; // F(0)
    long long prev1 = 1; // F(1)

    // Iteratively compute F(2) .. F(n)
    for (int i = 2; i <= n; i++) {
        long long curr = prev1 + prev2; // F(i)
        prev2 = prev1;                  // shift (F(i-2) <- F(i-1))
        prev1 = curr;                   // shift (F(i-1) <- F(i))
    }

    return prev1; // F(n)
}

/* ============================================================
   Climbing Stairs (DP with O(1) space)
   ============================================================ */

/**
 * climbStairs
 *
 * Purpose:
 *   Computes the number of distinct ways to reach step n if you can climb
 *   either 1 or 2 steps at a time.
 *
 * Standard recurrence:
 *   ways(0) = 1  (one way: do nothing / already there)
 *   ways(1) = 1  ([1])
 *   ways(n) = ways(n-1) + ways(n-2)  for n >= 2
 *
 * Parameters:
 *   n - number of steps (assumes n >= 0)
 *
 * Returns:
 *   ways(n) as a long long.
 *
 * Relationship:
 *   ways(n) corresponds to Fibonacci shifted by one index (ways(n) = F(n+1))
 *   under the F(0)=0,F(1)=1 convention.
 *
 * Complexity:
 *   - Time:  O(n)
 *   - Space: O(1)
 *
 * Notes:
 *   - This implementation returns 1 for n=0 and n=1 (consistent with the above).
 *   - Overflow is possible for large n (same growth as Fibonacci).
 */
long long climbStairs(int n) {
    // Base cases: ways(0)=1, ways(1)=1
    if (n <= 1) return 1;

    long long prev2 = 1; // ways(0)
    long long prev1 = 1; // ways(1)

    // Iteratively compute ways(2) .. ways(n)
    for (int i = 2; i <= n; i++) {
        long long curr = prev1 + prev2; // ways(i)
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
 *   Solves the 0/1 knapsack problem:
 *     - n items
 *     - item i has weight w[i] and value v[i]
 *     - capacity W
 *     - each item can be used at most once (0/1)
 *
 * Parameters:
 *   n - number of items (should equal w.size() and v.size())
 *   W - maximum allowed weight (capacity)
 *   w - vector of weights
 *   v - vector of values
 *
 * Returns:
 *   The maximum total value achievable without exceeding capacity W.
 *
 * DP definition:
 *   dp[c] = best value achievable with capacity up to c, considering items processed so far.
 *
 * Transition:
 *   For each item i:
 *     for c from W down to w[i]:
 *       dp[c] = max(dp[c], dp[c - w[i]] + v[i])
 *
 * Why iterate capacity downward:
 *   - Downward iteration ensures each item is only used once.
 *   - If we iterated upward, dp[c - w[i]] might already include item i,
 *     effectively turning the problem into an unbounded knapsack.
 *
 * Complexity:
 *   - Time:  O(n * W)
 *   - Space: O(W)
 *
 * Notes:
 *   - Assumes weights are non-negative and W >= 0.
 *   - If w[i] > W, the inner loop does nothing for that item, which is fine.
 */
int knapSack01(int n, int W, const vector<int>& w, const vector<int>& v) {
    // dp[c] = best value possible with capacity c
    vector<int> dp(W + 1, 0);

    // Process each item once
    for (int i = 0; i < n; i++) {
        // Go backward to prevent reusing item i multiple times
        for (int c = W; c >= w[i]; c--) {
            dp[c] = max(dp[c], dp[c - w[i]] + v[i]);
        }
    }

    // Best value for full capacity W (also represents "up to W" in this setup)
    return dp[W];
}

/* ============================================================
   Longest Common Subsequence (LCS) Length (2D DP)
   ============================================================ */

/**
 * lcsLength
 *
 * Purpose:
 *   Computes the length of the Longest Common Subsequence (LCS) between
 *   strings s and t.
 *
 * Subsequence reminder:
 *   - Characters must appear in order, but not necessarily contiguously.
 *
 * Parameters:
 *   s - first string
 *   t - second string
 *
 * Returns:
 *   Length of the LCS between s and t.
 *
 * DP definition:
 *   dp[i][j] = LCS length of prefixes:
 *     s[0..i-1] and t[0..j-1]
 *
 * Base:
 *   dp[0][*] = 0 and dp[*][0] = 0 (empty string vs anything)
 *
 * Transition:
 *   If s[i-1] == t[j-1]:
 *     dp[i][j] = dp[i-1][j-1] + 1
 *   Else:
 *     dp[i][j] = max(dp[i-1][j], dp[i][j-1])
 *
 * Complexity:
 *   - Time:  O(n * m)
 *   - Space: O(n * m)
 *
 * Notes:
 *   - This uses a full 2D DP table for clarity.
 *     You can optimize to O(min(n,m)) space with a rolling array.
 */
int lcsLength(const string& s, const string& t) {
    int n = (int)s.size();
    int m = (int)t.size();

    // dp table with (n+1) x (m+1), initialized to 0
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    // Fill table for i>=1, j>=1 using recurrence
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s[i - 1] == t[j - 1]) {
                // Matching characters extend the LCS by 1
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // Otherwise, best comes from skipping a char in s or in t
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Full strings correspond to dp[n][m]
    return dp[n][m];
}

/* ============================================================
   Test Harness
   ============================================================ */

int main() {

    /* ============================
       Test fib_dp
       ============================ */
    cout << "=== Test: fib_dp ===\n\n";

    /**
     * FibTest
     *
     * Holds one Fibonacci test case:
     *   n        - input index
     *   expected - expected Fibonacci value
     */
    struct FibTest { int n; long long expected; };

    // Table-driven tests for Fibonacci
    vector<FibTest> fibTests = {
        {0, 0},
        {1, 1},
        {2, 1},
        {3, 2},
        {4, 3},
        {5, 5},
        {10, 55},
        {20, 6765},
        {30, 832040},
        {50, 12586269025LL}
    };

    // Execute Fibonacci tests
    for (auto &t : fibTests) {
        long long r = fib_dp(t.n);
        cout << "fib_dp(" << t.n << ") = " << r
             << " (expected " << t.expected << ")\n";
    }


    /* ============================
       Test climbStairs
       ============================ */
    cout << "\n=== Test: climbStairs ===\n\n";

    /**
     * ClimbTest
     *
     * Holds one climbing-stairs test case:
     *   n        - number of steps
     *   expected - expected number of ways
     */
    struct ClimbTest { int n; long long expected; };

    // Table-driven tests for climbStairs
    vector<ClimbTest> climbTests = {
        {0, 1},
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 5},
        {5, 8},
        {10, 89}
    };

    // Execute climbStairs tests
    for (auto &t : climbTests) {
        long long r = climbStairs(t.n);
        cout << "climbStairs(" << t.n << ") = " << r
             << " (expected " << t.expected << ")\n";
    }


    /* ============================
       Test knapSack01
       ============================ */
    cout << "\n=== Test: knapSack01 ===\n\n";

    {
        // Example item weights and values
        vector<int> w = {2, 3, 4, 5};
        vector<int> v = {3, 4, 5, 6};

        // Number of items (assumes w and v have same size)
        int n = (int)w.size();

        // Capacity 5: best is take weights 2 and 3 -> value 3+4 = 7
        int W1 = 5, expected1 = 7;
        int r1 = knapSack01(n, W1, w, v);
        cout << "knapSack01(W=5) = " << r1 << " (expected 7)\n";

        // Capacity 7: best is take weights 2 and 5 -> value 3+6 = 9
        int W2 = 7, expected2 = 9;
        int r2 = knapSack01(n, W2, w, v);
        cout << "knapSack01(W=7) = " << r2 << " (expected 9)\n";

        // Capacity 3: best is take weight 3 -> value 4
        int W3 = 3, expected3 = 4;
        int r3 = knapSack01(n, W3, w, v);
        cout << "knapSack01(W=3) = " << r3 << " (expected 4)\n";

        // (expected1/2/3 are declared for readability; the prints use literals)
        (void)expected1; (void)expected2; (void)expected3; // suppress unused warnings if any
    }


    /* ============================
       Test lcsLength
       ============================ */
    cout << "\n=== Test: lcsLength ===\n\n";

    /**
     * LcsTest
     *
     * Holds one LCS test case:
     *   s        - first string
     *   t        - second string
     *   expected - expected LCS length
     */
    struct LcsTest { string s, t; int expected; };

    // Table-driven tests for LCS length
    vector<LcsTest> lcsTests = {
        {"abcde", "ace", 3},         // "ace"
        {"abc", "abc", 3},           // "abc"
        {"abc", "def", 0},           // no common subsequence
        {"", "abc", 0},              // empty vs non-empty
        {"AGGTAB", "GXTXAYB", 4}     // "GTAB"
    };

    // Execute LCS tests
    for (auto &t : lcsTests) {
        int r = lcsLength(t.s, t.t);
        cout << "lcsLength(\"" << t.s << "\", \"" << t.t
             << "\") = " << r << " (expected " << t.expected << ")\n";
    }

    return 0;
}
