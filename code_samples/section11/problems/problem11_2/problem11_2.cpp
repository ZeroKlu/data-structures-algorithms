#include <iostream>
#include <vector>
using namespace std;

// =================================
// DFS to assign component ID
// =================================
void dfs_comp(int u, int cid, const vector<vector<int>>& adj, vector<int>& comp) {
    comp[u] = cid;
    for (int v : adj[u]) {
        if (comp[v] == -1) {
            dfs_comp(v, cid, adj, comp);
        }
    }
}

// =================================
// Count components and assign IDs
// comp[u] = component ID of node u
// returns number of components
// =================================
int connectedComponents(int n, const vector<vector<int>>& adj, vector<int>& comp) {
    comp.assign(n, -1);
    int cid = 0;

    for (int u = 0; u < n; u++) {
        if (comp[u] == -1) {
            dfs_comp(u, cid, adj, comp);
            cid++;
        }
    }
    return cid;
}

// =================================
// Helper functions for tests
// =================================
void addEdge(vector<vector<int>>& adj, int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void printComp(const vector<int>& comp) {
    cout << "[";
    for (int i = 0; i < (int)comp.size(); i++) {
        cout << comp[i];
        if (i + 1 < (int)comp.size()) cout << ", ";
    }
    cout << "]";
}

void runTest(const string& name, int n, const vector<vector<int>>& adj) {
    cout << name << "\n";
    vector<int> comp;

    int count = connectedComponents(n, adj, comp);

    cout << "Component count: " << count << "\n";
    cout << "Component IDs   : ";
    printComp(comp);
    cout << "\n\n";
}

// =================================
// Main test driver
// =================================
int main() {
    // ------------------------------
    // Test 1: Fully connected graph
    // ------------------------------
    {
        cout << "=== Test 1: Fully connected graph ===\n";
        int n = 5;
        vector<vector<int>> adj(n);
        for (int u = 0; u < n; u++)
            for (int v = u + 1; v < n; v++)
                addEdge(adj, u, v);

        runTest("Complete graph", n, adj);
    }

    // ------------------------------
    // Test 2: Two components
    // ------------------------------
    {
        cout << "=== Test 2: Graph with 2 components ===\n";
        int n = 6;
        vector<vector<int>> adj(n);

        // Component 0: 0-1-2
        addEdge(adj, 0, 1);
        addEdge(adj, 1, 2);

        // Component 1: 3-4, and 5 isolated
        addEdge(adj, 3, 4);

        runTest("Two components", n, adj);
    }

    // ------------------------------
    // Test 3: Each node isolated
    // ------------------------------
    {
        cout << "=== Test 3: All nodes isolated ===\n";
        int n = 4;
        vector<vector<int>> adj(n); // no edges

        runTest("Four isolated nodes", n, adj);
    }

    // ------------------------------
    // Test 4: Mixed structure
    // ------------------------------
    {
        cout << "=== Test 4: Mixed graph ===\n";
        int n = 7;
        vector<vector<int>> adj(n);

        // Component 0: 0-1-2
        addEdge(adj, 0, 1);
        addEdge(adj, 1, 2);

        // Component 1: 3-4-5
        addEdge(adj, 3, 4);
        addEdge(adj, 4, 5);

        // Node 6 isolated

        runTest("Mixed graph", n, adj);
    }

    return 0;
}
