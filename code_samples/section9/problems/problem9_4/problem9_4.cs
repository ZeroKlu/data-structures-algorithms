static int Tsp(int[][] cost) {
    int n = cost.Length;
    int maxMask = 1 << n;
    int INF = int.MaxValue / 2;
    int[,] dp = new int[maxMask, n];
    for (int mask = 0; mask < maxMask; mask++) {
        for (int i = 0; i < n; i++) {
            dp[mask, i] = INF;
        }
    }
    dp[1, 0] = 0;

    for (int mask = 1; mask < maxMask; mask++) {
        for (int i = 0; i < n; i++) {
            if ((mask & (1 << i)) == 0) continue;
            int curCost = dp[mask, i];
            if (curCost == INF) continue;
            for (int j = 0; j < n; j++) {
                if ((mask & (1 << j)) != 0) continue;
                int nextMask = mask | (1 << j);
                int cand = curCost + cost[i][j];
                if (cand < dp[nextMask, j]) {
                    dp[nextMask, j] = cand;
                }
            }
        }
    }

    int fullMask = maxMask - 1;
    int ans = INF;
    for (int i = 0; i < n; i++) {
        int cand = dp[fullMask, i] + cost[i][0];
        if (cand < ans) ans = cand;
    }
    return ans;
}

static void Test(string name, int[][] cost, int expected)
{
    int result = Tsp(cost);
    Console.WriteLine(name);
    Console.WriteLine("Cost matrix:");
    foreach (var row in cost)
        Console.WriteLine("  [" + string.Join(",", row) + "]");
    Console.WriteLine($"Tsp = {result} (expected {expected})\n");
}

Console.WriteLine("=== Testing Tsp (Bitmask DP) ===\n");

// -----------------------------
// Test 1: Classic 4-city example
// -----------------------------
Test(
    "Test 1: Classic 4-city example",
    [
        [0,10,15,20],
        [10,0,35,25],
        [15,35,0,30],
        [20,25,30,0],
    ],
    80
);

// -----------------------------
// Test 2: 3-city triangle
// -----------------------------
Test(
    "Test 2: 3-city triangle",
    [
        [0,1,4],
        [1,0,2],
        [4,2,0],
    ],
    7
);

// -----------------------------
// Test 3: 4-city square loop
// -----------------------------
Test(
    "Test 3: 4-city square",
    [
        [0,1,2,1],
        [1,0,1,2],
        [2,1,0,1],
        [1,2,1,0],
    ],
    4
);
