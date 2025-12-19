#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 * firstUniqChar
 *
 * Purpose:
 *   Finds the index of the first character in a string that appears exactly once.
 *   This is the C++ version of the classic "first unique character" problem.
 *
 * Parameters:
 *   s - Reference to an input string (not modified).
 *
 * Returns:
 *   - The 0-based index of the first unique character.
 *   - -1 if no such character exists or the string is empty.
 *
 * Approach:
 *   Two-pass algorithm:
 *     1) Count frequencies of each possible byte value (0-255).
 *     2) Scan the string again to find the first index whose frequency is 1.
 *
 * Complexity:
 *   - Time:  O(n), where n = s.size()
 *   - Space: O(1) effectively, since the frequency table has fixed size (256).
 *
 * Important notes:
 *   - This implementation treats the string as a sequence of bytes.
 *     It works correctly for ASCII / single-byte encodings.
 *     For UTF-8 or other multi-byte encodings, characters may span
 *     multiple bytes and would require a different approach.
 *   - Casting to unsigned char is essential to avoid negative indexing
 *     when char is signed on some platforms.
 */
int firstUniqChar(const string &s) {
    // Frequency table: one slot for each possible unsigned char value
    vector<int> freq(256, 0);

    // First pass: count occurrences of each character
    for (unsigned char ch : s) {
        freq[ch]++;
    }

    // Second pass: find the first index whose character has frequency 1
    for (int i = 0; i < (int)s.size(); i++) {
        // Cast ensures correct indexing even if char is signed
        if (freq[(unsigned char)s[i]] == 1) {
            return i;
        }
    }

    // No unique character found
    return -1;
}

/**
 * main
 *
 * Simple test harness that runs firstUniqChar() against
 * several sample strings and prints the results.
 */
int main() {
    // Test cases with expected results documented inline
    vector<string> tests = {
        "leetcode",       // expected 0 ('l')
        "loveleetcode",   // expected 2 ('v')
        "aabbcc",         // expected -1
        "",               // expected -1 (empty string)
        "x",              // expected 0
        "aabbc",          // expected 4 ('c')
        "swiss"           // expected 1 ('w')
    };

    // Expected outputs aligned by index with the tests vector
    vector<int> expected = {
        0,
        2,
        -1,
        -1,
        0,
        4,
        1
    };

    cout << "=== Testing firstUniqChar ===\n\n";

    // Iterate through all test cases
    for (size_t i = 0; i < tests.size(); i++) {
        int result = firstUniqChar(tests[i]);

        cout << "Input: \"" << tests[i] << "\"\n";
        cout << "firstUniqChar -> " << result
             << " (expected " << expected[i] << ")\n\n";
    }

    return 0;
}
