/**
 * firstUniqChar
 *
 * Purpose:
 *   Finds the index of the first character in a string that appears exactly once.
 *   This is the JavaScript version of the classic "first unique character" problem.
 *
 * Parameters:
 *   s - Input string to examine.
 *
 * Returns:
 *   - The 0-based index of the first unique character.
 *   - -1 if no such character exists or the string is empty.
 *
 * Approach:
 *   Two-pass algorithm:
 *     1) Count character frequencies using a plain object.
 *     2) Scan the string again to find the first index with frequency === 1.
 *
 * Complexity:
 *   - Time:  O(n), where n = s.length
 *   - Space: O(k), where k = number of distinct characters
 *
 * Important notes:
 *   - This implementation iterates over JavaScript string *code units*.
 *     For most ASCII / BMP characters this is fine.
 *     Characters represented by surrogate pairs (e.g., some emojis)
 *     would be treated as two separate units.
 *   - A Map could also be used instead of an object to avoid any
 *     potential prototype-related edge cases.
 */
function firstUniqChar(s) {
    // Frequency table: character -> count
    const freq = {};

    // First pass: count occurrences of each character
    for (const ch of s) {
        // If freq[ch] is undefined, treat it as 0
        freq[ch] = (freq[ch] || 0) + 1;
    }

    // Second pass: find the first index whose character appears exactly once
    for (let i = 0; i < s.length; i++) {
        if (freq[s[i]] === 1) {
            return i;
        }
    }

    // No unique character found
    return -1;
}

// ======================
// Test cases
// ======================
//
// Simple console-based test harness to validate behavior.
//

const tests = [
    "leetcode",       // expected 0 ('l')
    "loveleetcode",   // expected 2 ('v')
    "aabbcc",         // expected -1
    "",               // expected -1 (empty string)
    "x",              // expected 0
    "aabbc",          // expected 4 ('c')
    "swiss"           // expected 1 ('w')
];

const expected = [
    0,
    2,
    -1,
    -1,
    0,
    4,
    1
];

console.log("=== Testing firstUniqChar ===\n");

// Run all test cases and print results
tests.forEach((s, i) => {
    const result = firstUniqChar(s);

    console.log(`Input: "${s}"`);
    console.log(`firstUniqChar -> ${result} (expected ${expected[i]})\n`);
});
