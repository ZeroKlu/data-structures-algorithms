#include <stdio.h>

int firstUniqChar(const char *s) {
    int freq[256] = {0};
    const unsigned char *p;

    // First pass: count frequencies
    for (p = (const unsigned char *)s; *p != '\0'; ++p) {
        freq[*p]++;
    }

    // Second pass: find first index with freq == 1
    int idx = 0;
    for (p = (const unsigned char *)s; *p != '\0'; ++p, ++idx) {
        if (freq[*p] == 1) {
            return idx;
        }
    }
    return -1;
}

// ==============================
// Top-level tests
// ==============================

int main(void) {
    const char *tests[] = {
        "leetcode",      // expected 0 ('l')
        "loveleetcode",  // expected 2 ('v')
        "aabbcc",        // expected -1
        "",              // empty → -1
        "x",             // expected 0
        "aabbc",         // expected 4 ('c')
        "swiss"          // expected 0 ('s' appears 3 times but first unique is 'w' at index 1)
    };

    int expected[] = {
        0,   // "leetcode"
        2,   // "loveleetcode"
        -1,  // "aabbcc"
        -1,  // ""
        0,   // "x"
        4,   // "aabbc"
        1    // "swiss"
    };

    int count = sizeof(tests) / sizeof(tests[0]);

    printf("=== Testing firstUniqChar ===\n\n");

    for (int i = 0; i < count; i++) {
        int result = firstUniqChar(tests[i]);
        printf("Input: \"%s\"\n", tests[i]);
        printf("firstUniqChar -> %d (expected %d)\n\n", result, expected[i]);
    }

    return 0;
}
