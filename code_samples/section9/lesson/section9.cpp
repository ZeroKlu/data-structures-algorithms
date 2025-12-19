#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

/*
    This file demonstrates THREE classic problems (and a test harness):

    1) maxRootToLeaf (tree recursion / tree DP)
       - Computes the maximum sum along any path starting at the root
         and ending at a leaf.
       - Uses recursion: answer(node) = node->val + max(answer(left), answer(right))

    2) tspBitmask (DP over subsets / bitmask DP)
       - Computes the minimum cost path that:
            * starts at node 0
            * visits every node exactly once
            * ends at ANY node (NOT necessarily returning to 0)
       - dp[mask][j] = minimum cost to start at 0, visit nodes in 'mask',
         and finish at node j.

    3) matrixChainOrder (interval DP)
       - Given matrix dimensions p = [p0, p1, ..., pn],
         where Ai is p[i-1] x p[i],
         compute the minimum scalar multiplications needed to compute A1..An.
       - dp[i][j] = min cost to multiply Ai..Aj

    All three functions are tested in main().
*/

/* =============================
   Basic Binary Tree Structure
   ============================= */

/*
    TreeNode represents a node in a binary tree.
    - val: value stored at this node
    - left/right: pointers to child nodes (may be nullptr)
*/
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// =============================
// Functions under test
// =============================

/* ============================================================
   1) maxRootToLeaf
   ============================================================ */
/*
    Return the maximum root-to-leaf path sum.

    Definitions:
      - A "root-to-leaf path" starts at 'root' and ends at a node
        that has no children (a leaf).
      - If the tree is empty (root == nullptr), there is no valid path.

    Base cases:
      (A) root == nullptr
          Return INT_MIN as a sentinel meaning "no path exists".
          This is useful when taking max(leftBest, rightBest):
          - a missing child should never beat a real path.
      (B) leaf node
          If both children are nullptr, the best path is just root->val.

    Recursive case:
      best(root) = root->val + max(best(root->left), best(root->right))

    Time complexity:  O(number of nodes)
    Space complexity: O(height of tree) due to recursion stack
*/
int maxRootToLeaf(TreeNode* root) {
    /* Empty tree: no root-to-leaf path exists. */
    if (!root) {
        return std::numeric_limits<int>::min();
    }

    /* Leaf node: the only path sum is the node itself. */
    if (!root->left && !root->right) {
        return root->val;
    }

    /* Recursively compute best sums from each subtree. */
    int leftBest = maxRootToLeaf(root->left);
    int rightBest = maxRootToLeaf(root->right);

    /* Choose the better child path and add current node's value. */
    return root->val + std::max(leftBest, rightBest);
}

/* ============================================================
   2) tspBitmask
   ============================================================ */
/*
    Bitmask DP for a TSP-like problem:

    Input:
      - n: number of nodes (0..n-1)
      - dist: adjacency matrix where dist[i][j] is cost from i to j

    Goal:
      - Start at node 0
      - Visit every node exactly once
      - Minimize total cost
      - END anywhere (we do NOT add cost to return to node 0)

    DP state:
      dp[mask][j] = min cost to:
        - start at node 0
        - visit exactly the nodes whose bits are set in 'mask'
        - end at node j

      Constraints:
        - j must be included in mask (mask & (1<<j)) != 0

    Initialization:
      dp[1<<0][0] = 0
      (only node 0 visited; we're at 0 with cost 0)

    Transition:
      From state (mask, j), try to go to an unvisited node k:
        nextMask = mask | (1<<k)
        dp[nextMask][k] = min(dp[nextMask][k], dp[mask][j] + dist[j][k])

    Answer:
      fullMask = (1<<n) - 1
      best = min over j of dp[fullMask][j]

    Complexity:
      - States: 2^n * n
      - Transitions: for each state, consider up to n next nodes
      => O(n^2 * 2^n) time, O(n * 2^n) space

    NOTE:
      If you wanted the classic TSP TOUR (return to start),
      you would compute:
        bestTour = min_j dp[fullMask][j] + dist[j][0]
*/
int tspBitmask(int n, const std::vector<std::vector<int>>& dist) {
    /* Total number of subsets of n nodes. */
    int maxMask = 1 << n;

    /*
        Use a large "infinity" value.
        We pick INT_MAX/4 (instead of INT_MAX) to reduce overflow risk when adding.
    */
    const int INF = std::numeric_limits<int>::max() / 4;

    /*
        dp[mask][j] initialized to INF:
          "unknown/unreachable until proven otherwise"
    */
    std::vector<std::vector<int>> dp(maxMask, std::vector<int>(n, INF));

    /* Base case: start at node 0 having visited only node 0. */
    dp[1 << 0][0] = 0;

    /*
        Iterate over all masks (subsets), then all possible endpoints j.
        Only proceed if j is actually in the subset AND dp state is reachable.
    */
    for (int mask = 0; mask < maxMask; mask++) {
        for (int j = 0; j < n; j++) {

            /* If node j isn't in the visited set, dp[mask][j] is invalid. */
            if (!(mask & (1 << j))) continue;

            int curr = dp[mask][j];

            /* Skip unreachable states. */
            if (curr == INF) continue;

            /* Try to go to each node k that is NOT yet visited. */
            for (int k = 0; k < n; k++) {
                if (mask & (1 << k)) continue;  // k already visited

                int nextMask = mask | (1 << k);
                int cand = curr + dist[j][k];

                /* Relax the DP value (keep the cheapest). */
                dp[nextMask][k] = std::min(dp[nextMask][k], cand);
            }
        }
    }

    /* All nodes visited. */
    int fullMask = maxMask - 1;

    /* Min cost among paths that end at any node j. */
    int best = INF;
    for (int j = 0; j < n; j++) {
        best = std::min(best, dp[fullMask][j]);
    }
    return best;
}

/* ============================================================
   3) matrixChainOrder
   ============================================================ */
/*
    Matrix Chain Multiplication:

    Input:
      p = [p0, p1, ..., pn]
      There are n matrices:
        A1: p0 x p1
        A2: p1 x p2
        ...
        An: p(n-1) x pn

    Goal:
      Compute minimal scalar multiplications to multiply A1..An
      by choosing an optimal parenthesization.

    DP state (1-based matrix indices):
      dp[i][j] = min cost to multiply Ai..Aj (inclusive)

    Base:
      dp[i][i] = 0 (single matrix needs no multiplication)

    Transition:
      For i < j, split at k (i <= k < j):
        (Ai..Ak) * (A(k+1)..Aj)

      Cost:
        dp[i][k] + dp[k+1][j] + (p[i-1] * p[k] * p[j])

      Explanation of the multiplication cost term:
        - Result of (Ai..Ak) has dimension p[i-1] x p[k]
        - Result of (A(k+1)..Aj) has dimension p[k] x p[j]
        - Multiplying them costs p[i-1] * p[k] * p[j] scalar multiplications.

    Order of computation:
      Fill by increasing interval length (len = 2..n),
      so smaller subproblems are already solved.

    Complexity:
      O(n^3) time, O(n^2) space
*/
int matrixChainOrder(const std::vector<int>& p) {
    /* Number of matrices. If p has length L, then n = L-1 matrices. */
    int n = (int)p.size() - 1;

    /* A large value for initialization. */
    const int INF = std::numeric_limits<int>::max();

    /*
        dp is (n+1) x (n+1) to use 1..n indexing cleanly.
        dp[i][j] = 0 by default; we'll set dp[i][j] for i<j explicitly.
    */
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));

    /*
        len is the chain length (# of matrices in subproblem).
        Start at 2 because length 1 has cost 0 already.
    */
    for (int len = 2; len <= n; len++) {

        /* i is the starting matrix index. */
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;     // ending matrix index
            dp[i][j] = INF;          // initialize to "infinity"

            /* Try all split positions k. */
            for (int k = i; k < j; k++) {

                /*
                    Use long long for cost to avoid integer overflow in:
                      p[i-1] * p[k] * p[j]
                    and summation of costs.
                */
                long long cost =
                    (long long)dp[i][k] +
                    (long long)dp[k + 1][j] +
                    (long long)p[i - 1] * p[k] * p[j];

                /* Keep the best (minimum) cost among all k splits. */
                dp[i][j] = std::min(dp[i][j], (int)cost);
            }
        }
    }

    /* Final answer: cost to multiply A1..An. */
    return dp[1][n];
}

// =============================
// Helpers
// =============================

/*
    Convenience allocator for a TreeNode.
    - Returns a heap-allocated node with given value and null children.
*/
TreeNode* node(int v) {
    TreeNode* t = new TreeNode{v, nullptr, nullptr};
    return t;
}

/*
    Recursively free a tree (post-order: children first).
    This prevents memory leaks in tests.
*/
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// =============================
// Test Harness
// =============================

int main() {
    std::cout << "=== Test 1: maxRootToLeaf ===\n\n";

    /*
        Build this tree:

              10
             /  \
            5    20
           / \     \
          3   7     25

        Root-to-leaf sums:
          10+5+3   = 18
          10+5+7   = 22
          10+20+25 = 55  <-- best

        So expected answer = 55.
    */
    TreeNode* root = node(10);
    root->left = node(5);
    root->right = node(20);
    root->left->left = node(3);
    root->left->right = node(7);
    root->right->right = node(25);

    int result1 = maxRootToLeaf(root);
    std::cout << "maxRootToLeaf(tree) = " << result1
              << " (expected 55)\n";

    /*
        Edge case: single node tree.
        Root is also a leaf, so max path sum is just its value.
    */
    TreeNode* single = node(42);
    int resultSingle = maxRootToLeaf(single);
    std::cout << "maxRootToLeaf(single=42) = " << resultSingle
              << " (expected 42)\n";

    /*
        Edge case: nullptr tree.
        Our function returns numeric_limits<int>::min() as a sentinel meaning "no path".
    */
    int resultNull = maxRootToLeaf(nullptr);
    std::cout << "maxRootToLeaf(nullptr) = " << resultNull
              << " (expected " << std::numeric_limits<int>::min() << ")\n\n";

    /* Free allocated nodes to avoid leaks. */
    freeTree(root);
    freeTree(single);

    // =============================
    // TSP BITMASK TEST
    // =============================
    std::cout << "=== Test 2: tspBitmask ===\n\n";

    /*
        Distance matrix for 4 nodes:

            0  10  15  20
           10   0  35  25
           15  35   0  30
           20  25  30   0

        We want the minimum-cost path that:
          - starts at node 0
          - visits all nodes exactly once
          - ends anywhere

        Best path:
          0 → 1 → 3 → 2
          cost = 10 + 25 + 30 = 65
    */
    std::vector<std::vector<int>> dist = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    int tspAnswer = tspBitmask(4, dist);
    std::cout << "tspBitmask(4 nodes) = " << tspAnswer
              << " (expected 65)\n\n";

    // =============================
    // MATRIX CHAIN ORDER TEST
    // =============================
    std::cout << "=== Test 3: matrixChainOrder ===\n\n";

    /*
        Standard CLRS example:
          p = [30,35,15,5,10,20,25]
          n = 6 matrices
          known optimal cost = 15125
    */
    std::vector<int> p1 = {30, 35, 15, 5, 10, 20, 25};
    int mco1 = matrixChainOrder(p1);
    std::cout << "matrixChainOrder(CLRS example) = "
              << mco1 << " (expected 15125)\n";

    /*
        Simple example with 2 matrices:
          A: 10x20
          B: 20x30
        Only one way to multiply, cost = 10*20*30 = 6000
    */
    std::vector<int> p2 = {10, 20, 30};
    int mco2 = matrixChainOrder(p2);
    std::cout << "matrixChainOrder(10×20×30) = "
              << mco2 << " (expected 6000)\n";

    /*
        Single matrix:
          No multiplication needed => 0 cost.
        p = [5,10] means one matrix 5x10.
    */
    std::vector<int> p3 = {5, 10};
    int mco3 = matrixChainOrder(p3);
    std::cout << "matrixChainOrder(single matrix) = "
              << mco3 << " (expected 0)\n";

    return 0;
}
