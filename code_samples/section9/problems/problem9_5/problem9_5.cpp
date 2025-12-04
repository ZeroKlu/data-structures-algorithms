#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

int minCost(int n, vector<int> cuts) {
    cuts.push_back(0);
    cuts.push_back(n);
    sort(cuts.begin(), cuts.end());
    int m = (int)cuts.size();
    const int INF = numeric_limits<int>::max() / 2;
    vector<vector<int> > dp(m, vector<int>(m, 0));

    for (int len = 2; len < m; len++) {
        for (int i = 0; i + len < m; i++) {
            int j = i + len;
            int best = INF;
            for (int k = i + 1; k < j; k++) {
                int cand = dp[i][k] + dp[k][j] + (cuts[j] - cuts[i]);
                best = min(best, cand);
            }
            dp[i][j] = (best == INF ? 0 : best);
        }
    }
    return dp[0][m - 1];
}

void test(const string &name, int n, const vector<int> &cuts, int expected) {
    int result = minCost(n, cuts);
    cout << name << "\n";
    cout << "n = " << n << ", cuts = {";
    for (size_t i = 0; i < cuts.size(); i++) {
        cout << cuts[i] << (i + 1 == cuts.size() ? "" : ",");
    }
    cout << "}\n";

    cout << "minCost = " << result 
         << " (expected " << expected << ")\n\n";
}

int main() {
    cout << "=== Testing minCost (Cutting Sticks DP) ===\n\n";

    // ----------------------------------------------
    // Test 1: Classic LC example
    // n = 7, cuts = {1,3,4,5}
    // Expected = 16
    // ----------------------------------------------
    test("Test 1: Classic example",
         7, {1, 3, 4, 5}, 16);

    // ----------------------------------------------
    // Test 2: Two cuts only
    // n = 9, cuts = {5,6}
    // Expected = 13
    // ----------------------------------------------
    test("Test 2: Two cuts",
         9, {5, 6}, 13);

    // ----------------------------------------------
    // Test 3: Single cut
    // n = 10, cuts = {4}
    // Expected = 10
    // ----------------------------------------------
    test("Test 3: Single cut",
         10, {4}, 10);

    // ----------------------------------------------
    // Test 4: Even spacing
    // n = 10, cuts = {2,4,6,8}
    // Correct expected cost = 24 (not 34!)
    // ----------------------------------------------
    test("Test 4: Even spacing",
         10, {2, 4, 6, 8}, 24);

    // ----------------------------------------------
    // Test 5: No cuts
    // n = 100, cuts = {}
    // Expected = 0
    // ----------------------------------------------
    test("Test 5: No cuts",
         100, {}, 0);

    return 0;
}
