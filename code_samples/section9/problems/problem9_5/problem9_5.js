/**
 * minCost
 * -------
 * Computes the minimum total cost required to perform all cuts on a stick
 * of length `n`.
 *
 * This is the classic "Cutting Sticks" / "Minimum Cost to Cut a Stick" problem
 * (e.g., LeetCode 1547), typically solved with interval dynamic programming.
 *
 * Problem statement:
 *   - You have a stick spanning positions [0, n].
 *   - You must cut at each position listed in `cuts`.
 *   - Each time you make a cut, the cost equals the length of the current
 *     segment being cut.
 *   - After cutting, that segment splits into two smaller segments.
 *   - The order of cuts affects the total cost.
 *   - Goal: minimize total cost.
 *
 * Key insight (interval DP):
 *   If we look at an interval [pos[i], pos[j]] and choose the first cut at some
 *   position pos[k] (where i < k < j), then:
 *     - We pay the segment length (pos[j] - pos[i]) once for that first cut.
 *     - The remaining cuts split into two independent subproblems:
 *         [pos[i], pos[k]] and [pos[k], pos[j]].
 *
 * Preprocessing:
 *   Create an augmented list of cut positions with endpoints:
 *     pos = [0, ...cuts, n]
 *   Sort pos so intervals are ordered.
 *
 * DP definition:
 *   dp[i][j] = minimum cost to fully cut the segment from pos[i] to pos[j],
 *             assuming we must perform all cuts strictly between i and j.
 *
 * Base case:
 *   If j = i + 1, there are no cuts inside the interval => dp[i][j] = 0.
 *
 * Transition:
 *   dp[i][j] = min over k in (i+1..j-1) of:
 *                dp[i][k] + dp[k][j] + (pos[j] - pos[i])
 *
 * Complexity:
 *   Let m = pos.length = cuts.length + 2
 *   Time:  O(m^3)
 *   Space: O(m^2)
 *
 * @param {number} n - Length of the stick
 * @param {number[]} cuts - Cut positions
 * @returns {number} Minimum total cost to perform all cuts
 */
function minCost(n, cuts) {
    // Build augmented array of cut positions including endpoints.
    // Use cuts.slice() to avoid mutating the caller's array.
    const pos = [0, ...cuts.slice(), n];

    // Sort cut positions so interval boundaries are in increasing order.
    pos.sort((a, b) => a - b);

    // Number of positions including endpoints.
    const m = pos.length;

    // dp[i][j] = minimum cost to cut the segment [pos[i], pos[j]] completely.
    // Initialized to 0, which naturally covers base intervals with no interior cuts.
    const dp = Array.from({ length: m }, () => new Array(m).fill(0));

    // len is the index distance between i and j (j = i + len).
    // Start from 2 because:
    //   len = 1 => adjacent endpoints => no cut inside => cost 0
    for (let len = 2; len < m; len++) {
        for (let i = 0; i + len < m; i++) {
            const j = i + len;

            // Track best cost found for interval [i, j].
            let best = Number.POSITIVE_INFINITY;

            // Try each possible first cut k between i and j.
            for (let k = i + 1; k < j; k++) {
                // Cost = left subinterval + right subinterval + current segment length.
                const cand = dp[i][k] + dp[k][j] + (pos[j] - pos[i]);
                if (cand < best) best = cand;
            }

            // If there were no valid k (i.e., no cuts inside), keep cost 0.
            dp[i][j] = best === Number.POSITIVE_INFINITY ? 0 : best;
        }
    }

    // Answer is cost to cut the entire stick [0, n].
    return dp[0][m - 1];
}

// ===========================
// Test Harness
// ===========================

/**
 * test
 * ----
 * Runs a single test case for minCost() and prints:
 *   - test name
 *   - input n and cuts
 *   - computed result vs expected
 *
 * @param {string} name - Descriptive test name
 * @param {number} n - Stick length
 * @param {number[]} cuts - Cut positions
 * @param {number} expected - Expected minimum cost
 */
function test(name, n, cuts, expected) {
    const result = minCost(n, cuts);
    console.log(name);
    console.log(`n = ${n}, cuts = [${cuts.join(",")}]`);
    console.log(`minCost = ${result} (expected ${expected})\n`);
}

console.log("=== Testing minCost (Cutting Sticks DP) ===\n");

// Test 1: Classic example
// n = 7, cuts = {1,3,4,5} => expected 16
test("Test 1: Classic example",
     7, [1, 3, 4, 5], 16);

// Test 2: Two cuts
// n = 9, cuts = {5,6} => expected 13
test("Test 2: Two cuts",
     9, [5, 6], 13);

// Test 3: Single cut
// n = 10, cuts = {4} => expected 10
test("Test 3: Single cut",
     10, [4], 10);

// Test 4: Even spacing
// n = 10, cuts = {2,4,6,8} => expected 24
test("Test 4: Even spacing",
     10, [2, 4, 6, 8], 24);

// Test 5: No cuts
// n = 100, cuts = {} => expected 0
test("Test 5: No cuts",
     100, [], 0);
