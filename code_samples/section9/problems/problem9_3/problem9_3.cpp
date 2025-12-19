#include <iostream>
#include <vector>
using namespace std;

/**
 * canPartition
 * ------------
 * Determines whether the input array can be partitioned into two subsets
 * whose sums are equal.
 *
 * This is the classic "Partition Equal Subset Sum" problem, which can be
 * reduced to a 0/1 subset-sum decision problem.
 *
 * Key observation:
 *   - If the total sum of all elements is S, then each subset must sum to S/2.
 *   - If S is odd, such a partition is impossible.
 *
 * DP formulation (1D boolean DP):
 *   dp[s] = true if there exists a subset of the processed elements
 *           whose sum is exactly s; otherwise false.
 *
 * Initialization:
 *   dp[0] = true
 *   (Sum 0 is always achievable by choosing no elements.)
 *
 * Transition:
 *   For each element x:
 *     for s from target down to x:
 *       if dp[s - x] is true, then dp[s] can be set to true.
 *
 * Why iterate s backwards?
 *   - Backward iteration ensures each element is used at most once.
 *   - Forward iteration would allow reusing the same element multiple times,
 *     effectively turning this into an unbounded subset-sum problem.
 *
 * Complexity:
 *   Time:  O(n * target), where target = total_sum / 2
 *   Space: O(target)
 *
 * @param nums Input array of positive integers
 * @return true if the array can be partitioned into two equal-sum subsets
 */
bool canPartition(const vector<int> &nums) {
    // Compute the total sum of all elements.
    int total = 0;
    for (int x : nums) total += x;

    // If total sum is odd, it cannot be split into two equal halves.
    if (total % 2 != 0) return false;

    // Target sum for one subset.
    int target = total / 2;

    // dp[s] indicates whether sum s is achievable.
    vector<bool> dp(target + 1, false);

    // Base case: sum 0 is always achievable.
    dp[0] = true;

    // Process each number in the array.
    for (int x : nums) {
        // Traverse sums backwards to enforce 0/1 usage of each element.
        for (int s = target; s >= x; s--) {
            // If we could form (s - x), then we can form s by adding x.
            if (dp[s - x]) dp[s] = true;
        }
    }

    // If we can form the target sum, a valid partition exists.
    return dp[target];
}

/**
 * test
 * ----
 * Helper function to run and display a single test case for canPartition().
 *
 * Prints:
 *   - Test name
 *   - Input array
 *   - Computed result
 *   - Expected result
 *
 * @param name     Descriptive test name
 * @param arr      Input array
 * @param expected Expected boolean result
 */
void test(const string &name, const vector<int> &arr, bool expected) {
    // Run the algorithm under test.
    bool result = canPartition(arr);

    // Print test details.
    cout << name << "\n";
    cout << "Input: {";
    for (size_t i = 0; i < arr.size(); i++) {
        cout << arr[i] << (i + 1 == arr.size() ? "" : ",");
    }
    cout << "}\n";

    // Print result versus expected outcome.
    cout << "canPartition = "
         << (result ? "true" : "false")
         << " (expected " << (expected ? "true" : "false") << ")\n\n";
}

/**
 * main
 * ----
 * Test harness for canPartition().
 *
 * Covers a variety of cases:
 *   - Classic positive and negative examples
 *   - Single-element input
 *   - Trivially partitionable cases
 *   - Larger sets with and without valid partitions
 */
int main() {
    cout << "=== Testing canPartition ===\n\n";

    // Test 1: Classic true example
    // {1,5,11,5} => total sum = 22, target = 11 => partition exists
    test("Test 1: Classic example", {1, 5, 11, 5}, true);

    // Test 2: Cannot partition (total sum is odd)
    test("Test 2: Cannot partition", {1, 2, 3, 5}, false);

    // Test 3: Single element cannot be partitioned
    test("Test 3: Single element", {7}, false);

    // Test 4: Two equal numbers can be partitioned trivially
    test("Test 4: Two equal numbers", {4, 4}, true);

    // Test 5: Larger mixed set with no valid partition
    test("Test 5: Larger mixed set", {2, 2, 3, 5}, false);

    // Test 6: Multiple valid partitions exist
    test("Test 6: Multiple partitions", {2, 2, 1, 1}, true);

    return 0;
}
