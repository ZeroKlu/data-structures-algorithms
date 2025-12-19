/**
 * LongestConsecutive
 *
 * Purpose:
 *   Computes the length of the longest run of consecutive integers in an array,
 *   regardless of their original order. This matches the common "Longest
 *   Consecutive Sequence" problem.
 *
 * Example:
 *   nums = [100, 4, 200, 1, 3, 2]
 *   Longest consecutive sequence is [1, 2, 3, 4] => returns 4.
 *
 * Parameters:
 *   nums - Input array of integers (may contain duplicates).
 *
 * Returns:
 *   Length of the longest consecutive sequence (0 if nums is empty).
 *
 * Approach:
 *   - Insert all numbers into a HashSet<int> for average O(1) membership checks.
 *   - For each unique number x in the set:
 *       Only start a sequence at x if (x - 1) is NOT in the set.
 *       This guarantees x is the beginning (smallest value) of its sequence,
 *       preventing us from recounting the same sequence multiple times.
 *   - Count upward: x, x+1, x+2, ... as long as the next number exists.
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       Each number is inserted once, and each sequence is scanned once.
 *   - Space: O(n)
 *       HashSet stores the unique values.
 *
 * Notes:
 *   - foreach over HashSet iterates in an arbitrary order (not sorted).
 *   - Duplicates in the input do not affect correctness because HashSet
 *     keeps only unique values.
 */
static int LongestConsecutive(int[] nums) {
    // Create a set of unique numbers for fast membership checks.
    var set = new HashSet<int>(nums);

    // Tracks the maximum sequence length found so far.
    int best = 0;

    // Iterate over each unique value in the set.
    foreach (var x in set) {
        // Start counting only if x is the beginning of a sequence.
        // If x-1 exists, then x is in the middle of a sequence and should be skipped.
        if (!set.Contains(x - 1)) {
            int cur = x;
            int length = 1;

            // Extend the sequence while consecutive numbers exist.
            while (set.Contains(cur + 1)) {
                cur++;
                length++;
            }

            // Update the best (maximum) length.
            if (length > best) best = length;
        }
    }

    return best;
}

// ============================
// Top-level tests
// ============================
//
// These are top-level statements (C# 9+), so they execute immediately
// without needing an explicit Main() method.
// The array literals used below are collection expressions (C# 12+).
//

int[][] tests =
[
    [100, 4, 200, 1, 3, 2],         // expected 4: 1,2,3,4
    [0, 3, 7, 2, 5, 8, 4, 6, 0, 1], // expected 9: 0..8 (duplicate 0 should not matter)
    [1, 2, 0, 1],                   // expected 3: 0,1,2 (duplicate 1 should not matter)
    [],                             // expected 0: empty input
    [-2, -1, 0, 2, 3, 4, 5]         // expected 4: 2,3,4,5 (also has -2,-1,0 => length 3)
];

int[] expected = [4, 9, 3, 0, 4];

Console.WriteLine("=== Test: LongestConsecutive ===\n");

// Run each test case and compare with the expected result.
for (int i = 0; i < tests.Length; i++)
{
    int[] nums = tests[i];

    // Call the function under test
    int result = LongestConsecutive(nums);

    // Print input and outcome
    Console.WriteLine($"Input: [{string.Join(", ", nums)}]");
    Console.WriteLine($"Longest consecutive length: {result} (expected {expected[i]})\n");
}
