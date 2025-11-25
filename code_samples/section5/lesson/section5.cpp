#include <iostream>
#include <queue>
#include <vector>
#include <limits>

struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : value(v), left(nullptr), right(nullptr) {}
};

void preorder(TreeNode* root) {
    if (!root) return;
    std::cout << root->value << std::endl;
    preorder(root->left);
    preorder(root->right);
}

void levelOrder(TreeNode* root) {
    if (!root) return;
    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        std::cout << node->value << std::endl;
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}

void dfs_util(int node, const std::vector<std::vector<int>>& graph,
              std::vector<bool>& visited) {
    visited[node] = true;
    std::cout << node << std::endl;
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfs_util(neighbor, graph, visited);
        }
    }
}

void dfs(int start, const std::vector<std::vector<int>>& graph) {
    int n = (int)graph.size();
    std::vector<bool> visited(n, false);
    dfs_util(start, graph, visited);
}

std::vector<int> shortestDistances(int start,
    const std::vector<std::vector<int>>& graph) {
    int n = (int)graph.size();
    std::vector<int> dist(n, -1);
    std::queue<int> q;

    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int neighbor : graph[node]) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[node] + 1;
                q.push(neighbor);
            }
        }
    }
    return dist;
}

int main() {
    std::cout << "==== TEST TREE (preorder + levelOrder) ====\n";

    // Build this tree:
    //
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    //
    TreeNode* n4 = new TreeNode(4);
    TreeNode* n5 = new TreeNode(5);
    TreeNode* n6 = new TreeNode(6);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n3 = new TreeNode(3);
    TreeNode* root = new TreeNode(1);

    n2->left = n4;
    n2->right = n5;
    n3->right = n6;
    root->left = n2;
    root->right = n3;

    std::cout << "Preorder traversal (expect 1 2 4 5 3 6):\n";
    preorder(root);

    std::cout << "\nLevel-order traversal (expect 1 2 3 4 5 6):\n";
    levelOrder(root);

    std::cout << "\n==== TEST GRAPH DFS ====\n";

    // Graph:
    // 0 -- 1
    // |  /
    // 2
    // |
    // 3
    //
    // Adjacency list (undirected)
    std::vector<std::vector<int>> graph(4);
    graph[0] = {1, 2};
    graph[1] = {0, 2};
    graph[2] = {0, 1, 3};
    graph[3] = {2};

    std::cout << "DFS starting at node 0 (e.g. 0 1 2 3):\n";
    dfs(0, graph);

    std::cout << "\n==== TEST shortestDistances (BFS) ====\n";
    auto dist = shortestDistances(0, graph);

    std::cout << "Shortest distances from node 0:\n";
    for (std::size_t i = 0; i < dist.size(); ++i) {
        std::cout << "dist to " << i << " = " << dist[i] << '\n';
    }
    // Expected:
    // dist[0] = 0
    // dist[1] = 1
    // dist[2] = 1
    // dist[3] = 2

    // Clean up tree nodes
    delete n4;
    delete n5;
    delete n6;
    delete n2;
    delete n3;
    delete root;

    std::cout << "\n==== ALL TESTS COMPLETE ====\n";
    return 0;
}
