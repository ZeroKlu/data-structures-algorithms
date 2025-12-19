#include <stdio.h>
#include <stdbool.h>
#include <limits.h>  // LONG_MIN, LONG_MAX
#include <stdlib.h>

// ==========================
// BINARY TREE NODE DEFINITION
// ==========================

// A simple binary tree node:
// - val:   value stored in the node
// - left:  pointer to left child (NULL if none)
// - right: pointer to right child (NULL if none)
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// ==========================
// VALIDATE BST (RANGE METHOD)
// ==========================

// Helper function that validates the BST property using allowable ranges.
//
// low / high define an exclusive range (low, high):
// - node->val must be strictly greater than low
// - node->val must be strictly less than high
//
// This catches "deep violations" where a node may look valid relative
// to its parent but violates constraints from ancestors.
static bool helper(TreeNode *node, long low, long high) {
    // Base case: empty subtree is valid
    if (!node) return true;

    // If current node violates the allowable range, not a BST
    if (node->val <= low || node->val >= high) return false;

    // Left subtree must be in (low, node->val)
    // Right subtree must be in (node->val, high)
    return helper(node->left, low, node->val) &&
           helper(node->right, node->val, high);
}

// Public wrapper: checks if a tree is a valid BST.
// Uses LONG_MIN/LONG_MAX as the initial allowable range so any int fits.
bool isValidBST(TreeNode *root) {
    return helper(root, LONG_MIN, LONG_MAX);
}

/* ----------------- Helpers for testing ----------------- */

// Allocate and initialize a new node on the heap
TreeNode *node(int val) {
    TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));
    if (!n) {
        // If allocation fails, print an error and exit
        perror("malloc");
        exit(1);
    }
    n->val = val;
    n->left = n->right = NULL;
    return n;
}

// Free an entire tree using postorder cleanup (free children before parent)
void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

/* Print tree sideways (right subtree at the top) */

// Recursive helper to print the tree sideways
// indent controls how many spaces to print before the node value.
static void print_tree_impl(TreeNode *root, int indent) {
    if (!root) return;

    // Spaces to indent per depth level
    const int INDENT_STEP = 4;

    // Print right subtree first so it appears "above" the node
    print_tree_impl(root->right, indent + INDENT_STEP);

    // Print current node with indentation
    for (int i = 0; i < indent; ++i) putchar(' ');
    printf("%d\n", root->val);

    // Print left subtree last so it appears "below" the node
    print_tree_impl(root->left, indent + INDENT_STEP);
}

// Public wrapper that prints a header then prints the tree
void print_tree(TreeNode *root) {
    printf("Tree (sideways, right is up):\n");
    print_tree_impl(root, 0);
    printf("\n");
}

// Runs a named test: prints the tree and whether it's a valid BST
void test_tree(const char *label, TreeNode *root) {
    printf("==== %s ====\n", label);
    print_tree(root);
    printf("isValidBST: %s\n\n", isValidBST(root) ? "true" : "false");
}

/* ----------------- Main test harness ----------------- */

int main(void) {
    // 1) Empty tree (should be true)
    TreeNode *empty = NULL;

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
    //    1   4   <-- 4 is in the right subtree of 5, but 4 < 5 (invalid)
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
    //       6   <-- 6 is in the LEFT subtree of 5, but 6 > 5 (invalid)
    TreeNode *invalid_deep = node(5);
    invalid_deep->left = node(2);
    invalid_deep->right = node(8);
    invalid_deep->left->right = node(6);

    // Run tests
    test_tree("Empty tree (should be true)", empty);
    test_tree("Single node (should be true)", single);
    test_tree("Valid BST (should be true)", valid);
    test_tree("Invalid BST - local violation (should be false)", invalid_local);
    test_tree("Invalid BST - deep violation (should be false)", invalid_deep);

    // Cleanup allocated memory
    free_tree(single);
    free_tree(valid);
    free_tree(invalid_local);
    free_tree(invalid_deep);
    // empty is NULL, nothing to free

    return 0;
}
