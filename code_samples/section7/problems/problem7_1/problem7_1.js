function firstUniqChar(s) {
    const freq = {};
    for (const ch of s) {
        freq[ch] = (freq[ch] || 0) + 1;
    }
    for (let i = 0; i < s.length; i++) {
        if (freq[s[i]] === 1) {
            return i;
        }
    }
    return -1;
}

// ======================
// Test cases
// ======================

const tests = [
    "leetcode",       // expected 0
    "loveleetcode",   // expected 2
    "aabbcc",         // expected -1
    "",               // expected -1
    "x",              // expected 0
    "aabbc",          // expected 4
    "swiss"           // expected 1
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

tests.forEach((s, i) => {
    const result = firstUniqChar(s);
    console.log(`Input: "${s}"`);
    console.log(`firstUniqChar -> ${result} (expected ${expected[i]})\n`);
});
