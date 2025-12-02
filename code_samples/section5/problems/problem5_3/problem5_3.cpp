#include <iostream>
#include <limits>
#include <cstdlib>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

static bool helper(TreeNode *node, long low, long high) {
    if (!node) return true;
    if (node->val <= low || node->val >= high) return false;
    return helper(node->left, low, node->val) &&
           helper(node->right, node->val, high);
}

bool isValidBST(TreeNode *root) {
    return helper(root,
                  std::numeric_limits<long>::min(),
                  std::numeric_limits<long>::max());
}

/* ----------------- Helpers for testing ----------------- */

TreeNode* node(int v) {
    return new TreeNode(v);
}

void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Print tree sideways (right subtree at the top)
void printTreeImpl(TreeNode *root, int indent) {
    if (!root) return;
    const int INDENT_STEP = 4;

    printTreeImpl(root->right, indent + INDENT_STEP);

    std::cout << std::string(indent, ' ') << root->val << "\n";

    printTreeImpl(root->left, indent + INDENT_STEP);
}

void printTree(TreeNode *root) {
    std::cout << "Tree (sideways, right is up):\n";
    printTreeImpl(root, 0);
    std::cout << "\n";
}

void testTree(const char *label, TreeNode *root) {
    std::cout << "==== " << label << " ====\n";
    printTree(root);
    std::cout << "isValidBST: " << (isValidBST(root) ? "true" : "false") << "\n\n";
}

/* ----------------- Main test harness ----------------- */

int main() {
    // 1) Empty tree (should be true)
    TreeNode *empty = nullptr;

    // 2) Single node (should be true)
    TreeNode *single = node(10);

    // 3) Valid BST:
    //
    //         4
    //       /   \
    //      2     6
    //     / \   / \
    //    1   3 5   7
    TreeNode *valid = node(4);
    valid->left = node(2);
    valid->right = node(6);
    valid->left->left = node(1);
    valid->left->right = node(3);
    valid->right->left = node(5);
    valid->right->right = node(7);

    // 4) Invalid BST (local violation):
    //
    //      5
    //     / \
    //    1   4   <-- 4 < 5 but in right subtree of 5
    //       / \
    //      3   6
    TreeNode *invalid_local = node(5);
    invalid_local->left = node(1);
    invalid_local->right = node(4);
    invalid_local->right->left = node(3);
    invalid_local->right->right = node(6);

    // 5) Invalid BST (deep violation):
    //
    //        5
    //      /   \
    //     2     8
    //      \
    //       6   <-- 6 > 5 but inside left subtree of 5
    TreeNode *invalid_deep = node(5);
    invalid_deep->left = node(2);
    invalid_deep->right = node(8);
    invalid_deep->left->right = node(6);

    // Run tests
    testTree("Empty tree (should be true)", empty);
    testTree("Single node (should be true)", single);
    testTree("Valid BST (should be true)", valid);
    testTree("Invalid BST - local violation (should be false)", invalid_local);
    testTree("Invalid BST - deep violation (should be false)", invalid_deep);

    // Cleanup
    freeTree(single);
    freeTree(valid);
    freeTree(invalid_local);
    freeTree(invalid_deep);
    // empty is nullptr, nothing to free

    return 0;
}
