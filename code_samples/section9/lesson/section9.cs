#nullable enable

/*
    This file contains THREE classic dynamic-programming / recursion problems,
    plus a simple top-level test harness:

      1) MaxRootToLeaf (tree recursion / tree DP)
         - Returns the maximum sum along any path that starts at the root
           and ends at a LEAF node.
         - Uses recursion:
             best(node) = node.Val + max(best(node.Left), best(node.Right))

      2) TspBitmask (DP over subsets / bitmask DP)
         - Computes the minimum-cost PATH that:
             * starts at node 0
             * visits every node exactly once
             * ends at ANY node (does NOT return to 0)
         - DP state:
             dp[mask, j] = min cost to start at 0, visit nodes in 'mask',
                           and finish at node j.

      3) MatrixChainOrder (interval DP)
         - Given dimensions p = [p0, p1, ..., pn] for matrices A1..An
           where Ai has dimension p[i-1] x p[i],
           returns the minimum scalar multiplications to compute A1..An.

    Notes on C# / nullable:
      - #nullable enable turns on nullable reference type analysis.
      - TreeNode? means "TreeNode reference or null".
*/

/* ============================================================
   1) MaxRootToLeaf
   ============================================================ */

/*
    Returns the maximum root-to-leaf sum in a binary tree.

    Edge behavior:
      - If root is null, there is no valid path, so we return int.MinValue
        as a sentinel meaning "no path".
        This makes max() logic convenient: a missing subtree never wins
        against a real subtree sum.

    Base cases:
      - root == null                 => int.MinValue (no path)
      - root is a leaf (no children) => root.Val

    Recursive case:
      - compute best sum from left subtree
      - compute best sum from right subtree
      - pick the larger one and add root.Val

    Complexity:
      - Time:  O(number of nodes) because each node is visited once.
      - Space: O(height) due to recursion stack.
*/
static int MaxRootToLeaf(TreeNode? root) {
    // Empty tree => no root-to-leaf path exists.
    // Return sentinel minimal value so it won't be chosen by Math.Max
    // when a real path exists.
    if (root == null) {
        return int.MinValue;
    }

    // Leaf node => the only root-to-leaf path is the node itself.
    if (root.Left == null && root.Right == null) {
        return root.Val;
    }

    // Recursively compute best sums from each child.
    int leftBest = MaxRootToLeaf(root.Left);
    int rightBest = MaxRootToLeaf(root.Right);

    // Best path through this node = node value + best child path.
    return root.Val + System.Math.Max(leftBest, rightBest);
}

/* ============================================================
   2) TspBitmask (subset DP)
   ============================================================ */

/*
    Solves a TSP-like variant using bitmask DP.

    Input:
      - n: number of nodes (0..n-1)
      - dist: n x n matrix where dist[i,j] is the cost from i -> j

    Goal:
      - Start at node 0
      - Visit all nodes exactly once
      - End at ANY node (no "return to start" requirement)
      - Minimize total cost

    DP meaning:
      dp[mask, j] = minimum cost to:
        - start at node 0
        - visit exactly the nodes indicated by 'mask'
        - end at node j
      (j must be included in mask)

    Initialization:
      dp[1<<0, 0] = 0
      All other states start as INF (unreachable).

    Transition:
      From (mask, j), try to go to any unvisited node k:
        nextMask = mask | (1<<k)
        dp[nextMask, k] = min(dp[nextMask, k], dp[mask, j] + dist[j,k])

    Answer:
      fullMask = (1<<n) - 1
      min over j of dp[fullMask, j]

    Complexity:
      - Time:  O(n^2 * 2^n)
      - Space: O(n * 2^n)

    IMPORTANT:
      This returns the best *Hamiltonian path* starting at 0.
      If you wanted a *tour* (return to 0), you would compute:
        min_j dp[fullMask, j] + dist[j, 0]
*/
static int TspBitmask(int n, int[,] dist) {
    // Total number of subsets of nodes.
    int maxMask = 1 << n;

    // "Infinity" large value; divide by 4 to reduce overflow risk when adding.
    int INF = int.MaxValue / 4;

    // dp[mask, j] = best cost for subset 'mask' ending at node j.
    // We'll initialize every entry to INF (meaning "unreachable").
    var dp = new int[maxMask, n];
    for (int m = 0; m < maxMask; m++) {
        for (int j = 0; j < n; j++) {
            dp[m, j] = INF;
        }
    }

    // Base case: only node 0 visited, and we are currently at node 0.
    dp[1 << 0, 0] = 0;

    // Iterate through every subset of nodes.
    for (int mask = 0; mask < maxMask; mask++) {
        // Try every possible "current endpoint" j.
        for (int j = 0; j < n; j++) {

            // If node j is not included in mask, dp[mask,j] is invalid.
            if ((mask & (1 << j)) == 0) continue;

            int curr = dp[mask, j];

            // Skip unreachable states.
            if (curr == INF) continue;

            // Try to extend the path to some unvisited node k.
            for (int k = 0; k < n; k++) {
                // If k is already visited in mask, we can't visit it again.
                if ((mask & (1 << k)) != 0) continue;

                int nextMask = mask | (1 << k);
                int cand = curr + dist[j, k];

                // Standard DP "relaxation": keep the cheapest cost.
                if (cand < dp[nextMask, k]) {
                    dp[nextMask, k] = cand;
                }
            }
        }
    }

    // All nodes visited.
    int fullMask = maxMask - 1;

    // Best cost can end at any node j.
    int best = INF;
    for (int j = 0; j < n; j++) {
        if (dp[fullMask, j] < best) {
            best = dp[fullMask, j];
        }
    }
    return best;
}

/* ============================================================
   3) MatrixChainOrder (interval DP)
   ============================================================ */

/*
    Matrix chain multiplication optimization.

    Input:
      p: dimensions array of length n+1
         matrices are:
           A1: p[0] x p[1]
           A2: p[1] x p[2]
           ...
           An: p[n-1] x p[n]
      Here, n = p.Length - 1.

    DP state:
      dp[i, j] = minimum cost to multiply Ai..Aj (1-based indexing)

    Base:
      dp[i, i] = 0 (one matrix needs no multiplication)

    Transition:
      For i < j, split at k (i <= k < j):
        cost = dp[i,k] + dp[k+1,j] + p[i-1]*p[k]*p[j]

      Why p[i-1]*p[k]*p[j]?
        - (Ai..Ak) produces matrix of size p[i-1] x p[k]
        - (A(k+1)..Aj) produces matrix of size p[k] x p[j]
        - Multiplying them costs p[i-1]*p[k]*p[j]

    Fill order:
      Increasing chain length (len = 2..n) ensures subproblems are ready.

    Overflow note:
      We compute the candidate cost using long because multiplication
      can exceed int range; then we cast back to int after minimizing.
*/
static int MatrixChainOrder(int[] p) {
    int n = p.Length - 1;      // number of matrices
    int INF = int.MaxValue;    // initial "worst" value for minimization

    // dp dimensions: (n+1) x (n+1) so we can use 1..n indices.
    var dp = new int[n + 1, n + 1];

    // len = number of matrices in the subchain (Ai..Aj)
    for (int len = 2; len <= n; len++) {

        // i = start matrix index
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;  // j = end matrix index
            dp[i, j] = INF;

            // try every split point k
            for (int k = i; k < j; k++) {

                // Use long to avoid overflow for the multiplication term.
                long cost =
                    (long)dp[i, k] + dp[k + 1, j] +
                    (long)p[i - 1] * p[k] * p[j];

                // Relax / minimize dp[i,j]
                if (cost < dp[i, j]) {
                    dp[i, j] = (int)cost;
                }
            }
        }
    }

    // Final answer: optimal cost for multiplying A1..An.
    return dp[1, n];
}

// ===========================
// Tests (top-level)
// ===========================

Console.WriteLine("=== Test 1: MaxRootToLeaf ===\n");

/*
    Build tree:

          10
         /  \
        5    20
       / \     \
      3   7     25

    Root-to-leaf sums:
      10+5+3   = 18
      10+5+7   = 22
      10+20+25 = 55  <-- maximum
*/
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

/*
    Single node tree:
      - Root is also a leaf, so answer is just its value.
*/
TreeNode single = new(42);
int singleBest = MaxRootToLeaf(single);
Console.WriteLine($"MaxRootToLeaf(single=42) = {singleBest} (expected 42)");

/*
    Null tree:
      - No valid root-to-leaf path, so we return int.MinValue sentinel.
*/
int nullBest = MaxRootToLeaf(null);
Console.WriteLine($"MaxRootToLeaf(null) = {nullBest} (expected {int.MinValue})");

Console.WriteLine();
Console.WriteLine("=== Test 2: TspBitmask ===\n");

/*
    4-node distance matrix (symmetric example).

    We want min-cost path:
      start at 0
      visit all nodes once
      end anywhere

    One optimal path:
      0 -> 1 -> 3 -> 2
      cost = 10 + 25 + 30 = 65
*/
int[,] dist = new int[4, 4]
{
    { 0, 10, 15, 20 },
    { 10, 0, 35, 25 },
    { 15, 35, 0, 30 },
    { 20, 25, 30, 0 }
};

int tspCost = TspBitmask(4, dist);
Console.WriteLine($"TspBitmask(4 nodes) = {tspCost} (expected 65)");

Console.WriteLine();
Console.WriteLine("=== Test 3: MatrixChainOrder ===\n");

/*
    CLRS matrix chain example:
      p = [30, 35, 15, 5, 10, 20, 25]
      n = 6 matrices
      optimal cost = 15125
*/
int[] p1 = { 30, 35, 15, 5, 10, 20, 25 };
int cost1 = MatrixChainOrder(p1);
Console.WriteLine($"MatrixChainOrder(CLRS) = {cost1} (expected 15125)");

/*
    Two matrices:
      A: 10x20
      B: 20x30
    Only one multiplication:
      cost = 10*20*30 = 6000
*/
int[] p2 = { 10, 20, 30 };
int cost2 = MatrixChainOrder(p2);
Console.WriteLine($"MatrixChainOrder([10,20,30]) = {cost2} (expected 6000)");

/*
    Single matrix:
      No multiplication needed => 0 cost.
*/
int[] p3 = { 5, 10 };
int cost3 = MatrixChainOrder(p3);
Console.WriteLine($"MatrixChainOrder(single) = {cost3} (expected 0)");

/* ============================================================
   TreeNode definition (simple binary tree node)
   ============================================================ */

/*
    Small helper class for the tree tests above.

    - Primary constructor sets Val.
    - Left/Right are nullable references (TreeNode?) to support missing children.
    - Fields are public for easy test construction.

    NOTE:
      Using fields (rather than properties) keeps the class minimal for examples.
*/
class TreeNode(int val)
{
    public int Val = val;
    public TreeNode? Left;
    public TreeNode? Right;
}
