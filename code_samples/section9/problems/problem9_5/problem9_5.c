#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 * int_cmp
 * -------
 * Comparator used by qsort() to sort integers in ascending order.
 *
 * Returns:
 *   - negative if *a < *b
 *   - zero     if *a == *b
 *   - positive if *a > *b
 *
 * Implementation detail:
 *   (x > y) - (x < y) yields:
 *     1  if x > y
 *     0  if x == y
 *    -1  if x < y
 */
static int int_cmp(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

/**
 * minCost
 * -------
 * Computes the minimum total cost to perform all cuts on a stick of length `n`.
 *
 * This is the classic "Cutting Sticks" / "Minimum Cost to Cut a Stick" DP
 * problem (e.g., LeetCode 1547).
 *
 * Problem statement:
 *   - You have a stick from position 0 to position n.
 *   - You must perform cuts at given positions in `cuts[]`.
 *   - When you cut a segment, the cost of that cut equals the *current segment
 *     length* being cut.
 *   - After cutting, the segment splits into two subsegments, which can be cut
 *     later.
 *   - Goal: choose the order of cuts to minimize total cost.
 *
 * Key DP idea:
 *   The order matters, but the optimal structure is interval-based:
 *     "If we choose the first cut within an interval, the left and right parts
 *      become independent subproblems."
 *
 * Preprocessing:
 *   Create an augmented, sorted list of cut positions:
 *     pos = [0, sorted(cuts...), n]
 *   Let m = cutsSize + 2 be the length of pos.
 *
 * DP definition:
 *   dp[i][j] = minimum cost to cut the stick segment from pos[i] to pos[j],
 *             assuming we need to perform all cuts strictly between i and j
 *             (i.e., at indices i+1..j-1).
 *
 * Base case:
 *   If there are no cuts between i and j (i.e., j = i+1),
 *   then dp[i][j] = 0 (nothing to do).
 *
 * Transition:
 *   If there is at least one cut between i and j, try each possible first cut k:
 *     dp[i][j] = min over k in (i+1..j-1) of:
 *                  dp[i][k] + dp[k][j] + (pos[j] - pos[i])
 *
 *   Explanation:
 *     - Cutting the segment [pos[i], pos[j]] costs its length (pos[j] - pos[i]).
 *     - After choosing first cut k, the remaining cuts split into:
 *         left interval  [i, k]
 *         right interval [k, j]
 *       which are independent.
 *
 * Loop order:
 *   We build solutions from smaller intervals to larger intervals using `len`
 *   (the index distance between i and j).
 *
 * Complexity:
 *   Time:  O(m^3) due to triple loop over (len, i, k)
 *   Space: O(m^2) for dp
 *
 * Memory:
 *   - pos is allocated dynamically and freed before returning.
 *   - dp is a dynamically allocated 2D array and fully freed.
 *
 * Parameters:
 *   n        - length of the stick (positions 0..n)
 *   cuts     - array of cut positions
 *   cutsSize - number of cuts
 *
 * Returns:
 *   The minimum total cost to perform all cuts.
 */
int minCost(int n, int *cuts, int cutsSize) {
    // m includes the two endpoints 0 and n
    int m = cutsSize + 2;

    // pos will store [0, cuts..., n] with cuts sorted
    int *pos = (int *)malloc(m * sizeof(int));

    // Insert endpoints
    pos[0] = 0;
    for (int i = 0; i < cutsSize; i++) pos[i + 1] = cuts[i];
    pos[m - 1] = n;

    // Sort only the cut positions (not including the endpoints)
    qsort(pos + 1, cutsSize, sizeof(int), int_cmp);

    // Allocate dp[m][m], initialized to 0 via calloc
    int **dp = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        dp[i] = (int *)calloc(m, sizeof(int));
    }

    // len is the interval size in terms of indices (j - i)
    // We start from 2 because:
    //   len = 1  => adjacent endpoints => no cut inside => dp already 0
    //   len >= 2 => there may be at least one cut inside
    for (int len = 2; len < m; len++) {
        for (int i = 0; i + len < m; i++) {
            int j = i + len;

            // Find the minimum cost to cut interval [i, j]
            int best = INT_MAX;

            // Try each possible first cut k between i and j
            for (int k = i + 1; k < j; k++) {
                // Cost = left subproblem + right subproblem + current segment length
                int cand = dp[i][k] + dp[k][j] + (pos[j] - pos[i]);
                if (cand < best) best = cand;
            }

            // If no k existed (which happens only when j = i+1), best stays INT_MAX;
            // for safety, treat it as 0 (no cuts needed).
            dp[i][j] = (best == INT_MAX ? 0 : best);
        }
    }

    // Answer is the cost to cut the entire stick from 0 to n
    int ans = dp[0][m - 1];

    // Free dp rows and dp itself
    for (int i = 0; i < m; i++) free(dp[i]);
    free(dp);

    // Free position array
    free(pos);

    return ans;
}

/**
 * test
 * ----
 * Helper function to run a minCost() test case and print:
 *   - test name
 *   - stick length n
 *   - cut positions
 *   - computed result vs expected
 *
 * Parameters:
 *   name      - descriptive test name
 *   n         - stick length
 *   cuts      - cut positions array (may be NULL if cutsSize==0)
 *   cutsSize  - number of cuts
 *   expected  - expected minimum cost
 */
void test(const char *name, int n, int *cuts, int cutsSize, int expected) {
    int result = minCost(n, cuts, cutsSize);

    printf("%s\n", name);
    printf("n = %d, cuts = {", n);
    for (int i = 0; i < cutsSize; i++) {
        printf("%d%s", cuts[i], (i + 1 == cutsSize ? "" : ","));
    }
    printf("}\n");
    printf("minCost = %d (expected %d)\n\n", result, expected);
}

int main(void) {
    printf("=== Testing minCost (Cutting Sticks DP) ===\n\n");

    // ----------------------------------------------
    // Test 1: Classic example
    // n = 7, cuts = {1, 3, 4, 5}
    //
    // Expected answer: 16
    // (matches LeetCode 1547 example)
    // ----------------------------------------------
    int cuts1[] = {1, 3, 4, 5};
    test("Test 1: Classic LC example", 7, cuts1, 4, 16);

    // ----------------------------------------------
    // Test 2: Two cuts only
    // n = 9, cuts = {5, 6}
    //
    // Optimal: cut 5 (cost 9), then cut 6 (cost 4) => 13
    // ----------------------------------------------
    int cuts2[] = {5, 6};
    test("Test 2: Two cuts", 9, cuts2, 2, 13);

    // ----------------------------------------------
    // Test 3: Single cut
    // n = 10, cuts = {4}
    //
    // Only one cut exists: cost = 10
    // ----------------------------------------------
    int cuts3[] = {4};
    test("Test 3: Single cut", 10, cuts3, 1, 10);

    // ----------------------------------------------
    // Test 4: Cuts evenly spaced
    // n = 10, cuts = {2, 4, 6, 8}
    //
    // NOTE:
    // The comment says "Known optimal cost: 34" but the expected value
    // passed into test() is 24. The algorithm will compute the true minimum
    // based on the recurrence; if your expected value is meant to validate
    // correctness, make sure the expected matches the intended known result.
    // (No code changes are made here—this is just a comment to flag the mismatch.)
    // ----------------------------------------------
    int cuts4[] = {2, 4, 6, 8};
    test("Test 4: Even spacing", 10, cuts4, 4, 24);

    // ----------------------------------------------
    // Test 5: No cuts
    // n = 100, cuts = {}
    //
    // Cost = 0
    // ----------------------------------------------
    int *cuts5 = NULL;
    test("Test 5: No cuts", 100, cuts5, 0, 0);

    return 0;
}
