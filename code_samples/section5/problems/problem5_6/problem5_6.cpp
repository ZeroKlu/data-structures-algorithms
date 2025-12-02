#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <iostream>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

string serialize(TreeNode *root) {
    string out;
    function<void(TreeNode *)> dfs = [&](TreeNode *node) {
        if (!node) {
            out += "#,";
            return;
        }
        out += to_string(node->val) + ",";
        dfs(node->left);
        dfs(node->right);
    };
    dfs(root);
    return out;
}

TreeNode *deserialize(const string &data) {
    vector<string> tokens;
    string token;
    stringstream ss(data);
    while (getline(ss, token, ',')) {
        if (!token.empty()) tokens.push_back(token);
    }
    int idx = 0;
    function<TreeNode *()> dfs = [&]() -> TreeNode * {
        if (idx >= (int)tokens.size()) return nullptr;
        const string &t = tokens[idx++];
        if (t == "#") return nullptr;
        int val = stoi(t);
        TreeNode *node = new TreeNode(val);
        node->left = dfs();
        node->right = dfs();
        return node;
    };
    return dfs();
}

// ----------------------------
// Helpers for testing
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

    cout << string(indent, ' ') << root->val << "\n";

    printTreeImpl(root->left, indent + STEP);
}

void printTree(TreeNode *root) {
    cout << "Tree (sideways, right is up):\n";
    printTreeImpl(root, 0);
    cout << "\n";
}

// Round-trip test: root -> serialize -> deserialize -> serialize
void roundTripTest(const string &label, TreeNode *root) {
    cout << "==== " << label << " ====\n";
    printTree(root);

    string s1 = serialize(root);
    cout << "Serialized:   " << s1 << "\n";

    TreeNode *copy = deserialize(s1);
    cout << "Deserialized tree:\n";
    printTree(copy);

    string s2 = serialize(copy);
    cout << "Re-serialized:" << s2 << "\n";

    if (s1 == s2) {
        cout << "Round-trip OK (strings match)\n\n";
    } else {
        cout << "Round-trip MISMATCH!\n\n";
    }

    freeTree(copy);
}

// ----------------------------
// Main test harness
// ----------------------------

int main() {
    // 1. Empty tree
    TreeNode *empty = nullptr;

    // 2. Single node
    TreeNode *single = node(42);

    // 3. Larger tree:
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

    roundTripTest("Empty tree", empty);
    roundTripTest("Single-node tree", single);
    roundTripTest("Larger example tree", root);

    freeTree(single);
    freeTree(root);
    // empty is nullptr

    return 0;
}
