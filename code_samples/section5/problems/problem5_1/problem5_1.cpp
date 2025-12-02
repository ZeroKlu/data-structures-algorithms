#include <iostream>
#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

void preorder(TreeNode *root, vector<int> &out) {
    if (!root) return;
    out.push_back(root->val);
    preorder(root->left, out);
    preorder(root->right, out);
}

void inorder(TreeNode *root, vector<int> &out) {
    if (!root) return;
    inorder(root->left, out);
    out.push_back(root->val);
    inorder(root->right, out);
}

void postorder(TreeNode *root, vector<int> &out) {
    if (!root) return;
    postorder(root->left, out);
    postorder(root->right, out);
    out.push_back(root->val);
}

// --------------------------------------------
// Helpers for testing & visualization
// --------------------------------------------

// Recursive helper to print tree sideways
void printTreeImpl(TreeNode *root, int indent) {
    if (!root) return;

    const int INDENT_STEP = 4;

    printTreeImpl(root->right, indent + INDENT_STEP);

    for (int i = 0; i < indent; i++)
        cout << ' ';
    cout << root->val << "\n";

    printTreeImpl(root->left, indent + INDENT_STEP);
}

void printTree(TreeNode *root) {
    cout << "Tree (sideways):\n";
    printTreeImpl(root, 0);
    cout << "\n";
}

void printVector(const string &label, const vector<int> &v) {
    cout << label << ": ";
    for (int x : v) cout << x << " ";
    cout << "\n";
}

void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    /*
           1
         /   \
        2     3
       / \   /
      4   5 6
    */

    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);

    // Print the tree visually
    printTree(root);

    vector<int> pre, in, post;

    preorder(root, pre);
    inorder(root, in);
    postorder(root, post);

    printVector("Preorder", pre);
    printVector("Inorder", in);
    printVector("Postorder", post);

    freeTree(root);

    return 0;
}
