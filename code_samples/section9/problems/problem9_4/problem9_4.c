#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

/**
 * tsp
 * ---
 * Solves the Traveling Salesman Problem (TSP) for a complete graph of `n` cities
 * using bitmask dynamic programming (Held–Karp algorithm).
 *
 * Problem variant solved here:
 *   - Start at city 0
 *   - Visit every city exactly once
 *   - Return to city 0
 *   - Minimize total tour cost
 *
 * Input:
 *   cost is an n x n matrix where cost[i][j] is the travel cost from city i to j.
 *
 * DP definition:
 *   dp[mask][i] = minimum cost to start at city 0, visit exactly the set of cities
 *                indicated by `mask`, and end at city i.
 *
 * Representation:
 *   - `mask` is a bitmask over cities 0..n-1 (bit k set means city k is visited).
 *   - maxMask = 1 << n is the total number of subsets.
 *
 * Initialization:
 *   dp[1][0] = 0
 *   (mask=1 means only city 0 is visited, and we are currently at city 0.)
 *   All other states start as "infinite" (unreachable).
 *
 * Transition:
 *   For each state (mask, i) where city i is in mask:
 *     try to go to any city j not in mask:
 *       nextMask = mask | (1<<j)
 *       dp[nextMask][j] = min(dp[nextMask][j], dp[mask][i] + cost[i][j])
 *
 * Final answer:
 *   After all cities are visited (fullMask), we must return to city 0:
 *     ans = min_i (dp[fullMask][i] + cost[i][0])
 *
 * Complexity:
 *   Time:  O(2^n * n^2)
 *   Space: O(2^n * n)
 *
 * Notes on "infinity":
 *   - Uses INT_MAX / 2 to reduce risk of overflow when adding costs
 *     (dp value + edge cost).
 *
 * Memory management:
 *   - Allocates dp as an array of pointers, then each dp[mask] row.
 *   - Frees all rows and the dp pointer array before returning.
 *
 * Parameters:
 *   cost - cost matrix, provided as int** where cost[i] points to row i
 *   n    - number of cities
 *
 * Returns:
 *   Minimum tour cost starting and ending at city 0, visiting all cities once.
 */
int tsp(int **cost, int n) {
    // Total number of subsets of cities.
    int maxMask = 1 << n;

    // Allocate dp as an array of maxMask rows, each with n columns.
    int **dp = (int **)malloc(maxMask * sizeof(int *));
    for (int mask = 0; mask < maxMask; mask++) {
        dp[mask] = (int *)malloc(n * sizeof(int));
        // Initialize all states as "infinite/unreachable".
        for (int i = 0; i < n; i++) {
            dp[mask][i] = INT_MAX / 2;
        }
    }

    // Starting condition: only city 0 visited (mask=1), cost=0, at city 0.
    dp[1][0] = 0; // mask 1: only city 0 visited

    // Iterate over all visited-subset masks.
    for (int mask = 1; mask < maxMask; mask++) {
        // Try all possible endpoints i that are included in mask.
        for (int i = 0; i < n; i++) {
            // If city i not visited in this mask, dp[mask][i] is not a valid endpoint.
            if (!(mask & (1 << i))) continue;

            int curCost = dp[mask][i];
            // Skip unreachable states.
            if (curCost == INT_MAX / 2) continue;

            // Try to extend the tour to any unvisited city j.
            for (int j = 0; j < n; j++) {
                // If city j is already visited, we cannot visit it again.
                if (mask & (1 << j)) continue;

                int nextMask = mask | (1 << j);
                int cand = curCost + cost[i][j];

                // Relaxation: keep best known cost to reach (nextMask, j).
                if (cand < dp[nextMask][j]) {
                    dp[nextMask][j] = cand;
                }
            }
        }
    }

    // Mask with all cities visited.
    int fullMask = maxMask - 1;

    // Close the tour by returning to city 0 from the final city i.
    int ans = INT_MAX;
    for (int i = 0; i < n; i++) {
        // dp[fullMask][i] is the best cost ending at i after visiting all cities.
        // Add cost to return to start (city 0).
        int cand = dp[fullMask][i] + cost[i][0];
        if (cand < ans) ans = cand;
    }

    // Free allocated dp rows and the dp pointer array.
    for (int mask = 0; mask < maxMask; mask++) {
        free(dp[mask]);
    }
    free(dp);

    return ans;
}

// ===========================
// Tests
// ===========================

/**
 * test_tsp
 * --------
 * Helper function to run a single TSP test case and print:
 *   - test name
 *   - number of cities
 *   - computed tour cost (including return to start)
 *   - expected result
 *
 * Parameters:
 *   name     - descriptive test label
 *   cost     - cost matrix (int**)
 *   n        - number of cities
 *   expected - expected optimal tour cost
 */
void test_tsp(const char *name, int **cost, int n, int expected) {
    int result = tsp(cost, n);
    printf("%s\n", name);
    printf("n = %d\n", n);
    printf("tsp tour cost (with return to start) = %d (expected %d)\n\n",
           result, expected);
}

/**
 * main
 * ----
 * Test harness for tsp() bitmask DP implementation.
 *
 * Each test constructs a small cost matrix and verifies the returned tour cost.
 *
 * Note on matrix setup:
 *   The tsp() function expects an int** where each row pointer points to
 *   contiguous int elements. In these tests, rawX is a stack-allocated 2D array,
 *   and costX is an array of row pointers referencing rawX rows.
 */
int main(void) {
    printf("=== Testing tsp (bitmask DP) ===\n\n");

    // ---- Test 1: 4-node classic example ----
    // Cost matrix:
    //   0   10  15  20
    //   10  0   35  25
    //   15  35  0   30
    //   20  25  30  0
    //
    // Best tour cost including return to 0 is 80:
    //   0 -> 1 -> 3 -> 2 -> 0
    //   10 + 25 + 30 + 15 = 80
    int n1 = 4;
    int raw1[4][4] = {
        { 0, 10, 15, 20 },
        { 10, 0, 35, 25 },
        { 15, 35, 0, 30 },
        { 20, 25, 30, 0 }
    };

    // cost1 is an array of pointers, each pointing to a row in raw1.
    int *cost1[4];
    for (int i = 0; i < n1; i++) {
        cost1[i] = raw1[i];
    }
    test_tsp("Test 1: 4-node classic example", cost1, n1, 80);

    // ---- Test 2: 3-node triangle ----
    // Cost matrix:
    //   0  1  4
    //   1  0  2
    //   4  2  0
    //
    // Possible tours:
    //   0 -> 1 -> 2 -> 0 = 1 + 2 + 4 = 7
    //   0 -> 2 -> 1 -> 0 = 4 + 2 + 1 = 7
    // So optimal TSP cost = 7
    int n2 = 3;
    int raw2[3][3] = {
        { 0, 1, 4 },
        { 1, 0, 2 },
        { 4, 2, 0 }
    };

    int *cost2[3];
    for (int i = 0; i < n2; i++) {
        cost2[i] = raw2[i];
    }
    test_tsp("Test 2: 3-node triangle", cost2, n2, 7);

    // ---- Test 3: 4-node square loop ----
    //   0 --1-- 1
    //   |       |
    //   1       1
    //   |       |
    //   3 --1-- 2
    //
    // Cost matrix (symmetric):
    //   0  1  2  1
    //   1  0  1  2
    //   2  1  0  1
    //   1  2  1  0
    //
    // One optimal tour: 0 -> 1 -> 2 -> 3 -> 0
    // cost = 1 + 1 + 1 + 1 = 4
    int n3 = 4;
    int raw3[4][4] = {
        { 0, 1, 2, 1 },
        { 1, 0, 1, 2 },
        { 2, 1, 0, 1 },
        { 1, 2, 1, 0 }
    };

    int *cost3[4];
    for (int i = 0; i < n3; i++) {
        cost3[i] = raw3[i];
    }
    test_tsp("Test 3: 4-node square", cost3, n3, 4);

    return 0;
}
