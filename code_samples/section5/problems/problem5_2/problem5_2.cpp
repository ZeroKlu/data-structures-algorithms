#include <iostream>
#include <cstdlib>   // std::abs
#include <algorithm> // std::max

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

    // Constructor initializes value and sets children to nullptr
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// ==========================
// INFO RETURN TYPE FOR CHECK()
// ==========================

// check() needs to return two things at once:
// - balanced: whether subtree is height-balanced
// - height:   height of subtree
struct Info {
    bool balanced;
    int height;
};

// ==========================
// BALANCE CHECK (O(n) SINGLE PASS)
// ==========================

// Returns Info for the subtree rooted at 'root'.
// Convention used here: empty tree height = -1.
static Info check(TreeNode *root) {
    // Base case: empty subtree is balanced; height is -1
    if (!root) return { true, -1 };

    // Recursively check the left subtree
    Info leftInfo = check(root->left);

    // Early exit: if left subtree is unbalanced, no need to continue
    if (!leftInfo.balanced) return { false, 0 };

    // Recursively check the right subtree
    Info rightInfo = check(root->right);

    // Early exit: if right subtree is unbalanced, no need to continue
    if (!rightInfo.balanced) return { false, 0 };

    // A node is balanced if the heights differ by at most 1
    bool bal = std::abs(leftInfo.height - rightInfo.height) <= 1;

    // Height of this subtree is 1 + max(left height, right height)
    int h = 1 + std::max(leftInfo.height, rightInfo.height);

    // Return both balance info and height in one struct
    return { bal, h };
}

// ==========================
// HEIGHT (O(n))
// ==========================

// Computes the height of a tree.
// Convention: empty tree height = -1, single node height = 0.
int height(TreeNode *root) {
    // Base case: empty subtree
    if (!root) return -1;

    // Height = 1 + max(left height, right height)
    return 1 + std::max(height(root->left), height(root->right));
}

// ==========================
// PUBLIC BALANCE CHECK WRAPPER
// ==========================

// Returns true if the tree is height-balanced.
// Uses check() which computes balance + height together in a single pass (O(n)).
bool isBalanced(TreeNode *root) {
    return check(root).balanced;
}

// -----------------------------------------------------------
// Helpers
// -----------------------------------------------------------

// Convenience function to allocate a node on the heap
TreeNode* node(int v) {
    return new TreeNode(v);
}

// Frees the entire tree using postorder cleanup:
// children must be deleted before deleting the parent.
void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Print tree sideways (same style as previous languages)
//
// - Right subtree is printed first so it appears "above" the node
// - indent controls the number of spaces before printing a node
void printTreeImpl(TreeNode *root, int indent) {
    if (!root) return;

    // Spaces to indent per depth level
    const int INDENT = 4;

    // Print right subtree first (top)
    printTreeImpl(root->right, indent + INDENT);

    // Print current node with indentation
    std::cout << std::string(indent, ' ') << root->val << "\n";

    // Print left subtree last (bottom)
    printTreeImpl(root->left, indent + INDENT);
}

// Public wrapper to print the tree with a header
void printTree(TreeNode *root) {
    std::cout << "Tree (sideways, right is up):\n";
    printTreeImpl(root, 0);
    std::cout << "\n";
}

// Runs a named test: prints the tree, its height, and whether it's balanced
void testTree(const std::string &label, TreeNode *root) {
    std::cout << "==== " << label << " ====\n";
    printTree(root);
    std::cout << "height: " << height(root) << "\n";
    std::cout << "isBalanced: " << (isBalanced(root) ? "true" : "false") << "\n\n";
}

// -----------------------------------------------------------
// Main test harness
// -----------------------------------------------------------

int main() {

    // 1) Empty tree (nullptr root)
    TreeNode *empty = nullptr;

    // 2) Single node tree
    TreeNode *single = node(1);

    // 3) Perfectly balanced:
    //         1
    //       /   \
    //      2     3
    //     / \   / \
    //    4  5  6  7
    TreeNode *balanced = node(1);
    balanced->left = node(2);
    balanced->right = node(3);
    balanced->left->left = node(4);
    balanced->left->right = node(5);
    balanced->right->left = node(6);
    balanced->right->right = node(7);

    // 4) Slightly unbalanced but still balanced (height diff <= 1 everywhere):
    //      1
    //    /   \
    //   2     3
    //  /
    // 4
    TreeNode *shallow = node(1);
    shallow->left = node(2);
    shallow->right = node(3);
    shallow->left->left = node(4);

    // 5) Deep unbalanced (height diff > 1 somewhere):
    //     1
    //    /
    //   2
    //  /
    // 3
    //  \
    //   4
    TreeNode *deep = node(1);
    deep->left = node(2);
    deep->left->left = node(3);
    deep->left->left->right = node(4);

    // Run tests
    testTree("Empty tree", empty);
    testTree("Single node", single);
    testTree("Perfectly balanced tree", balanced);
    testTree("Shallow unbalanced but balanced", shallow);
    testTree("Deep unbalanced (should be false)", deep);

    // Cleanup heap allocations
    freeTree(single);
    freeTree(balanced);
    freeTree(shallow);
    freeTree(deep);

    return 0;
}
