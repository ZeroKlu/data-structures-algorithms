static int CountUniquePairs(int[] nums, int target) {
    var seen = new HashSet<int>();
    var pairs = new HashSet<string>(); // store "a,b" with a <= b

    foreach (var x in nums) {
        int y = target - x;
        if (seen.Contains(y)) {
            int a = Math.Min(x, y);
            int b = Math.Max(x, y);
            pairs.Add($"{a},{b}");
        }
        seen.Add(x);
    }
    return pairs.Count;
}

// ============================
// Tests (top-level code)
// ============================

int[][] testArrays = new int[][]
{
    [1, 1, 2, 2, 3, 3],  // {1,3}, {2,2}
    [2, 7, 11, 15],      // {2,7}
    [3, 3, 3],           // {3,3}
    [1, 2, 3, 4],        // no pairs
    [1, 5, 1, 5]         // {1,5}
};

int[] targets   = [4, 9, 6, 10, 6];
int[] expected  = [2, 1, 1, 0, 1];
string[] notes  = [
    "{1,3} and {2,2}",
    "{2,7}",
    "{3,3}",
    "none",
    "{1,5}"
];

Console.WriteLine("=== Test: CountUniquePairs ===\n");

for (int i = 0; i < testArrays.Length; i++)
{
    int[] nums = testArrays[i];
    int target = targets[i];

    int result = CountUniquePairs(nums, target);

    Console.WriteLine($"Input nums = [{string.Join(", ", nums)}], target = {target}");
    Console.WriteLine($"Result:   {result}");
    Console.WriteLine($"Expected: {expected[i]} ({notes[i]})\n");
}
