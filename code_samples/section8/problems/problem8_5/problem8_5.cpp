#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int longestCommonSubsequence(const string &text1, const string &text2) {
    int m = (int)text1.size();
    int n = (int)text2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

int main() {
    cout << "=== Test: longestCommonSubsequence ===\n\n";

    struct TestCase {
        string s1;
        string s2;
        int expected;
    };

    vector<TestCase> tests = {
        {"abcde", "ace", 3},
        {"abc", "abc", 3},
        {"abc", "def", 0},
        {"AGGTAB", "GXTXAYB", 4},
        {"bl", "yby", 1},
        {"aaaa", "aa", 2},
        {"", "", 0},
        {"abc", "", 0},
        {"", "abc", 0}
    };

    for (const auto &t : tests) {
        int result = longestCommonSubsequence(t.s1, t.s2);
        cout << "LCS(\"" << t.s1 << "\", \"" << t.s2
             << "\") = " << result
             << " (expected " << t.expected << ")\n";
    }

    return 0;
}
