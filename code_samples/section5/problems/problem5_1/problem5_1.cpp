#include <iostream>
#include <vector>
using namespace std;

// ==========================
// BINARY TREE NODE DEFINITION
// ==========================

// A simple binary tree node:
// - val:   value stored in the node
// - left:  pointer to left child (nullptr if none)
// - right: pointer to right child (nullptr if none)
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    // Constructor initializes the node's value and sets children to nullptr
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// ==========================
// TREE TRAVERSALS (DFS)
// ==========================

// Preorder traversal: Root -> Left -> Right
// Appends visited node values into 'out'.
void preorder(TreeNode *root, vector<int> &out) {
    // Base case: empty subtree
    if (!root) return;

    // Visit current node first
    out.push_back(root->val);

    // Traverse left subtree
    preorder(root->left, out);

    // Traverse right subtree
    preorder(root->right, out);
}

// Inorder traversal: Left -> Root -> Right
void inorder(TreeNode *root, vector<int> &out) {
    // Base case: empty subtree
    if (!root) return;

    // Traverse left subtree first
    inorder(root->left, out);

    // Visit current node
    out.push_back(root->val);

    // Traverse right subtree
    inorder(root->right, out);
}

// Postorder traversal: Left -> Right -> Root
void postorder(TreeNode *root, vector<int> &out) {
    // Base case: empty subtree
    if (!root) return;

    // Traverse left subtree
    postorder(root->left, out);

    // Traverse right subtree
    postorder(root->right, out);

    // Visit current node last
    out.push_back(root->val);
}

// --------------------------------------------
// Helpers for testing & visualization
// --------------------------------------------

// Recursive helper to print the tree "sideways"
// - Right subtree is printed first so it appears "above" the node
// - indent controls how many spaces to print before the node value
void printTreeImpl(TreeNode *root, int indent) {
    // Base case: nothing to print
    if (!root) return;

    // How many spaces to indent per depth level
    const int INDENT_STEP = 4;

    // Print right subtree first (appears toward the top)
    printTreeImpl(root->right, indent + INDENT_STEP);

    // Print current node with indentation
    for (int i = 0; i < indent; i++)
        cout << ' ';
    cout << root->val << "\n";

    // Print left subtree last (appears toward the bottom)
    printTreeImpl(root->left, indent + INDENT_STEP);
}

// Public wrapper that prints a header and calls the recursive helper
void printTree(TreeNode *root) {
    cout << "Tree (sideways):\n";
    printTreeImpl(root, 0);
    cout << "\n";
}

// Print a vector of ints with a label (e.g., "Preorder: 1 2 3")
void printVector(const string &label, const vector<int> &v) {
    cout << label << ": ";
    for (int x : v) cout << x << " ";
    cout << "\n";
}

// Free all nodes in the tree using postorder cleanup
// (children must be deleted before the parent)
void freeTree(TreeNode *root) {
    if (!root) return;

    // Delete subtrees first
    freeTree(root->left);
    freeTree(root->right);

    // Then delete this node
    delete root;
}

// ==========================
// MAIN TEST DRIVER
// ==========================

int main() {
    /*
           1
         /   \
        2     3
       / \   /
      4   5 6
    */

    // Build the test tree by allocating nodes and linking pointers
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);

    // Print the tree visually (sideways)
    printTree(root);

    // Vectors to store traversal results
    vector<int> pre, in, post;

    // Run traversals and collect outputs
    preorder(root, pre);
    inorder(root, in);
    postorder(root, post);

    // Print results
    printVector("Preorder", pre);
    printVector("Inorder", in);
    printVector("Postorder", post);

    // Cleanup: free allocated memory
    freeTree(root);

    return 0;
}
