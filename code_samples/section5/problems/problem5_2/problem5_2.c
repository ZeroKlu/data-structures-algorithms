#include <stdio.h>
#include <stdbool.h>
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
// "INFO" RETURN TYPE FOR CHECK
// ==========================

// Used by check() to return two pieces of information at once:
// - balanced: whether the subtree is height-balanced
// - height:   height of the subtree
typedef struct {
    bool balanced;
    int height;
} Info;

// ==========================
// BALANCE CHECK (O(n))
// ==========================

// Returns Info about the subtree:
// - balanced: true if subtree is height-balanced
// - height: height of subtree (using empty tree height = -1)
static Info check(TreeNode *root) {
    // Base case: empty tree is balanced with height -1
    if (!root) {
        Info base = { true, -1 };  // convention: empty tree height = -1
        return base;
    }

    // Recursively check left subtree
    Info leftInfo = check(root->left);

    // Early exit: if left subtree is not balanced, whole tree is not balanced
    if (!leftInfo.balanced) {
        Info res = { false, 0 };
        return res;
    }

    // Recursively check right subtree
    Info rightInfo = check(root->right);

    // Early exit: if right subtree is not balanced, whole tree is not balanced
    if (!rightInfo.balanced) {
        Info res = { false, 0 };
        return res;
    }

    // Compute absolute height difference between left and right subtrees
    int diff = leftInfo.height - rightInfo.height;
    if (diff < 0) diff = -diff;

    // A node is balanced if its subtrees differ in height by at most 1
    bool bal = diff <= 1;

    // Height of this subtree is 1 + max(left height, right height)
    Info res = {
        bal,
        1 + (leftInfo.height > rightInfo.height ? leftInfo.height : rightInfo.height)
    };

    return res;
}

// ==========================
// HEIGHT (O(n))
// ==========================

// Computes tree height where:
// - empty tree height = -1
// - single node height = 0
int height(TreeNode *root) {
    if (!root) return -1;

    // Compute left and right heights recursively
    int lh = height(root->left);
    int rh = height(root->right);

    // Height is 1 + maximum child height
    return 1 + (lh > rh ? lh : rh);
}

// ==========================
// PUBLIC BALANCE CHECK WRAPPER
// ==========================

// Returns true if the tree is height-balanced.
// Uses check() which runs in O(n) time (single pass).
bool isBalanced(TreeNode *root) {
    return check(root).balanced;
}

// ==========================
// HELPERS: CREATE / FREE TREE
// ==========================

// Allocate and initialize a new TreeNode on the heap
TreeNode* node(int val) {
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

// Free an entire tree using postorder cleanup:
// free children before freeing the parent node.
void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// ==========================
// VISUAL TREE PRINTING (SIDEWAYS)
// ==========================

// Recursive helper to print tree sideways.
// - Right subtree is printed first so it appears "above" the node.
// - indent controls how many spaces precede the node.
static void print_tree_impl(TreeNode *root, int indent) {
    if (!root) return;

    // Spaces to add per depth level
    const int INDENT_STEP = 4;

    // Print right subtree first (appears toward the top)
    print_tree_impl(root->right, indent + INDENT_STEP);

    // Print current node with indentation
    for (int i = 0; i < indent; ++i) putchar(' ');
    printf("%d\n", root->val);

    // Print left subtree last (appears toward the bottom)
    print_tree_impl(root->left, indent + INDENT_STEP);
}

// Public wrapper that prints a header and then prints the tree
void print_tree(TreeNode *root) {
    printf("Tree (sideways, right is up):\n");
    print_tree_impl(root, 0);
    printf("\n");
}

// ==========================
// TEST UTILITIES
// ==========================

// Prints a label + tree visualization + computed height + balance result
void test_tree(const char *label, TreeNode *root) {
    printf("==== %s ====\n", label);
    print_tree(root);

    // height() and isBalanced() are independent checks:
    // - height() is O(n)
    // - isBalanced() uses check() which is O(n)
    printf("height: %d\n", height(root));
    printf("isBalanced: %s\n\n", isBalanced(root) ? "true" : "false");
}

// ==========================
// MAIN TEST DRIVER
// ==========================

int main(void) {
    // 1) Empty tree (NULL root pointer)
    TreeNode *empty = NULL;

    // 2) Single node tree
    TreeNode *single = node(1);

    // 3) Perfectly balanced tree:
    //        1
    //      /   \
    //     2     3
    //    / \   / \
    //   4   5 6   7
    TreeNode *balanced = node(1);
    balanced->left = node(2);
    balanced->right = node(3);
    balanced->left->left = node(4);
    balanced->left->right = node(5);
    balanced->right->left = node(6);
    balanced->right->right = node(7);

    // 4) Slightly unbalanced but still balanced (diff <= 1 at every node):
    //        1
    //      /   \
    //     2     3
    //    /
    //   4
    TreeNode *shallow_unbalanced = node(1);
    shallow_unbalanced->left = node(2);
    shallow_unbalanced->right = node(3);
    shallow_unbalanced->left->left = node(4);

    // 5) Clearly unbalanced (height difference > 1 somewhere):
    //     1
    //    /
    //   2
    //  /
    // 3
    //  \
    //   4
    TreeNode *deep_unbalanced = node(1);
    deep_unbalanced->left = node(2);
    deep_unbalanced->left->left = node(3);
    deep_unbalanced->left->left->right = node(4);

    // Run tests
    test_tree("Empty tree", empty);
    test_tree("Single node", single);
    test_tree("Perfectly balanced tree", balanced);
    test_tree("Shallow unbalanced but balanced", shallow_unbalanced);
    test_tree("Deep unbalanced (should be false)", deep_unbalanced);

    // Cleanup allocated trees
    free_tree(single);
    free_tree(balanced);
    free_tree(shallow_unbalanced);
    free_tree(deep_unbalanced);
    // empty is NULL, so nothing to free

    return 0;
}
