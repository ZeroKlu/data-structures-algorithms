package code_samples.section9.lesson;

/**
 * Basic binary tree node used by maxRootToLeaf().
 *
 * Note: This class is package-private (no explicit modifier), meaning it is
 * visible within code_samples.section9.lesson.
 */
class TreeNode {
    // The integer value stored at this node.
    int val;

    // References to left and right children (may be null).
    TreeNode left;
    TreeNode right;

    /**
     * Constructs a TreeNode with the given value and null children.
     */
    TreeNode(int v) { val = v; }
}

/**
 * section9 contains three classic dynamic-programming / recursion examples:
 *  1) Max root-to-leaf path sum in a binary tree (recursive).
 *  2) Traveling Salesman Problem (TSP) "visit all nodes" variant using bitmask DP.
 *  3) Matrix Chain Multiplication optimal parenthesization cost via DP.
 *
 * The main() method provides simple console-based tests for each algorithm.
 */
public class section9 {

    /**
     * Computes the maximum sum along any path that starts at the given root and
     * ends at any leaf node (a node with no children).
     *
     * This is a standard recursion:
     *  - Base case: null subtree: return Integer.MIN_VALUE to represent "invalid"
     *    so it won't be chosen over real paths when taking Math.max().
     *  - Base case: leaf: best sum is the leaf's value itself.
     *  - Recursive: best sum = root value + max(best left path, best right path).
     *
     * Important detail:
     * Returning Integer.MIN_VALUE for null ensures a missing child doesn't
     * accidentally look like a zero-cost path. It also means that if you call
     * maxRootToLeaf(null) directly, you'll get Integer.MIN_VALUE, as tested in main().
     */
    int maxRootToLeaf(TreeNode root) {
        // If there's no node here, treat it as an invalid path.
        if (root == null) {
            return Integer.MIN_VALUE;
        }

        // If this node is a leaf, the best root-to-leaf sum is just its value.
        if (root.left == null && root.right == null) {
            return root.val;
        }

        // Recursively compute best sums from each child down to some leaf.
        int leftBest = maxRootToLeaf(root.left);
        int rightBest = maxRootToLeaf(root.right);

        // Choose the better of the two child paths and add the current node's value.
        return root.val + Math.max(leftBest, rightBest);
    }

    /**
     * Solves a "TSP-like" problem using bitmask DP:
     * Given:
     *  - n nodes labeled [0..n-1]
     *  - dist[i][j] cost to travel from i to j
     * Returns:
     *  - The minimum cost to start at node 0 and visit ALL nodes exactly once,
     *    ending anywhere (does NOT add cost to return to 0).
     *
     * DP definition:
     *  dp[mask][j] = minimum cost to start at 0, visit exactly the set of nodes
     *               in 'mask', and end at node j.
     *
     * Transitions:
     *  From state (mask, j), try going to an unvisited node k:
     *    nextMask = mask | (1<<k)
     *    dp[nextMask][k] = min(dp[nextMask][k], dp[mask][j] + dist[j][k])
     *
     * Initialization:
     *  dp[1<<0][0] = 0 (only node 0 visited, currently at 0, cost 0)
     *
     * Answer:
     *  min over j of dp[fullMask][j]
     *
     * Complexity:
     *  Time:  O(2^n * n^2)
     *  Space: O(2^n * n)
     */
    int tspBitmask(int n, int[][] dist) {
        // Total number of subsets of n nodes: 2^n.
        int maxMask = 1 << n;

        // A large sentinel "infinity" value used to represent unreachable states.
        // Using MAX_VALUE/4 helps avoid overflow when adding costs.
        final int INF = Integer.MAX_VALUE / 4;

        // dp[mask][j] = best known cost to reach node j having visited 'mask'.
        int[][] dp = new int[maxMask][n];

        // Initialize all states as unreachable (INF).
        for (int m = 0; m < maxMask; m++) {
            for (int j = 0; j < n; j++) {
                dp[m][j] = INF;
            }
        }

        // Start at node 0 with only node 0 visited.
        dp[1 << 0][0] = 0;

        // Iterate over all visited-subsets.
        for (int mask = 0; mask < maxMask; mask++) {
            // Try each possible current end node j.
            for (int j = 0; j < n; j++) {
                // If j is not in the visited set for this mask, skip: invalid state.
                if ((mask & (1 << j)) == 0) continue;

                int curr = dp[mask][j];
                // If this state is unreachable, skip.
                if (curr == INF) continue;

                // Try to extend the tour by visiting an unvisited node k.
                for (int k = 0; k < n; k++) {
                    // If k is already visited, we cannot go there (visit exactly once).
                    if ((mask & (1 << k)) != 0) continue;

                    int nextMask = mask | (1 << k);
                    int cand = curr + dist[j][k];

                    // Relax the DP transition if we found a cheaper way.
                    if (cand < dp[nextMask][k]) {
                        dp[nextMask][k] = cand;
                    }
                }
            }
        }

        // All nodes visited => mask with all bits set.
        int fullMask = maxMask - 1;

        // We can end at ANY node, so take the min over all possible endpoints.
        int best = INF;
        for (int j = 0; j < n; j++) {
            if (dp[fullMask][j] < best) {
                best = dp[fullMask][j];
            }
        }
        return best;
    }

    /**
     * Matrix Chain Multiplication (MCM) DP:
     *
     * Given an array p of length (n+1), where matrix i has dimension:
     *    A_i is p[i-1] x p[i]
     * for i = 1..n
     *
     * Returns the minimum number of scalar multiplications needed to compute:
     *    A_1 A_2 ... A_n
     * by choosing the optimal parenthesization.
     *
     * DP definition (classic CLRS):
     *  dp[i][j] = minimum cost to multiply matrices A_i..A_j
     *
     * Base:
     *  dp[i][i] = 0 (a single matrix requires no multiplication)
     *
     * Transition:
     *  dp[i][j] = min over k in [i..j-1] of:
     *      dp[i][k] + dp[k+1][j] + (p[i-1] * p[k] * p[j])
     *
     * Implementation details:
     *  - Uses 1-based indexing for matrices (i and j), matching the textbook formula.
     *  - Uses long for intermediate cost to reduce overflow risk before casting to int.
     *
     * Complexity:
     *  Time:  O(n^3)
     *  Space: O(n^2)
     */
    int matrixChainOrder(int[] p) {
        // Number of matrices is one less than the number of dimensions.
        int n = p.length - 1;

        // Sentinel infinity for minimization.
        final int INF = Integer.MAX_VALUE;

        // dp table sized [n+1][n+1] so we can use indices 1..n.
        int[][] dp = new int[n + 1][n + 1];

        // len is the chain length (#matrices) in the subproblem.
        // Start from 2 because length 1 subproblems have cost 0 by default.
        for (int len = 2; len <= n; len++) {
            // i is the starting matrix index.
            for (int i = 1; i <= n - len + 1; i++) {
                // j is the ending matrix index.
                int j = i + len - 1;

                // Initialize dp[i][j] as "infinite" before taking minima.
                dp[i][j] = INF;

                // Try splitting the product between k and k+1.
                for (int k = i; k < j; k++) {
                    // Cost = cost(left) + cost(right) + cost(multiply results)
                    // Multiply dimensions:
                    //   (A_i..A_k) is p[i-1] x p[k]
                    //   (A_{k+1}..A_j) is p[k] x p[j]
                    // Scalar multiply cost: p[i-1] * p[k] * p[j]
                    long cost = (long)dp[i][k] + dp[k + 1][j]
                        + (long)p[i - 1] * p[k] * p[j];

                    // Keep the minimum cost split.
                    if (cost < dp[i][j]) {
                        dp[i][j] = (int)cost;
                    }
                }
            }
        }

        // Full chain is A_1..A_n.
        return dp[1][n];
    }

    // ==========================
    // Helpers for tests
    // ==========================

    /**
     * Convenience helper to allocate a TreeNode with a given value.
     * Used by the test harness to build trees tersely.
     */
    private static TreeNode node(int v) {
        return new TreeNode(v);
    }

    // ==========================
    // Test Harness
    // ==========================

    /**
     * Simple manual tests for the three algorithms.
     * Prints computed results alongside expected values.
     */
    public static void main(String[] args) {
        // Instantiate the class that contains the algorithms.
        section9 solver = new section9();

        System.out.println("=== Test 1: maxRootToLeaf ===\n");

        // Build this tree:
        //       10
        //      /  \
        //     5    20
        //    / \     \
        //   3   7     25
        //
        // Best root-to-leaf sum = 10 + 20 + 25 = 55
        //
        // Note: This is not necessarily the "maximum path sum anywhere in the tree";
        // it's specifically root-to-leaf.

        TreeNode root = node(10);
        root.left = node(5);
        root.right = node(20);
        root.left.left = node(3);
        root.left.right = node(7);
        root.right.right = node(25);

        // Compute and print result for the sample tree.
        int bestPath = solver.maxRootToLeaf(root);
        System.out.println("maxRootToLeaf(tree) = " + bestPath +
                           " (expected 55)");

        // Edge case: single node (also a leaf).
        TreeNode single = node(42);
        int singleBest = solver.maxRootToLeaf(single);
        System.out.println("maxRootToLeaf(single=42) = " + singleBest +
                           " (expected 42)");

        // Edge case: null root returns Integer.MIN_VALUE by design.
        int nullBest = solver.maxRootToLeaf(null);
        System.out.println("maxRootToLeaf(null) = " + nullBest +
                           " (expected " + Integer.MIN_VALUE + ")\n");

        System.out.println("=== Test 2: tspBitmask ===\n");

        // 4-node example distance matrix:
        // 0  10 15 20
        // 10  0 35 25
        // 15 35  0 30
        // 20 25 30  0
        //
        // This test expects:
        // "Best path from 0 visiting all nodes once: cost 65"
        //
        // Reminder: This implementation minimizes the cost to start at 0 and visit
        // all nodes once, ending anywhere (no return-to-start cost added).

        int[][] dist = {
            {0, 10, 15, 20},
            {10, 0, 35, 25},
            {15, 35, 0, 30},
            {20, 25, 30, 0}
        };

        int tspCost = solver.tspBitmask(4, dist);
        System.out.println("tspBitmask(4 nodes) = " + tspCost +
                           " (expected 65)\n");

        System.out.println("=== Test 3: matrixChainOrder ===\n");

        // Classic CLRS example:
        // p = [30,35,15,5,10,20,25], n=6 => optimal cost 15125
        //
        // Interpretation:
        // A1: 30x35
        // A2: 35x15
        // A3: 15x5
        // A4: 5x10
        // A5: 10x20
        // A6: 20x25

        int[] p1 = {30, 35, 15, 5, 10, 20, 25};
        int cost1 = solver.matrixChainOrder(p1);
        System.out.println("matrixChainOrder(CLRS) = " + cost1 +
                           " (expected 15125)");

        // Simple 2-matrix example:
        // A(10x20), B(20x30)
        // cost = 10 * 20 * 30 = 6000
        int[] p2 = {10, 20, 30};
        int cost2 = solver.matrixChainOrder(p2);
        System.out.println("matrixChainOrder([10,20,30]) = " + cost2 +
                           " (expected 6000)");

        // Single matrix:
        // Only one matrix means no multiplication is performed => cost 0.
        int[] p3 = {5, 10};
        int cost3 = solver.matrixChainOrder(p3);
        System.out.println("matrixChainOrder(single matrix) = " + cost3 +
                           " (expected 0)");
    }
}
