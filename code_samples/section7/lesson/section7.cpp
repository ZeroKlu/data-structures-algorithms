/**
 * Demonstration program: hashing-based containers in C++
 *
 * This file shows common patterns using:
 *  - std::unordered_map for counting / lookups (average O(1) expected per op)
 *  - std::unordered_set for membership checks / duplicate detection
 *
 * NOTE:
 *  - unordered_map/unordered_set provide *average* O(1) operations, but can degrade
 *    in pathological cases (hash collisions) to O(n).
 *  - operator[] on unordered_map *inserts* a default-constructed value when the key
 *    is missing. This is often convenient for counting, but can be surprising if you
 *    only intended to "read" a value.
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

/**
 * demo_hash()
 *
 * Purpose:
 *  - Demonstrates how std::unordered_map operator[] behaves for incrementing counts.
 *
 * Key behavior:
 *  - freq["apple"]++:
 *      If "apple" doesn't exist, operator[] inserts ("apple", 0), then increments to 1.
 *  - freq["banana"] += 2:
 *      If "banana" doesn't exist, inserts ("banana", 0), then adds 2 => 2.
 *
 * Output:
 *  - Prints the final counts and shows capturing a count into a separate variable.
 */
void demo_hash() {
    // Map: word -> frequency count
    std::unordered_map<std::string, int> freq;

    // operator[] creates entries if missing; default int is 0
    freq["apple"]++;        // "apple" becomes 1
    freq["banana"] += 2;    // "banana" becomes 2

    // Reading using operator[] also inserts if missing (not the case here).
    // This line reads the already-existing key "apple".
    int n = freq["apple"]; // if missing, default constructed 0 then incremented

    std::cout << "[demo_hash] apple count: " << freq["apple"]
              << ", banana count: " << freq["banana"]
              << " (n variable: " << n << ")\n";
}

/**
 * countWords(words)
 *
 * Purpose:
 *  - Given a list of strings, compute the frequency of each string.
 *
 * Parameters:
 *  - words: vector of tokens/words to count
 *
 * Returns:
 *  - unordered_map<string, int> where:
 *      key   = the word
 *      value = number of occurrences
 *
 * Complexity:
 *  - Time:  O(n) average, where n = words.size()
 *  - Space: O(k), where k = number of unique words
 *
 * Notes:
 *  - Uses operator[] for counting; missing keys are inserted with value 0, then incremented.
 */
std::unordered_map<std::string, int>
countWords(const std::vector<std::string>& words) {
    std::unordered_map<std::string, int> freq;

    // Range-based loop over all words; const auto& avoids copying strings.
    for (const auto& w : words) {
        freq[w]++; // Inserts w with 0 if missing; then increments.
    }

    // Return by value (NRVO / move elision typically makes this efficient).
    return freq;
}

/**
 * firstDuplicate(arr)
 *
 * Purpose:
 *  - Returns the first value that appears more than once as you scan left-to-right.
 *
 * Example:
 *  - arr = {2, 5, 1, 2, 3, 5, 1} => returns 2
 *    because 2 is the first number that repeats when scanning.
 *
 * Parameters:
 *  - arr: list of integers to scan
 *
 * Returns:
 *  - The first duplicated value encountered.
 *  - Returns 0 as a sentinel if no duplicates are found.
 *
 * Complexity:
 *  - Time:  O(n) average
 *  - Space: O(n) in the worst case (when all are unique)
 *
 * Notes / Design considerations:
 *  - Returning 0 is a convenient sentinel, but ambiguous if 0 could be a valid element.
 *    Alternatives include:
 *      - return -1 (if inputs are positive)
 *      - return std::optional<int> (C++17+) to represent "not found" cleanly
 *      - throw an exception (usually overkill here)
 */
int firstDuplicate(const std::vector<int>& arr) {
    // Set storing values we've already seen
    std::unordered_set<int> seen;

    // Scan in order; as soon as we encounter a previously seen number, return it.
    for (int x : arr) {
        // count(x) is 1 if present, 0 if not present (for unordered_set)
        if (seen.count(x)) {
            return x;
        }
        // Remember we've seen x
        seen.insert(x);
    }

    // No duplicate found.
    return 0; // or some sentinel
}

/**
 * twoSum(nums, target)
 *
 * Purpose:
 *  - Classic "Two Sum" problem:
 *      Find indices (i, j) such that nums[i] + nums[j] == target.
 *
 * Approach:
 *  - Use a hash map from value -> index of that value seen so far.
 *  - For each element nums[i], compute need = target - nums[i].
 *    If need exists in the map, we found a prior index j such that nums[j] == need.
 *
 * Parameters:
 *  - nums: vector of integers
 *  - target: desired sum
 *
 * Returns:
 *  - pair {index_of_need, i} if found
 *  - {-1, -1} if no valid pair exists
 *
 * Complexity:
 *  - Time:  O(n) average
 *  - Space: O(n)
 *
 * Notes / Edge cases:
 *  - This implementation returns the first pair found during a left-to-right scan.
 *  - If there are duplicate values, the map will store the most recent index for a value
 *    (because we overwrite indexOf[nums[i]] = i).
 */
std::pair<int, int> twoSum(const std::vector<int>& nums, int target) {
    // Map: value -> index where we last saw that value
    std::unordered_map<int, int> indexOf; // value -> index

    // Iterate through all elements with indices
    for (int i = 0; i < (int)nums.size(); i++) {
        // The value we need to have seen before to reach target
        int need = target - nums[i];

        // find() checks without inserting (unlike operator[])
        auto it = indexOf.find(need);
        if (it != indexOf.end()) {
            // Found a previous index where nums[that_index] == need
            return {it->second, i};
        }

        // Store current number with its index for future matches.
        // If nums[i] appears again later, we'll overwrite with the later index.
        indexOf[nums[i]] = i;
    }

    // No pair found
    return {-1, -1};
}

// ================== Test helpers ==================

/**
 * printVector(label, v)
 *
 * Purpose:
 *  - Utility to print a vector with a text label.
 *
 * Template:
 *  - Works for any type T that supports operator<< to std::cout.
 *
 * Parameters:
 *  - label: printed before elements (e.g., "Array: ")
 *  - v: the vector to print
 */
template <typename T>
void printVector(const std::string& label, const std::vector<T>& v) {
    std::cout << label;
    for (const auto& x : v) std::cout << x << " ";
    std::cout << "\n";
}

/**
 * test_countWords()
 *
 * Purpose:
 *  - Tests countWords() with a sample input and prints the results.
 *
 * What it checks:
 *  - Correct counts for keys present.
 *  - Proper "not found" behavior for missing key ("pear") using find().
 *
 * Note:
 *  - Using freq["pear"] here would *insert* "pear" with a value of 0,
 *    which is why the test uses freq.find(k) instead.
 */
void test_countWords() {
    std::cout << "=== Test: countWords (word frequency) ===\n";

    // Sample input: apple appears 3x, banana 2x, orange 1x
    std::vector<std::string> words = {
        "apple", "banana", "apple", "orange", "banana", "apple"
    };

    // Compute frequency map
    auto freq = countWords(words);

    // Probe several keys, including one that doesn't exist ("pear")
    std::vector<std::string> keys = {"apple", "banana", "orange", "pear"};
    for (const auto& k : keys) {
        auto it = freq.find(k); // does NOT insert if missing
        if (it != freq.end()) {
            std::cout << "'" << k << "' -> " << it->second << "\n";
        } else {
            std::cout << "'" << k << "' -> (not found)\n";
        }
    }

    std::cout << "Expected: apple=3, banana=2, orange=1, pear not found\n\n";
}

/**
 * test_firstDuplicate()
 *
 * Purpose:
 *  - Tests firstDuplicate() on:
 *      1) An array that has duplicates (expects first duplicate 2)
 *      2) An array with no duplicates (expects sentinel 0)
 */
void test_firstDuplicate() {
    std::cout << "=== Test: firstDuplicate ===\n";

    // Case 1: duplicates exist; the first repeated value in scan order is 2
    std::vector<int> arr1 = {2, 5, 1, 2, 3, 5, 1};
    printVector("Array 1: ", arr1);
    int dup1 = firstDuplicate(arr1);
    std::cout << "First duplicate: " << dup1 << " (expected 2)\n\n";

    // Case 2: all unique; function should return sentinel 0
    std::vector<int> arr2 = {1, 2, 3, 4, 5};
    printVector("Array 2: ", arr2);
    int dup2 = firstDuplicate(arr2);
    std::cout << "First duplicate: " << dup2
              << " (expected 0 = no duplicate)\n\n";
}

/**
 * test_twoSum()
 *
 * Purpose:
 *  - Tests twoSum() on:
 *      1) A positive case where a pair exists (2 + 7 = 9)
 *      2) A negative case where no pair exists (target 100)
 *
 * Notes:
 *  - The expected indices depend on implementation details; for {2,7,11,15} target 9,
 *    the canonical answer is indices (0,1).
 */
void test_twoSum() {
    std::cout << "=== Test: twoSum (hash map) ===\n";

    // Positive case
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;

    printVector("Array: ", nums);
    std::cout << "Target: " << target << "\n";

    auto pair1 = twoSum(nums, target);
    if (pair1.first != -1) {
        // Found a valid pair; print indices and the corresponding values.
        std::cout << "twoSum found indices (" << pair1.first << ", "
                  << pair1.second << ") -> values (" << nums[pair1.first]
                  << ", " << nums[pair1.second] << ")\n";
        std::cout << "Expected: indices (0,1) or (1,0) with values 2 and 7\n\n";
    } else {
        std::cout << "twoSum did not find a pair (unexpected here)\n\n";
    }

    // Negative test: no pair sums to 100 in {1,2,3}
    std::cout << "--- Negative case (no pair) ---\n";
    std::vector<int> nums2 = {1, 2, 3};
    int target2 = 100;

    printVector("Array: ", nums2);
    std::cout << "Target: " << target2 << "\n";

    auto pair2 = twoSum(nums2, target2);
    if (pair2.first == -1) {
        std::cout << "twoSum correctly found no pair.\n\n";
    } else {
        std::cout << "twoSum incorrectly found a pair.\n\n";
    }
}

/**
 * main()
 *
 * Purpose:
 *  - Entry point that runs:
 *      - demo_hash() to show unordered_map counting basics
 *      - unit-style tests for each helper function
 *
 * Returns:
 *  - 0 on normal completion (conventional success code)
 */
int main() {
    std::cout << "=== demo_hash ===\n";
    demo_hash();
    std::cout << "\n";

    // Run test suite
    test_countWords();
    test_firstDuplicate();
    test_twoSum();

    return 0;
}
