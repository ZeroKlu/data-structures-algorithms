#include <iostream>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

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
    return nullptr;
}

// ----------------------------
// Helpers
// ----------------------------

TreeNode* node(int v) {
    return new TreeNode(v);
}

void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Sideways tree printer
void printTreeImpl(TreeNode *root, int indent) {
    if (!root) return;
    const int STEP = 4;

    printTreeImpl(root->right, indent + STEP);

    std::cout << std::string(indent, ' ') << root->val << "\n";

    printTreeImpl(root->left, indent + STEP);
}

void printTree(TreeNode *root) {
    std::cout << "Tree (sideways, right is up):\n";
    printTreeImpl(root, 0);
    std::cout << "\n";
}

void testLCA(const char *label, TreeNode *root, TreeNode *p, TreeNode *q) {
    std::cout << "==== " << label << " ====\n";
    std::cout << "Find LCA(" << p->val << ", " << q->val << ")\n";
    TreeNode *ans = lowestCommonAncestor(root, p, q);
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

    TreeNode *root = node(6);
    root->left = node(2);
    root->right = node(8);

    root->left->left = node(0);
    root->left->right = node(4);
    root->left->right->left = node(3);
    root->left->right->right = node(5);

    root->right->left = node(7);
    root->right->right = node(9);

    printTree(root);

    TreeNode *n2 = root->left;               // 2
    TreeNode *n8 = root->right;              // 8
    TreeNode *n4 = root->left->right;        // 4
    TreeNode *n3 = root->left->right->left;  // 3
    TreeNode *n5 = root->left->right->right; // 5
    TreeNode *n0 = root->left->left;         // 0
    TreeNode *n7 = root->right->left;        // 7
    TreeNode *n9 = root->right->right;       // 9

    testLCA("LCA(2, 8) → expected 6", root, n2, n8);
    testLCA("LCA(2, 4) → expected 2", root, n2, n4);
    testLCA("LCA(3, 5) → expected 4", root, n3, n5);
    testLCA("LCA(0, 5) → expected 2", root, n0, n5);
    testLCA("LCA(7, 9) → expected 8", root, n7, n9);

    freeTree(root);
    return 0;
}
