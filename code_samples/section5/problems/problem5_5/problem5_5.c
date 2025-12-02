#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
    TreeNode *node = root;
    int pv = p->val;
    int qv = q->val;
    while (node) {
        if (pv < node->val && qv < node->val) {
            node = node->left;
        } else if (pv > node->val && qv > node->val) {
            node = node->right;
        } else {
            return node;
        }
    }
    return NULL;
}

/* ------------------------------
   Helpers
   ------------------------------ */

TreeNode *node(int v) {
    TreeNode *n = malloc(sizeof(TreeNode));
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

/* sideways tree printer */
void print_tree_impl(TreeNode *root, int indent) {
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

void test_lca(const char *label, TreeNode *root, TreeNode *p, TreeNode *q) {
    printf("==== %s ====\n", label);
    printf("Find LCA(%d, %d)\n", p->val, q->val);
    TreeNode *ans = lowestCommonAncestor(root, p, q);
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

    TreeNode *root = node(6);
    root->left = node(2);
    root->right = node(8);

    root->left->left = node(0);
    root->left->right = node(4);
    root->left->right->left = node(3);
    root->left->right->right = node(5);

    root->right->left = node(7);
    root->right->right = node(9);

    // print tree for clarity
    print_tree(root);

    // references into the tree
    TreeNode *n2 = root->left;               // 2
    TreeNode *n8 = root->right;              // 8
    TreeNode *n4 = root->left->right;        // 4
    TreeNode *n3 = root->left->right->left;  // 3
    TreeNode *n5 = root->left->right->right; // 5
    TreeNode *n0 = root->left->left;         // 0

    // Run LCA test cases
    test_lca("LCA(2, 8) → expected 6", root, n2, n8);
    test_lca("LCA(2, 4) → expected 2", root, n2, n4);
    test_lca("LCA(3, 5) → expected 4", root, n3, n5);
    test_lca("LCA(0, 5) → expected 2", root, n0, n5);
    test_lca("LCA(7, 9) → expected 8", root, root->right->left, root->right->right);

    free_tree(root);
    return 0;
}
