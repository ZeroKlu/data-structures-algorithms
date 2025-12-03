static int FirstUniqChar(string s) {
    var freq = new Dictionary<char, int>();
    foreach (char ch in s) {
        freq[ch] = freq.TryGetValue(ch, out int c) ? c + 1 : 1;
    }
    for (int i = 0; i < s.Length; i++) {
        if (freq[s[i]] == 1) {
            return i;
        }
    }
    return -1;
}

// ============================
// Top-level tests
// ============================

string[] tests = {
    "leetcode",      // expected 0
    "loveleetcode",  // expected 2
    "aabbcc",        // expected -1
    "",              // expected -1
    "x",             // expected 0
    "aabbc",         // expected 4
    "swiss"          // expected 1 ('w')
};

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

for (int i = 0; i < tests.Length; i++) {
    int result = FirstUniqChar(tests[i]);
    Console.WriteLine($"Input: \"{tests[i]}\"");
    Console.WriteLine($"FirstUniqChar -> {result} (expected {expected[i]})\n");
}
