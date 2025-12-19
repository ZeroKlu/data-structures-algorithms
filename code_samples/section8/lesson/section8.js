/**
 * Section: Dynamic Programming (DP) examples + tests
 *
 * This file includes 4 classic DP problems:
 *   1) fibDp       - Fibonacci using O(1) space (iterative DP)
 *   2) climbStairs - number of ways to climb n steps (1 or 2 at a time), O(1) space
 *   3) knapSack01  - 0/1 knapsack using 1D DP array
 *   4) lcsLength   - Longest Common Subsequence length using a 2D DP table
 *
 * Then it runs table-driven tests for each function.
 *
 * Note on JavaScript numbers:
 *   - JavaScript's Number type is a floating-point (IEEE-754) value.
 *   - It can represent integers exactly only up to 2^53 - 1 (~9e15).
 *   - Fibonacci(50) = 12,586,269,025 which is still well within exact range.
 *   - The test harness prints Fibonacci results as BigInt for consistent formatting
 *     with the BigInt literal in fibExpected.
 */

/* ============================================================
   Fibonacci (DP with O(1) space)
   ============================================================ */

/**
 * fibDp
 *
 * Purpose:
 *   Compute the nth Fibonacci number using an iterative DP approach with
 *   constant extra space.
 *
 * Fibonacci definition:
 *   F(0) = 0
 *   F(1) = 1
 *   F(n) = F(n-1) + F(n-2) for n >= 2
 *
 * Parameters:
 *   n (number) - index of Fibonacci number to compute (assumes n >= 0)
 *
 * Returns:
 *   (number) - F(n)
 *
 * Approach:
 *   Keep the last two Fibonacci values:
 *     prev2 = F(i-2)
 *     prev1 = F(i-1)
 *   Compute curr = prev1 + prev2, then shift forward.
 *
 * Complexity:
 *   Time:  O(n)
 *   Space: O(1)
 *
 * Notes / caveats:
 *   - If n is negative, this function returns n due to `if (n <= 1) return n;`.
 *     In production code you'd typically validate inputs and throw an error.
 *   - For large n, Fibonacci values exceed the safe integer range for Number.
 *     If you want exact values for large n, implement a BigInt version.
 */
function fibDp(n) {
    // Base cases: F(0)=0, F(1)=1
    if (n <= 1) return n;

    let prev2 = 0; // F(0)
    let prev1 = 1; // F(1)

    // Compute F(2) .. F(n)
    for (let i = 2; i <= n; i++) {
        const curr = prev1 + prev2; // F(i) = F(i-1) + F(i-2)
        prev2 = prev1;              // shift window forward
        prev1 = curr;
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
 *   Count the number of distinct ways to reach step n when you can climb
 *   either 1 step or 2 steps at a time.
 *
 * Recurrence:
 *   ways(0) = 1  (one way: do nothing / already there)
 *   ways(1) = 1
 *   ways(n) = ways(n-1) + ways(n-2) for n >= 2
 *
 * Parameters:
 *   n (number) - number of steps (assumes n >= 0)
 *
 * Returns:
 *   (number) - ways(n)
 *
 * Relationship:
 *   ways(n) equals Fibonacci shifted by 1 (ways(n) = F(n+1)) under the
 *   standard Fibonacci definition.
 *
 * Complexity:
 *   Time:  O(n)
 *   Space: O(1)
 *
 * Notes:
 *   - This implementation returns 1 for n=0 and n=1, matching the standard
 *     combinatorial interpretation.
 *   - For very large n, you can exceed Number's safe integer range.
 */
function climbStairs(n) {
    // Base cases: ways(0)=1, ways(1)=1
    if (n <= 1) return 1;

    let prev2 = 1; // ways(0)
    let prev1 = 1; // ways(1)

    // Compute ways(2) .. ways(n)
    for (let i = 2; i <= n; i++) {
        const curr = prev1 + prev2; // ways(i) = ways(i-1) + ways(i-2)
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
 *     - n items
 *     - item i has weight w[i] and value v[i]
 *     - capacity W
 *     - each item can be used at most once (0/1)
 *
 * Parameters:
 *   n (number)        - number of items (should match w.length and v.length)
 *   W (number)        - capacity (max total weight)
 *   w (number[])      - weights
 *   v (number[])      - values
 *
 * Returns:
 *   (number) - maximum total value achievable within capacity W
 *
 * DP definition:
 *   dp[c] = best value achievable with capacity c after considering items so far
 *
 * Transition (0/1):
 *   For each item i:
 *     for c from W down to w[i]:
 *       dp[c] = max(dp[c], dp[c - w[i]] + v[i])
 *
 * Why the downward loop:
 *   - Prevents reusing item i multiple times.
 *   - Upward iteration would allow dp[c - w[i]] to already include item i,
 *     effectively becoming an unbounded knapsack.
 *
 * Complexity:
 *   Time:  O(n * W)
 *   Space: O(W)
 *
 * Notes:
 *   - Assumes W >= 0 and weights are non-negative.
 */
function knapSack01(n, W, w, v) {
    // dp[c] initially 0 => with capacity c and no items, value is 0
    const dp = new Array(W + 1).fill(0);

    // Process each item once
    for (let i = 0; i < n; i++) {
        // Backward capacity loop to enforce 0/1 constraint
        for (let c = W; c >= w[i]; c--) {
            const withItem = dp[c - w[i]] + v[i]; // take item i
            if (withItem > dp[c]) dp[c] = withItem; // keep best
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
 *   Compute the length of the Longest Common Subsequence (LCS) between strings
 *   s and t.
 *
 * Subsequence reminder:
 *   Characters appear in order but are not required to be contiguous.
 *
 * DP definition:
 *   dp[i][j] = LCS length between prefixes:
 *     s[0..i-1] and t[0..j-1]
 *
 * Base:
 *   dp[0][*] = 0, dp[*][0] = 0 (empty prefix vs anything)
 *
 * Transition:
 *   If s[i-1] === t[j-1]:
 *     dp[i][j] = dp[i-1][j-1] + 1
 *   Else:
 *     dp[i][j] = max(dp[i-1][j], dp[i][j-1])
 *
 * Parameters:
 *   s (string) - first string
 *   t (string) - second string
 *
 * Returns:
 *   (number) - length of the LCS
 *
 * Complexity:
 *   Time:  O(n * m)
 *   Space: O(n * m)
 *
 * Notes:
 *   - This uses a full 2D table for clarity.
 *   - Space can be reduced to O(min(n,m)) with a rolling-array optimization.
 */
function lcsLength(s, t) {
    const n = s.length;
    const m = t.length;

    // dp is (n+1) x (m+1), initialized to 0
    const dp = Array.from({ length: n + 1 }, () =>
        new Array(m + 1).fill(0)
    );

    // Fill dp for non-empty prefixes (start from 1)
    for (let i = 1; i <= n; i++) {
        for (let j = 1; j <= m; j++) {
            if (s[i - 1] === t[j - 1]) {
                // Matching characters extend LCS by 1
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // Best LCS comes from skipping one char in s or t
                dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    return dp[n][m]; // LCS length for full strings
}

/* ============================================================
   Tests
   ============================================================ */

// ---------- fibDp ----------
console.log("=== Test: fibDp ===\n");

// Test inputs
const fibTests = [0, 1, 2, 3, 4, 5, 10, 20, 30, 50];

// Expected outputs. Note: last element is BigInt literal (12586269025n).
// Fibonacci(50) is small enough to be represented exactly as a Number,
// but we print as BigInt to match this array's type.
const fibExpected = [0, 1, 1, 2, 3, 5, 55, 6765, 832040, 12586269025n];

for (let i = 0; i < fibTests.length; i++) {
    // Compute as Number
    const result = fibDp(fibTests[i]);

    // Convert to BigInt solely for consistent printing vs fibExpected.
    // (Safe here because result is a small integer.)
    const resultBig = BigInt(result);

    console.log(`fibDp(${fibTests[i]}) = ${resultBig} (expected ${fibExpected[i]})`);
}

// ---------- climbStairs ----------
console.log("\n=== Test: climbStairs ===\n");

const climbTests = [0, 1, 2, 3, 4, 5, 10];
const climbExpected = [1, 1, 2, 3, 5, 8, 89];

for (let i = 0; i < climbTests.length; i++) {
    const result = climbStairs(climbTests[i]);
    console.log(`climbStairs(${climbTests[i]}) = ${result} (expected ${climbExpected[i]})`);
}

// ---------- knapSack01 ----------
console.log("\n=== Test: knapSack01 ===\n");

{
    // Example items: weights and values
    const w = [2, 3, 4, 5];
    const v = [3, 4, 5, 6];
    const n = w.length;

    // Expected:
    //   W=5 -> take weights 2+3 => value 3+4 = 7
    //   W=7 -> take weights 2+5 => value 3+6 = 9
    //   W=3 -> take weight 3 => value 4
    console.log(`knapSack01(W=5) = ${knapSack01(n, 5, w, v)} (expected 7)`);
    console.log(`knapSack01(W=7) = ${knapSack01(n, 7, w, v)} (expected 9)`);
    console.log(`knapSack01(W=3) = ${knapSack01(n, 3, w, v)} (expected 4)`);
}

// ---------- lcsLength ----------
console.log("\n=== Test: lcsLength ===\n");

// Each entry: [s, t, expectedLcsLength]
const lcsTests = [
    ["abcde", "ace", 3],          // "ace"
    ["abc", "abc", 3],            // "abc"
    ["abc", "def", 0],            // no common subsequence
    ["", "abc", 0],               // empty vs non-empty
    ["AGGTAB", "GXTXAYB", 4]      // "GTAB"
];

for (const [s, t, expected] of lcsTests) {
    const result = lcsLength(s, t);
    console.log(`lcsLength("${s}", "${t}") = ${result} (expected ${expected})`);
}
