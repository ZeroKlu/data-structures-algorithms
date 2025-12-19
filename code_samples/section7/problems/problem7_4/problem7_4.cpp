#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

/**
 * subarraySum
 *
 * Purpose:
 *   Counts how many contiguous subarrays in `nums` sum exactly to `k`.
 *   This is the classic "Subarray Sum Equals K" problem.
 *
 * Parameters:
 *   nums - Input vector of integers (may be empty; may contain negatives).
 *   k    - Target sum.
 *
 * Returns:
 *   The number of contiguous subarrays whose sum is exactly k.
 *
 * Core idea (prefix sums + frequency map):
 *   Let prefix be the running sum of elements seen so far.
 *   The sum of a subarray from index (j+1) to i is:
 *
 *       prefix[i] - prefix[j]
 *
 *   We want:
 *
 *       prefix[i] - prefix[j] == k
 *       => prefix[j] == prefix[i] - k
 *
 *   So while scanning the array:
 *     - Maintain `freq`, a map from prefix sum value -> how many times it has appeared.
 *     - For current prefix sum P, the number of subarrays ending here with sum k is:
 *         freq[P - k]
 *     - Then increment freq[P] because we've now seen this prefix sum.
 *
 * Why freq[0] = 1 matters:
 *   - It represents an "empty prefix" before reading any elements.
 *   - This allows subarrays that start at index 0 to be counted correctly.
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       Each step does O(1) average hash map operations.
 *   - Space: O(n)
 *       In the worst case, all prefix sums are distinct.
 *
 * Notes:
 *   - Works with negative numbers (unlike many sliding-window techniques).
 *   - If nums can be very large or sums can overflow int, consider using
 *     long long for prefix and key type: unordered_map<long long, int>.
 */
int subarraySum(const vector<int> &nums, int k) {
    // Map: prefix_sum -> frequency count (how many times we've seen this prefix sum)
    unordered_map<int, int> freq;

    // Base case: prefix sum 0 occurs once before processing elements.
    freq[0] = 1;

    int prefix = 0; // running prefix sum
    int count = 0;  // number of qualifying subarrays found

    // Scan through the array once
    for (int x : nums) {
        // Update running sum
        prefix += x;

        // We want previous prefix sums equal to (prefix - k)
        int need = prefix - k;

        // If we've seen that prefix sum before, each occurrence creates
        // a subarray ending here that sums to k.
        if (freq.count(need)) {
            count += freq[need];
        }

        // Record that we've now seen this prefix sum.
        // operator[] default-initializes to 0 if key doesn't exist.
        freq[prefix]++;
    }

    return count;
}

/**
 * printVec
 *
 * Helper function that prints a vector<int> in [a, b, c] format.
 */
static void printVec(const vector<int>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

/**
 * main
 *
 * Simple console-based test harness for subarraySum().
 * Runs several test cases including:
 *   - multiple solutions in a small array
 *   - different subarray lengths
 *   - negatives (to show correctness beyond sliding-window methods)
 *   - empty input
 */
int main() {
    cout << "=== Test: subarraySum ===\n\n";

    {
        // Two subarrays sum to 2: [1,1] at indices (0..1) and (1..2)
        vector<int> nums = {1, 1, 1};
        int k = 2;

        cout << "Input: ";
        printVec(nums);
        cout << ", k = " << k << "\n";

        int r = subarraySum(nums, k);
        cout << "Result: " << r << " (expected 2)\n\n";
    }

    {
        // Two subarrays sum to 3: [1,2] and [3]
        vector<int> nums = {1, 2, 3};
        int k = 3;

        cout << "Input: ";
        printVec(nums);
        cout << ", k = " << k << "\n";

        int r = subarraySum(nums, k);
        cout << "Result: " << r << " (expected 2: [1,2], [3])\n\n";
    }

    {
        // Expected 4 subarrays sum to 7 in this mixed/negative case
        vector<int> nums = {3, 4, 7, 2, -3, 1, 4, 2};
        int k = 7;

        cout << "Input: ";
        printVec(nums);
        cout << ", k = " << k << "\n";

        int r = subarraySum(nums, k);
        cout << "Result: " << r << " (expected 4)\n\n";
    }

    {
        // Empty input: no subarrays exist
        vector<int> nums;  // empty
        int k = 0;

        cout << "Input: []\n";

        int r = subarraySum(nums, k);
        cout << "Result: " << r << " (expected 0)\n\n";
    }

    return 0;
}
