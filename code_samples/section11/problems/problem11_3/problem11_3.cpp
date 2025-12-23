#include <iostream> // std::cout for printing output
#include <vector>   // std::vector for adjacency list and result storage
#include <queue>    // std::queue for Kahn's algorithm BFS frontier
using namespace std; // convenient for small demo files (avoid in large projects)

// =======================================
// Kahn's Algorithm (Topological Sort)
// =======================================
vector<int> topoSortKahn(int n, const vector<vector<int>>& adj) {
    // Computes a topological ordering of a DIRECTED graph using Kahn's algorithm.
    //
    // Parameters:
    //   n   : number of nodes (assumed labeled 0..n-1)
    //   adj : adjacency list for a directed graph; edges are u -> v stored in adj[u]
    //
    // Returns:
    //   order : a vector of nodes in topological order if the graph is a DAG
    //   {}    : empty vector if a cycle is detected (topological ordering impossible)
    //
    // Kahn's algorithm overview:
    //   1) Compute indegree[v] = number of incoming edges to v
    //   2) Initialize a queue with all nodes having indegree 0
    //   3) Repeatedly:
    //        - pop a node u from the queue
    //        - append u to the ordering
    //        - "remove" outgoing edges u->v by decrementing indegree[v]
    //        - if indegree[v] becomes 0, push v into the queue
    //   4) If the ordering contains all n nodes, success; otherwise, a cycle exists
    vector<int> indeg(n, 0);

    // Step 1: Compute indegree of every node by scanning all edges.
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) {
            indeg[v]++;
        }
    }

    // Step 2: Start with all nodes that have no incoming edges.
    queue<int> q;
    for (int u = 0; u < n; u++) {
        if (indeg[u] == 0) q.push(u);
    }

    // Step 3: Process nodes in BFS-like manner.
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); 
        q.pop();

        // u has no remaining prerequisites, so it can appear next in the topo order.
        order.push_back(u);

        // "Remove" all outgoing edges u -> v.
        for (int v : adj[u]) {
            // Decrement indegree; if it becomes 0, v is now ready to process.
            if (--indeg[v] == 0)
                q.push(v);
        }
    }

    // Step 4: If we didn't process all nodes, some nodes were part of a cycle
    // (they never reached indegree 0).
    if ((int)order.size() != n)
        return {};  // cycle detected: no valid topological order

    return order;   // success
}

// =======================================
// Helper Functions
// =======================================
void printOrder(const vector<int>& ord) {
    // Prints a topological order result.
    //
    // Parameters:
    //   ord : vector of node ids in topological order
    //
    // Convention used in this file:
    //   - An empty vector means "cycle detected" (no topo ordering exists).
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
    // Adds a DIRECTED edge u -> v to the adjacency list.
    //
    // Parameters:
    //   adj : adjacency list (mutable)
    //   u   : source node
    //   v   : destination node
    //
    // Note:
    //   - No bounds checking is performed for u and v.
    //   - Parallel edges are allowed if this is called multiple times with same pair.
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
        //
        // Many valid topological orders exist; one possible result is:
        //   [4, 5, 2, 3, 1, 0] (or similar)
        addEdge(adj, 5, 2);
        addEdge(adj, 5, 0);
        addEdge(adj, 4, 0);
        addEdge(adj, 4, 1);
        addEdge(adj, 2, 3);
        addEdge(adj, 3, 1);

        auto order = topoSortKahn(n, adj);

        cout << "Topological order: ";
        printOrder(order);
        cout << endl; // extra blank line after this test's output
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
        //
        // Valid topological orders include:
        //   [0, 1, 2, 3] or [0, 2, 1, 3]
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
        //
        // Cyclic graphs do not have a topological ordering.
        // topoSortKahn should return an empty vector in this case.
        addEdge(adj, 0, 1);
        addEdge(adj, 1, 2);
        addEdge(adj, 2, 0);

        auto order = topoSortKahn(n, adj);

        cout << "Result: ";
        printOrder(order);   // expected: [] indicating cycle
        cout << endl;
    }

    return 0; // successful termination
}
