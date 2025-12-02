#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

static bool helper(TreeNode *node, long low, long high) {
    if (!node) return true;
    if (node->val <= low || node->val >= high) return false;
    return helper(node->left, low, node->val) &&
           helper(node->right, node->val, high);
}

bool isValidBST(TreeNode *root) {
    return helper(root, LONG_MIN, LONG_MAX);
}

/* ----------------- Helpers for testing ----------------- */

TreeNode *node(int val) {
    TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));
    if (!n) {
        perror("malloc");
        exit(1);
    }
    n->val = val;
    n->left = n->right = NULL;
    return n;
}

void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

/* Print tree sideways (right subtree at the top) */

static void print_tree_impl(TreeNode *root, int indent) {
    if (!root) return;
    const int INDENT_STEP = 4;

    print_tree_impl(root->right, indent + INDENT_STEP);

    for (int i = 0; i < indent; ++i) putchar(' ');
    printf("%d\n", root->val);

    print_tree_impl(root->left, indent + INDENT_STEP);
}

void print_tree(TreeNode *root) {
    printf("Tree (sideways, right is up):\n");
    print_tree_impl(root, 0);
    printf("\n");
}

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
    test_tree("Empty tree (should be true)", empty);
    test_tree("Single node (should be true)", single);
    test_tree("Valid BST (should be true)", valid);
    test_tree("Invalid BST - local violation (should be false)", invalid_local);
    test_tree("Invalid BST - deep violation (should be false)", invalid_deep);

    // Cleanup
    free_tree(single);
    free_tree(valid);
    free_tree(invalid_local);
    free_tree(invalid_deep);
    // empty is NULL, nothing to free

    return 0;
}
