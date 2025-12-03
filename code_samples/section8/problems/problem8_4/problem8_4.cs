static int UniquePaths(int m, int n)
{
    int[,] dp = new int[m, n];
    for (int i = 0; i < m; i++) {
        dp[i, 0] = 1;
    }
    for (int j = 0; j < n; j++) {
        dp[0, j] = 1;
    }
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[i, j] = dp[i - 1, j] + dp[i, j - 1];
        }
    }
    return dp[m - 1, n - 1];
}

// ===============================
// Tests (top-level)
// ===============================

var tests = new (int m, int n, int expected)[]
{
    (1, 1, 1),
    (1, 5, 1),
    (3, 3, 6),
    (3, 7, 28),
    (3, 2, 3),
    (7, 3, 28),
    (10, 10, 48620)
};

Console.WriteLine("=== Test: UniquePaths ===\n");

foreach (var t in tests)
{
    int result = UniquePaths(t.m, t.n);
    Console.WriteLine($"UniquePaths({t.m}, {t.n}) = {result} (expected {t.expected})");
}
