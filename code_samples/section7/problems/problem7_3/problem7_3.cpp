#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

/**
 * longestConsecutive
 *
 * Purpose:
 *   Computes the length of the longest run of consecutive integers in an array,
 *   regardless of their original ordering.
 *
 * Example:
 *   nums = {100, 4, 200, 1, 3, 2}
 *   The longest consecutive sequence is {1, 2, 3, 4} => returns 4.
 *
 * Parameters:
 *   nums - Input vector of integers (may contain duplicates).
 *
 * Returns:
 *   Length of the longest consecutive sequence (0 if nums is empty).
 *
 * Approach:
 *   - Insert all values into an unordered_set for average O(1) membership checks.
 *   - For each unique value x:
 *       Only attempt to build a sequence starting at x if (x - 1) is NOT present.
 *       This guarantees x is the *start* of a run, so each run is counted once.
 *   - Then count upward: x, x+1, x+2, ... as long as values exist in the set.
 *
 * Why the "start of sequence" check matters:
 *   Without it, you might count the same sequence many times (once per element),
 *   leading to O(n^2) behavior in dense sequences.
 *
 * Complexity (average / expected):
 *   - Time:  O(n)
 *       Each element is inserted once, and each number is visited a constant
 *       number of times across all while-loops when using the start check.
 *   - Space: O(n)
 *       The set stores all unique values.
 *
 * Notes:
 *   - unordered_set iteration order is arbitrary.
 *   - set.count(value) returns 1 if present, 0 if absent (for unordered_set).
 */
int longestConsecutive(const vector<int> &nums) {
    // Build a hash set of all values (duplicates are automatically removed).
    unordered_set<int> set(nums.begin(), nums.end());

    // Tracks the best (maximum) sequence length found so far.
    int best = 0;

    // Iterate over each unique value in the set.
    for (int x : set) {
        // If (x - 1) does not exist, x is the beginning of a sequence.
        // This ensures we only start counting from the smallest element in each run.
        if (!set.count(x - 1)) { // start of a sequence
            int cur = x;
            int length = 1;

            // Count consecutive numbers upward.
            // As long as cur+1 exists, extend the current run.
            while (set.count(cur + 1)) {
                cur++;
                length++;
            }

            // Update best if we found a longer run.
            if (length > best) best = length;
        }
    }

    return best;
}

/**
 * printVec
 *
 * Purpose:
 *   Helper function to print a vector<int> in a readable [a, b, c] format.
 *
 * Parameters:
 *   v - vector to print
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
 * Simple console-based test harness for longestConsecutive().
 * Runs several test cases covering:
 *   - typical case
 *   - duplicates
 *   - empty input
 *   - negative values
 */
int main() {
    cout << "=== Test: longestConsecutive ===\n\n";

    {
        // Classic example: longest run is 1,2,3,4 => length 4
        vector<int> nums = {100, 4, 200, 1, 3, 2};
        cout << "Input: ";
        printVec(nums);
        cout << "\n";

        int r = longestConsecutive(nums);
        cout << "Longest consecutive length: " << r
             << " (expected 4: sequence 1,2,3,4)\n\n";
    }

    {
        // Includes duplicates (0 appears twice), but duplicates should not affect result.
        // Longest run is 0..8 => length 9.
        vector<int> nums = {0,3,7,2,5,8,4,6,0,1};
        cout << "Input: ";
        printVec(nums);
        cout << "\n";

        int r = longestConsecutive(nums);
        cout << "Longest consecutive length: " << r
             << " (expected 9: sequence 0..8)\n\n";
    }

    {
        // Duplicates again (1 appears twice).
        // Unique values are {0,1,2} => longest run length 3.
        vector<int> nums = {1,2,0,1};
        cout << "Input: ";
        printVec(nums);
        cout << "\n";

        int r = longestConsecutive(nums);
        cout << "Longest consecutive length: " << r
             << " (expected 3: 0,1,2)\n\n";
    }

    {
        // Empty vector should return 0.
        vector<int> nums; // empty
        cout << "Input: []\n";

        int r = longestConsecutive(nums);
        cout << "Longest consecutive length: " << r
             << " (expected 0)\n\n";
    }

    {
        // Contains negatives and a longer positive run.
        // Runs: (-2,-1,0) length 3 and (2,3,4,5) length 4 => answer 4.
        vector<int> nums = {-2, -1, 0, 2, 3, 4, 5};
        cout << "Input: ";
        printVec(nums);
        cout << "\n";

        int r = longestConsecutive(nums);
        cout << "Longest consecutive length: " << r
             << " (expected 4: 2,3,4,5)\n\n";
    }

    return 0;
}
