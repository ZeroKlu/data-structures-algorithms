#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// =======================================
// Kahn's Algorithm (Topological Sort)
// =======================================
vector<int> topoSortKahn(int n, const vector<vector<int>>& adj) {
    vector<int> indeg(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) {
            indeg[v]++;
        }
    }

    queue<int> q;
    for (int u = 0; u < n; u++) {
        if (indeg[u] == 0) q.push(u);
    }

    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (--indeg[v] == 0)
                q.push(v);
        }
    }

    if ((int)order.size() != n)
        return {};  // cycle

    return order;
}

// =======================================
// Helper Functions
// =======================================
void printOrder(const vector<int>& ord) {
    if (ord.empty()) {
        cout << "[] (cycle detected)\n";
        return;
    }
    cout << "[";
    for (int i = 0; i < (int)ord.size(); i++) {
        cout << ord[i];
        if (i + 1 < (int)ord.size()) cout << ", ";
    }
    cout << "]\n";
}

void addEdge(vector<vector<int>>& adj, int u, int v) {
    adj[u].push_back(v);
}

// =======================================
// Test Harness
// =======================================
int main() {

    // ================================
    // Test 1: Simple DAG
    // ================================
    {
        cout << "=== Test 1: Simple DAG ===\n";
        int n = 6;
        vector<vector<int>> adj(n);

        // DAG:
        // 5 → 2, 5 → 0
        // 4 → 0, 4 → 1
        // 2 → 3
        // 3 → 1
        addEdge(adj, 5, 2);
        addEdge(adj, 5, 0);
        addEdge(adj, 4, 0);
        addEdge(adj, 4, 1);
        addEdge(adj, 2, 3);
        addEdge(adj, 3, 1);

        auto order = topoSortKahn(n, adj);
        cout << "Topological order: ";
        printOrder(order);
        cout << endl;
    }

    // ================================
    // Test 2: Another DAG
    // ================================
    {
        cout << "=== Test 2: Another DAG ===\n";
        int n = 4;
        vector<vector<int>> adj(n);

        // DAG:
        // 0 → 1, 0 → 2
        // 1 → 3
        // 2 → 3
        addEdge(adj, 0, 1);
        addEdge(adj, 0, 2);
        addEdge(adj, 1, 3);
        addEdge(adj, 2, 3);

        auto order = topoSortKahn(n, adj);
        cout << "Topological order: ";
        printOrder(order);
        cout << endl;
    }

    // ================================
    // Test 3: Graph with a cycle
    // ================================
    {
        cout << "=== Test 3: Graph containing a cycle ===\n";
        int n = 3;
        vector<vector<int>> adj(n);

        // Cycle: 0 → 1 → 2 → 0
        addEdge(adj, 0, 1);
        addEdge(adj, 1, 2);
        addEdge(adj, 2, 0);

        auto order = topoSortKahn(n, adj);
        cout << "Result: ";
        printOrder(order);   // expected: [] indicating cycle
        cout << endl;
    }

    return 0;
}
