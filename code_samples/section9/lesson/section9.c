#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
    This file contains THREE classic dynamic-programming / recursion examples:

    1) max_root_to_leaf:
       - Given a binary tree, compute the maximum sum along any path from
         the root down to ANY leaf.
       - This is a recursion problem (tree DP).

    2) tsp_bitmask:
       - Traveling Salesman style DP using a bitmask:
         "Start at node 0, visit every node exactly once, minimize total cost."
       - NOTE: This version finds the best HAMILTONIAN PATH that starts at 0
         and ends at ANY node (it does NOT force returning to node 0).

    3) matrix_chain_order:
       - Matrix Chain Multiplication DP:
         Given dimensions p[0..n], where matrix Ai has dimension
         p[i-1] x p[i], compute the minimum scalar multiplications needed to
         multiply A1*A2*...*An.
       - This is a classic interval DP.

    The bottom portion provides a simple test harness for each.
*/

/* ===========================
   Binary Tree Definition
   =========================== */

typedef struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

/* Utility: return max(a, b) */
static int max_int(int a, int b) {
    return a > b ? a : b;
}

/* ============================================================
   1) max_root_to_leaf
   ============================================================ */
/*
    Returns the maximum root-to-leaf path sum.

    Idea:
      - For each node, the best root-to-leaf sum *through* that node is:
            node->val + max(best(left), best(right))
      - Base cases:
          * NULL node: "no path" → return INT_MIN
            (so it won't be chosen over a real path)
          * leaf node: best sum = node->val

    Why INT_MIN for NULL?
      - If one subtree is missing, we want the other subtree's sum to win.
      - Using INT_MIN ensures missing paths are never chosen.
*/
int max_root_to_leaf(TreeNode* root) {
    /* If there is no node, there is no valid root-to-leaf path. */
    if (!root) {
        return INT_MIN; // sentinel: indicates "no path"
    }

    /* Leaf node: the only path is the node itself. */
    if (!root->left && !root->right) {
        return root->val;
    }

    /* Recursively compute best root-to-leaf sums from children. */
    int left_best = max_root_to_leaf(root->left);
    int right_best = max_root_to_leaf(root->right);

    /* Choose the better child path and add current node value. */
    return root->val + max_int(left_best, right_best);
}

/* ============================================================
   2) tsp_bitmask (DP over subsets)
   ============================================================ */
/*
    Problem:
      - n nodes labeled [0..n-1]
      - dist[i][j] is the cost from i to j (assumed non-negative here)
      - We start at node 0, must visit all nodes exactly once
      - We want the minimum cost among all paths that satisfy this

    State definition:
      dp[mask][j] = minimum cost to start at node 0,
                   visit exactly the set of nodes in 'mask',
                   and end at node j.

      Where:
        - mask is an n-bit number. Bit k is 1 if node k is included/visited.
        - j must be in mask.

    Initialization:
      dp[1<<0][0] = 0
      (We have visited only node 0, and we are at node 0 with cost 0.)

    Transition:
      From dp[mask][j], try to go to a new node k not in mask:
        nextMask = mask | (1<<k)
        dp[nextMask][k] = min(dp[nextMask][k], dp[mask][j] + dist[j][k])

    Answer:
      - fullMask = (1<<n) - 1  (all nodes visited)
      - We take min over dp[fullMask][j] for all j
        (end anywhere; no "return to start" requirement)

    Complexity:
      - States: 2^n * n
      - Transitions: for each state, we try up to n next nodes
      => O(n^2 * 2^n) time, O(n * 2^n) space
*/
int tsp_bitmask(int n, int** dist) {
    int maxMask = 1 << n;

    /*
        INF: a large number representing "unreachable"
        We use INT_MAX/4 to avoid overflow when adding costs.
    */
    const int INF = INT_MAX / 4;

    /*
        Allocate dp as a 2D array:
          dp[mask] is an int array of length n.
    */
    int** dp = (int**)malloc(maxMask * sizeof(int*));
    if (!dp) {
        fprintf(stderr, "Out of memory allocating dp rows\n");
        exit(EXIT_FAILURE);
    }

    for (int mask = 0; mask < maxMask; mask++) {
        dp[mask] = (int*)malloc(n * sizeof(int));
        if (!dp[mask]) {
            fprintf(stderr, "Out of memory allocating dp[%d]\n", mask);
            exit(EXIT_FAILURE);
        }

        /* Initialize all endpoints to INF (unknown/unreachable). */
        for (int j = 0; j < n; j++) {
            dp[mask][j] = INF;
        }
    }

    /* Base case: start at node 0 with only node 0 visited. */
    dp[1 << 0][0] = 0;

    /*
        Iterate over all subsets (masks).
        For each subset, for each endpoint j in that subset,
        attempt to extend path to a new node k not yet visited.
    */
    for (int mask = 0; mask < maxMask; mask++) {

        for (int j = 0; j < n; j++) {

            /* If j is not in the subset, dp[mask][j] is not a valid state. */
            if (!(mask & (1 << j))) continue;

            int curr = dp[mask][j];
            /* If unreachable, skip. */
            if (curr == INF) continue;

            /* Try adding a next node k not yet visited in mask. */
            for (int k = 0; k < n; k++) {
                if (mask & (1 << k)) continue; // already visited

                int nextMask = mask | (1 << k);
                int cand = curr + dist[j][k];

                /* Relax (take the minimum). */
                if (cand < dp[nextMask][k]) {
                    dp[nextMask][k] = cand;
                }
            }
        }
    }

    /* Compute answer from fullMask: min over all endpoints j. */
    int fullMask = maxMask - 1;
    int best = INF;
    for (int j = 0; j < n; j++) {
        if (dp[fullMask][j] < best) {
            best = dp[fullMask][j];
        }
    }

    /* Free dp memory. */
    for (int mask = 0; mask < maxMask; mask++) {
        free(dp[mask]);
    }
    free(dp);

    return best;
}

/* ============================================================
   3) matrix_chain_order (Interval DP)
   ============================================================ */
/*
    Problem:
      We have n matrices A1..An.
      Dimensions are given by array p of length (n+1):
        A1 is p[0] x p[1]
        A2 is p[1] x p[2]
        ...
        An is p[n-1] x p[n]

      We want the MINIMUM number of scalar multiplications needed to compute:
        A1 * A2 * ... * An
      by choosing the best parenthesization.

    State:
      dp[i][j] = minimum cost to multiply matrices Ai..Aj (1-based indices)

    Base:
      dp[i][i] = 0 (single matrix: no multiplication)

    Transition:
      For i < j, pick a split point k:
        (Ai..Ak) * (A(k+1)..Aj)
      Cost = dp[i][k] + dp[k+1][j] + cost_to_multiply_the_two_results

      The resulting dimensions:
        (Ai..Ak) has dimension p[i-1] x p[k]
        (A(k+1)..Aj) has dimension p[k] x p[j]
      Multiply them cost:
        p[i-1] * p[k] * p[j]

      dp[i][j] = min over k in [i..j-1] of that cost.

    Iteration order:
      We compute by increasing segment length (len = 2..n),
      ensuring subproblems are solved before bigger intervals.

    Complexity:
      O(n^3) time, O(n^2) space
*/
int matrix_chain_order(int n, const int* p) {
    /*
        Allocate dp as (n+1) x (n+1) so we can use 1..n indexing.
        dp[0][*] is unused, but simplifies the formulas.
    */
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    if (!dp) {
        fprintf(stderr, "Out of memory allocating dp rows\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)malloc((n + 1) * sizeof(int));
        if (!dp[i]) {
            fprintf(stderr, "Out of memory allocating dp[%d]\n", i);
            exit(EXIT_FAILURE);
        }
    }

    /* Base case: cost of multiplying one matrix is 0. */
    for (int i = 1; i <= n; i++) {
        dp[i][i] = 0;
    }

    /*
        Consider chain lengths from 2 up to n.
        len = 2 means two matrices, the smallest non-trivial multiplication.
    */
    for (int len = 2; len <= n; len++) {

        /* i is the starting matrix index for this interval. */
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;  // j is the ending matrix index

            dp[i][j] = INT_MAX;   // initialize to "infinity"

            /* Try every possible split point k. */
            for (int k = i; k < j; k++) {

                /*
                    Use long long for intermediate to avoid overflow when computing:
                      p[i-1] * p[k] * p[j]
                    and summing dp values.
                */
                long long cost =
                    (long long)dp[i][k] +
                    (long long)dp[k + 1][j] +
                    (long long)p[i - 1] * p[k] * p[j];

                if (cost < dp[i][j]) {
                    dp[i][j] = (int)cost;
                }
            }
        }
    }

    int result = dp[1][n];

    /* Free dp memory. */
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
}

/* ============================================================
   Helpers for Testing (Tree creation / cleanup)
   ============================================================ */

/* Create a new TreeNode with given value (heap allocated). */
TreeNode* new_node(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) {
        fprintf(stderr, "Out of memory allocating TreeNode\n");
        exit(EXIT_FAILURE);
    }
    node->val = val;
    node->left = node->right = NULL;
    return node;
}

/* Post-order free (children first, then the node). */
void free_tree(TreeNode* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

/* ============================================================
   Main Test Harness
   ============================================================ */
int main(void) {
    printf("=== Test 1: max_root_to_leaf ===\n\n");

    /*
        Build this tree:

              10
             /  \
            5    20
           / \     \
          3   7     25

        Root-to-leaf sums:
          10+5+3  = 18
          10+5+7  = 22
          10+20+25 = 55   <-- maximum
    */
    TreeNode* root = new_node(10);
    root->left = new_node(5);
    root->right = new_node(20);
    root->left->left = new_node(3);
    root->left->right = new_node(7);
    root->right->right = new_node(25);

    /* Verify main case. */
    int best_path_sum = max_root_to_leaf(root);
    printf("max_root_to_leaf(tree) = %d (expected 55)\n", best_path_sum);

    /* Edge case: single-node tree. */
    TreeNode* single = new_node(42);
    int single_sum = max_root_to_leaf(single);
    printf("max_root_to_leaf(single-node 42) = %d (expected 42)\n", single_sum);

    /*
        Edge case: NULL pointer.
        By our design, this returns INT_MIN to indicate "no path exists".
        (This is useful when called from parents so missing branches lose.)
    */
    int null_sum = max_root_to_leaf(NULL);
    printf("max_root_to_leaf(NULL) = %d (expected INT_MIN=%d)\n\n",
           null_sum, INT_MIN);

    /* Cleanup tree allocations. */
    free_tree(root);
    free_tree(single);

    printf("=== Test 2: tsp_bitmask (path visiting all nodes, start at 0) ===\n\n");

    /*
        Example 4-node symmetric distance matrix:

            0  10  15  20
           10   0  35  25
           15  35   0  30
           20  25  30   0

        We want the minimum-cost path:
          - starts at node 0
          - visits all nodes exactly once
          - ends anywhere

        Enumerated possibilities (starting at 0):
          0-1-2-3: 10+35+30 = 75
          0-1-3-2: 10+25+30 = 65  <-- min
          0-2-1-3: 15+35+25 = 75
          0-2-3-1: 15+30+25 = 70
          0-3-1-2: 20+25+35 = 80
          0-3-2-1: 20+30+35 = 85
    */
    int n = 4;

    /* Allocate dist as int** and copy rawDist into it. */
    int** dist = (int**)malloc(n * sizeof(int*));
    if (!dist) {
        fprintf(stderr, "Out of memory allocating dist rows\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        dist[i] = (int*)malloc(n * sizeof(int));
        if (!dist[i]) {
            fprintf(stderr, "Out of memory allocating dist[%d]\n", i);
            exit(EXIT_FAILURE);
        }
    }

    int rawDist[4][4] = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = rawDist[i][j];
        }
    }

    /* Run and verify. */
    int tsp_cost = tsp_bitmask(n, dist);
    printf("tsp_bitmask(n=4) = %d (expected 65)\n\n", tsp_cost);

    /* Free dist. */
    for (int i = 0; i < n; i++) {
        free(dist[i]);
    }
    free(dist);

    printf("=== Test 3: matrix_chain_order ===\n\n");

    /*
        Classic matrix-chain example:
          p = [30, 35, 15, 5, 10, 20, 25]
          n = 6 matrices:
            A1: 30x35
            A2: 35x15
            A3: 15x5
            A4: 5x10
            A5: 10x20
            A6: 20x25
          Known optimal cost = 15125
    */
    int p1[] = {30, 35, 15, 5, 10, 20, 25};
    int n1 = 6;
    int cost1 = matrix_chain_order(n1, p1);
    printf("matrix_chain_order(n=6, classic) = %d (expected 15125)\n", cost1);

    /*
        Two matrices:
          A: 10x20
          B: 20x30
        Cost = 10*20*30 = 6000
    */
    int p2[] = {10, 20, 30};
    int n2 = 2;
    int cost2 = matrix_chain_order(n2, p2);
    printf("matrix_chain_order(n=2, [10,20,30]) = %d (expected 6000)\n", cost2);

    /*
        Single matrix:
          No multiplication required → cost = 0
    */
    int p3[] = {5, 10};
    int n3 = 1;
    int cost3 = matrix_chain_order(n3, p3);
    printf("matrix_chain_order(n=1, [5,10]) = %d (expected 0)\n", cost3);

    return 0;
}
