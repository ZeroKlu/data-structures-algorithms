#include <iostream>

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
// HAS PATH SUM (ROOT-TO-LEAF)
// ==========================

// Returns true if there exists a ROOT-to-LEAF path where the sum of values
// along that path equals targetSum.
//
// Important: the sum must end at a leaf node (node with no children).
bool hasPathSum(TreeNode *root, int targetSum) {
    // Base case: empty subtree has no root-to-leaf path
    if (!root) return false;

    // If this is a leaf node, check whether we've matched the remaining sum
    if (!root->left && !root->right) {
        return root->val == targetSum;
    }

    // Subtract the current node's value and search children
    int remaining = targetSum - root->val;

    // If either subtree contains a valid path, return true
    return hasPathSum(root->left, remaining) ||
           hasPathSum(root->right, remaining);
}

// ----------------------------
// Helpers
// ----------------------------

// Convenience function to allocate a node on the heap
TreeNode* node(int v) {
    return new TreeNode(v);
}

// Frees the entire tree using postorder cleanup:
// delete children before deleting the parent.
void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Sideways tree printer
// - Right subtree is printed first so it appears "above" the node
// - indent controls the number of spaces before printing the node value
void printTreeImpl(TreeNode *root, int indent) {
    // Base case: nothing to print
    if (!root) return;

    // Spaces to indent per tree level
    const int STEP = 4;

    // Print right subtree first (appears "up")
    printTreeImpl(root->right, indent + STEP);

    // Print current node with indentation
    std::cout << std::string(indent, ' ') << root->val << "\n";

    // Print left subtree last (appears "down")
    printTreeImpl(root->left, indent + STEP);
}

// Public wrapper that prints a header and then prints the tree
void printTree(TreeNode *root) {
    std::cout << "Tree (sideways, right is up):\n";
    printTreeImpl(root, 0);
    std::cout << "\n";
}

// Runs a labeled test: prints the tree, target sum, and result
void testCase(const char *label, TreeNode *root, int target) {
    std::cout << "==== " << label << " ====\n";
    printTree(root);
    std::cout << "Target sum: " << target << "\n";
    std::cout << "hasPathSum: " << (hasPathSum(root, target) ? "true" : "false") << "\n\n";
}

// ----------------------------
// Main test harness
// ----------------------------

int main() {
    // 1) Empty tree
    TreeNode *empty = nullptr;

    // 2) Single node tree
    TreeNode *single = node(7);

    // 3) Classic LeetCode-style tree:
    //
    //         5
    //       /   \
    //      4     8
    //     /     / \
    //    11    13  4
    //   /  \        \
    //  7    2        1
    TreeNode *t = node(5);
    t->left = node(4);
    t->right = node(8);

    t->left->left = node(11);
    t->left->left->left = node(7);
    t->left->left->right = node(2);

    t->right->left = node(13);
    t->right->right = node(4);
    t->right->right->right = node(1);

    // Path sums present (root-to-leaf):
    // - 5 + 4 + 11 + 7 = 27
    // - 5 + 4 + 11 + 2 = 22
    // - 5 + 8 + 13     = 26   (13 is a leaf in this tree)
    // - 5 + 8 + 4 + 1  = 18

    // Run tests
    testCase("Empty tree", empty, 10);
    testCase("Single node (7 == 7 → true)", single, 7);
    testCase("Single node (7 != 10 → false)", single, 10);

    testCase("Large tree (has path sum 22 → true)", t, 22);
    testCase("Large tree (has path sum 26 → true)", t, 26);
    testCase("Large tree (has path sum 18 → true)", t, 18);

    // This should be false because the path must end at a leaf; value 5 is not a leaf.
    testCase("Large tree (no path sum == 5 → false)", t, 5);

    // Cleanup heap allocations
    freeTree(single);
    freeTree(t);
    // empty is nullptr, nothing to free

    return 0;
}
