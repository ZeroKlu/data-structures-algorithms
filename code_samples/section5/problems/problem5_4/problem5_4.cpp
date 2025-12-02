#include <iostream>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

bool hasPathSum(TreeNode *root, int targetSum) {
    if (!root) return false;
    if (!root->left && !root->right) {
        return root->val == targetSum;
    }
    int remaining = targetSum - root->val;
    return hasPathSum(root->left, remaining) ||
           hasPathSum(root->right, remaining);
}

// ----------------------------
// Helpers
// ----------------------------

TreeNode* node(int v) {
    return new TreeNode(v);
}

void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Sideways tree printer
void printTreeImpl(TreeNode *root, int indent) {
    if (!root) return;
    const int STEP = 4;

    printTreeImpl(root->right, indent + STEP);

    std::cout << std::string(indent, ' ') << root->val << "\n";

    printTreeImpl(root->left, indent + STEP);
}

void printTree(TreeNode *root) {
    std::cout << "Tree (sideways, right is up):\n";
    printTreeImpl(root, 0);
    std::cout << "\n";
}

void testCase(const char *label, TreeNode *root, int target) {
    std::cout << "==== " << label << " ====\n";
    printTree(root);
    std::cout << "Target sum: " << target << "\n";
    std::cout << "hasPathSum: " << (hasPathSum(root, target) ? "true" : "false") << "\n\n";
}

// ----------------------------
// Main test harness
// ----------------------------

int main() {
    // 1. Empty tree
    TreeNode *empty = nullptr;

    // 2. Single node tree
    TreeNode *single = node(7);

    // 3. Classic LeetCode-style tree:
    //
    //         5
    //       /   \
    //      4     8
    //     /     / \
    //    11    13  4
    //   /  \        \
    //  7    2        1
    TreeNode *t = node(5);
    t->left = node(4);
    t->right = node(8);

    t->left->left = node(11);
    t->left->left->left = node(7);
    t->left->left->right = node(2);

    t->right->left = node(13);
    t->right->right = node(4);
    t->right->right->right = node(1);

    // Path sums present:
    // 5+4+11+7 = 27
    // 5+4+11+2 = 22
    // 5+8+13   = 26
    // 5+8+4+1  = 18

    testCase("Empty tree", empty, 10);
    testCase("Single node (7 == 7 → true)", single, 7);
    testCase("Single node (7 != 10 → false)", single, 10);

    testCase("Large tree (has path sum 22 → true)", t, 22);
    testCase("Large tree (has path sum 26 → true)", t, 26);
    testCase("Large tree (has path sum 18 → true)", t, 18);
    testCase("Large tree (no path sum == 5 → false)", t, 5); // 5 not a leaf

    freeTree(single);
    freeTree(t);
    // empty is nullptr

    return 0;
}
