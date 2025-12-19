/**
 * Simple binary tree node class used by maxRootToLeaf().
 *
 * Each node stores:
 *  - val: numeric value at the node
 *  - left: reference to left child (or null)
 *  - right: reference to right child (or null)
 */
class TreeNode {
    /**
     * @param {number} val   Value stored in this node
     * @param {TreeNode|null} left  Left child (default null)
     * @param {TreeNode|null} right Right child (default null)
     */
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

/**
 * Computes the maximum sum along any path that starts at `root`
 * and ends at a leaf node (a node with no children).
 *
 * This is NOT the "maximum path sum anywhere in the tree" problem.
 * It is specifically root-to-leaf.
 *
 * Base cases:
 *  - If root is null: return -Infinity to represent "no valid path"
 *    (so it won't win in Math.max()).
 *  - If root is a leaf: the best sum is simply root.val.
 *
 * Recursive case:
 *  - best(root) = root.val + max(best(left), best(right))
 *
 * @param {TreeNode|null} root
 * @returns {number} maximum root-to-leaf sum, or -Infinity if root is null
 */
function maxRootToLeaf(root) {
    // Null subtree => no path exists; use -Infinity as an "invalid" sentinel.
    if (!root) {
        return Number.NEGATIVE_INFINITY;
    }

    // Leaf node => path ends here, so return its value.
    if (!root.left && !root.right) {
        return root.val;
    }

    // Recursively compute the best root-to-leaf sum for each child.
    const leftBest = maxRootToLeaf(root.left);
    const rightBest = maxRootToLeaf(root.right);

    // Choose the better child path and include the current node's value.
    return root.val + Math.max(leftBest, rightBest);
}

/**
 * Solves a "TSP-like" problem with bitmask DP:
 *  - Start at node 0
 *  - Visit every node exactly once
 *  - End anywhere (does NOT add cost to return to node 0)
 *
 * DP meaning:
 *  dp[mask][j] = minimum cost to start at 0, visit exactly the nodes in `mask`,
 *               and finish at node j.
 *
 * Initialization:
 *  dp[1<<0][0] = 0  (only node 0 visited, currently at 0, cost 0)
 *
 * Transition:
 *  From (mask, j), try an unvisited node k:
 *   nextMask = mask | (1<<k)
 *   dp[nextMask][k] = min(dp[nextMask][k], dp[mask][j] + dist[j][k])
 *
 * Answer:
 *  min_j dp[fullMask][j]
 *
 * Complexity:
 *  Time:  O(2^n * n^2)
 *  Space: O(2^n * n)
 *
 * Notes about INF:
 *  - Uses Number.MAX_SAFE_INTEGER as a sentinel for "unreachable".
 *  - If distances and path lengths can grow large, be mindful that JS numbers are
 *    IEEE-754 doubles; MAX_SAFE_INTEGER is the largest integer with exact precision.
 *
 * @param {number} n              number of nodes (0..n-1)
 * @param {number[][]} dist       distance/cost matrix dist[i][j]
 * @returns {number} minimum cost to visit all nodes starting from 0 (end anywhere)
 */
function tspBitmask(n, dist) {
    // There are 2^n subsets of nodes; each subset is represented by a bitmask.
    const maxMask = 1 << n;

    // Large sentinel meaning "unreachable"; chosen as a safe large integer.
    const INF = Number.MAX_SAFE_INTEGER;

    // dp[mask][j] initialized to INF.
    const dp = Array.from({ length: maxMask }, () =>
        new Array(n).fill(INF)
    );

    // Starting state: only node 0 visited, at node 0, cost 0.
    dp[1 << 0][0] = 0;

    // Iterate over every visited-set mask.
    for (let mask = 0; mask < maxMask; mask++) {
        // Consider ending node j for this mask.
        for (let j = 0; j < n; j++) {
            // If j isn't included in mask, dp[mask][j] is not a valid "end at j" state.
            if (!(mask & (1 << j))) continue;

            const curr = dp[mask][j];
            // Skip unreachable states.
            if (curr === INF) continue;

            // Try to go to an unvisited node k next.
            for (let k = 0; k < n; k++) {
                // If k already visited, we cannot visit it again.
                if (mask & (1 << k)) continue;

                const nextMask = mask | (1 << k);
                const cand = curr + dist[j][k];

                // Relaxation step: keep the best cost found so far.
                if (cand < dp[nextMask][k]) {
                    dp[nextMask][k] = cand;
                }
            }
        }
    }

    // Mask with all nodes visited.
    const fullMask = maxMask - 1;

    // End anywhere, so take the minimum cost over all possible endpoints j.
    let best = INF;
    for (let j = 0; j < n; j++) {
        best = Math.min(best, dp[fullMask][j]);
    }
    return best;
}

/**
 * Matrix Chain Multiplication (MCM) optimal cost DP.
 *
 * Input:
 *  p is an array of length n+1 describing n matrices:
 *    A1 is p[0] x p[1]
 *    A2 is p[1] x p[2]
 *    ...
 *    An is p[n-1] x p[n]
 *
 * Output:
 *  Minimum number of scalar multiplications needed to compute A1..An.
 *
 * DP meaning (classic CLRS formulation, 1-based matrix indices):
 *  dp[i][j] = minimum cost to multiply Ai..Aj
 *
 * Base:
 *  dp[i][i] = 0 (one matrix requires no multiplications)
 *
 * Transition:
 *  dp[i][j] = min_{k in [i..j-1]}(
 *      dp[i][k] + dp[k+1][j] + p[i-1] * p[k] * p[j]
 *  )
 *
 * Complexity:
 *  Time:  O(n^3)
 *  Space: O(n^2)
 *
 * Notes about numeric precision:
 *  - This uses standard JS numbers; for very large dimensions the result could
 *    exceed MAX_SAFE_INTEGER and lose integer precision.
 *
 * @param {number[]} p dimension array
 * @returns {number} minimum multiplication cost for the full chain
 */
function matrixChainOrder(p) {
    // Number of matrices.
    const n = p.length - 1;

    // Sentinel infinity for minimization.
    const INF = Number.MAX_SAFE_INTEGER;

    // dp sized (n+1)x(n+1) so we can use indices 1..n conveniently.
    // dp defaults to 0; we'll explicitly set dp[i][j] = INF for i < j as we compute.
    const dp = Array.from({ length: n + 1 }, () =>
        new Array(n + 1).fill(0)
    );

    // len = number of matrices in the subchain (from 2 to n).
    for (let len = 2; len <= n; len++) {
        // i = start index of the chain.
        for (let i = 1; i <= n - len + 1; i++) {
            // j = end index of the chain.
            const j = i + len - 1;

            // Initialize as INF before finding the minimum split.
            dp[i][j] = INF;

            // Try every split point k where:
            //   (Ai..Ak) * (A{k+1}..Aj)
            for (let k = i; k < j; k++) {
                // Cost of left subchain + right subchain + cost to multiply results.
                // Dimensions:
                //   Left result:  p[i-1] x p[k]
                //   Right result: p[k] x p[j]
                //   Multiply cost: p[i-1] * p[k] * p[j]
                const cost =
                    dp[i][k] +
                    dp[k + 1][j] +
                    p[i - 1] * p[k] * p[j];

                // Keep the minimal cost.
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                }
            }
        }
    }

    // Full chain cost.
    return dp[1][n];
}

// ======================================================
// Test Harness
// ======================================================

/**
 * Quick console tests for:
 *  1) maxRootToLeaf
 *  2) tspBitmask
 *  3) matrixChainOrder
 *
 * Prints computed values and expected results to help validate correctness.
 */

console.log("=== Test 1: maxRootToLeaf ===\n");

// Build test tree:
//       10
//      /  \
//     5    20
//    / \     \
//   3   7     25
//
// Best path sum = 10 + 20 + 25 = 55
//
// Note: This is a root-to-leaf best path; it must start at 10 and end at a leaf.

const root = new TreeNode(
    10,
    new TreeNode(
        5,
        new TreeNode(3),
        new TreeNode(7)
    ),
    new TreeNode(
        20,
        null,
        new TreeNode(25)
    )
);

const result1 = maxRootToLeaf(root);
console.log("maxRootToLeaf(tree) =", result1, "(expected 55)");

console.log("\n=== Test 2: tspBitmask ===\n");

// Distance matrix for 4 nodes (0..3).
// This test expects 65 as the minimal cost starting at 0, visiting all nodes once,
// and ending anywhere (no return-to-start cost included).

const dist = [
    [0, 10, 15, 20],
    [10, 0, 35, 25],
    [15, 35, 0, 30],
    [20, 25, 30, 0]
];

const tspCost = tspBitmask(4, dist);
console.log("tspBitmask(4 nodes) =", tspCost, "(expected 65)");

console.log("\n=== Test 3: matrixChainOrder ===\n");

// CLRS example → expected 15125
// p describes 6 matrices:
// A1 30x35, A2 35x15, A3 15x5, A4 5x10, A5 10x20, A6 20x25
const p1 = [30, 35, 15, 5, 10, 20, 25];
console.log(
    "matrixChainOrder(CLRS) =",
    matrixChainOrder(p1),
    "(expected 15125)"
);

// Simple 2-matrix → expected 6000
// A1 10x20, A2 20x30 => cost = 10*20*30
const p2 = [10, 20, 30];
console.log(
    "matrixChainOrder([10,20,30]) =",
    matrixChainOrder(p2),
    "(expected 6000)"
);

// Single matrix → expected 0
// Only A1 5x10 => no multiplication needed.
const p3 = [5, 10];
console.log(
    "matrixChainOrder(single) =",
    matrixChainOrder(p3),
    "(expected 0)"
);
