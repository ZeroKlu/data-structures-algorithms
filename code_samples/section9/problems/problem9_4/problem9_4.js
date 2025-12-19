/**
 * tsp
 * ---
 * Solves the Traveling Salesman Problem (TSP) using bitmask dynamic programming
 * (the Held–Karp algorithm).
 *
 * Problem variant solved:
 *   - Start at city 0
 *   - Visit every city exactly once
 *   - Return to city 0
 *   - Minimize total tour cost
 *
 * Input:
 *   cost[i][j] is the travel cost from city i to city j.
 *
 * DP definition:
 *   dp[mask][i] = minimum cost to start at city 0, visit exactly the set of
 *                cities represented by `mask`, and end at city i.
 *
 * Bitmask representation:
 *   - mask has n bits (0..n-1)
 *   - bit k = 1 means city k has been visited
 *   - maxMask = 1 << n is the number of subsets of cities
 *
 * Complexity:
 *   Time:  O(2^n * n^2)
 *   Space: O(2^n * n)
 *
 * @param {number[][]} cost - Square cost matrix
 * @returns {number} Minimum tour cost starting and ending at city 0
 */
function tsp(cost) {
    // Number of cities
    const n = cost.length;

    // Total number of subsets of cities
    const maxMask = 1 << n;

    // Sentinel value representing "infinite" cost
    const INF = 1e9;

    // dp[mask][i] = best cost to start at 0, visit cities in mask, and end at i
    const dp = Array.from({ length: maxMask }, () => new Array(n).fill(INF));

    // Base case:
    // mask = 1 (binary 000...001) means only city 0 is visited
    // cost to be at city 0 is 0
    dp[1][0] = 0;

    // Iterate over all subsets of visited cities
    for (let mask = 1; mask < maxMask; mask++) {
        // Try each city i as the current endpoint
        for (let i = 0; i < n; i++) {
            // If city i is not included in mask, skip
            if ((mask & (1 << i)) === 0) continue;

            const curCost = dp[mask][i];

            // Skip unreachable states
            if (curCost === INF) continue;

            // Try extending the path to an unvisited city j
            for (let j = 0; j < n; j++) {
                // If city j is already visited, skip
                if (mask & (1 << j)) continue;

                // New visited set after including city j
                const nextMask = mask | (1 << j);

                // Cost if we go from city i to city j
                const cand = curCost + cost[i][j];

                // Relaxation step: keep the minimum cost
                if (cand < dp[nextMask][j]) {
                    dp[nextMask][j] = cand;
                }
            }
        }
    }

    // Mask where all cities have been visited
    const fullMask = maxMask - 1;

    // Close the tour by returning to city 0
    let ans = INF;
    for (let i = 0; i < n; i++) {
        // Cost to end at city i after visiting all cities,
        // plus cost to return to the start (city 0)
        const cand = dp[fullMask][i] + cost[i][0];
        if (cand < ans) ans = cand;
    }

    // Final answer: minimum tour cost
    return ans;
}

// ===========================
// Test Harness
// ===========================

/**
 * Runs a single TSP test case and prints:
 *   - test name
 *   - cost matrix
 *   - computed tsp() value
 *   - expected value
 *
 * @param {string} name - Descriptive test name
 * @param {number[][]} cost - Cost matrix
 * @param {number} expected - Expected optimal tour cost
 */
function test(name, cost, expected) {
    const result = tsp(cost);
    console.log(name);
    console.log("Cost matrix:");
    cost.forEach(row => console.log("  " + JSON.stringify(row)));
    console.log(`tsp() = ${result} (expected ${expected})\n`);
}

// Header for all tests
console.log("=== Testing tsp (bitmask DP) ===\n");

// -----------------------------------------
// Test 1: Classic 4-city example
// -----------------------------------------
// Best tour: 0 → 1 → 3 → 2 → 0
// Cost = 10 + 25 + 30 + 15 = 80
test(
    "Test 1: Classic 4-city example",
    [
        [0, 10, 15, 20],
        [10, 0, 35, 25],
        [15, 35, 0, 30],
        [20, 25, 30, 0]
    ],
    80
);

// -----------------------------------------
// Test 2: 3-city triangle
// -----------------------------------------
// Both possible tours cost 7
test(
    "Test 2: 3-city triangle",
    [
        [0, 1, 4],
        [1, 0, 2],
        [4, 2, 0]
    ],
    7
);

// -----------------------------------------
// Test 3: 4-city square loop
// -----------------------------------------
// One optimal tour: 0 → 1 → 2 → 3 → 0
// Cost = 1 + 1 + 1 + 1 = 4
test(
    "Test 3: 4-city square",
    [
        [0, 1, 2, 1],
        [1, 0, 1, 2],
        [2, 1, 0, 1],
        [1, 2, 1, 0]
    ],
    4
);
