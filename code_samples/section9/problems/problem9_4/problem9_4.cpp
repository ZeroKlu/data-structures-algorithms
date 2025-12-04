#include <iostream>
#include <vector>
#include <limits>
using namespace std;

int tsp(const vector<vector<int> > &cost) {
    int n = (int)cost.size();
    int maxMask = 1 << n;
    const int INF = numeric_limits<int>::max() / 2;
    vector<vector<int> > dp(maxMask, vector<int>(n, INF));

    dp[1][0] = 0; // start at city 0

    for (int mask = 1; mask < maxMask; mask++) {
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i))) continue;
            int curCost = dp[mask][i];
            if (curCost == INF) continue;
            for (int j = 0; j < n; j++) {
                if (mask & (1 << j)) continue;
                int nextMask = mask | (1 << j);
                int cand = curCost + cost[i][j];
                if (cand < dp[nextMask][j]) {
                    dp[nextMask][j] = cand;
                }
            }
        }
    }

    int fullMask = maxMask - 1;
    int ans = INF;
    for (int i = 0; i < n; i++) {
        int cand = dp[fullMask][i] + cost[i][0];
        if (cand < ans) ans = cand;
    }
    return ans;
}

void test(const string &name, const vector<vector<int>> &cost, int expected) {
    int result = tsp(cost);
    cout << name << "\n";
    cout << "Matrix:\n";
    for (const auto &row : cost) {
        cout << "  ";
        for (int x : row) cout << x << " ";
        cout << "\n";
    }
    cout << "tsp() = " << result 
         << " (expected " << expected << ")\n\n";
}

int main() {
    cout << "=== Testing tsp (bitmask DP) ===\n\n";

    // --------------------------------------------------------
    // Test 1: Classic 4-city example
    // --------------------------------------------------------
    // Cost matrix:
    //   0  10 15 20
    //   10  0 35 25
    //   15 35  0 30
    //   20 25 30  0
    //
    // Best complete tour (returning to 0) costs 80:
    //   0 → 1 → 3 → 2 → 0
    //   10 + 25 + 30 + 15 = 80
    {
        vector<vector<int>> cost = {
            {0, 10, 15, 20},
            {10, 0, 35, 25},
            {15, 35, 0, 30},
            {20, 25, 30, 0}
        };
        test("Test 1: Classic 4-city example", cost, 80);
    }

    // --------------------------------------------------------
    // Test 2: 3-city triangle
    // --------------------------------------------------------
    // Cost matrix:
    //   0  1  4
    //   1  0  2
    //   4  2  0
    //
    // Both tours cost 7:
    //   0 → 1 → 2 → 0 = 1 + 2 + 4 = 7
    //   0 → 2 → 1 → 0 = 4 + 2 + 1 = 7
    {
        vector<vector<int>> cost = {
            {0, 1, 4},
            {1, 0, 2},
            {4, 2, 0}
        };
        test("Test 2: 3-city triangle", cost, 7);
    }

    // --------------------------------------------------------
    // Test 3: Symmetric 4-city square
    // --------------------------------------------------------
    // Cost matrix:
    //   0 1 2 1
    //   1 0 1 2
    //   2 1 0 1
    //   1 2 1 0
    //
    // One optimal tour: 0 → 1 → 2 → 3 → 0
    //   cost = 1 + 1 + 1 + 1 = 4
    {
        vector<vector<int>> cost = {
            {0, 1, 2, 1},
            {1, 0, 1, 2},
            {2, 1, 0, 1},
            {1, 2, 1, 0}
        };
        test("Test 3: 4-city square", cost, 4);
    }

    return 0;
}
