#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

long long fib_dp(int n) {
    if (n <= 1) return n;
    long long prev2 = 0; // F(0)
    long long prev1 = 1; // F(1)
    for (int i = 2; i <= n; i++) {
        long long curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

long long climbStairs(int n) {
    if (n <= 1) return 1;
    long long prev2 = 1; // ways(0)
    long long prev1 = 1; // ways(1)
    for (int i = 2; i <= n; i++) {
        long long curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

int knapSack01(int n, int W, const vector<int>& w, const vector<int>& v) {
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < n; i++) {
        for (int c = W; c >= w[i]; c--) {
            dp[c] = max(dp[c], dp[c - w[i]] + v[i]);
        }
    }
    return dp[W];
}

int lcsLength(const string& s, const string& t) {
    int n = (int)s.size();
    int m = (int)t.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s[i - 1] == t[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n][m];
}

int main() {

    /* ============================
       Test fib_dp
       ============================ */
    cout << "=== Test: fib_dp ===\n\n";

    struct FibTest { int n; long long expected; };
    vector<FibTest> fibTests = {
        {0, 0},
        {1, 1},
        {2, 1},
        {3, 2},
        {4, 3},
        {5, 5},
        {10, 55},
        {20, 6765},
        {30, 832040},
        {50, 12586269025LL}
    };

    for (auto &t : fibTests) {
        long long r = fib_dp(t.n);
        cout << "fib_dp(" << t.n << ") = " << r
             << " (expected " << t.expected << ")\n";
    }


    /* ============================
       Test climbStairs
       ============================ */
    cout << "\n=== Test: climbStairs ===\n\n";

    struct ClimbTest { int n; long long expected; };
    vector<ClimbTest> climbTests = {
        {0, 1},
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 5},
        {5, 8},
        {10, 89}
    };

    for (auto &t : climbTests) {
        long long r = climbStairs(t.n);
        cout << "climbStairs(" << t.n << ") = " << r
             << " (expected " << t.expected << ")\n";
    }


    /* ============================
       Test knapSack01
       ============================ */
    cout << "\n=== Test: knapSack01 ===\n\n";

    {
        vector<int> w = {2, 3, 4, 5};
        vector<int> v = {3, 4, 5, 6};
        int n = w.size();

        int W1 = 5, expected1 = 7;
        int r1 = knapSack01(n, W1, w, v);
        cout << "knapSack01(W=5) = " << r1 << " (expected 7)\n";

        int W2 = 7, expected2 = 9;
        int r2 = knapSack01(n, W2, w, v);
        cout << "knapSack01(W=7) = " << r2 << " (expected 9)\n";

        int W3 = 3, expected3 = 4;
        int r3 = knapSack01(n, W3, w, v);
        cout << "knapSack01(W=3) = " << r3 << " (expected 4)\n";
    }


    /* ============================
       Test lcsLength
       ============================ */
    cout << "\n=== Test: lcsLength ===\n\n";

    struct LcsTest { string s, t; int expected; };
    vector<LcsTest> lcsTests = {
        {"abcde", "ace", 3},
        {"abc", "abc", 3},
        {"abc", "def", 0},
        {"", "abc", 0},
        {"AGGTAB", "GXTXAYB", 4}
    };

    for (auto &t : lcsTests) {
        int r = lcsLength(t.s, t.t);
        cout << "lcsLength(\"" << t.s << "\", \"" << t.t
             << "\") = " << r << " (expected " << t.expected << ")\n";
    }

    return 0;
}
