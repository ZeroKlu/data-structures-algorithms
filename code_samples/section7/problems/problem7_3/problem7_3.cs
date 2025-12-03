static int LongestConsecutive(int[] nums) {
    var set = new HashSet<int>(nums);
    int best = 0;

    foreach (var x in set) {
        if (!set.Contains(x - 1)) {
            int cur = x;
            int length = 1;
            while (set.Contains(cur + 1)) {
                cur++;
                length++;
            }
            if (length > best) best = length;
        }
    }
    return best;
}

// ============================
// Top-level tests
// ============================

int[][] tests = new int[][]
{
    [100, 4, 200, 1, 3, 2],         // expected 4
    [0, 3, 7, 2, 5, 8, 4, 6, 0, 1], // expected 9
    [1, 2, 0, 1],                   // expected 3
    [],                             // expected 0
    [-2, -1, 0, 2, 3, 4, 5]         // expected 4
};

int[] expected = [4, 9, 3, 0, 4];

Console.WriteLine("=== Test: LongestConsecutive ===\n");

for (int i = 0; i < tests.Length; i++)
{
    int[] nums = tests[i];
    int result = LongestConsecutive(nums);

    Console.WriteLine($"Input: [{string.Join(", ", nums)}]");
    Console.WriteLine($"Longest consecutive length: {result} (expected {expected[i]})\n");
}
