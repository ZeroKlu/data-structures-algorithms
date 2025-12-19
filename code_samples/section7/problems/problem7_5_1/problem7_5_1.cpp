#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

/**
 * twoSumIndices
 *
 * Purpose:
 *   Finds indices of two distinct elements in the array `nums` whose
 *   values sum to the given `target`.
 *
 * Parameters:
 *   nums   - Input vector of integers.
 *   target - Target sum we want to achieve.
 *
 * Returns:
 *   A vector containing two indices {i, j} such that:
 *       nums[i] + nums[j] == target
 *
 *   If no such pair exists, returns an empty vector.
 *
 * Approach (single-pass hash map):
 *   - Use an unordered_map to store previously seen values and their indices.
 *   - As we iterate through nums:
 *       * Compute the value needed to reach the target.
 *       * If that value already exists in the map, we have found a solution.
 *       * Otherwise, store the current value and its index in the map.
 *
 * Why this works:
 *   - Each element is checked exactly once.
 *   - The hash map provides O(1) average-time lookups.
 *
 * Complexity (average / expected):
 *   - Time:  O(n)
 *   - Space: O(n)
 *
 * Notes:
 *   - If the same value appears multiple times, the map keeps the most
 *     recently inserted index for that value.
 *   - This behavior is sufficient for the classic Two Sum problem.
 */
vector<int> twoSumIndices(const vector<int> &nums, int target) {
    // Hash map: value -> index of that value in nums
    unordered_map<int, int> mp;

    // Iterate through the array
    for (int i = 0; i < (int)nums.size(); i++) {
        // Compute the complement needed to reach the target
        int need = target - nums[i];

        // Check if the complement has already been seen
        auto it = mp.find(need);
        if (it != mp.end()) {
            // Found a valid pair: previous index and current index
            return {it->second, i};
        }

        // Record the current value and its index for future lookups
        mp[nums[i]] = i;
    }

    // No valid pair found
    return {}; // empty vector signals "no solution"
}

/**
 * printVec
 *
 * Helper function to print a vector<int> in [a, b, c] format.
 */
static void printVec(const vector<int> &v) {
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
 * Simple test harness for twoSumIndices().
 * Demonstrates:
 *   - a standard case with a solution
 *   - a case where the solution uses non-adjacent elements
 *   - a negative case with no solution
 */
int main() {
    cout << "=== Test: twoSumIndices ===\n\n";

    {
        // Classic example: 2 + 7 = 9
        vector<int> nums = {2, 7, 11, 15};
        int target = 9;

        cout << "Input: nums = ";
        printVec(nums);
        cout << ", target = " << target << "\n";

        auto result = twoSumIndices(nums, target);
        cout << "Result: ";
        printVec(result);
        cout << " (expected [0, 1])\n\n";
    }

    {
        // Example where solution is not the first two elements
        vector<int> nums = {3, 2, 4};
        int target = 6;

        cout << "Input: nums = ";
        printVec(nums);
        cout << ", target = " << target << "\n";

        auto result = twoSumIndices(nums, target);
        cout << "Result: ";
        printVec(result);
        cout << " (expected [1, 2])\n\n";
    }

    {
        // No two numbers sum to 10
        vector<int> nums = {1, 2, 3, 4};
        int target = 10;

        cout << "Input: nums = ";
        printVec(nums);
        cout << ", target = " << target << "\n";

        auto result = twoSumIndices(nums, target);
        cout << "Result: ";
        printVec(result);
        cout << " (expected [])\n\n";
    }

    return 0;
}
