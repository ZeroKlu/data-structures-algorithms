#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// =============================
// Functions under test
// =============================

int maxRootToLeaf(TreeNode* root) {
    if (!root) {
        return std::numeric_limits<int>::min();
    }
    if (!root->left && !root->right) {
        return root->val;
    }
    int leftBest = maxRootToLeaf(root->left);
    int rightBest = maxRootToLeaf(root->right);
    return root->val + std::max(leftBest, rightBest);
}

int tspBitmask(int n, const std::vector<std::vector<int>>& dist) {
    int maxMask = 1 << n;
    const int INF = std::numeric_limits<int>::max() / 4;

    std::vector<std::vector<int>> dp(maxMask, std::vector<int>(n, INF));
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
                dp[nextMask][k] = std::min(dp[nextMask][k], cand);
            }
        }
    }

    int fullMask = maxMask - 1;
    int best = INF;
    for (int j = 0; j < n; j++) {
        best = std::min(best, dp[fullMask][j]);
    }
    return best;
}

int matrixChainOrder(const std::vector<int>& p) {
    int n = (int)p.size() - 1;
    const int INF = std::numeric_limits<int>::max();

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));

    for (int len = 2; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = INF;
            for (int k = i; k < j; k++) {
                long long cost = (long long)dp[i][k] + dp[k + 1][j]
                    + (long long)p[i - 1] * p[k] * p[j];
                dp[i][j] = std::min(dp[i][j], (int)cost);
            }
        }
    }
    return dp[1][n];
}

// =============================
// Helpers
// =============================

TreeNode* node(int v) {
    TreeNode* t = new TreeNode{v, nullptr, nullptr};
    return t;
}

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

    // Build this tree:
    //       10
    //      /  \
    //     5    20
    //    / \     \
    //   3   7     25
    //
    // Best root-to-leaf path = 10 + 20 + 25 = 55

    TreeNode* root = node(10);
    root->left = node(5);
    root->right = node(20);
    root->left->left = node(3);
    root->left->right = node(7);
    root->right->right = node(25);

    int result1 = maxRootToLeaf(root);
    std::cout << "maxRootToLeaf(tree) = " << result1
              << " (expected 55)\n";

    TreeNode* single = node(42);
    int resultSingle = maxRootToLeaf(single);
    std::cout << "maxRootToLeaf(single=42) = " << resultSingle
              << " (expected 42)\n";

    int resultNull = maxRootToLeaf(nullptr);
    std::cout << "maxRootToLeaf(nullptr) = " << resultNull
              << " (expected " << std::numeric_limits<int>::min() << ")\n\n";

    freeTree(root);
    freeTree(single);

    // =============================
    // TSP BITMASK TEST
    // =============================
    std::cout << "=== Test 2: tspBitmask ===\n\n";

    std::vector<std::vector<int>> dist = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    // Best tour from node 0 visiting all nodes:
    // 0 → 1 → 3 → 2 = 10 + 25 + 30 = 65

    int tspAnswer = tspBitmask(4, dist);
    std::cout << "tspBitmask(4 nodes) = " << tspAnswer
              << " (expected 65)\n\n";

    // =============================
    // MATRIX CHAIN ORDER TEST
    // =============================
    std::cout << "=== Test 3: matrixChainOrder ===\n\n";

    // Standard CLRS example:
    // p = [30,35,15,5,10,20,25]
    // 6 matrices => optimal cost = 15125
    std::vector<int> p1 = {30, 35, 15, 5, 10, 20, 25};
    int mco1 = matrixChainOrder(p1);
    std::cout << "matrixChainOrder(CLRS example) = "
              << mco1 << " (expected 15125)\n";

    // Simple example: A(10×20), B(20×30)
    // cost = 10*20*30 = 6000
    std::vector<int> p2 = {10, 20, 30};
    int mco2 = matrixChainOrder(p2);
    std::cout << "matrixChainOrder(10×20×30) = "
              << mco2 << " (expected 6000)\n";

    // Single matrix: cost = 0
    std::vector<int> p3 = {5, 10};
    int mco3 = matrixChainOrder(p3);
    std::cout << "matrixChainOrder(single matrix) = "
              << mco3 << " (expected 0)\n";

    return 0;
}
