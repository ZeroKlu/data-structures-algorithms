#include <iostream>
#include <cstdlib>
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

struct Info {
    bool balanced;
    int height;
};

static Info check(TreeNode *root) {
    if (!root) return { true, -1 };
    Info leftInfo = check(root->left);
    if (!leftInfo.balanced) return { false, 0 };
    Info rightInfo = check(root->right);
    if (!rightInfo.balanced) return { false, 0 };
    bool bal = std::abs(leftInfo.height - rightInfo.height) <= 1;
    int h = 1 + std::max(leftInfo.height, rightInfo.height);
    return { bal, h };
}

int height(TreeNode *root) {
    if (!root) return -1;
    return 1 + std::max(height(root->left), height(root->right));
}

bool isBalanced(TreeNode *root) {
    return check(root).balanced;
}

// -----------------------------------------------------------
// Helpers
// -----------------------------------------------------------

TreeNode* node(int v) {
    return new TreeNode(v);
}

void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Print tree sideways (same style as previous languages)
void printTreeImpl(TreeNode *root, int indent) {
    if (!root) return;

    const int INDENT = 4;

    printTreeImpl(root->right, indent + INDENT);

    std::cout << std::string(indent, ' ') << root->val << "\n";

    printTreeImpl(root->left, indent + INDENT);
}

void printTree(TreeNode *root) {
    std::cout << "Tree (sideways, right is up):\n";
    printTreeImpl(root, 0);
    std::cout << "\n";
}

void testTree(const std::string &label, TreeNode *root) {
    std::cout << "==== " << label << " ====\n";
    printTree(root);
    std::cout << "height: " << height(root) << "\n";
    std::cout << "isBalanced: " << (isBalanced(root) ? "true" : "false") << "\n\n";
}

// -----------------------------------------------------------
// Main test harness
// -----------------------------------------------------------

int main() {

    // 1. Empty tree
    TreeNode *empty = nullptr;

    // 2. Single node
    TreeNode *single = node(1);

    // 3. Perfectly balanced:
    //         1
    //       /   \
    //      2     3
    //     / \   / \
    //    4  5  6  7
    TreeNode *balanced = node(1);
    balanced->left = node(2);
    balanced->right = node(3);
    balanced->left->left = node(4);
    balanced->left->right = node(5);
    balanced->right->left = node(6);
    balanced->right->right = node(7);

    // 4. Slightly unbalanced but still balanced:
    //      1
    //    /   \
    //   2     3
    //  /
    // 4
    TreeNode *shallow = node(1);
    shallow->left = node(2);
    shallow->right = node(3);
    shallow->left->left = node(4);

    // 5. Deep unbalanced:
    //     1
    //    /
    //   2
    //  /
    // 3
    //  \
    //   4
    TreeNode *deep = node(1);
    deep->left = node(2);
    deep->left->left = node(3);
    deep->left->left->right = node(4);

    // Run tests
    testTree("Empty tree", empty);
    testTree("Single node", single);
    testTree("Perfectly balanced tree", balanced);
    testTree("Shallow unbalanced but balanced", shallow);
    testTree("Deep unbalanced (should be false)", deep);

    // Cleanup
    freeTree(single);
    freeTree(balanced);
    freeTree(shallow);
    freeTree(deep);

    return 0;
}
