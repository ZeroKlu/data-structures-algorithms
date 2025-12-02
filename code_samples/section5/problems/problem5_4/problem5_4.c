#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

bool hasPathSum(TreeNode *root, int targetSum) {
    if (!root) return false;
    if (!root->left && !root->right) {
        return root->val == targetSum;
    }
    int remaining = targetSum - root->val;
    return hasPathSum(root->left, remaining) ||
           hasPathSum(root->right, remaining);
}

/* ----------------------------
   Helpers
   ---------------------------- */

TreeNode *node(int v) {
    TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));
    n->val = v;
    n->left = n->right = NULL;
    return n;
}

void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

/* Sideways tree printer */

static void print_tree_impl(TreeNode *root, int indent) {
    if (!root) return;
    const int STEP = 4;

    print_tree_impl(root->right, indent + STEP);

    for (int i = 0; i < indent; ++i) putchar(' ');
    printf("%d\n", root->val);

    print_tree_impl(root->left, indent + STEP);
}

void print_tree(TreeNode *root) {
    printf("Tree (sideways, right is up):\n");
    print_tree_impl(root, 0);
    printf("\n");
}

void test_case(const char *label, TreeNode *root, int target) {
    printf("==== %s ====\n", label);
    print_tree(root);
    printf("Target Sum: %d\n", target);
    printf("hasPathSum: %s\n\n", hasPathSum(root, target) ? "true" : "false");
}

/* ----------------------------
   Main Test Harness
   ---------------------------- */

int main(void) {

    // 1. Empty tree
    TreeNode *empty = NULL;

    // 2. Single node tree
    TreeNode *single = node(7);

    // 3. A typical test tree:
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

    // Run tests
    test_case("Empty tree", empty, 10);
    test_case("Single node (7 == 7 → true)", single, 7);
    test_case("Single node (7 != 10 → false)", single, 10);

    test_case("Large tree (has path sum 22)", t, 22);
    test_case("Large tree (has path sum 26)", t, 26);
    test_case("Large tree (has path sum 18)", t, 18);
    test_case("Large tree (no path sum == 5)", t, 5); // 5 not a leaf

    // Cleanup
    free_tree(single);
    free_tree(t);

    return 0;
}
