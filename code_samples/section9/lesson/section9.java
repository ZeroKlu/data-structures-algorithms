package code_samples.section9.lesson;

class TreeNode {
    int val;
    TreeNode left;
    TreeNode right;
    TreeNode(int v) { val = v; }
}

public class section9 {

    int maxRootToLeaf(TreeNode root) {
        if (root == null) {
            return Integer.MIN_VALUE;
        }
        if (root.left == null && root.right == null) {
            return root.val;
        }
        int leftBest = maxRootToLeaf(root.left);
        int rightBest = maxRootToLeaf(root.right);
        return root.val + Math.max(leftBest, rightBest);
    }

    int tspBitmask(int n, int[][] dist) {
        int maxMask = 1 << n;
        final int INF = Integer.MAX_VALUE / 4;
        int[][] dp = new int[maxMask][n];
        for (int m = 0; m < maxMask; m++) {
            for (int j = 0; j < n; j++) {
                dp[m][j] = INF;
            }
        }
        dp[1 << 0][0] = 0;

        for (int mask = 0; mask < maxMask; mask++) {
            for (int j = 0; j < n; j++) {
                if ((mask & (1 << j)) == 0) continue;
                int curr = dp[mask][j];
                if (curr == INF) continue;
                for (int k = 0; k < n; k++) {
                    if ((mask & (1 << k)) != 0) continue;
                    int nextMask = mask | (1 << k);
                    int cand = curr + dist[j][k];
                    if (cand < dp[nextMask][k]) {
                        dp[nextMask][k] = cand;
                    }
                }
            }
        }

        int fullMask = maxMask - 1;
        int best = INF;
        for (int j = 0; j < n; j++) {
            if (dp[fullMask][j] < best) {
                best = dp[fullMask][j];
            }
        }
        return best;
    }

    int matrixChainOrder(int[] p) {
        int n = p.length - 1;
        final int INF = Integer.MAX_VALUE;
        int[][] dp = new int[n + 1][n + 1];

        for (int len = 2; len <= n; len++) {
            for (int i = 1; i <= n - len + 1; i++) {
                int j = i + len - 1;
                dp[i][j] = INF;
                for (int k = i; k < j; k++) {
                    long cost = (long)dp[i][k] + dp[k + 1][j]
                        + (long)p[i - 1] * p[k] * p[j];
                    if (cost < dp[i][j]) {
                        dp[i][j] = (int)cost;
                    }
                }
            }
        }
        return dp[1][n];
    }

    // ==========================
    // Helpers for tests
    // ==========================

    private static TreeNode node(int v) {
        return new TreeNode(v);
    }

    // ==========================
    // Test Harness
    // ==========================
    public static void main(String[] args) {
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

        TreeNode root = node(10);
        root.left = node(5);
        root.right = node(20);
        root.left.left = node(3);
        root.left.right = node(7);
        root.right.right = node(25);

        int bestPath = solver.maxRootToLeaf(root);
        System.out.println("maxRootToLeaf(tree) = " + bestPath +
                           " (expected 55)");

        TreeNode single = node(42);
        int singleBest = solver.maxRootToLeaf(single);
        System.out.println("maxRootToLeaf(single=42) = " + singleBest +
                           " (expected 42)");

        int nullBest = solver.maxRootToLeaf(null);
        System.out.println("maxRootToLeaf(null) = " + nullBest +
                           " (expected " + Integer.MIN_VALUE + ")\n");

        System.out.println("=== Test 2: tspBitmask ===\n");

        // 4-node example:
        // 0  10 15 20
        // 10  0 35 25
        // 15 35  0 30
        // 20 25 30  0
        //
        // Best path from 0 visiting all nodes once: cost 65
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

        // CLRS example:
        // p = [30,35,15,5,10,20,25], n=6 => cost 15125
        int[] p1 = {30, 35, 15, 5, 10, 20, 25};
        int cost1 = solver.matrixChainOrder(p1);
        System.out.println("matrixChainOrder(CLRS) = " + cost1 +
                           " (expected 15125)");

        // Simple 2-matrix example: A(10x20), B(20x30)
        // cost = 10 * 20 * 30 = 6000
        int[] p2 = {10, 20, 30};
        int cost2 = solver.matrixChainOrder(p2);
        System.out.println("matrixChainOrder([10,20,30]) = " + cost2 +
                           " (expected 6000)");

        // Single matrix: cost = 0
        int[] p3 = {5, 10};
        int cost3 = solver.matrixChainOrder(p3);
        System.out.println("matrixChainOrder(single matrix) = " + cost3 +
                           " (expected 0)");
    }
}
