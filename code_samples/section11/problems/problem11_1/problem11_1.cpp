#include <iostream>   // std::cout for printing output
#include <vector>     // std::vector for adjacency lists and distance arrays
#include <queue>      // std::queue for BFS traversal
using namespace std;  // bring standard library names into global scope (acceptable for small examples)

/*
 * bfsShortestPaths
 * ----------------
 * Computes shortest-path distances from a single source in an UNWEIGHTED graph
 * using Breadth-First Search (BFS).
 *
 * Parameters:
 *   n   : number of vertices in the graph (assumed labeled 0..n-1)
 *   adj : adjacency list representation of the graph
 *         adj[u] contains all neighbors of vertex u
 *   s   : source vertex index
 *
 * Returns:
 *   dist : vector of length n where:
 *          - dist[v] is the minimum number of edges from s to v
 *          - dist[v] == -1 if v is unreachable from s
 *
 * Notes:
 *   - BFS guarantees shortest paths in unweighted graphs because it explores
 *     vertices in increasing distance order (layer by layer).
 *   - The value -1 is used as a sentinel to mark unvisited/unreachable vertices.
 */
vector<int> bfsShortestPaths(int n, const vector<vector<int> > &adj, int s) {
    // Initialize all distances to -1 (meaning "unvisited/unreachable").
    vector<int> dist(n, -1);

    // Queue used to process vertices in FIFO order for BFS.
    queue<int> q;

    // Initialize BFS with the source vertex.
    dist[s] = 0;   // distance from source to itself is 0
    q.push(s);     // enqueue source

    // Standard BFS loop.
    while (!q.empty()) {
        int u = q.front();  // vertex at the front of the queue
        q.pop();

        // Explore all neighbors of u.
        for (int v : adj[u]) {
            // If v has not been visited yet, assign distance and enqueue it.
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist;
}

// ===========================
// Tests for bfsShortestPaths
// ===========================

/*
 * printVec
 * --------
 * Utility function to print a vector<int> in the format:
 *   [a, b, c]
 *
 * Parameters:
 *   v : vector to print
 */
void printVec(const vector<int>& v) {
    cout << "[";
    for (int i = 0; i < (int)v.size(); i++) {
        cout << v[i];
        if (i + 1 < (int)v.size()) cout << ", ";
    }
    cout << "]";
}

/*
 * test
 * ----
 * Runs bfsShortestPaths on a given graph and prints:
 *   - test name
 *   - source vertex
 *   - computed distances
 *   - expected distances
 *
 * Parameters:
 *   name     : descriptive name of the test
 *   n        : number of vertices
 *   adj      : adjacency list of the graph
 *   src      : source vertex for BFS
 *   expected : expected distance vector (for visual verification)
 *
 * Note:
 *   - This function does not assert correctness programmatically;
 *     it is intended for visual comparison.
 */
void test(const string& name,
          int n,
          const vector<vector<int>>& adj,
          int src,
          const vector<int>& expected) {
    cout << name << "\n";
    cout << "Source = " << src << "\n";

    // Run BFS shortest paths.
    auto dist = bfsShortestPaths(n, adj, src);

    // Print results.
    cout << "Distances: ";
    printVec(dist);
    cout << "\nExpected : ";
    printVec(expected);
    cout << "\n\n";
}

int main() {
    // ------------------------------
    // Test 1: Connected graph
    // ------------------------------
    //
    // Graph structure:
    //
    // 0 - 1 - 2 - 4
    //     |
    //     3
    //
    // From source 0:
    // dist = [0, 1, 2, 2, 3]

    {
        int n = 5;

        // Initialize adjacency list with n empty vectors.
        vector<vector<int>> adj(n);

        // Helper lambda to add an undirected edge (u <-> v).
        auto add = [&](int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        };

        // Add edges for the connected graph.
        add(0,1);
        add(1,2);
        add(1,3);
        add(2,4);

        test("Test 1: Connected graph", n, adj, 0, {0,1,2,2,3});
    }

    // ------------------------------
    // Test 2: Disconnected graph
    // ------------------------------
    //
    // Graph structure:
    //
    // 0 - 1     2     3 - 4
    //
    // From source 0:
    // dist = [0, 1, -1, -1, -1]

    {
        int n = 5;
        vector<vector<int>> adj(n);

        // Helper lambda to add an undirected edge.
        auto add = [&](int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        };

        // Add edges for two separate components.
        add(0,1);
        add(3,4);

        test("Test 2: Disconnected graph", n, adj, 0, {0,1,-1,-1,-1});
    }

    // ------------------------------
    // Test 3: Single node graph
    // ------------------------------
    //
    // Graph:
    // n = 1, no edges
    //
    // From source 0:
    // dist = [0]

    {
        int n = 1;

        // Adjacency list with a single vertex and no edges.
        vector<vector<int>> adj(1);

        test("Test 3: Single node", n, adj, 0, {0});
    }

    return 0;  // indicate successful program termination
}
