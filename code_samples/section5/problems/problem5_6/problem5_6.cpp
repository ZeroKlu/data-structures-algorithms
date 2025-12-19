#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <iostream>
using namespace std;

// ==========================
// BINARY TREE NODE DEFINITION
// ==========================

// Represents a single node in a binary tree
struct TreeNode {
    int val;            // value stored in this node
    TreeNode *left;     // pointer to left child (nullptr if none)
    TreeNode *right;    // pointer to right child (nullptr if none)

    // Constructor initializes value and sets children to nullptr
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// ==========================
// SERIALIZATION (PREORDER + NULL MARKERS)
// ==========================

// Serializes a binary tree into a single string using preorder traversal:
//
//   node value, then left subtree, then right subtree
//
// Null pointers are written as "#," so the structure can be reconstructed.
// Example (single node 1):
//   "1,#,#,"
string serialize(TreeNode *root) {
    string out; // output string that we will append tokens into

    // DFS function that writes preorder tokens into "out"
    function<void(TreeNode *)> dfs = [&](TreeNode *node) {
        // Null child => write marker and return
        if (!node) {
            out += "#,";
            return;
        }

        // Write the node value followed by a comma delimiter
        out += to_string(node->val) + ",";

        // Serialize left subtree then right subtree
        dfs(node->left);
        dfs(node->right);
    };

    // Start DFS from root
    dfs(root);

    return out;
}

// ==========================
// DESERIALIZATION
// ==========================

// Reconstructs the tree from the serialized string.
//
// Steps:
// 1) Split the string by commas into tokens
// 2) Rebuild the tree using preorder recursion
//    - token "#" => nullptr
//    - number token => create node, then build left, then build right
TreeNode *deserialize(const string &data) {
    // 1) Tokenize the input string by commas
    vector<string> tokens;
    string token;
    stringstream ss(data);

    while (getline(ss, token, ',')) {
        // Skip empty token (happens at end if string ends with comma)
        if (!token.empty()) tokens.push_back(token);
    }

    // idx tracks which token we are currently consuming
    int idx = 0;

    // 2) DFS rebuild function (mirrors serialization order)
    function<TreeNode *()> dfs = [&]() -> TreeNode * {
        // Safety check: if we run out of tokens, return nullptr
        if (idx >= (int)tokens.size()) return nullptr;

        // Read current token and advance
        const string &t = tokens[idx++];

        // "#" represents null pointer
        if (t == "#") return nullptr;

        // Otherwise parse integer and create a node
        int val = stoi(t);
        TreeNode *node = new TreeNode(val);

        // Rebuild left subtree then right subtree (preorder format)
        node->left = dfs();
        node->right = dfs();

        return node;
    };

    // Start rebuild from the first token
    return dfs();
}

// ----------------------------
// Helpers for testing
// ----------------------------

// Convenience function to allocate a node on the heap
TreeNode* node(int v) {
    return new TreeNode(v);
}

// Frees an entire tree using postorder cleanup:
// delete children before deleting the parent.
void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Sideways tree printer
// - Right subtree appears above the node
// - Left subtree appears below the node
void printTreeImpl(TreeNode *root, int indent) {
    if (!root) return;

    // Spaces to indent per level
    const int STEP = 4;

    // Print right subtree first (appears "up")
    printTreeImpl(root->right, indent + STEP);

    // Print current node with indentation
    cout << string(indent, ' ') << root->val << "\n";

    // Print left subtree last (appears "down")
    printTreeImpl(root->left, indent + STEP);
}

// Public wrapper for printing the tree
void printTree(TreeNode *root) {
    cout << "Tree (sideways, right is up):\n";
    printTreeImpl(root, 0);
    cout << "\n";
}

// Round-trip test: root -> serialize -> deserialize -> serialize
// If serialization is correct and deserialization matches it,
// the two serialized strings should match exactly.
void roundTripTest(const string &label, TreeNode *root) {
    cout << "==== " << label << " ====\n";

    // Print original tree
    printTree(root);

    // Serialize original tree
    string s1 = serialize(root);
    cout << "Serialized:   " << s1 << "\n";

    // Deserialize into a new tree
    TreeNode *copy = deserialize(s1);
    cout << "Deserialized tree:\n";
    printTree(copy);

    // Serialize the new tree again
    string s2 = serialize(copy);
    cout << "Re-serialized:" << s2 << "\n";

    // Compare the strings to confirm structure was preserved
    if (s1 == s2) {
        cout << "Round-trip OK (strings match)\n\n";
    } else {
        cout << "Round-trip MISMATCH!\n\n";
    }

    // Cleanup the deserialized tree
    freeTree(copy);
}

// ----------------------------
// Main test harness
// ----------------------------

int main() {
    // 1) Empty tree
    TreeNode *empty = nullptr;

    // 2) Single node tree
    TreeNode *single = node(42);

    // 3) Larger example tree:
    //
    //         1
    //       /   \
    //      2     3
    //     / \   /
    //    4   5 6
    TreeNode *root = node(1);
    root->left = node(2);
    root->right = node(3);
    root->left->left = node(4);
    root->left->right = node(5);
    root->right->left = node(6);

    // Run tests
    roundTripTest("Empty tree", empty);
    roundTripTest("Single-node tree", single);
    roundTripTest("Larger example tree", root);

    // Cleanup original trees
    freeTree(single);
    freeTree(root);
    // empty is nullptr, nothing to free

    return 0;
}
