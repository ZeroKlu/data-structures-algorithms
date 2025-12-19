#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * lengthOfLIS
 * -----------
 * Computes the length of the Longest Increasing Subsequence (LIS) in `nums`.
 *
 * This uses the classic O(n log n) "tails" / patience-sorting approach.
 *
 * Core idea / invariant:
 *   tails[i] holds the smallest possible tail value of an increasing subsequence
 *   of length (i + 1) that we've seen so far.
 *
 * Why "smallest tail" matters:
 *   A smaller tail for a given subsequence length is always better because it
 *   leaves more room to extend with future values.
 *
 * How we update:
 *   For each value x in nums:
 *     - Find the first position in tails where tails[pos] >= x (lower_bound).
 *     - If no such position exists (x is larger than all tails), append x:
 *         -> we have found a longer increasing subsequence.
 *     - Otherwise, replace tails[pos] with x:
 *         -> we improve the tail value for subsequences of length pos+1.
 *
 * Note:
 *   This function returns only the LENGTH of the LIS, not the subsequence itself.
 *   Reconstructing the actual subsequence requires additional parent pointers.
 *
 * Complexity:
 *   - Time:  O(n log n) due to lower_bound for each element
 *   - Space: O(n) for the tails array in the worst case
 *
 * @param nums Input array of integers.
 * @return Length of the longest strictly increasing subsequence.
 */
int lengthOfLIS(const vector<int> &nums) {
    // tails will remain sorted (non-decreasing) as we update it with lower_bound.
    vector<int> tails;

    // Iterate through each value in the input.
    for (int x : nums) {
        // Find the first tails element that is >= x.
        // This gives the position where x can either:
        //   - extend tails (if it goes at the end), or
        //   - replace an existing tail to keep tails minimal.
        auto it = lower_bound(tails.begin(), tails.end(), x);

        if (it == tails.end()) {
            // x is bigger than all existing tails:
            // we can extend the longest subsequence found so far.
            tails.push_back(x);
        } else {
            // Replace the first tail >= x with x.
            // This does not change the LIS length so far, but it improves
            // (lowers) the tail value for subsequences of this length.
            *it = x;
        }
    }

    // The number of tails entries equals the length of the LIS.
    return (int)tails.size();
}

/**
 * main
 * ----
 * Simple test harness for lengthOfLIS().
 *
 * Prints:
 *   - Test name
 *   - Input array (as a human-readable string)
 *   - Computed LIS length
 *   - Expected LIS length
 *
 * These tests cover common patterns:
 *   1) strictly increasing (LIS = n)
 *   2) classic mixed example (LIS = 4)
 *   3) all identical (strictly increasing => LIS = 1)
 *   4) empty input (LIS = 0)
 *   5) strictly decreasing (LIS = 1)
 *   6) random pattern with repeats/inversions (LIS = 3)
 */
int main() {
    cout << "=== Test 1: Basic Increasing Sequence ===\n";
    vector<int> arr1 = {1, 2, 3, 4, 5};
    cout << "Input: [1,2,3,4,5]\n";
    cout << "lengthOfLIS = " << lengthOfLIS(arr1)
         << " (expected 5)\n\n";

    cout << "=== Test 2: Mixed Sequence ===\n";
    vector<int> arr2 = {10, 9, 2, 5, 3, 7, 101, 18};
    cout << "Input: [10,9,2,5,3,7,101,18]\n";
    cout << "lengthOfLIS = " << lengthOfLIS(arr2)
         << " (expected 4)\n\n";

    cout << "=== Test 3: All Identical ===\n";
    vector<int> arr3 = {7, 7, 7, 7, 7};
    cout << "Input: [7,7,7,7,7]\n";
    // Because the subsequence must be strictly increasing,
    // duplicates cannot extend an increasing subsequence.
    cout << "lengthOfLIS = " << lengthOfLIS(arr3)
         << " (expected 1)\n\n";

    cout << "=== Test 4: Empty Array ===\n";
    vector<int> arr4 = {};
    cout << "Input: []\n";
    // Empty array has LIS length 0.
    cout << "lengthOfLIS = " << lengthOfLIS(arr4)
         << " (expected 0)\n\n";

    cout << "=== Test 5: Decreasing Sequence ===\n";
    vector<int> arr5 = {9, 7, 5, 3, 1};
    cout << "Input: [9,7,5,3,1]\n";
    // In a strictly decreasing sequence, the best increasing subsequence
    // consists of any single element.
    cout << "lengthOfLIS = " << lengthOfLIS(arr5)
         << " (expected 1)\n\n";

    cout << "=== Test 6: Random Pattern ===\n";
    vector<int> arr6 = {4, 10, 4, 3, 8, 9};
    cout << "Input: [4,10,4,3,8,9]\n";
    cout << "lengthOfLIS = " << lengthOfLIS(arr6)
         << " (expected 3)\n\n";

    return 0;
}
