#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

static int max_int(int a, int b) {
    return a > b ? a : b;
}

int max_root_to_leaf(TreeNode* root) {
    if (!root) {
        return INT_MIN; // no path
    }
    if (!root->left && !root->right) {
        return root->val;
    }
    int left_best = max_root_to_leaf(root->left);
    int right_best = max_root_to_leaf(root->right);
    return root->val + max_int(left_best, right_best);
}

int tsp_bitmask(int n, int** dist) {
    int maxMask = 1 << n;
    const int INF = INT_MAX / 4;

    int** dp = (int**)malloc(maxMask * sizeof(int*));
    for (int m = 0; m < maxMask; m++) {
        dp[m] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            dp[m][j] = INF;
        }
    }
    dp[1 << 0][0] = 0;  // start at node 0

    for (int mask = 0; mask < maxMask; mask++) {
        for (int j = 0; j < n; j++) {
            if (!(mask & (1 << j))) continue;
            int curr = dp[mask][j];
            if (curr == INF) continue;
            for (int k = 0; k < n; k++) {
                if (mask & (1 << k)) continue;
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

    for (int m = 0; m < maxMask; m++) {
        free(dp[m]);
    }
    free(dp);
    return best;
}

int matrix_chain_order(int n, const int* p) {
    // p has length n + 1, matrices A1..An
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)malloc((n + 1) * sizeof(int));
    }

    for (int i = 1; i <= n; i++) {
        dp[i][i] = 0;
    }

    for (int len = 2; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; k++) {
                long long cost = (long long)dp[i][k] + dp[k + 1][j]
                    + (long long)p[i - 1] * p[k] * p[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = (int)cost;
                }
            }
        }
    }

    int result = dp[1][n];
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);
    return result;
}

// Helper to create a new tree node
TreeNode* new_node(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = node->right = NULL;
    return node;
}

// Helper to free a tree
void free_tree(TreeNode* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    printf("=== Test 1: max_root_to_leaf ===\n\n");

    // Build this tree:
    //       10
    //      /  \
    //     5    20
    //    / \     \
    //   3   7     25
    //
    // Root-to-leaf sums:
    // 10+5+3 = 18
    // 10+5+7 = 22
    // 10+20+25 = 55  <-- max

    TreeNode* root = new_node(10);
    root->left = new_node(5);
    root->right = new_node(20);
    root->left->left = new_node(3);
    root->left->right = new_node(7);
    root->right->right = new_node(25);

    int best_path_sum = max_root_to_leaf(root);
    printf("max_root_to_leaf(tree) = %d (expected 55)\n", best_path_sum);

    // Edge case: single node
    TreeNode* single = new_node(42);
    int single_sum = max_root_to_leaf(single);
    printf("max_root_to_leaf(single-node 42) = %d (expected 42)\n", single_sum);

    // Edge case: NULL
    int null_sum = max_root_to_leaf(NULL);
    printf("max_root_to_leaf(NULL) = %d (expected INT_MIN=%d)\n\n",
           null_sum, INT_MIN);

    free_tree(root);
    free_tree(single);

    printf("=== Test 2: tsp_bitmask (path visiting all nodes, start at 0) ===\n\n");

    // Example 4-node graph (symmetric TSP-like):
    // Distances:
    // 0  10 15 20
    // 10  0 35 25
    // 15 35  0 30
    // 20 25 30  0
    //
    // We want min cost path starting at 0 visiting all nodes exactly once.
    //
    // Possible paths:
    // 0-1-2-3: 10+35+30 = 75
    // 0-1-3-2: 10+25+30 = 65  <-- min
    // 0-2-1-3: 15+35+25 = 75
    // 0-2-3-1: 15+30+25 = 70
    // 0-3-1-2: 20+25+35 = 80
    // 0-3-2-1: 20+30+35 = 85

    int n = 4;
    int** dist = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        dist[i] = (int*)malloc(n * sizeof(int));
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

    int tsp_cost = tsp_bitmask(n, dist);
    printf("tsp_bitmask(n=4) = %d (expected 65)\n\n", tsp_cost);

    for (int i = 0; i < n; i++) {
        free(dist[i]);
    }
    free(dist);

    printf("=== Test 3: matrix_chain_order ===\n\n");

    // Classic example:
    // p = [30, 35, 15, 5, 10, 20, 25] => 6 matrices
    // Optimal cost = 15125
    int p1[] = {30, 35, 15, 5, 10, 20, 25};
    int n1 = 6;
    int cost1 = matrix_chain_order(n1, p1);
    printf("matrix_chain_order(n=6, classic) = %d (expected 15125)\n", cost1);

    // Simple example: two matrices A(10x20), B(20x30)
    // p = [10, 20, 30], n=2 => cost = 10*20*30 = 6000
    int p2[] = {10, 20, 30};
    int n2 = 2;
    int cost2 = matrix_chain_order(n2, p2);
    printf("matrix_chain_order(n=2, [10,20,30]) = %d (expected 6000)\n", cost2);

    // Single matrix: no multiplication needed => cost = 0
    int p3[] = {5, 10};
    int n3 = 1;
    int cost3 = matrix_chain_order(n3, p3);
    printf("matrix_chain_order(n=1, [5,10]) = %d (expected 0)\n", cost3);

    return 0;
}
