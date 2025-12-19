#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

/**
 * minCost
 * -------
 * Computes the minimum total cost to perform all required cuts on a stick
 * of length `n`.
 *
 * This is the classic "Cutting Sticks" / "Minimum Cost to Cut a Stick" dynamic
 * programming problem (e.g., LeetCode 1547).
 *
 * Problem statement:
 *   - You have a stick spanning positions [0, n].
 *   - You must cut at each position listed in `cuts`.
 *   - Each time you cut a segment, the cost of that cut equals the *length of
 *     the current segment* being cut.
 *   - After cutting, that segment splits into two smaller segments, and future
 *     cuts are performed on those smaller segments.
 *   - Goal: choose the order of cuts to minimize total cost.
 *
 * Core insight:
 *   - The optimal strategy is interval-based: for any segment [cuts[i], cuts[j]],
 *     if you choose the first cut at some k between them, the left and right
 *     sides become independent subproblems.
 *
 * Preprocessing:
 *   - Add endpoints 0 and n to the cuts list.
 *   - Sort the cuts so we can refer to contiguous intervals.
 *
 * DP definition:
 *   dp[i][j] = minimum cost to fully cut the segment from cuts[i] to cuts[j],
 *             assuming we must perform all cuts strictly between i and j
 *             (i.e., at indices i+1..j-1).
 *
 * Base case:
 *   - If j = i + 1, there are no cuts in between => dp[i][j] = 0.
 *
 * Transition:
 *   - Try each possible first cut k in (i+1..j-1):
 *       dp[i][j] = min_k ( dp[i][k] + dp[k][j] + (cuts[j] - cuts[i]) )
 *
 *   Explanation of the + (cuts[j] - cuts[i]) term:
 *     - No matter which cut you choose first inside [i, j], you must pay the
 *       segment length once for that first cut.
 *     - Then you pay the optimal costs of cutting the left and right subsegments.
 *
 * Complexity:
 *   - Let m = number of cut positions after adding endpoints (cutsSize + 2).
 *   - Time:  O(m^3) due to the triple nested loops (len, i, k).
 *   - Space: O(m^2) for the dp table.
 *
 * @param n    Total stick length.
 * @param cuts List of cut positions (passed by value so we can modify it safely).
 * @return     Minimum total cutting cost.
 */
int minCost(int n, vector<int> cuts) {
    // Add stick endpoints; they are not "cuts" but define the outer interval.
    cuts.push_back(0);
    cuts.push_back(n);

    // Sort to ensure intervals are well-defined and increasing.
    sort(cuts.begin(), cuts.end());

    // m is the number of positions (original cuts + 2 endpoints).
    int m = (int)cuts.size();

    // INF used as a large sentinel for "best not yet found".
    const int INF = numeric_limits<int>::max() / 2;

    // dp[i][j] initially 0; base cases (no internal cuts) naturally remain 0.
    vector<vector<int> > dp(m, vector<int>(m, 0));

    // len is the distance between i and j in index space (j = i + len).
    // Start at 2 because:
    //   len = 1 => adjacent endpoints => no cut inside => cost 0
    //   len >= 2 => may contain at least one cut inside
    for (int len = 2; len < m; len++) {
        for (int i = 0; i + len < m; i++) {
            int j = i + len;

            // Find the minimum cost for interval [i, j].
            int best = INF;

            // Try each possible first cut k between i and j.
            for (int k = i + 1; k < j; k++) {
                // Cost of cutting left + cost of cutting right + cost of first cut
                // on the current segment (cuts[j] - cuts[i]).
                int cand = dp[i][k] + dp[k][j] + (cuts[j] - cuts[i]);
                best = min(best, cand);
            }

            // If there were no valid k (happens only if j == i+1), best stays INF,
            // and the correct value is 0 (no cuts needed).
            dp[i][j] = (best == INF ? 0 : best);
        }
    }

    // Answer: minimum cost to cut the entire stick from 0 to n.
    return dp[0][m - 1];
}

/**
 * test
 * ----
 * Helper function to run a single minCost() test case and print:
 *   - test name
 *   - stick length n
 *   - cut positions
 *   - computed result and expected result
 *
 * @param name     Descriptive test name
 * @param n        Stick length
 * @param cuts     Cut positions (original list, without endpoints)
 * @param expected Expected minimum cost
 */
void test(const string &name, int n, const vector<int> &cuts, int expected) {
    // Run minCost; note minCost takes cuts by value, so it will not mutate `cuts`.
    int result = minCost(n, cuts);

    // Print test metadata
    cout << name << "\n";
    cout << "n = " << n << ", cuts = {";
    for (size_t i = 0; i < cuts.size(); i++) {
        cout << cuts[i] << (i + 1 == cuts.size() ? "" : ",");
    }
    cout << "}\n";

    // Print result vs expected
    cout << "minCost = " << result
         << " (expected " << expected << ")\n\n";
}

/**
 * main
 * ----
 * Test harness for the Cutting Sticks / Minimum Cost to Cut a Stick solution.
 *
 * Includes:
 *   - a classic known example (LeetCode)
 *   - small cut sets
 *   - edge cases (single cut, no cuts)
 *   - evenly spaced cuts example
 */
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
    // Expected = 24
    //
    // Note: This test includes a helpful clarification that the expected cost
    // is 24 (not 34), aligning the comment and the assertion.
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
