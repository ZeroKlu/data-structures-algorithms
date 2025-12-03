static long FibDp(int n) {
    if (n <= 1) return n;
    long prev2 = 0; // F(0)
    long prev1 = 1; // F(1)
    for (int i = 2; i <= n; i++) {
        long curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

static long ClimbStairs(int n) {
    if (n <= 1) return 1;
    long prev2 = 1; // ways(0)
    long prev1 = 1; // ways(1)
    for (int i = 2; i <= n; i++) {
        long curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

static int KnapSack01(int n, int W, int[] w, int[] v) {
    var dp = new int[W + 1];
    for (int i = 0; i < n; i++) {
        for (int c = W; c >= w[i]; c--) {
            int withItem = dp[c - w[i]] + v[i];
            if (withItem > dp[c]) {
                dp[c] = withItem;
            }
        }
    }
    return dp[W];
}

static int LcsLength(string s, string t) {
    int n = s.Length;
    int m = t.Length;
    var dp = new int[n + 1, m + 1];
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s[i - 1] == t[j - 1]) {
                dp[i, j] = dp[i - 1, j - 1] + 1;
            } else {
                dp[i, j] = Math.Max(dp[i - 1, j], dp[i, j - 1]);
            }
        }
    }
    return dp[n, m];
}

// ============================
// Tests (Top-Level)
// ============================

// ---- fibDp tests ----
int[] tests    = [0, 1, 2, 3, 4, 5, 10, 20, 30, 50];
long[] expected = [0, 1, 1, 2, 3, 5, 55, 6765, 832040, 12586269025L];

Console.WriteLine("=== Test: FibDp ===\n");

for (int i = 0; i < tests.Length; i++) {
    int n = tests[i];
    long result = FibDp(n);
    Console.WriteLine($"FibDp({n}) = {result} (expected {expected[i]})");
}

// ---- climbStairs tests ----
Console.WriteLine("\n=== Test: ClimbStairs ===\n");

int[] climbTests    = [0, 1, 2, 3, 4, 5, 10];
long[] climbExpected = [1, 1, 2, 3, 5, 8, 89];

for (int i = 0; i < climbTests.Length; i++) {
    int n = climbTests[i];
    long result = ClimbStairs(n);
    Console.WriteLine($"ClimbStairs({n}) = {result} (expected {climbExpected[i]})");
}

// ---- knapsack tests ----
Console.WriteLine("\n=== Test: KnapSack01 ===\n");

{
    int[] w = [2, 3, 4, 5];
    int[] v = [3, 4, 5, 6];
    int n = w.Length;

    Console.WriteLine($"KnapSack01(W=5) = {KnapSack01(n, 5, w, v)} (expected 7)");
    Console.WriteLine($"KnapSack01(W=7) = {KnapSack01(n, 7, w, v)} (expected 9)");
    Console.WriteLine($"KnapSack01(W=3) = {KnapSack01(n, 3, w, v)} (expected 4)");
}

// ---- LCS tests ----
Console.WriteLine("\n=== Test: LcsLength ===\n");

(string s, string t, int expected)[] lcsTests = [
    ("abcde", "ace", 3),
    ("abc", "abc", 3),
    ("abc", "def", 0),
    ("", "abc", 0),
    ("AGGTAB", "GXTXAYB", 4)
];

foreach (var test in lcsTests) {
    int result = LcsLength(test.s, test.t);
    Console.WriteLine($"LcsLength(\"{test.s}\", \"{test.t}\") = {result} (expected {test.expected})");
}
