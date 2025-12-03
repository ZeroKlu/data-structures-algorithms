#include <iostream>
#include <string>
#include <vector>
using namespace std;

int firstUniqChar(const string &s) {
    vector<int> freq(256, 0);
    for (unsigned char ch : s) {
        freq[ch]++;
    }
    for (int i = 0; i < (int)s.size(); i++) {
        if (freq[(unsigned char)s[i]] == 1) {
            return i;
        }
    }
    return -1;
}

int main() {
    vector<string> tests = {
        "leetcode",       // expected 0 ('l')
        "loveleetcode",   // expected 2 ('v')
        "aabbcc",         // expected -1
        "",               // expected -1
        "x",              // expected 0
        "aabbc",          // expected 4 ('c')
        "swiss"           // expected 1 ('w')
    };

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

    for (size_t i = 0; i < tests.size(); i++) {
        int result = firstUniqChar(tests[i]);
        cout << "Input: \"" << tests[i] << "\"\n";
        cout << "firstUniqChar -> " << result
             << " (expected " << expected[i] << ")\n\n";
    }

    return 0;
}
