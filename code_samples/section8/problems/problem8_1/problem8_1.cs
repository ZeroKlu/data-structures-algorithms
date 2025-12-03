static int ClimbStairs(int n) {
    if (n <= 2) return n;
    int prev2 = 1;
    int prev1 = 2;
    for (int i = 3; i <= n; i++) {
        int cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

// ============================
// Tests (top-level)
// ============================

int[] tests    = [0, 1, 2, 3, 4, 5, 6, 10, 20];
int[] expected = [0, 1, 2, 3, 5, 8, 13, 89, 10946];

Console.WriteLine("=== Test: ClimbStairs ===\n");

for (int i = 0; i < tests.Length; i++)
{
    int n = tests[i];
    int result = ClimbStairs(n);

    Console.WriteLine($"ClimbStairs({n}) = {result} (expected {expected[i]})");
}
