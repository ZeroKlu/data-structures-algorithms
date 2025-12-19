#include <iostream>

// ==========================
// BINARY SEARCH TREE NODE
// ==========================

// Represents one node in a Binary Search Tree (BST).
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
// LOWEST COMMON ANCESTOR (BST)
// ==========================

// Returns the Lowest Common Ancestor (LCA) of nodes p and q in a BST.
//
// BST property used:
// - Values in left subtree  < node->val
// - Values in right subtree > node->val
//
// Key idea:
// - If BOTH p and q are < current node, LCA must be in left subtree.
// - If BOTH p and q are > current node, LCA must be in right subtree.
// - Otherwise, current node is the "split point" and is the LCA.
TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
    // Start from the root and walk downward
    TreeNode *node = root;

    // Cache values for comparisons (avoid repeated p->val / q->val)
    int pv = p->val;
    int qv = q->val;

    // Iteratively move down the tree until the split point is found
    while (node) {
        // If both targets are smaller, go left
        if (pv < node->val && qv < node->val) {
            node = node->left;

        // If both targets are larger, go right
        } else if (pv > node->val && qv > node->val) {
            node = node->right;

        // Otherwise:
        // - one value is on the left and the other on the right, OR
        // - current node equals p or q
        // In both cases, current node is the LCA.
        } else {
            return node;
        }
    }

    // If root was nullptr, or p/q aren't actually present in the tree,
    // we might fall out of the loop.
    return nullptr;
}

// ----------------------------
// Helpers
// ----------------------------

// Convenience function to allocate a new node on the heap
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

    // Number of spaces to indent per level
    const int STEP = 4;

    // Print right subtree first (appears "up")
    printTreeImpl(root->right, indent + STEP);

    // Print current node with indentation based on depth
    std::cout << std::string(indent, ' ') << root->val << "\n";

    // Print left subtree last (appears "down")
    printTreeImpl(root->left, indent + STEP);
}

// Public wrapper to print the tree with a header
void printTree(TreeNode *root) {
    std::cout << "Tree (sideways, right is up):\n";
    printTreeImpl(root, 0);
    std::cout << "\n";
}

// Runs a labeled LCA test and prints the result
void testLCA(const char *label, TreeNode *root, TreeNode *p, TreeNode *q) {
    std::cout << "==== " << label << " ====\n";
    std::cout << "Find LCA(" << p->val << ", " << q->val << ")\n";

    // Compute LCA using BST logic
    TreeNode *ans = lowestCommonAncestor(root, p, q);

    // Print result
    if (ans) {
        std::cout << "LCA = " << ans->val << "\n\n";
    } else {
        std::cout << "LCA = nullptr\n\n";
    }
}

// ----------------------------
// Main test harness
// ----------------------------

int main() {
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
    printTree(root);

    // Grab references (pointers) to nodes inside the tree
    TreeNode *n2 = root->left;               // value 2
    TreeNode *n8 = root->right;              // value 8
    TreeNode *n4 = root->left->right;        // value 4
    TreeNode *n3 = root->left->right->left;  // value 3
    TreeNode *n5 = root->left->right->right; // value 5
    TreeNode *n0 = root->left->left;         // value 0
    TreeNode *n7 = root->right->left;        // value 7
    TreeNode *n9 = root->right->right;       // value 9

    // Run LCA test cases
    testLCA("LCA(2, 8) \xE2\x86\x92 expected 6", root, n2, n8);
    testLCA("LCA(2, 4) \xE2\x86\x92 expected 2", root, n2, n4);
    testLCA("LCA(3, 5) \xE2\x86\x92 expected 4", root, n3, n5);
    testLCA("LCA(0, 5) \xE2\x86\x92 expected 2", root, n0, n5);
    testLCA("LCA(7, 9) \xE2\x86\x92 expected 8", root, n7, n9);

    // Cleanup heap allocations
    freeTree(root);
    return 0;
}
