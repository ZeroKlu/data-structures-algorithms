static int[] TwoSumIndices(int[] nums, int target) {
    var map = new Dictionary<int, int>(); // value -> index
    for (int i = 0; i < nums.Length; i++) {
        int need = target - nums[i];
        if (map.TryGetValue(need, out int j)) {
            return [j, i];
        }
        map[nums[i]] = i;
    }
    return Array.Empty<int>();
}

// ============================
// Top-level tests
// ============================

int[][] tests = new int[][]
{
    [2, 7, 11, 15],  // expected [0,1]
    [3, 2, 4],       // expected [1,2]
    [1, 2, 3, 4]     // expected []
};

int[] targets = [9, 6, 10];

int[][] expected =
{
    [0, 1],
    [1, 2],
    []
};

Console.WriteLine("=== Test: TwoSumIndices ===\n");

for (int t = 0; t < tests.Length; t++)
{
    int[] nums = tests[t];
    int target = targets[t];

    int[] result = TwoSumIndices(nums, target);
    string resultStr = "[" + string.Join(", ", result) + "]";
    string expectedStr = "[" + string.Join(", ", expected[t]) + "]";

    Console.WriteLine($"Input nums = [{string.Join(", ", nums)}], target = {target}");
    Console.WriteLine($"Result: {resultStr} (expected {expectedStr})\n");
}
