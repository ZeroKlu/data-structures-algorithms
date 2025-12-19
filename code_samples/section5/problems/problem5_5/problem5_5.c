#include <stdio.h>
#include <stdlib.h>

// ==========================
// BINARY TREE NODE DEFINITION
// ==========================

// A simple BST node:
// - val:   value stored in the node
// - left:  pointer to left child (NULL if none)
// - right: pointer to right child (NULL if none)
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// ==========================
// LOWEST COMMON ANCESTOR IN A BST
// ==========================

// Returns the Lowest Common Ancestor (LCA) of nodes p and q in a Binary Search Tree.
//
// BST property used:
// - All values in left subtree < node->val
// - All values in right subtree > node->val
//
// Key idea:
// - If BOTH p and q are smaller than current node, LCA must be in left subtree.
// - If BOTH p and q are larger than current node, LCA must be in right subtree.
// - Otherwise, current node is the "split point" and is the LCA.
TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
    // Start walking down the tree from the root
    TreeNode *node = root;

    // Store p and q values for faster comparisons
    int pv = p->val;
    int qv = q->val;

    // Iteratively traverse until we find the split point
    while (node) {
        // If both values are less than node->val, go left
        if (pv < node->val && qv < node->val) {
            node = node->left;

        // If both values are greater than node->val, go right
        } else if (pv > node->val && qv > node->val) {
            node = node->right;

        // Otherwise:
        // - node is between pv and qv, OR
        // - node equals one of them
        // In both cases, this node is the LCA.
        } else {
            return node;
        }
    }

    // If we fall out of the loop, root was NULL or p/q weren't in the tree
    return NULL;
}

/* ------------------------------
   Helpers
   ------------------------------ */

// Allocate and initialize a new TreeNode on the heap
TreeNode *node(int v) {
    TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));
    // NOTE: In production code, you'd check n != NULL.
    n->val = v;
    n->left = n->right = NULL;
    return n;
}

// Free the entire tree using postorder cleanup:
// free children before freeing the parent.
void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

/* sideways tree printer */

// Recursive helper to print the BST sideways:
// - Right subtree appears above the node
// - Left subtree appears below the node
// indent controls how many spaces precede the node value.
void print_tree_impl(TreeNode *root, int indent) {
    // Base case: nothing to print
    if (!root) return;

    // Spaces to indent per depth level
    const int STEP = 4;

    // Print right subtree first (appears "up")
    print_tree_impl(root->right, indent + STEP);

    // Print current node with indentation
    for (int i = 0; i < indent; ++i) putchar(' ');
    printf("%d\n", root->val);

    // Print left subtree last (appears "down")
    print_tree_impl(root->left, indent + STEP);
}

// Public wrapper that prints a header then prints the tree
void print_tree(TreeNode *root) {
    printf("Tree (sideways, right is up):\n");
    print_tree_impl(root, 0);
    printf("\n");
}

// Runs an LCA test case and prints the result
void test_lca(const char *label, TreeNode *root, TreeNode *p, TreeNode *q) {
    printf("==== %s ====\n", label);

    // Show what we are computing
    printf("Find LCA(%d, %d)\n", p->val, q->val);

    // Compute LCA using BST logic
    TreeNode *ans = lowestCommonAncestor(root, p, q);

    // Print result
    if (ans)
        printf("LCA = %d\n\n", ans->val);
    else
        printf("LCA = NULL\n\n");
}

/* ------------------------------
   Main Test Harness
   ------------------------------ */

int main(void) {
    /*
           6
         /   \
        2     8
       / \   / \
      0   4 7   9
         / \
        3   5
    */

    // Build the test BST
    TreeNode *root = node(6);
    root->left = node(2);
    root->right = node(8);

    root->left->left = node(0);
    root->left->right = node(4);
    root->left->right->left = node(3);
    root->left->right->right = node(5);

    root->right->left = node(7);
    root->right->right = node(9);

    // Print tree for clarity
    print_tree(root);

    // Grab pointers (references) to nodes inside the tree
    TreeNode *n2 = root->left;               // value 2
    TreeNode *n8 = root->right;              // value 8
    TreeNode *n4 = root->left->right;        // value 4
    TreeNode *n3 = root->left->right->left;  // value 3
    TreeNode *n5 = root->left->right->right; // value 5
    TreeNode *n0 = root->left->left;         // value 0

    // Run LCA test cases (BST LCA)
    test_lca("LCA(2, 8) \xE2\x86\x92 expected 6", root, n2, n8);
    test_lca("LCA(2, 4) \xE2\x86\x92 expected 2", root, n2, n4);
    test_lca("LCA(3, 5) \xE2\x86\x92 expected 4", root, n3, n5);
    test_lca("LCA(0, 5) \xE2\x86\x92 expected 2", root, n0, n5);
    test_lca("LCA(7, 9) \xE2\x86\x92 expected 8", root, root->right->left, root->right->right);

    // Cleanup
    free_tree(root);
    return 0;
}
