#include <stdio.h>

/**
 * firstUniqChar
 *
 * Purpose:
 *   Finds the index of the first character in a C-string that appears exactly once.
 *   This is the classic "first unique character in a string" problem.
 *
 * Signature:
 *   int firstUniqChar(const char *s)
 *
 * Parameters:
 *   s - NUL-terminated C string (must not be NULL)
 *
 * Returns:
 *   - The 0-based index of the first character with frequency == 1
 *   - -1 if no unique character exists (or string is empty)
 *
 * Approach:
 *   Two-pass algorithm:
 *     1) Count frequencies of each byte value (0..255)
 *     2) Scan again to find the first index whose byte has frequency 1
 *
 * Complexity:
 *   - Time:  O(n) where n is the length of the string (two linear passes)
 *   - Space: O(1) fixed size array freq[256]
 *
 * Important notes / assumptions:
 *   - This implementation counts raw *bytes*, not Unicode code points.
 *     For ASCII / extended single-byte encodings it's fine. For UTF-8,
 *     multi-byte characters will be treated as multiple separate bytes.
 *   - We use unsigned char for indexing to avoid negative array indices
 *     when chars have the high bit set (implementation-defined if char is signed).
 */
int firstUniqChar(const char *s) {
    // Frequency table for all possible unsigned byte values.
    // freq[b] will store the number of times byte value b appears in the string.
    int freq[256] = {0};

    // Use unsigned char* so that values range 0..255, safe for array indexing.
    const unsigned char *p;

    // ----------------------------
    // First pass: count frequencies
    // ----------------------------
    //
    // Walk the string until the NUL terminator. For each byte, increment its counter.
    for (p = (const unsigned char *)s; *p != '\0'; ++p) {
        freq[*p]++;
    }

    // --------------------------------------------
    // Second pass: find first index with freq == 1
    // --------------------------------------------
    //
    // idx tracks the current character index in the original string.
    // We scan again so we can return the earliest (left-most) unique character.
    int idx = 0;
    for (p = (const unsigned char *)s; *p != '\0'; ++p, ++idx) {
        // If this character's total count is exactly 1, it's the first unique.
        if (freq[*p] == 1) {
            return idx;
        }
    }

    // No unique character found (or string empty).
    return -1;
}

// ==============================
// Top-level tests
// ==============================
//
// This is a simple console-driven test harness (not a unit test framework).
// It calls firstUniqChar on several inputs and prints results vs expectations.
//

int main(void) {
    // Test cases as NUL-terminated C strings.
    // Comments state the intended expected output and reasoning.
    const char *tests[] = {
        "leetcode",      // expected 0 ('l' appears once and is first)
        "loveleetcode",  // expected 2 ('v' is first char that appears once)
        "aabbcc",        // expected -1 (all chars appear twice)
        "",              // empty → -1 (no characters at all)
        "x",             // expected 0 (single char is unique)
        "aabbc",         // expected 4 ('c' appears once at end)
        "swiss"          // expected 1 ('s' appears 3 times; 'w' appears once at index 1)
    };

    // Parallel array holding the expected result for each test case above.
    int expected[] = {
        0,   // "leetcode"
        2,   // "loveleetcode"
        -1,  // "aabbcc"
        -1,  // ""
        0,   // "x"
        4,   // "aabbc"
        1    // "swiss"
    };

    // Compute number of test cases safely.
    // sizeof(tests) is total bytes in array; sizeof(tests[0]) is bytes per element.
    int count = (int)(sizeof(tests) / sizeof(tests[0]));

    printf("=== Testing firstUniqChar ===\n\n");

    // Run each test and compare result to expected.
    for (int i = 0; i < count; i++) {
        int result = firstUniqChar(tests[i]);

        // Print input and outputs.
        // Note: Printing "" works fine (it prints nothing between quotes).
        printf("Input: \"%s\"\n", tests[i]);
        printf("firstUniqChar -> %d (expected %d)\n\n", result, expected[i]);
    }

    return 0;
}
