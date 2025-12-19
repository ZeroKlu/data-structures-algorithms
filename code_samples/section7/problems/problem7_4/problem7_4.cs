/**
 * SubarraySum
 *
 * Purpose:
 *   Counts the number of contiguous subarrays whose elements sum to k.
 *   This is the classic "Subarray Sum Equals K" problem.
 *
 * Parameters:
 *   nums - Input array of integers (may be empty; may include negative values).
 *   k    - Target sum.
 *
 * Returns:
 *   Number of contiguous subarrays whose sum is exactly k.
 *
 * Core idea (prefix sums + frequency dictionary):
 *   Let prefix be the running sum of nums[0..i].
 *   The sum of a subarray (j+1..i) is:
 *
 *       prefix[i] - prefix[j]
 *
 *   We want prefix[i] - prefix[j] == k
 *       => prefix[j] == prefix[i] - k
 *
 *   So for each position i:
 *     1) Compute current prefix sum P
 *     2) Add how many times (P - k) has appeared as a prefix sum before
 *     3) Record that we've seen prefix sum P one more time
 *
 * Why we seed freq[0] = 1:
 *   - Represents the "empty prefix" before reading any elements.
 *   - Allows subarrays starting at index 0 (where prefix == k) to be counted.
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       Each iteration performs O(1) average dictionary operations.
 *   - Space: O(n)
 *       In the worst case, every prefix sum is distinct.
 *
 * Notes:
 *   - Works with negative numbers (unlike many sliding-window approaches).
 *   - If sums might overflow int for large inputs, consider using long for prefix
 *     and Dictionary<long, int>.
 */
static int SubarraySum(int[] nums, int k) {
    // Frequency map: prefixSum -> number of times we've seen that prefix sum
    var freq = new Dictionary<int, int>
    {
        // Base case: prefix sum 0 occurs once before processing elements
        [0] = 1
    };

    int prefix = 0; // running prefix sum
    int count = 0;  // total number of subarrays with sum k

    // Process each element once
    foreach (var x in nums) {
        // Update running sum
        prefix += x;

        // We need prior prefix sums equal to (prefix - k)
        int need = prefix - k;

        // If such prefix sums exist, each occurrence forms a subarray ending here
        if (freq.TryGetValue(need, out int c)) {
            count += c;
        }

        // Record that we've seen this prefix sum.
        // If prefix already exists, increment its frequency; otherwise set to 1.
        freq[prefix] = freq.TryGetValue(prefix, out int cur) ? cur + 1 : 1;
    }

    return count;
}

// ============================
// Top-level tests
// ============================
//
// These are top-level statements (C# 9+). The array literals below use
// collection expressions (C# 12+).
//

int[][] tests =
[
    [1, 1, 1],                      // expected 2
    [1, 2, 3],                      // expected 2: [1,2], [3]
    [3, 4, 7, 2, -3, 1, 4, 2],      // expected 4
    []                              // expected 0
];

// Target k values aligned with tests[]
int[] kVals = [2, 3, 7, 0];

// Expected results aligned with tests[]
int[] expected = [2, 2, 4, 0];

Console.WriteLine("=== Test: SubarraySum ===\n");

// Run each test case and compare to expected output
for (int i = 0; i < tests.Length; i++)
{
    int[] nums = tests[i];
    int k = kVals[i];

    Console.WriteLine($"Input: [{string.Join(", ", nums)}], k = {k}");

    // Call the function under test
    int result = SubarraySum(nums, k);

    Console.WriteLine($"Result: {result} (expected {expected[i]})\n");
}
