#include <stdio.h>
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

// ==============================================
// DYNAMIC ARRAY (IntList) FOR TRAVERSAL RESULTS
// ==============================================

// A simple growable array of ints used to collect traversal outputs.
// (Similar to ArrayList in Java / List<int> in C#.)
typedef struct {
    int *data;     // pointer to heap-allocated array
    int size;      // number of valid elements stored
    int capacity;  // allocated length of data[]
} IntList;

// Initialize the dynamic array with a small starting capacity.
static void list_init(IntList *list) {
    list->size = 0;
    list->capacity = 8; // start with room for 8 integers
    list->data = (int *)malloc(sizeof(int) * list->capacity);
    // NOTE: In production code, you'd typically check list->data != NULL.
}

// Append a value to the end of the dynamic array,
// resizing (doubling capacity) if needed.
static void list_push_back(IntList *list, int v) {
    // If full, grow the underlying array
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->data = (int *)realloc(list->data, sizeof(int) * list->capacity);
        // NOTE: In production code, you'd typically handle realloc failure.
    }

    // Store the new value and increment size
    list->data[list->size++] = v;
}

// ==========================
// TREE TRAVERSALS (DFS)
// ==========================

// Preorder traversal (DFS): Root -> Left -> Right
// Adds visited node values to 'out'.
static void preorder_dfs(TreeNode *root, IntList *out) {
    // Base case: empty subtree
    if (!root) return;

    // Visit current node first
    list_push_back(out, root->val);

    // Then traverse left subtree
    preorder_dfs(root->left, out);

    // Then traverse right subtree
    preorder_dfs(root->right, out);
}

// Inorder traversal (DFS): Left -> Root -> Right
static void inorder_dfs(TreeNode *root, IntList *out) {
    // Base case: empty subtree
    if (!root) return;

    // Traverse left subtree first
    inorder_dfs(root->left, out);

    // Visit current node
    list_push_back(out, root->val);

    // Traverse right subtree
    inorder_dfs(root->right, out);
}

// Postorder traversal (DFS): Left -> Right -> Root
static void postorder_dfs(TreeNode *root, IntList *out) {
    // Base case: empty subtree
    if (!root) return;

    // Traverse left subtree
    postorder_dfs(root->left, out);

    // Traverse right subtree
    postorder_dfs(root->right, out);

    // Visit current node last
    list_push_back(out, root->val);
}

// ==========================
// HELPERS FOR TESTING / OUTPUT
// ==========================

// Allocate and initialize a new TreeNode on the heap.
TreeNode* node(int val) {
    TreeNode* n = (TreeNode*)malloc(sizeof(TreeNode));
    // NOTE: In production code, you'd check n != NULL.
    n->val = val;
    n->left = n->right = NULL;
    return n;
}

// Print an IntList with a label (e.g., "Preorder: 1 2 3")
void print_list(const char *label, IntList *list) {
    printf("%s: ", label);
    for (int i = 0; i < list->size; ++i) {
        printf("%d ", list->data[i]);
    }
    printf("\n");
}

// Free an entire binary tree (postorder-style cleanup).
// Important: must free children before freeing parent.
void free_tree(TreeNode *root) {
    if (!root) return;

    // Free left and right subtrees first
    free_tree(root->left);
    free_tree(root->right);

    // Then free the current node
    free(root);
}

// Recursive helper to print the tree "sideways"
// so you can visualize its structure.
//
// indent controls the horizontal spacing.
// Right subtree is printed first so it appears "above" the node.
void print_tree_impl(TreeNode *root, int indent) {
    if (!root) return;

    // Amount to indent per depth level (more = wider spacing)
    const int INDENT_STEP = 4;

    // Print right subtree first (appears at top in sideways view)
    print_tree_impl(root->right, indent + INDENT_STEP);

    // Print current node, preceded by indent spaces
    for (int i = 0; i < indent; ++i) {
        putchar(' ');
    }
    printf("%d\n", root->val);

    // Print left subtree last (appears at bottom)
    print_tree_impl(root->left, indent + INDENT_STEP);
}

// Public wrapper that prints a header then prints the tree.
void print_tree(TreeNode *root) {
    printf("Tree (sideways, right is up, left is down):\n");
    print_tree_impl(root, 0);
    printf("\n");
}

// ==========================
// MAIN TEST DRIVER
// ==========================

int main(void) {
    /*
           1
         /   \
        2     3
       / \   /
      4   5 6
    */

    // Build the test tree by allocating nodes and linking pointers
    TreeNode *root = node(1);
    root->left = node(2);
    root->right = node(3);
    root->left->left = node(4);
    root->left->right = node(5);
    root->right->left = node(6);

    // Visual print of the tree (sideways view)
    print_tree(root);

    // Lists to hold traversal outputs
    IntList preorder, inorder, postorder;

    // Initialize each dynamic array
    list_init(&preorder);
    list_init(&inorder);
    list_init(&postorder);

    // Run traversals and store results
    preorder_dfs(root, &preorder);
    inorder_dfs(root, &inorder);
    postorder_dfs(root, &postorder);

    // Print traversal results
    print_list("Preorder", &preorder);
    print_list("Inorder", &inorder);
    print_list("Postorder", &postorder);

    // Cleanup: free dynamic arrays and the tree nodes
    free(preorder.data);
    free(inorder.data);
    free(postorder.data);
    free_tree(root);

    return 0;
}
