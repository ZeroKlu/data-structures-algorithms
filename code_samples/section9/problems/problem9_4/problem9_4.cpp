#include <iostream>
#include <vector>
#include <limits>
using namespace std;

/**
 * tsp
 * ---
 * Solves the Traveling Salesman Problem (TSP) using bitmask dynamic programming
 * (the Held–Karp algorithm).
 *
 * Problem variant solved here:
 *   - Start at city 0
 *   - Visit every city exactly once
 *   - Return to city 0
 *   - Minimize total tour cost
 *
 * Input:
 *   `cost` is an n x n matrix where cost[i][j] is the travel cost from city i to j.
 *
 * DP definition:
 *   dp[mask][i] = minimum cost to start at city 0, visit exactly the set of cities
 *                represented by `mask`, and finish at city i.
 *
 * Bitmask representation:
 *   - mask has n bits (0..n-1).
 *   - If bit k is set, city k has been visited.
 *   - maxMask = 1 << n is the number of subsets of cities.
 *
 * Initialization:
 *   dp[1][0] = 0
 *   (Only city 0 visited, and current location is city 0 with cost 0.)
 *   All other states start as INF (unreachable).
 *
 * Transition:
 *   For each visited-set mask and endpoint i:
 *     try to go to an unvisited city j:
 *       nextMask = mask | (1 << j)
 *       dp[nextMask][j] = min(dp[nextMask][j], dp[mask][i] + cost[i][j])
 *
 * Finalization:
 *   Once all cities are visited (fullMask), we must return to city 0:
 *     ans = min_i (dp[fullMask][i] + cost[i][0])
 *
 * Complexity:
 *   Time:  O(2^n * n^2)
 *   Space: O(2^n * n)
 *
 * Notes about INF:
 *   Using max()/2 reduces overflow risk when adding costs:
 *     dp + cost might overflow if dp were near INT_MAX.
 *
 * @param cost Square cost matrix (n x n)
 * @return Minimum tour cost starting/ending at city 0 visiting all cities once
 */
int tsp(const vector<vector<int> > &cost) {
    // Number of cities is derived from matrix size.
    int n = (int)cost.size();

    // Total number of subsets of cities.
    int maxMask = 1 << n;

    // Large sentinel value for "unreachable" DP states.
    const int INF = numeric_limits<int>::max() / 2;

    // dp[mask][i] initialized to INF: initially, no states are reachable except dp[1][0].
    vector<vector<int> > dp(maxMask, vector<int>(n, INF));

    // Base state: only city 0 visited (mask=1), currently at city 0, cost 0.
    dp[1][0] = 0; // start at city 0

    // Iterate through all subsets of visited cities.
    for (int mask = 1; mask < maxMask; mask++) {
        // Consider each city i as the current endpoint.
        for (int i = 0; i < n; i++) {
            // If i is not included in mask, we cannot be "ending at i" for this mask.
            if (!(mask & (1 << i))) continue;

            int curCost = dp[mask][i];

            // Skip unreachable states.
            if (curCost == INF) continue;

            // Try to go to any unvisited city j next.
            for (int j = 0; j < n; j++) {
                // If j is already visited, we cannot visit it again.
                if (mask & (1 << j)) continue;

                int nextMask = mask | (1 << j);
                int cand = curCost + cost[i][j];

                // Relaxation step: keep the best cost for reaching (nextMask, j).
                if (cand < dp[nextMask][j]) {
                    dp[nextMask][j] = cand;
                }
            }
        }
    }

    // Mask where all cities are visited.
    int fullMask = maxMask - 1;

    // Close the tour by returning to city 0 from whichever city we end at.
    int ans = INF;
    for (int i = 0; i < n; i++) {
        // dp[fullMask][i] is best cost to visit all cities and end at i.
        // Add cost[i][0] to return to start and form a complete cycle.
        int cand = dp[fullMask][i] + cost[i][0];
        if (cand < ans) ans = cand;
    }

    return ans;
}

/**
 * test
 * ----
 * Helper function to run a TSP test case and print:
 *   - test name
 *   - the cost matrix
 *   - computed tsp tour cost (including return to 0)
 *   - expected result
 *
 * @param name      Descriptive test name
 * @param cost      Cost matrix
 * @param expected  Expected optimal tour cost
 */
void test(const string &name, const vector<vector<int>> &cost, int expected) {
    // Run the TSP solver
    int result = tsp(cost);

    // Print test details
    cout << name << "\n";
    cout << "Matrix:\n";
    for (const auto &row : cost) {
        cout << "  ";
        for (int x : row) cout << x << " ";
        cout << "\n";
    }

    // Print result vs expected
    cout << "tsp() = " << result
         << " (expected " << expected << ")\n\n";
}

/**
 * main
 * ----
 * Test harness for tsp() bitmask DP implementation.
 *
 * Includes:
 *   - Classic 4-city symmetric example
 *   - 3-city triangle with equal optimal tours
 *   - 4-city "square" structure with minimal cycle cost
 */
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
