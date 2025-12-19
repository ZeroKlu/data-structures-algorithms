#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * canPartition
 * ------------
 * Determines whether the given array can be partitioned into two subsets
 * whose sums are equal.
 *
 * This is the classic "Partition Equal Subset Sum" problem, which reduces to
 * a 0/1 subset-sum decision problem:
 *
 * Key observation:
 *   If the total sum of all elements is S, then we need to find a subset that
 *   sums to S/2. This is only possible if S is even.
 *
 * DP formulation (1D boolean subset-sum):
 *   dp[s] = true if we can form sum `s` using some subset of the numbers
 *           processed so far; otherwise false.
 *
 * Initialization:
 *   dp[0] = true (sum 0 is always achievable by taking no elements)
 *
 * Transition:
 *   For each element x:
 *     for s from target down to x:
 *       dp[s] = dp[s] OR dp[s - x]
 *
 * Why iterate s backwards?
 *   - Backward iteration ensures each element x is used at most once (0/1 choice).
 *   - If we iterated forward, we could reuse the same x multiple times in the
 *     same iteration, which would be the unbounded subset-sum variant.
 *
 * Complexity:
 *   Time:  O(n * target), where target = (sum(nums) / 2)
 *   Space: O(target)
 *
 * Parameters:
 *   nums     - input array
 *   numsSize - number of elements in nums
 *
 * Returns:
 *   true if the array can be partitioned into two equal-sum subsets, else false.
 */
bool canPartition(int *nums, int numsSize) {
    // Compute total sum of all numbers.
    int total = 0;
    for (int i = 0; i < numsSize; i++) {
        total += nums[i];
    }

    // If total sum is odd, it cannot be split into two equal integers.
    if (total % 2 != 0) return false;

    // Target sum for one subset (the other subset will also sum to target).
    int target = total / 2;

    // dp[s] indicates whether sum s is achievable.
    // calloc initializes all entries to false (0).
    bool *dp = (bool *)calloc(target + 1, sizeof(bool));

    // Base case: sum 0 is always achievable (choose nothing).
    dp[0] = true;

    // Process each element, updating subset-sum reachability.
    for (int i = 0; i < numsSize; i++) {
        int x = nums[i];

        // Iterate backwards to enforce 0/1 usage of each element.
        for (int s = target; s >= x; s--) {
            // If we could previously make (s - x), then we can make s by adding x.
            if (dp[s - x]) dp[s] = true;
        }
    }

    // Answer is whether we can reach exactly target.
    bool ans = dp[target];

    // Free heap memory to avoid leaks.
    free(dp);

    return ans;
}

/**
 * test
 * ----
 * Helper function to run a single test case for canPartition() and print:
 *   - test name
 *   - input array
 *   - computed boolean result and expected result
 *
 * Parameters:
 *   name     - descriptive test label
 *   arr      - input array
 *   n        - number of elements
 *   expected - expected boolean answer
 */
void test(const char *name, int *arr, int n, bool expected) {
    // Run the algorithm under test.
    bool result = canPartition(arr, n);

    // Print test header and input array.
    printf("%s\n", name);
    printf("Input: {");
    for (int i = 0; i < n; i++) {
        printf("%d%s", arr[i], i + 1 == n ? "" : ",");
    }
    printf("}\n");

    // Print result vs expected.
    printf("canPartition = %s (expected %s)\n\n",
           result ? "true" : "false",
           expected ? "true" : "false");
}

/**
 * main
 * ----
 * Test harness for canPartition().
 *
 * Exercises several typical scenarios:
 *   - classic positive/negative examples
 *   - single element (cannot split into two non-empty equal sums)
 *   - two equal elements (trivially partitionable)
 *   - larger sets with/without solutions
 */
int main(void) {
    printf("=== Testing canPartition ===\n\n");

    // Test 1: Classic example (often cited as a partitionable case)
    // {1,5,11,5} has total sum 22, target 11.
    // Subset {11} (or {5,5,1}) achieves 11 => true.
    int arr1[] = {1, 5, 11, 5};
    test("Test 1: Classic true example", arr1, 4, true);

    // Test 2: Cannot partition
    // Total sum = 11 (odd) => immediately false.
    int arr2[] = {1, 2, 3, 5};
    test("Test 2: Cannot partition", arr2, 4, false);

    // Test 3: Single element cannot partition into two equal sums
    // Total sum = 7 (odd) => false (also cannot split meaningfully).
    int arr3[] = {7};
    test("Test 3: Single element", arr3, 1, false);

    // Test 4: Two equal elements can partition
    // {4} and {4} both sum to 4.
    int arr4[] = {4, 4};
    test("Test 4: Two equal numbers", arr4, 2, true);

    // Test 5: Larger set that cannot be partitioned
    // Total sum = 12 (even), target = 6, but no subset sums to 6 => false.
    int arr5[] = {2, 2, 3, 5};
    test("Test 5: Larger mixed set", arr5, 4, false);

    // Test 6: Multiple ways to partition
    // Total sum = 6, target = 3. Examples: {2,1} and {2,1} => true.
    int arr6[] = {2, 2, 1, 1};
    test("Test 6: Multiple partitions", arr6, 4, true);

    return 0;
}
