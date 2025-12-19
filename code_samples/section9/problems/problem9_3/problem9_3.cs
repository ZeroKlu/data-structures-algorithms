/**
 * CanPartition
 * ------------
 * Determines whether the given array of integers can be partitioned
 * into two subsets whose sums are equal.
 *
 * This is the classic "Partition Equal Subset Sum" problem, which can be
 * reduced to a 0/1 subset-sum decision problem.
 *
 * Key observation:
 *   - Let S be the total sum of all elements.
 *   - If S is odd, it is impossible to split the array into two subsets
 *     with equal integer sums.
 *   - If S is even, the problem becomes:
 *       "Is there a subset of elements that sums to S / 2?"
 *
 * DP formulation (1D boolean DP):
 *   dp[s] = true if some subset of the processed elements can form sum s.
 *
 * Initialization:
 *   dp[0] = true
 *   (A sum of 0 is always achievable by selecting no elements.)
 *
 * Transition:
 *   For each element x:
 *     for s from target down to x:
 *       dp[s] = dp[s] || dp[s - x]
 *
 * Why iterate s backwards?
 *   - Backward iteration ensures each element is used at most once.
 *   - Iterating forward would allow the same element to be reused multiple
 *     times in the same iteration, turning this into an unbounded subset-sum
 *     problem.
 *
 * Complexity:
 *   Time:  O(n * target), where target = totalSum / 2
 *   Space: O(target)
 *
 * @param nums Input array of positive integers
 * @return true if the array can be partitioned into two equal-sum subsets
 */
static bool CanPartition(int[] nums) {
    // Compute the total sum of all elements
    int total = 0;
    foreach (var x in nums) total += x;

    // If total sum is odd, equal partition is impossible
    if ((total & 1) == 1) return false;

    // Target sum for one subset
    int target = total / 2;

    // dp[s] indicates whether sum s is achievable
    bool[] dp = new bool[target + 1];

    // Base case: sum 0 is always achievable
    dp[0] = true;

    // Process each number in the input array
    foreach (var x in nums) {
        // Traverse sums backwards to enforce 0/1 usage
        for (int s = target; s >= x; s--) {
            // If we could form (s - x), then we can form s by adding x
            if (dp[s - x]) dp[s] = true;
        }
    }

    // If target sum is achievable, a valid partition exists
    return dp[target];
}

/**
 * Test
 * ----
 * Helper method to run a single test case for CanPartition() and print:
 *   - Test name
 *   - Input array
 *   - Computed result
 *   - Expected result
 *
 * @param name     Descriptive test name
 * @param arr      Input array
 * @param expected Expected boolean result
 */
static void Test(string name, int[] arr, bool expected)
{
    // Run the partition check
    bool result = CanPartition(arr);

    // Print test details
    Console.WriteLine(name);
    Console.WriteLine($"Input: [{string.Join(",", arr)}]");
    Console.WriteLine($"CanPartition = {result} (expected {expected})\n");
}

// ===========================
// Test Harness
// ===========================

// Header for all tests
Console.WriteLine("=== Testing CanPartition ===\n");

// Test 1: Classic example
// {1,5,11,5} => total sum = 22, target = 11 => partition exists
Test("Test 1: Classic example", [1, 5, 11, 5], true);

// Test 2: Cannot partition (total sum is odd)
Test("Test 2: Cannot partition", [1, 2, 3, 5], false);

// Test 3: Single element cannot be partitioned
Test("Test 3: Single element", [7], false);

// Test 4: Two equal numbers can be partitioned trivially
Test("Test 4: Two equal numbers", [4, 4], true);

// Test 5: Larger mixed set with no valid partition
Test("Test 5: Larger mixed set", [2, 2, 3, 5], false);

// Test 6: Multiple valid subsets exist
// Example partition: {2,1} and {2,1}
Test("Test 6: Multiple valid subsets", [2, 2, 1, 1], true);
