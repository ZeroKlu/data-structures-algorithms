/**
 * Tsp
 * ---
 * Solves the Traveling Salesman Problem (TSP) using bitmask dynamic programming
 * (Held–Karp algorithm).
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
 *   dp[mask, i] = minimum cost to start at city 0, visit exactly the set of cities
 *                represented by `mask`, and end at city i.
 *
 * Bitmask representation:
 *   - mask is an integer with n bits (0..n-1).
 *   - If bit k is set, city k has been visited.
 *   - maxMask = 1 << n is the number of subsets of cities.
 *
 * Initialization:
 *   dp[1, 0] = 0
 *   (Only city 0 visited, current city is 0, cost so far is 0.)
 *   All other states start at INF (unreachable).
 *
 * Transition:
 *   For each state (mask, i) where i is included in mask:
 *     try to go to an unvisited city j:
 *       nextMask = mask | (1 << j)
 *       dp[nextMask, j] = min(dp[nextMask, j], dp[mask, i] + cost[i][j])
 *
 * Finalization:
 *   After visiting all cities (fullMask), close the tour by returning to city 0:
 *     ans = min_i (dp[fullMask, i] + cost[i][0])
 *
 * Complexity:
 *   Time:  O(2^n * n^2)
 *   Space: O(2^n * n)
 *
 * Notes about INF:
 *   Using int.MaxValue / 2 reduces overflow risk when computing:
 *     dp + cost (since dp is never near int.MaxValue).
 *
 * @param cost Cost matrix (n x n)
 * @return Minimum tour cost starting and ending at city 0, visiting all cities once
 */
static int Tsp(int[][] cost) {
    // Number of cities
    int n = cost.Length;

    // Total number of subsets of cities (2^n)
    int maxMask = 1 << n;

    // Sentinel value for "unreachable" DP states
    int INF = int.MaxValue / 2;

    // dp[mask, i] = best cost to start at 0, visit 'mask', and end at i
    int[,] dp = new int[maxMask, n];

    // Initialize all states to INF
    for (int mask = 0; mask < maxMask; mask++) {
        for (int i = 0; i < n; i++) {
            dp[mask, i] = INF;
        }
    }

    // Base case: only city 0 visited (mask=1), cost=0, ending at city 0
    dp[1, 0] = 0;

    // Iterate over all visited-city subsets
    for (int mask = 1; mask < maxMask; mask++) {
        // Try each city i as the current endpoint
        for (int i = 0; i < n; i++) {
            // If city i is not in the visited set for this mask, skip
            if ((mask & (1 << i)) == 0) continue;

            int curCost = dp[mask, i];

            // Skip unreachable states
            if (curCost == INF) continue;

            // Try extending the path to an unvisited city j
            for (int j = 0; j < n; j++) {
                // If j is already visited, we cannot visit it again
                if ((mask & (1 << j)) != 0) continue;

                int nextMask = mask | (1 << j);
                int cand = curCost + cost[i][j];

                // Relaxation: keep the best cost to reach (nextMask, j)
                if (cand < dp[nextMask, j]) {
                    dp[nextMask, j] = cand;
                }
            }
        }
    }

    // Mask with all cities visited
    int fullMask = maxMask - 1;

    // Close the tour by returning to city 0 from the final city i
    int ans = INF;
    for (int i = 0; i < n; i++) {
        // dp[fullMask, i] is best cost to visit all cities and end at i
        // Add cost to return to start and form a cycle
        int cand = dp[fullMask, i] + cost[i][0];
        if (cand < ans) ans = cand;
    }

    return ans;
}

/**
 * Test
 * ----
 * Helper method to run a single TSP test case and print:
 *   - test name
 *   - cost matrix
 *   - computed tour cost (including return to city 0)
 *   - expected result
 *
 * @param name      Descriptive test name
 * @param cost      Cost matrix
 * @param expected  Expected optimal tour cost
 */
static void Test(string name, int[][] cost, int expected)
{
    // Run the solver
    int result = Tsp(cost);

    // Print test details
    Console.WriteLine(name);
    Console.WriteLine("Cost matrix:");
    foreach (var row in cost)
        Console.WriteLine("  [" + string.Join(",", row) + "]");

    // Print result vs expected
    Console.WriteLine($"Tsp = {result} (expected {expected})\n");
}

// ===========================
// Test Harness
// ===========================

Console.WriteLine("=== Testing Tsp (Bitmask DP) ===\n");

// -----------------------------
// Test 1: Classic 4-city example
// -----------------------------
// Best tour cost including return to 0 is 80:
// 0 -> 1 -> 3 -> 2 -> 0
Test(
    "Test 1: Classic 4-city example",
    [
        [0,10,15,20],
        [10,0,35,25],
        [15,35,0,30],
        [20,25,30,0],
    ],
    80
);

// -----------------------------
// Test 2: 3-city triangle
// -----------------------------
// Both tours cost 7:
// 0 -> 1 -> 2 -> 0 or 0 -> 2 -> 1 -> 0
Test(
    "Test 2: 3-city triangle",
    [
        [0,1,4],
        [1,0,2],
        [4,2,0],
    ],
    7
);

// -----------------------------
// Test 3: 4-city square loop
// -----------------------------
// One optimal tour: 0 -> 1 -> 2 -> 3 -> 0, cost = 4
Test(
    "Test 3: 4-city square",
    [
        [0,1,2,1],
        [1,0,1,2],
        [2,1,0,1],
        [1,2,1,0],
    ],
    4
);
