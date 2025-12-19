#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
using namespace std;

/**
 * countUniquePairs
 *
 * Purpose:
 *   Counts the number of *unique value pairs* {a, b} such that:
 *       a + b == target
 *
 *   Uniqueness is by VALUE, not by index.
 *
 * Example:
 *   nums   = [1, 1, 2, 2, 3, 3]
 *   target = 4
 *   Valid value pairs are {1,3} and {2,2} -> answer is 2.
 *
 * Parameters:
 *   nums   - input numbers (may contain duplicates, may be unsorted)
 *   target - the desired sum
 *
 * Returns:
 *   The number of distinct value pairs that sum to target.
 *
 * Approach:
 *   - Use `seen` to store numbers we've already processed.
 *   - For each x in nums:
 *       y = target - x
 *       If y is in seen, then (x,y) forms a pair.
 *       Normalize pair order as (a=min(x,y), b=max(x,y)) so order doesn't matter.
 *       Convert that normalized pair into a unique key string "a,b" and insert
 *       it into the `pairs` set to deduplicate.
 *
 * Why we need both sets:
 *   - `seen` prevents us from requiring two passes and ensures we only match
 *     against values that appeared earlier.
 *   - `pairs` ensures duplicates in nums do not inflate the count.
 *
 * Complexity (expected / average):
 *   - Time:  O(n) average for hash lookups/inserts, but with extra overhead:
 *       Creating strings for each found pair adds extra work (and allocations).
 *   - Space: O(n) for the sets.
 *
 * Notes / potential improvements:
 *   - Using string keys is simple and readable, but it is not the most efficient
 *     approach (string creation + hashing).
 *   - A more efficient alternative is to store pairs as a struct with a custom
 *     hash (or pack into 64 bits) in an unordered_set.
 */
int countUniquePairs(const vector<int> &nums, int target) {
    // Stores values we've already seen while scanning left-to-right
    unordered_set<int> seen;

    // Stores unique normalized pair identifiers ("a,b") so duplicates are ignored
    unordered_set<string> pairs;

    // Process each number in the input
    for (int x : nums) {
        // Complement needed to reach the target
        int y = target - x;

        // If we've already seen y, then x and y can form a valid pair
        if (seen.count(y)) {
            // Normalize order so (x,y) and (y,x) are treated the same
            int a = min(x, y);
            int b = max(x, y);

            // Encode the normalized pair into a string key.
            // Example: a=1, b=5 -> "1,5"
            //
            // Because we normalize (a<=b), we avoid double-counting.
            string key = to_string(a) + "," + to_string(b);

            // Insert into set; duplicates are automatically ignored
            pairs.insert(key);
        }

        // Record x as seen for future elements
        seen.insert(x);
    }

    // Number of unique normalized pairs found
    return (int)pairs.size();
}

/**
 * printVec
 *
 * Helper function to print a vector<int> in [a, b, c] format.
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
 * Simple console-based test harness for countUniquePairs().
 * Demonstrates:
 *   - duplicates that create only a few unique value pairs
 *   - standard two-sum case
 *   - repeated single value case (3+3)
 *   - no-solution case
 *   - duplicates arranged differently (1,5 repeated)
 */
int main() {
    cout << "=== Test: countUniquePairs ===\n\n";

    /**
     * TestCase
     *
     * Bundles one test input with its expected output and a short description
     * of what unique pair(s) should exist.
     */
    struct TestCase {
        vector<int> nums;
        int target;
        int expected;
        string description;
    };

    // Collection of test cases
    vector<TestCase> tests = {
        { {1,1,2,2,3,3}, 4, 2, "{1,3} and {2,2}" },
        { {2,7,11,15},   9, 1, "{2,7}" },
        { {3,3,3},       6, 1, "{3,3}" },
        { {1,2,3,4},    10, 0, "no pairs" },
        { {1,5,1,5},     6, 1, "{1,5}" },
    };

    // Run each test case
    for (const auto& t : tests) {
        cout << "Input: nums = ";
        printVec(t.nums);
        cout << ", target = " << t.target << "\n";

        // Execute the function under test
        int result = countUniquePairs(t.nums, t.target);

        // Print results
        cout << "Result:   " << result << "\n";
        cout << "Expected: " << t.expected
             << " (" << t.description << ")\n\n";
    }

    return 0;
}
