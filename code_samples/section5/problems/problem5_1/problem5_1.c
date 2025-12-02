#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Simple dynamic array helpers for collecting traversal result.
typedef struct {
    int *data;
    int size;
    int capacity;
} IntList;

static void list_init(IntList *list) {
    list->size = 0;
    list->capacity = 8;
    list->data = (int *)malloc(sizeof(int) * list->capacity);
}

static void list_push_back(IntList *list, int v) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->data = (int *)realloc(list->data, sizeof(int) * list->capacity);
    }
    list->data[list->size++] = v;
}

static void preorder_dfs(TreeNode *root, IntList *out) {
    if (!root) return;
    list_push_back(out, root->val);
    preorder_dfs(root->left, out);
    preorder_dfs(root->right, out);
}

static void inorder_dfs(TreeNode *root, IntList *out) {
    if (!root) return;
    inorder_dfs(root->left, out);
    list_push_back(out, root->val);
    inorder_dfs(root->right, out);
}

static void postorder_dfs(TreeNode *root, IntList *out) {
    if (!root) return;
    postorder_dfs(root->left, out);
    postorder_dfs(root->right, out);
    list_push_back(out, root->val);
}

// --- Helpers for testing / visualization ---

// Create a new node
TreeNode* node(int val) {
    TreeNode* n = (TreeNode*)malloc(sizeof(TreeNode));
    n->val = val;
    n->left = n->right = NULL;
    return n;
}

// Print IntList with a label
void print_list(const char *label, IntList *list) {
    printf("%s: ", label);
    for (int i = 0; i < list->size; ++i) {
        printf("%d ", list->data[i]);
    }
    printf("\n");
}

// Free the entire tree
void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// Recursive helper to print tree sideways
// 'indent' controls horizontal indentation.
void print_tree_impl(TreeNode *root, int indent) {
    if (!root) return;

    // increase indent for each level
    const int INDENT_STEP = 4;

    // print right subtree first (so it's at the top)
    print_tree_impl(root->right, indent + INDENT_STEP);

    // print current node
    for (int i = 0; i < indent; ++i) {
        putchar(' ');
    }
    printf("%d\n", root->val);

    // then print left subtree
    print_tree_impl(root->left, indent + INDENT_STEP);
}

// Public wrapper to print the tree
void print_tree(TreeNode *root) {
    printf("Tree (sideways, right is up, left is down):\n");
    print_tree_impl(root, 0);
    printf("\n");
}

int main(void) {
    /*
           1
         /   \
        2     3
       / \   /
      4   5 6
    */

    TreeNode *root = node(1);
    root->left = node(2);
    root->right = node(3);
    root->left->left = node(4);
    root->left->right = node(5);
    root->right->left = node(6);

    // Visual print of the tree
    print_tree(root);

    IntList preorder, inorder, postorder;
    list_init(&preorder);
    list_init(&inorder);
    list_init(&postorder);

    preorder_dfs(root, &preorder);
    inorder_dfs(root, &inorder);
    postorder_dfs(root, &postorder);

    print_list("Preorder", &preorder);
    print_list("Inorder", &inorder);
    print_list("Postorder", &postorder);

    // Cleanup
    free(preorder.data);
    free(inorder.data);
    free(postorder.data);
    free_tree(root);

    return 0;
}
