#include <iostream>  // std::cout for printing test output
#include <vector>    // std::vector for adjacency lists and component labels
using namespace std; // bring standard library names into the global namespace (fine for small demos)

// =================================
// DFS to assign component ID
// =================================
void dfs_comp(int u, int cid, const vector<vector<int>>& adj, vector<int>& comp) {
    // Depth-First Search (DFS) that labels every node reachable from `u`
    // with the same connected-component id `cid`.
    //
    // Parameters:
    //   u    : current node being visited
    //   cid  : component id to assign to nodes in this connected component
    //   adj  : adjacency list of an UNDIRECTED graph (adj[u] contains neighbors)
    //   comp : output array where comp[x] == -1 means "unvisited",
    //          otherwise comp[x] stores the component id for node x
    //
    // Behavior:
    //   - Marks u as belonging to component cid
    //   - Recursively visits any neighbor v that has not yet been labeled
    //
    // Note:
    //   - This is recursive DFS; very large graphs may cause stack overflow.
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
    // Computes connected components of an UNDIRECTED graph.
    //
    // Parameters:
    //   n    : number of nodes (assumed labeled 0..n-1)
    //   adj  : adjacency list for the graph
    //   comp : output vector that will be resized to n and filled with component ids
    //
    // Returns:
    //   cid  : the number of connected components found
    //
    // Approach:
    //   - Initialize comp[u] = -1 for all nodes (meaning unvisited).
    //   - Iterate all nodes; whenever an unvisited node u is found,
    //     start a DFS from u and label its entire component with the current cid.
    //   - Increment cid after finishing each DFS.
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
    // Adds an UNDIRECTED edge between u and v.
    //
    // Parameters:
    //   adj : adjacency list (mutable)
    //   u,v : endpoints of the edge
    //
    // Behavior:
    //   - Adds v to u's neighbor list
    //   - Adds u to v's neighbor list
    //
    // Note:
    //   - No bounds checking is performed for u and v.
    //   - Parallel edges/self-loops are allowed as provided.
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void printComp(const vector<int>& comp) {
    // Prints the component id array in a compact "[a, b, c]" format.
    //
    // Parameters:
    //   comp : component id array; comp[u] is the component id for node u
    cout << "[";
    for (int i = 0; i < (int)comp.size(); i++) {
        cout << comp[i];
        if (i + 1 < (int)comp.size()) cout << ", ";
    }
    cout << "]";
}

void runTest(const string& name, int n, const vector<vector<int>>& adj) {
    // Runs connectedComponents() on the provided graph and prints:
    //   - the number of connected components
    //   - the component id assigned to each node
    //
    // Parameters:
    //   name : test label printed to the console
    //   n    : number of nodes
    //   adj  : adjacency list
    //
    // Note:
    //   - This is a "print and inspect" harness; it does not assert expected values.
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

        // Build a complete graph (clique) on 5 nodes.
        // In a complete graph, every node can reach every other node,
        // so the number of connected components should be 1.
        int n = 5;
        vector<vector<int>> adj(n);

        // Add an undirected edge between every pair (u, v) with u < v.
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

        // Despite the printed label, this setup actually creates:
        //   - component A: {0,1,2}
        //   - component B: {3,4}
        //   - component C: {5} (isolated)
        // so it will typically produce 3 total components.
        //
        // The algorithm and output are still valid; the comment is here to clarify
        // what the constructed graph contains.
        int n = 6;
        vector<vector<int>> adj(n);

        // Component 0: 0-1-2 (a chain)
        addEdge(adj, 0, 1);
        addEdge(adj, 1, 2);

        // Component 1: 3-4, and 5 isolated (its own component)
        addEdge(adj, 3, 4);

        runTest("Two components", n, adj);
    }

    // ------------------------------
    // Test 3: Each node isolated
    // ------------------------------
    {
        cout << "=== Test 3: All nodes isolated ===\n";

        // No edges: each node is isolated and thus forms its own component.
        // For n=4, expected component count is 4.
        int n = 4;
        vector<vector<int>> adj(n); // no edges

        runTest("Four isolated nodes", n, adj);
    }

    // ------------------------------
    // Test 4: Mixed structure
    // ------------------------------
    {
        cout << "=== Test 4: Mixed graph ===\n";

        // Graph with multiple non-trivial components plus an isolated node.
        //   - component A: 0-1-2
        //   - component B: 3-4-5
        //   - component C: {6} isolated
        // Expected total components: 3.
        int n = 7;
        vector<vector<int>> adj(n);

        // Component 0: 0-1-2
        addEdge(adj, 0, 1);
        addEdge(adj, 1, 2);

        // Component 1: 3-4-5
        addEdge(adj, 3, 4);
        addEdge(adj, 4, 5);

        // Node 6 isolated (no edges added)

        runTest("Mixed graph", n, adj);
    }

    return 0; // successful termination
}
