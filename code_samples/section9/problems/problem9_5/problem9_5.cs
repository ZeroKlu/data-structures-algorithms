/**
 * MinCost
 * -------
 * Computes the minimum total cost required to perform all cuts on a stick
 * of length `n`.
 *
 * This is the classic "Cutting Sticks" / "Minimum Cost to Cut a Stick"
 * dynamic programming problem (e.g., LeetCode 1547).
 *
 * Problem statement:
 *   - You have a stick spanning positions [0, n].
 *   - You must cut the stick at all positions listed in `cuts`.
 *   - When you cut a segment, the cost of that cut equals the *length of the
 *     current segment* being cut.
 *   - After a cut, the segment splits into two independent subsegments.
 *   - The order of cuts matters.
 *   - Goal: minimize the total cost of all cuts.
 *
 * Key idea:
 *   Use interval dynamic programming.
 *   For any interval [pos[i], pos[j]], if we choose the first cut at position k
 *   (i < k < j), then:
 *     - The cost of that cut is (pos[j] - pos[i])
 *     - The remaining work splits into two independent subproblems:
 *         [i, k] and [k, j]
 *
 * Preprocessing:
 *   - Add sentinel endpoints 0 and n to the cuts array.
 *   - Sort all cut positions so intervals are contiguous.
 *
 * DP definition:
 *   dp[i, j] = minimum cost to fully cut the stick segment
 *              from pos[i] to pos[j], assuming all cuts strictly between
 *              i and j must be performed.
 *
 * Base case:
 *   - If j = i + 1, there are no cuts inside the interval → cost = 0.
 *
 * Transition:
 *   dp[i, j] = min over k in (i+1 .. j-1):
 *                 dp[i, k] + dp[k, j] + (pos[j] - pos[i])
 *
 * Complexity:
 *   Let m = cuts.Length + 2
 *   Time:  O(m^3)
 *   Space: O(m^2)
 *
 * @param n     Length of the stick
 * @param cuts  Positions where cuts must be made
 * @return      Minimum total cost of all cuts
 */
static int MinCost(int n, int[] cuts) {
    // Total number of positions including endpoints (0 and n)
    int m = cuts.Length + 2;

    // pos[] will store: [0, sorted cuts..., n]
    int[] pos = new int[m];
    pos[0] = 0;          // Left endpoint of the stick
    pos[m - 1] = n;      // Right endpoint of the stick

    // Copy the cut positions into pos starting at index 1
    Array.Copy(cuts, 0, pos, 1, cuts.Length);

    // Sort only the interior cut positions (ignore 0 and n)
    Array.Sort(pos, 1, m - 1);

    // dp[i, j] = minimum cost to cut the segment [pos[i], pos[j]]
    int[,] dp = new int[m, m];

    // len represents the distance between indices (j - i)
    // Start from len = 2 because:
    //   len = 1 → adjacent endpoints → no cuts inside → cost 0
    for (int len = 2; len < m; len++) {
        for (int i = 0; i + len < m; i++) {
            int j = i + len;

            // Track the minimum cost for interval [i, j]
            int best = int.MaxValue;

            // Try each possible first cut k inside (i, j)
            for (int k = i + 1; k < j; k++) {
                // Cost of:
                //   left subinterval + right subinterval + cost of current cut
                int cand = dp[i, k] + dp[k, j] + (pos[j] - pos[i]);
                if (cand < best) best = cand;
            }

            // If there were no valid k (i.e., no cuts inside),
            // best stays int.MaxValue → correct cost is 0
            dp[i, j] = best == int.MaxValue ? 0 : best;
        }
    }

    // Final answer: minimum cost to cut the entire stick [0, n]
    return dp[0, m - 1];
}

/**
 * Test
 * ----
 * Helper function to run a single MinCost() test case and print:
 *   - Test name
 *   - Stick length
 *   - Cut positions
 *   - Computed result vs expected result
 *
 * @param name      Descriptive test name
 * @param n         Stick length
 * @param cuts      Cut positions
 * @param expected  Expected minimum cost
 */
static void Test(string name, int n, int[] cuts, int expected)
{
    int result = MinCost(n, cuts);
    Console.WriteLine(name);
    Console.WriteLine($"n = {n}, cuts = [{string.Join(",", cuts)}]");
    Console.WriteLine($"MinCost = {result} (expected {expected})\n");
}

// ===========================
// Test Harness
// ===========================

Console.WriteLine("=== Testing MinCost (Cutting Sticks DP) ===\n");

// Test 1: Classic example
// n = 7, cuts = {1, 3, 4, 5}
// Expected = 16
Test("Test 1: Classic example",
     7, [1, 3, 4, 5], 16);

// Test 2: Two cuts
// n = 9, cuts = {5, 6}
// Expected = 13
Test("Test 2: Two cuts",
     9, [5, 6], 13);

// Test 3: Single cut
// n = 10, cuts = {4}
// Expected = 10
Test("Test 3: Single cut",
     10, [4], 10);

// Test 4: Even spacing
// n = 10, cuts = {2, 4, 6, 8}
// Expected = 24
Test("Test 4: Even spacing",
     10, [2, 4, 6, 8], 24);

// Test 5: No cuts
// n = 100, cuts = {}
// Expected = 0
Test("Test 5: No cuts",
     100, [], 0);
