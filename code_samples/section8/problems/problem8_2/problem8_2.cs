static int Rob(int[] nums) {
    if (nums.Length == 0) return 0;
    if (nums.Length == 1) return nums[0];
    int prev2 = nums[0];
    int prev1 = Math.Max(nums[0], nums[1]);
    for (int i = 2; i < nums.Length; i++) {
        int take = prev2 + nums[i];
        int skip = prev1;
        int cur = Math.Max(take, skip);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

// ============================
// Tests (top-level)
// ============================

int[][] tests = new int[][]
{
    Array.Empty<int>(),
    [5],
    [1, 2],
    [2, 7, 9, 3, 1],
    [1, 2, 3, 1],
    [2, 1, 1, 2],
    [10, 5, 20, 15, 30],
    [1, 3, 1, 3, 100]
};

int[] expected =
[
    0,
    5,
    2,
    12,
    4,
    4,
    60,
    103
];

Console.WriteLine("=== Test: Rob (House Robber) ===\n");

for (int i = 0; i < tests.Length; i++)
{
    int result = Rob(tests[i]);
    Console.WriteLine($"Test {i + 1}: Rob([{string.Join(", ", tests[i])}]) = {result} (expected {expected[i]})");
}
