/*
 * Section: Dynamic Programming (DP) examples + tests (top-level statements)
 *
 * This file includes:
 *   1) FibDp       - Fibonacci using O(1) space DP (iterative)
 *   2) ClimbStairs - classic "1 or 2 steps" count using O(1) space DP
 *   3) KnapSack01  - 0/1 knapsack using 1D DP array
 *   4) LcsLength   - Longest Common Subsequence length using 2D DP table
 *
 * Then, it runs a set of table-driven tests for each function.
 */

/* ============================================================
   Fibonacci (DP with O(1) space)
   ============================================================ */

/**
 * FibDp
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
 *   n - index of Fibonacci number to compute (assumes n >= 0)
 *
 * Returns:
 *   F(n) as a `long`.
 *
 * Approach:
 *   Instead of storing dp[0..n], we only keep the previous two values:
 *     prev2 = F(i-2)
 *     prev1 = F(i-1)
 *   Then compute curr = prev1 + prev2 and slide the window forward.
 *
 * Complexity:
 *   Time:  O(n)
 *   Space: O(1)
 *
 * Notes / caveats:
 *   - Overflow: Fibonacci grows quickly. On a typical .NET runtime, `long` is
 *     a signed 64-bit integer. Overflow occurs past F(92).
 *   - For negative n: this code returns n due to `if (n <= 1) return n;`.
 *     In production code, you'd usually validate and throw an exception.
 */
static long FibDp(int n) {
    // Base cases: F(0)=0, F(1)=1
    if (n <= 1) return n;

    long prev2 = 0; // F(0)
    long prev1 = 1; // F(1)

    // Build iteratively from i=2 up to n
    for (int i = 2; i <= n; i++) {
        long curr = prev1 + prev2; // F(i) = F(i-1) + F(i-2)
        prev2 = prev1;             // shift: prev2 <- F(i-1)
        prev1 = curr;              // shift: prev1 <- F(i)
    }

    // prev1 now holds F(n)
    return prev1;
}

/* ============================================================
   Climbing Stairs (DP with O(1) space)
   ============================================================ */

/**
 * ClimbStairs
 *
 * Purpose:
 *   Count the number of distinct ways to reach step n when you can climb
 *   either 1 or 2 steps at a time.
 *
 * Recurrence:
 *   ways(0) = 1  (one way: do nothing)
 *   ways(1) = 1
 *   ways(n) = ways(n-1) + ways(n-2) for n >= 2
 *
 * Parameters:
 *   n - number of steps (assumes n >= 0)
 *
 * Returns:
 *   ways(n) as a `long`.
 *
 * Relationship:
 *   ways(n) equals Fibonacci shifted by one:
 *     ways(n) = F(n+1) with F(0)=0,F(1)=1.
 *
 * Complexity:
 *   Time:  O(n)
 *   Space: O(1)
 *
 * Notes:
 *   - This implementation treats n=0 as 1 (one way to stay put).
 *   - Like Fibonacci, values grow quickly and can overflow for large n.
 */
static long ClimbStairs(int n) {
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
 * KnapSack01
 *
 * Purpose:
 *   Solve the 0/1 knapsack problem:
 *     - You have `n` items.
 *     - item i has weight w[i] and value v[i].
 *     - Knapsack capacity is W.
 *     - Each item can be used at most once (0/1).
 *
 * Parameters:
 *   n - number of items (should match w.Length and v.Length)
 *   W - capacity (maximum total weight)
 *   w - weights array
 *   v - values array
 *
 * Returns:
 *   Maximum achievable value without exceeding capacity W.
 *
 * DP definition:
 *   dp[c] = best value achievable with capacity c after processing some items.
 *
 * Transition:
 *   For each item i:
 *     for c from W down to w[i]:
 *         dp[c] = max(dp[c], dp[c - w[i]] + v[i])
 *
 * Why iterate capacity downward:
 *   - Ensures item i is used at most once.
 *   - If you iterate upward, dp[c - w[i]] might already include item i,
 *     turning the solution into an unbounded knapsack.
 *
 * Complexity:
 *   Time:  O(n * W)
 *   Space: O(W)
 *
 * Notes / caveats:
 *   - Assumes W >= 0 and weights are non-negative.
 *   - This returns dp[W], representing the best value with capacity up to W.
 */
static int KnapSack01(int n, int W, int[] w, int[] v) {
    // dp[c] starts at 0 meaning: with capacity c, best value is 0 using no items
    var dp = new int[W + 1];

    // Process each item once
    for (int i = 0; i < n; i++) {
        // Loop backward to enforce 0/1 usage of item i
        for (int c = W; c >= w[i]; c--) {
            int withItem = dp[c - w[i]] + v[i]; // value if we take item i
            if (withItem > dp[c]) {
                dp[c] = withItem;               // keep the best option
            }
        }
    }

    // Best value achievable with capacity W
    return dp[W];
}

/* ============================================================
   Longest Common Subsequence (LCS) length (2D DP)
   ============================================================ */

/**
 * LcsLength
 *
 * Purpose:
 *   Compute the length of the Longest Common Subsequence (LCS) between two
 *   strings s and t.
 *
 * Subsequence reminder:
 *   Characters must appear in order but do not need to be contiguous.
 *
 * DP definition:
 *   dp[i,j] = LCS length for prefixes:
 *     s[0..i-1] and t[0..j-1]
 *
 * Base:
 *   dp[0,*] = 0 and dp[*,0] = 0 (empty string vs anything)
 *
 * Transition:
 *   If s[i-1] == t[j-1]:
 *       dp[i,j] = dp[i-1,j-1] + 1
 *   Else:
 *       dp[i,j] = max(dp[i-1,j], dp[i,j-1])
 *
 * Parameters:
 *   s - first string
 *   t - second string
 *
 * Returns:
 *   Length of the LCS between s and t.
 *
 * Complexity:
 *   Time:  O(n * m)
 *   Space: O(n * m)
 *
 * Notes:
 *   - This uses a full 2D array for clarity.
 *   - Space can be optimized to O(min(n,m)) with a rolling 1D/2-row approach.
 */
static int LcsLength(string s, string t) {
    int n = s.Length;
    int m = t.Length;

    // dp has (n+1) x (m+1) cells; dp[0,*] and dp[*,0] are 0 by default
    var dp = new int[n + 1, m + 1];

    // Fill dp table for non-empty prefixes
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s[i - 1] == t[j - 1]) {
                // Matching chars extend LCS by 1
                dp[i, j] = dp[i - 1, j - 1] + 1;
            } else {
                // Best LCS comes from skipping one char in s or in t
                dp[i, j] = Math.Max(dp[i - 1, j], dp[i, j - 1]);
            }
        }
    }

    // Full strings correspond to dp[n,m]
    return dp[n, m];
}

/* ============================================================
   Tests (Top-Level)
   ============================================================ */

/*
 * The code below is "top-level statements" style (C# 9+).
 * Collection expressions like [1,2,3] are C# 12+ syntax.
 */

// ---- FibDp tests ----

// Inputs for Fibonacci test cases
int[] tests = [0, 1, 2, 3, 4, 5, 10, 20, 30, 50];

// Expected Fibonacci outputs for the corresponding inputs above
long[] expected = [0, 1, 1, 2, 3, 5, 55, 6765, 832040, 12586269025L];

Console.WriteLine("=== Test: FibDp ===\n");

// Table-driven test loop: compute FibDp(n) and compare to expected[i]
for (int i = 0; i < tests.Length; i++) {
    int n = tests[i];
    long result = FibDp(n);
    Console.WriteLine($"FibDp({n}) = {result} (expected {expected[i]})");
}

// ---- ClimbStairs tests ----
Console.WriteLine("\n=== Test: ClimbStairs ===\n");

// Inputs for climb-stairs test cases
int[] climbTests = [0, 1, 2, 3, 4, 5, 10];

// Expected outputs for climbStairs
long[] climbExpected = [1, 1, 2, 3, 5, 8, 89];

// Table-driven test loop for ClimbStairs
for (int i = 0; i < climbTests.Length; i++) {
    int n = climbTests[i];
    long result = ClimbStairs(n);
    Console.WriteLine($"ClimbStairs({n}) = {result} (expected {climbExpected[i]})");
}

// ---- Knapsack tests ----
Console.WriteLine("\n=== Test: KnapSack01 ===\n");

{
    // Example items:
    // weights: 2,3,4,5
    // values:  3,4,5,6
    int[] w = [2, 3, 4, 5];
    int[] v = [3, 4, 5, 6];
    int n = w.Length;

    // Capacity 5: best is weights 2+3 => values 3+4 = 7
    Console.WriteLine($"KnapSack01(W=5) = {KnapSack01(n, 5, w, v)} (expected 7)");

    // Capacity 7: best is weights 2+5 => values 3+6 = 9
    Console.WriteLine($"KnapSack01(W=7) = {KnapSack01(n, 7, w, v)} (expected 9)");

    // Capacity 3: best is weight 3 => value 4
    Console.WriteLine($"KnapSack01(W=3) = {KnapSack01(n, 3, w, v)} (expected 4)");
}

// ---- LCS tests ----
Console.WriteLine("\n=== Test: LcsLength ===\n");

/*
 * Each LCS test case includes:
 *   - s: first string
 *   - t: second string
 *   - expected: expected LCS length
 */
(string s, string t, int expected)[] lcsTests = [
    ("abcde", "ace", 3),
    ("abc", "abc", 3),
    ("abc", "def", 0),
    ("", "abc", 0),
    ("AGGTAB", "GXTXAYB", 4)
];

// Run each LCS test case
foreach (var test in lcsTests) {
    int result = LcsLength(test.s, test.t);
    Console.WriteLine(
        $"LcsLength(\"{test.s}\", \"{test.t}\") = {result} (expected {test.expected})"
    );
}
