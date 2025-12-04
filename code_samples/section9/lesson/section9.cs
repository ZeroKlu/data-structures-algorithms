#nullable enable

static int MaxRootToLeaf(TreeNode? root) {
    if (root == null) {
        return int.MinValue;
    }
    if (root.Left == null && root.Right == null) {
        return root.Val;
    }
    int leftBest = MaxRootToLeaf(root.Left);
    int rightBest = MaxRootToLeaf(root.Right);
    return root.Val + System.Math.Max(leftBest, rightBest);
}

static int TspBitmask(int n, int[,] dist) {
    int maxMask = 1 << n;
    int INF = int.MaxValue / 4;
    var dp = new int[maxMask, n];
    for (int m = 0; m < maxMask; m++) {
        for (int j = 0; j < n; j++) {
            dp[m, j] = INF;
        }
    }
    dp[1 << 0, 0] = 0;

    for (int mask = 0; mask < maxMask; mask++) {
        for (int j = 0; j < n; j++) {
            if ((mask & (1 << j)) == 0) continue;
            int curr = dp[mask, j];
            if (curr == INF) continue;
            for (int k = 0; k < n; k++) {
                if ((mask & (1 << k)) != 0) continue;
                int nextMask = mask | (1 << k);
                int cand = curr + dist[j, k];
                if (cand < dp[nextMask, k]) {
                    dp[nextMask, k] = cand;
                }
            }
        }
    }

    int fullMask = maxMask - 1;
    int best = INF;
    for (int j = 0; j < n; j++) {
        if (dp[fullMask, j] < best) {
            best = dp[fullMask, j];
        }
    }
    return best;
}

static int MatrixChainOrder(int[] p) {
    int n = p.Length - 1;
    int INF = int.MaxValue;
    var dp = new int[n + 1, n + 1];

    for (int len = 2; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            dp[i, j] = INF;
            for (int k = i; k < j; k++) {
                long cost = (long)dp[i, k] + dp[k + 1, j]
                    + (long)p[i - 1] * p[k] * p[j];
                if (cost < dp[i, j]) {
                    dp[i, j] = (int)cost;
                }
            }
        }
    }
    return dp[1, n];
}

// ===========================
// Tests (top-level)
// ===========================

Console.WriteLine("=== Test 1: MaxRootToLeaf ===\n");

// Build tree:
//       10
//      /  \
//     5    20
//    / \     \
//   3   7     25
//
// Best root-to-leaf sum = 10 + 20 + 25 = 55
TreeNode root = new(10)
{
    Left = new TreeNode(5)
    {
        Left = new TreeNode(3),
        Right = new TreeNode(7)
    },
    Right = new TreeNode(20)
    {
        Right = new TreeNode(25)
    }
};

int bestPath = MaxRootToLeaf(root);
Console.WriteLine($"MaxRootToLeaf(tree) = {bestPath} (expected 55)");

TreeNode single = new(42);
int singleBest = MaxRootToLeaf(single);
Console.WriteLine($"MaxRootToLeaf(single=42) = {singleBest} (expected 42)");

int nullBest = MaxRootToLeaf(null);
Console.WriteLine($"MaxRootToLeaf(null) = {nullBest} (expected {int.MinValue})");

Console.WriteLine();
Console.WriteLine("=== Test 2: TspBitmask ===\n");

// 4-node example:
int[,] dist = new int[4, 4]
{
    { 0, 10, 15, 20 },
    { 10, 0, 35, 25 },
    { 15, 35, 0, 30 },
    { 20, 25, 30, 0 }
};

// Best path from 0 visiting all nodes once has cost 65
int tspCost = TspBitmask(4, dist);
Console.WriteLine($"TspBitmask(4 nodes) = {tspCost} (expected 65)");

Console.WriteLine();
Console.WriteLine("=== Test 3: MatrixChainOrder ===\n");

// CLRS example
int[] p1 = { 30, 35, 15, 5, 10, 20, 25 }; // 6 matrices
int cost1 = MatrixChainOrder(p1);
Console.WriteLine($"MatrixChainOrder(CLRS) = {cost1} (expected 15125)");

// Simple 2-matrix example: 10x20, 20x30 -> 10*20*30 = 6000
int[] p2 = { 10, 20, 30 };
int cost2 = MatrixChainOrder(p2);
Console.WriteLine($"MatrixChainOrder([10,20,30]) = {cost2} (expected 6000)");

// Single matrix: cost 0
int[] p3 = { 5, 10 };
int cost3 = MatrixChainOrder(p3);
Console.WriteLine($"MatrixChainOrder(single) = {cost3} (expected 0)");

class TreeNode(int val)
{
    public int Val = val;
    public TreeNode? Left;
    public TreeNode? Right;
}
