static bool CanPartition(int[] nums) {
    int total = 0;
    foreach (var x in nums) total += x;
    if ((total & 1) == 1) return false;
    int target = total / 2;

    bool[] dp = new bool[target + 1];
    dp[0] = true;
    foreach (var x in nums) {
        for (int s = target; s >= x; s--) {
            if (dp[s - x]) dp[s] = true;
        }
    }
    return dp[target];
}

static void Test(string name, int[] arr, bool expected)
{
    bool result = CanPartition(arr);
    Console.WriteLine(name);
    Console.WriteLine($"Input: [{string.Join(",", arr)}]");
    Console.WriteLine($"CanPartition = {result} (expected {expected})\n");
}

Console.WriteLine("=== Testing CanPartition ===\n");

Test("Test 1: Classic example", [1, 5, 11, 5], true);
Test("Test 2: Cannot partition", [1, 2, 3, 5], false);
Test("Test 3: Single element", [7], false);
Test("Test 4: Two equal numbers", [4, 4], true);
Test("Test 5: Larger mixed set", [2, 2, 3, 5], false);
Test("Test 6: Multiple valid subsets", [2, 2, 1, 1], true);
