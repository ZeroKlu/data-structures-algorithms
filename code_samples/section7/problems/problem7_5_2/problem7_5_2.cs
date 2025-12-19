/**
 * CountUniquePairs
 *
 * Purpose:
 *   Counts the number of *unique value pairs* {a, b} such that:
 *       a + b == target
 *
 *   "Unique" here is by VALUE, not by index. That means duplicates in the array
 *   do not create additional pairs if they represent the same values.
 *
 * Example:
 *   nums   = [1, 1, 2, 2, 3, 3]
 *   target = 4
 *   Valid unique value pairs are:
 *     {1,3} and {2,2}  => answer = 2
 *
 * Parameters:
 *   nums   - Input array of integers (may contain duplicates).
 *   target - Target sum.
 *
 * Returns:
 *   Count of distinct value pairs that sum to target.
 *
 * High-level approach:
 *   - Maintain a HashSet<int> called `seen` containing values we've visited so far.
 *   - Maintain a HashSet<string> called `pairs` containing normalized pairs encoded
 *     as strings "a,b" where a <= b.
 *
 *   For each x in nums:
 *     y = target - x
 *     If y is in `seen`, then {x,y} is a valid pair.
 *       Normalize it so order doesn't matter:
 *         a = min(x,y), b = max(x,y)
 *       Store it as "a,b" in `pairs` to deduplicate.
 *     Add x to `seen`.
 *
 * Why this works:
 *   - `seen` ensures we only match x with values that appeared earlier, avoiding
 *     reusing the same element at the same index.
 *   - `pairs` ensures duplicates are counted once per *value pair*.
 *
 * Complexity (expected / average):
 *   - Time:  O(n) average, because HashSet operations are O(1) average.
 *     However, building/hashing strings for pairs adds overhead.
 *   - Space: O(n) for the `seen` set and potentially O(n) for `pairs`.
 *
 * Notes / potential improvements:
 *   - Using strings as pair keys is simple, but not the fastest.
 *     A more efficient approach is to store pairs as:
 *       - ValueTuple<int,int> in a HashSet<(int a, int b)>, or
 *       - a packed 64-bit integer key (similar to your C version).
 *   - This version is easy to read and is perfectly fine for learning/testing.
 */
static int CountUniquePairs(int[] nums, int target) {
    // Set of values we've already processed while scanning left-to-right
    var seen = new HashSet<int>();

    // Set of unique normalized pair keys.
    // We store "a,b" where a <= b so {1,5} and {5,1} become the same string.
    var pairs = new HashSet<string>();

    // Process each number in nums
    foreach (var x in nums) {
        // Complement needed so that x + y == target
        int y = target - x;

        // If y has appeared before, we found a candidate pair
        if (seen.Contains(y)) {
            // Normalize order to ensure uniqueness regardless of encounter order
            int a = Math.Min(x, y);
            int b = Math.Max(x, y);

            // Add normalized pair string; duplicates are ignored by HashSet
            pairs.Add($"{a},{b}");
        }

        // Record x for future complement checks
        seen.Add(x);
    }

    // The number of unique pair strings stored is the answer
    return pairs.Count;
}

// ============================
// Tests (top-level code)
// ============================
//
// These are top-level statements (C# 9+). The array literals like [1,2,3]
// use collection expressions (C# 12+).
//

int[][] testArrays = new int[][]
{
    [1, 1, 2, 2, 3, 3],  // {1,3}, {2,2}
    [2, 7, 11, 15],      // {2,7}
    [3, 3, 3],           // {3,3}
    [1, 2, 3, 4],        // no pairs
    [1, 5, 1, 5]         // {1,5}
};

// Each test has a corresponding target, expected result count, and a short note
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

// Run each test case
for (int i = 0; i < testArrays.Length; i++)
{
    int[] nums = testArrays[i];
    int target = targets[i];

    // Execute function under test
    int result = CountUniquePairs(nums, target);

    // Print results in a consistent, readable format
    Console.WriteLine($"Input nums = [{string.Join(", ", nums)}], target = {target}");
    Console.WriteLine($"Result:   {result}");
    Console.WriteLine($"Expected: {expected[i]} ({notes[i]})\n");
}
