#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

/**
 * groupAnagrams
 *
 * Purpose:
 *   Groups strings that are anagrams of each other.
 *
 * Definition:
 *   Two strings are anagrams if they contain the same characters
 *   with the same frequencies, possibly in a different order.
 *
 * Parameters:
 *   strs - A vector of input strings to be grouped.
 *
 * Returns:
 *   A vector of groups, where each group is a vector<string>
 *   containing strings that are anagrams of each other.
 *
 * Approach:
 *   - For each string s in strs:
 *       1) Create a key by sorting the characters of s.
 *       2) Use the sorted string as a key in an unordered_map.
 *       3) Append the original string s to the vector at mp[key].
 *
 *   - Finally, extract all grouped vectors from the map into
 *     the result vector.
 *
 * Complexity:
 *   Let:
 *     n = number of strings
 *     k = average length of a string
 *
 *   - Time:  O(n * k log k)
 *       Sorting each string dominates the cost.
 *   - Space: O(n * k)
 *       For storing keys and grouped strings.
 *
 * Notes:
 *   - unordered_map provides average O(1) insertion and lookup.
 *   - The order of groups and the order within groups is unspecified
 *     because unordered_map does not preserve insertion order.
 */
vector<vector<string> > groupAnagrams(const vector<string> &strs) {
    // Map: sorted string (anagram key) -> list of original strings
    unordered_map<string, vector<string> > mp;

    // Iterate through each input string
    for (const string &s : strs) {
        // Create a canonical key by sorting the characters of s
        string key = s;
        sort(key.begin(), key.end());

        // Insert the original string into the group for this key
        mp[key].push_back(s);
    }

    // Collect grouped anagrams into the result vector
    vector<vector<string> > result;

    // Reserve space to avoid repeated reallocations
    result.reserve(mp.size());

    // Move each group out of the map into the result
    for (auto &kv : mp) {
        // kv.second is the vector<string> for one anagram group
        // std::move avoids copying the vector contents
        result.push_back(move(kv.second));
    }

    return result;
}

/**
 * main
 *
 * Simple test harness that demonstrates groupAnagrams().
 */
int main() {
    // Input list of words
    vector<string> words = {"eat", "tea", "tan", "ate", "nat", "bat"};

    cout << "=== Test: groupAnagrams ===\n\n";

    // Print input words
    cout << "Input words: ";
    for (const auto &w : words) cout << w << " ";
    cout << "\n\n";

    // Group anagrams
    auto groups = groupAnagrams(words);

    // Print resulting groups
    cout << "Anagram Groups:\n";
    for (const auto &group : groups) {
        cout << "  [ ";
        for (const auto &s : group) cout << s << " ";
        cout << "]\n";
    }

    // Expected output (order may vary due to unordered_map)
    cout << "\nExpected groups (order may vary):\n"
         << "  [eat, tea, ate]\n"
         << "  [tan, nat]\n"
         << "  [bat]\n";

    return 0;
}
