static int MinCost(int n, int[] cuts) {
    int m = cuts.Length + 2;
    int[] pos = new int[m];
    pos[0] = 0;
    pos[m - 1] = n;
    Array.Copy(cuts, 0, pos, 1, cuts.Length);
    Array.Sort(pos, 1, m - 1);

    int[,] dp = new int[m, m];

    for (int len = 2; len < m; len++) {
        for (int i = 0; i + len < m; i++) {
            int j = i + len;
            int best = int.MaxValue;
            for (int k = i + 1; k < j; k++) {
                int cand = dp[i, k] + dp[k, j] + (pos[j] - pos[i]);
                if (cand < best) best = cand;
            }
            dp[i, j] = best == int.MaxValue ? 0 : best;
        }
    }
    return dp[0, m - 1];
}

static void Test(string name, int n, int[] cuts, int expected)
{
    int result = MinCost(n, cuts);
    Console.WriteLine(name);
    Console.WriteLine($"n = {n}, cuts = [{string.Join(",", cuts)}]");
    Console.WriteLine($"MinCost = {result} (expected {expected})\n");
}

Console.WriteLine("=== Testing MinCost (Cutting Sticks DP) ===\n");

// Test 1: Classic example
Test("Test 1: Classic example",
     7, [1, 3, 4, 5], 16);

// Test 2: Two cuts
Test("Test 2: Two cuts",
     9, [5, 6], 13);

// Test 3: Single cut
Test("Test 3: Single cut",
     10, [4], 10);

// Test 4: Even spacing (Correct expected = 24)
Test("Test 4: Even spacing",
     10, [2, 4, 6, 8], 24);

// Test 5: No cuts
Test("Test 5: No cuts",
     100, Array.Empty<int>(), 0);
