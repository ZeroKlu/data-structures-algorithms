/**
 * Computes the length of the Longest Increasing Subsequence (LIS) in `nums`.
 *
 * This uses the classic O(n log n) "tails" technique (often described via
 * patience sorting):
 *
 * Invariant:
 *   tails[i] is the smallest possible tail value of any strictly increasing
 *   subsequence of length (i + 1) seen so far.
 *
 * Why this works:
 *   Keeping tails as small as possible maximizes the chance to extend these
 *   subsequences with future numbers. `tails` remains sorted, enabling binary search.
 *
 * Update step for each element x:
 *   - Find the first index `lo` such that tails[lo] >= x (a lower_bound).
 *   - If lo == tails.Count: x extends the longest subsequence so far => append.
 *   - Else: replace tails[lo] with x to lower the tail for that subsequence length.
 *
 * Note:
 *   This returns only the LIS length, not the subsequence itself. Reconstructing
 *   the subsequence requires tracking predecessor indices.
 *
 * Complexity:
 *   Time:  O(n log n) (binary search per element)
 *   Space: O(n) (tails list)
 *
 * Edge cases:
 *   - Empty input => tails remains empty => returns 0.
 */
static int LengthOfLIS(int[] nums) {
    // `tails` holds the best (smallest) ending values for subsequences by length.
    var tails = new List<int>();

    // Process each number in the input array.
    foreach (int x in nums) {
        // Binary search for the first index where tails[index] >= x.
        // Search range is [lo, hi), i.e., hi is exclusive.
        int lo = 0, hi = tails.Count;

        while (lo < hi) {
            // Midpoint of the current search range.
            // Using (lo + hi) / 2 is safe here because indices are small.
            int mid = (lo + hi) / 2;

            // If tails[mid] < x, then x must go to the right of mid
            // to maintain sorted order (we're finding first >= x).
            if (tails[mid] < x) {
                lo = mid + 1;
            } else {
                // tails[mid] >= x, so mid is a candidate insertion point;
                // continue searching left to find the first such position.
                hi = mid;
            }
        }

        // After the loop, `lo` is the lower_bound position for x.
        if (lo == tails.Count) {
            // x is larger than all tails values => extends LIS length by 1.
            tails.Add(x);
        } else {
            // Replace existing tail with x to improve (lower) the tail value
            // for subsequences of length lo+1.
            tails[lo] = x;
        }
    }

    // The number of tails entries equals the length of the LIS.
    return tails.Count;
}

/**
 * Simple helper to run a test case and print:
 *  - test name
 *  - input array
 *  - computed LIS length
 *  - expected LIS length
 *
 * @param name     Descriptive test name/header.
 * @param arr      Input array for the LIS function.
 * @param expected Expected LIS length.
 */
static void Test(string name, int[] arr, int expected)
{
    // Run the algorithm under test.
    int result = LengthOfLIS(arr);

    // Print a small, readable report.
    Console.WriteLine(name);
    Console.WriteLine($"Input: [{string.Join(",", arr)}]");
    Console.WriteLine($"LengthOfLIS = {result} (expected {expected})\n");
}

// Header for the test run output.
Console.WriteLine("=== Testing LengthOfLIS ===\n");

// Test 1: Strictly increasing => LIS uses all elements.
Test("Test 1: Increasing sequence", [1, 2, 3, 4, 5], 5);

// Test 2: Classic mixed case => known LIS length is 4 (e.g., 2,3,7,101).
Test("Test 2: Mixed sequence", [10, 9, 2, 5, 3, 7, 101, 18], 4);

// Test 3: All identical => strictly increasing disallows duplicates => LIS = 1.
Test("Test 3: All identical", [7, 7, 7, 7, 7], 1);

// Test 4: Empty input => LIS = 0.
Test("Test 4: Empty array", Array.Empty<int>(), 0);

// Test 5: Strictly decreasing => best you can do is any single element => LIS = 1.
Test("Test 5: Decreasing sequence", [9, 7, 5, 3, 1], 1);

// Test 6: Random pattern => LIS length is 3 (e.g., 4,8,9).
Test("Test 6: Random pattern", [4, 10, 4, 3, 8, 9], 3);
