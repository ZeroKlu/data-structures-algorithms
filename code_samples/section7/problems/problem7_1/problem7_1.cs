/**
 * FirstUniqChar
 *
 * Purpose:
 *   Finds the index of the first character in a string that appears exactly once.
 *   This is the C# version of the classic "first unique character" problem.
 *
 * Parameters:
 *   s - Input string to examine (may be empty).
 *
 * Returns:
 *   - The 0-based index of the first unique character.
 *   - -1 if no such character exists or the string is empty.
 *
 * Approach:
 *   Two-pass algorithm:
 *     1) Count character frequencies using a Dictionary<char, int>.
 *     2) Scan the string again to find the first index with frequency == 1.
 *
 * Complexity:
 *   - Time:  O(n), where n is s.Length
 *   - Space: O(k), where k is the number of distinct characters
 *
 * Notes:
 *   - Using char counts UTF-16 code units, not Unicode grapheme clusters.
 *     Characters outside the BMP or composed characters may require
 *     different handling if full Unicode correctness is needed.
 */
static int FirstUniqChar(string s) {
    // Frequency table: character -> count
    var freq = new Dictionary<char, int>();

    // First pass: count occurrences of each character
    foreach (char ch in s) {
        // TryGetValue avoids KeyNotFoundException for new characters
        freq[ch] = freq.TryGetValue(ch, out int c) ? c + 1 : 1;
    }

    // Second pass: find the first index whose character appears exactly once
    for (int i = 0; i < s.Length; i++) {
        if (freq[s[i]] == 1) {
            return i;
        }
    }

    // No unique character found
    return -1;
}

// ============================
// Top-level tests
// ============================
//
// These statements execute immediately in a C# top-level program
// (C# 9+), without requiring an explicit Main() method.
//

// Test inputs with expected results
string[] tests = {
    "leetcode",      // expected 0 ('l')
    "loveleetcode",  // expected 2 ('v')
    "aabbcc",        // expected -1
    "",              // expected -1 (empty string)
    "x",             // expected 0
    "aabbc",         // expected 4 ('c')
    "swiss"          // expected 1 ('w')
};

// Expected outputs aligned with the tests array
int[] expected = {
    0,
    2,
    -1,
    -1,
    0,
    4,
    1
};

Console.WriteLine("=== Testing FirstUniqChar ===\n");

// Run all tests and print results
for (int i = 0; i < tests.Length; i++) {
    int result = FirstUniqChar(tests[i]);

    Console.WriteLine($"Input: \"{tests[i]}\"");
    Console.WriteLine($"FirstUniqChar -> {result} (expected {expected[i]})\n");
}
