#include <iostream>
#include <vector>
using namespace std;

int uniquePaths(int m, int n) {
    vector<vector<int>> dp(m, vector<int>(n, 1));
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }
    return dp[m - 1][n - 1];
}

int main() {
    cout << "=== Test: uniquePaths ===\n\n";

    struct TestCase {
        int m, n;
        int expected;
    };

    vector<TestCase> tests = {
        {1, 1, 1},
        {1, 5, 1},
        {3, 3, 6},
        {3, 7, 28},
        {3, 2, 3},
        {7, 3, 28},
        {10, 10, 48620}
    };

    for (const auto& t : tests) {
        int result = uniquePaths(t.m, t.n);
        cout << "uniquePaths(" << t.m << ", " << t.n << ") = "
             << result << " (expected " << t.expected << ")\n";
    }

    return 0;
}
