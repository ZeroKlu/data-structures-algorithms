#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct {
    bool balanced;
    int height;
} Info;

static Info check(TreeNode *root) {
    if (!root) {
        Info base = { true, -1 };  // empty tree height = -1
        return base;
    }
    Info leftInfo = check(root->left);
    if (!leftInfo.balanced) {
        Info res = { false, 0 };
        return res;
    }
    Info rightInfo = check(root->right);
    if (!rightInfo.balanced) {
        Info res = { false, 0 };
        return res;
    }
    int diff = leftInfo.height - rightInfo.height;
    if (diff < 0) diff = -diff;
    bool bal = diff <= 1;
    Info res = { bal, 1 + (leftInfo.height > rightInfo.height ? leftInfo.height : rightInfo.height) };
    return res;
}

int height(TreeNode *root) {
    if (!root) return -1;
    int lh = height(root->left);
    int rh = height(root->right);
    return 1 + (lh > rh ? lh : rh);
}

bool isBalanced(TreeNode *root) {
    return check(root).balanced;
}

// Helper to create a new node
TreeNode* node(int val) {
    TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));
    if (!n) {
        perror("malloc");
        exit(1);
    }
    n->val = val;
    n->left = n->right = NULL;
    return n;
}

// Free an entire tree
void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// ---- Visual tree printing (sideways) ----

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

// ---- Tests ----

void test_tree(const char *label, TreeNode *root) {
    printf("==== %s ====\n", label);
    print_tree(root);
    printf("height: %d\n", height(root));
    printf("isBalanced: %s\n\n", isBalanced(root) ? "true" : "false");
}

int main(void) {
    // 1) Empty tree
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

    // 4) Slightly unbalanced but still balanced (diff <= 1):
    //        1
    //      /   \
    //     2     3
    //    /
    //   4
    TreeNode *shallow_unbalanced = node(1);
    shallow_unbalanced->left = node(2);
    shallow_unbalanced->right = node(3);
    shallow_unbalanced->left->left = node(4);

    // 5) Clearly unbalanced (left-skewed):
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

    // Cleanup
    free_tree(single);
    free_tree(balanced);
    free_tree(shallow_unbalanced);
    free_tree(deep_unbalanced);
    // empty is NULL, nothing to free

    return 0;
}
