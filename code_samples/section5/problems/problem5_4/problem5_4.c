#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
// HAS PATH SUM (ROOT-TO-LEAF)
// ==========================

// Returns true if the tree has a ROOT-to-LEAF path where the sum of node values
// along that path equals targetSum.
//
// Important: the sum must end at a leaf node (a node with no children).
bool hasPathSum(TreeNode *root, int targetSum) {
    // Base case: empty subtree has no path
    if (!root) return false;

    // If this is a leaf node, check whether its value completes the sum
    if (!root->left && !root->right) {
        return root->val == targetSum;
    }

    // Otherwise, subtract current node's value and search children
    int remaining = targetSum - root->val;

    // If either subtree has a valid path, return true
    return hasPathSum(root->left, remaining) ||
           hasPathSum(root->right, remaining);
}

/* ----------------------------
   Helpers
   ---------------------------- */

// Allocate and initialize a new node on the heap
TreeNode *node(int v) {
    TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));
    // NOTE: In production code, you'd check n != NULL.
    n->val = v;
    n->left = n->right = NULL;
    return n;
}

// Free an entire tree using postorder cleanup:
// free children before freeing the parent.
void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

/* Sideways tree printer */

// Recursive helper to print the tree sideways:
// - Right subtree appears above the node
// - Left subtree appears below the node
// indent controls the number of spaces before the node value.
static void print_tree_impl(TreeNode *root, int indent) {
    // Base case: nothing to print
    if (!root) return;

    // Number of spaces to indent per depth level
    const int STEP = 4;

    // Print right subtree first (appears "up")
    print_tree_impl(root->right, indent + STEP);

    // Print current node with indentation
    for (int i = 0; i < indent; ++i) putchar(' ');
    printf("%d\n", root->val);

    // Print left subtree last (appears "down")
    print_tree_impl(root->left, indent + STEP);
}

// Public wrapper to print the tree with a header
void print_tree(TreeNode *root) {
    printf("Tree (sideways, right is up):\n");
    print_tree_impl(root, 0);
    printf("\n");
}

// Runs a labeled test: prints the tree, target sum, and result
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

    // 1) Empty tree (should always be false for any target)
    TreeNode *empty = NULL;

    // 2) Single node tree
    TreeNode *single = node(7);

    // 3) A typical test tree:
    //
    //         5
    //       /   \
    //      4     8
    //     /     / \
    //    11    13  4
    //   /  \        \
    //  7    2        1
    //
    // Example root-to-leaf paths:
    // - 5 -> 4 -> 11 -> 2   (sum = 22)
    // - 5 -> 8 -> 13        (not a leaf in this tree? actually 13 is a leaf here, sum = 26)
    // - 5 -> 8 -> 4 -> 1    (sum = 18)
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
    test_case("Single node (7 == 7 \xE2\x86\x92 true)", single, 7);
    test_case("Single node (7 != 10 \xE2\x86\x92 false)", single, 10);

    test_case("Large tree (has path sum 22)", t, 22);
    test_case("Large tree (has path sum 26)", t, 26);
    test_case("Large tree (has path sum 18)", t, 18);

    // This should be false because 5 is not a leaf; path sums must end at a leaf node
    test_case("Large tree (no path sum == 5)", t, 5);

    // Cleanup allocated memory
    free_tree(single);
    free_tree(t);

    return 0;
}
