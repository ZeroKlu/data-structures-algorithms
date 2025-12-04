static int KnapSack(int W, int[] weight, int[] value) {
    int[] dp = new int[W + 1];
    for (int i = 0; i < weight.Length; i++) {
        int wt = weight[i];
        int val = value[i];
        for (int w = W; w >= wt; w--) {
            dp[w] = Math.Max(dp[w], dp[w - wt] + val);
        }
    }
    return dp[W];
}

static void Test(string name, int W, int[] weight, int[] value, int expected)
{
    int result = KnapSack(W, weight, value);
    Console.WriteLine(name);
    Console.WriteLine($"Capacity = {W}");
    Console.WriteLine($"Weights: [{string.Join(",", weight)}]");
    Console.WriteLine($"Values:  [{string.Join(",", value)}]");
    Console.WriteLine($"KnapSack = {result} (expected {expected})\n");
}

Console.WriteLine("=== Testing KnapSack ===\n");

Test("Test 1: Classic example",
    50,
    [10, 20, 30],
    [60, 100, 120],
    220);

Test("Test 2: Small weights",
    6,
    [1, 2, 3],
    [10, 15, 40],
    65);

Test("Test 3: Too small capacity",
    4,
    [5, 6, 7],
    [10, 20, 30],
    0);

Test("Test 4: Zero capacity",
    0,
    [2, 3, 4],
    [20, 30, 40],
    0);

Test("Test 5: Larger mixed set",
    10,
    [2, 3, 5, 7],
    [20, 30, 50, 70],
    100);
