#include <iostream>
#include <limits>   // std::numeric_limits
#include <cstdlib>

// ==========================
// BINARY TREE NODE DEFINITION
// ==========================

// A simple binary tree node:
// - val:   value stored in the node
// - left:  pointer to left child (nullptr if none)
// - right: pointer to right child (nullptr if none)
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    // Constructor initializes value and sets children to nullptr
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// ==========================
// VALIDATE BST (RANGE METHOD)
// ==========================

// Helper function that validates the BST property using an exclusive range (low, high).
//
// At every node:
// - node->val must be strictly greater than low
// - node->val must be strictly less than high
//
// This catches "deep violations" where a node may be in the left subtree of some
// ancestor but still be greater than that ancestor (or vice versa).
static bool helper(TreeNode *node, long low, long high) {
    // Base case: empty subtree is valid
    if (!node) return true;

    // If current value violates the allowable range, tree is not a BST
    if (node->val <= low || node->val >= high) return false;

    // Left subtree values must be in (low, node->val)
    // Right subtree values must be in (node->val, high)
    return helper(node->left, low, node->val) &&
           helper(node->right, node->val, high);
}

// Public wrapper: validates the entire tree as a BST.
// Starts with the widest possible range for a long to safely cover int values.
bool isValidBST(TreeNode *root) {
    return helper(
        root,
        std::numeric_limits<long>::min(),
        std::numeric_limits<long>::max()
    );
}

/* ----------------- Helpers for testing ----------------- */

// Convenience function to allocate a node on the heap
TreeNode* node(int v) {
    return new TreeNode(v);
}

// Free an entire tree using postorder cleanup:
// children must be deleted before their parent.
void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Print tree sideways (right subtree at the top)
//
// - Right subtree printed first so it appears "above" the node.
// - indent controls how many spaces precede the printed value.
void printTreeImpl(TreeNode *root, int indent) {
    // Base case: nothing to print
    if (!root) return;

    // Spaces to indent per depth level
    const int INDENT_STEP = 4;

    // Print right subtree first (top)
    printTreeImpl(root->right, indent + INDENT_STEP);

    // Print current node with indentation
    std::cout << std::string(indent, ' ') << root->val << "\n";

    // Print left subtree last (bottom)
    printTreeImpl(root->left, indent + INDENT_STEP);
}

// Public wrapper to print the tree with a header
void printTree(TreeNode *root) {
    std::cout << "Tree (sideways, right is up):\n";
    printTreeImpl(root, 0);
    std::cout << "\n";
}

// Runs a named test: prints the tree and whether it is a valid BST
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
    //    1   4   <-- 4 is in right subtree of 5 but 4 < 5 (invalid)
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
    //       6   <-- 6 is in LEFT subtree of 5 but 6 > 5 (invalid)
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

    // Cleanup allocated memory
    freeTree(single);
    freeTree(valid);
    freeTree(invalid_local);
    freeTree(invalid_deep);
    // empty is nullptr, nothing to free

    return 0;
}
