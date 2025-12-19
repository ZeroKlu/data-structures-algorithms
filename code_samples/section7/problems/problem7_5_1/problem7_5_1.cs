/**
 * TwoSumIndices
 *
 * Purpose:
 *   Finds two distinct indices i and j in the array `nums` such that:
 *       nums[i] + nums[j] == target
 *
 * Parameters:
 *   nums   - Input array of integers.
 *   target - Target sum.
 *
 * Returns:
 *   An array of two indices [i, j] (here returned as [j, i] based on lookup order),
 *   or an empty array [] if no solution exists.
 *
 * Approach (single-pass hash table):
 *   - Use a Dictionary<int, int> to map:
 *       value -> index where that value was last seen
 *   - Iterate through nums:
 *       1) Compute need = target - nums[i]
 *       2) If need is already in the map, we've found a prior value that pairs
 *          with nums[i], so return its index and i.
 *       3) Otherwise, record nums[i] -> i and continue.
 *
 * Why this works:
 *   - Any time we are at position i, all indices < i have already been recorded
 *     in the dictionary, so finding `need` guarantees we found a distinct earlier
 *     element.
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       Each element does O(1) average dictionary operations.
 *   - Space: O(n)
 *       The dictionary may store up to n entries.
 *
 * Notes:
 *   - If the same value appears multiple times, the dictionary stores the most
 *     recent index for that value (because we overwrite map[nums[i]] = i).
 *     This is fine for the classic "return any one valid pair" Two Sum problem.
 *   - The array literal syntax [j, i] and [] uses collection expressions (C# 12+).
 */
static int[] TwoSumIndices(int[] nums, int target) {
    // Map: value -> index
    var map = new Dictionary<int, int>();

    // Iterate through the array once
    for (int i = 0; i < nums.Length; i++) {
        // Complement we need to reach target using nums[i]
        int need = target - nums[i];

        // If we've already seen `need`, we have a solution
        if (map.TryGetValue(need, out int j)) {
            return [j, i];
        }

        // Record (or update) the index for this value
        map[nums[i]] = i;
    }

    // No solution found
    return [];
}

// ============================
// Top-level tests
// ============================
//
// These are top-level statements (C# 9+). The array literals used here
// are collection expressions (C# 12+).
//

int[][] tests =
[
    [2, 7, 11, 15],  // expected [0,1]
    [3, 2, 4],       // expected [1,2]
    [1, 2, 3, 4]     // expected []
];

int[] targets = [9, 6, 10];

int[][] expected =
[
    [0, 1],
    [1, 2],
    []
];

Console.WriteLine("=== Test: TwoSumIndices ===\n");

// Run each test case
for (int t = 0; t < tests.Length; t++)
{
    int[] nums = tests[t];
    int target = targets[t];

    // Call the function under test
    int[] result = TwoSumIndices(nums, target);

    // Pretty-print results
    string resultStr = "[" + string.Join(", ", result) + "]";
    string expectedStr = "[" + string.Join(", ", expected[t]) + "]";

    Console.WriteLine($"Input nums = [{string.Join(", ", nums)}], target = {target}");
    Console.WriteLine($"Result: {resultStr} (expected {expectedStr})\n");
}
