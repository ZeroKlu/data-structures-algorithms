static int SubarraySum(int[] nums, int k) {
    var freq = new Dictionary<int, int>();
    freq[0] = 1;
    int prefix = 0;
    int count = 0;

    foreach (var x in nums) {
        prefix += x;
        int need = prefix - k;
        if (freq.TryGetValue(need, out int c)) {
            count += c;
        }
        freq[prefix] = freq.TryGetValue(prefix, out int cur) ? cur + 1 : 1;
    }
    return count;
}

// ============================
// Top-level tests
// ============================

int[][] tests = new int[][]
{
    [1, 1, 1],                      // expected 2
    [1, 2, 3],                      // expected 2: [1,2], [3]
    [3, 4, 7, 2, -3, 1, 4, 2],      // expected 4
    []                              // expected 0
};

int[] kVals = [2, 3, 7, 0];
int[] expected = [2, 2, 4, 0];

Console.WriteLine("=== Test: SubarraySum ===\n");

for (int i = 0; i < tests.Length; i++)
{
    int[] nums = tests[i];
    int k = kVals[i];

    Console.WriteLine($"Input: [{string.Join(", ", nums)}], k = {k}");
    int result = SubarraySum(nums, k);
    Console.WriteLine($"Result: {result} (expected {expected[i]})\n");
}
