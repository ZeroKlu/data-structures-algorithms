#include <iostream>  // std::cout for printing results
#include <vector>    // std::vector for adjacency lists and distance array
#include <queue>     // std::priority_queue for Dijkstra's min-priority queue
#include <limits>    // std::numeric_limits for an "infinity" sentinel
using namespace std; // convenience for small sample code

/*
 * Edge:
 * Represents a directed weighted edge u -> to with weight w.
 * Stored inside the adjacency list for each node.
 */
struct Edge {
    int to; // destination node id
    int w;  // edge weight (must be non-negative for Dijkstra to be correct)
};

vector<int> dijkstra(int n, const vector<vector<Edge>>& adj, int s) {
    /*
     * Computes single-source shortest paths using Dijkstra's algorithm
     * on a DIRECTED graph with NON-NEGATIVE edge weights.
     *
     * Parameters:
     *   n   : number of nodes (assumed labeled 0..n-1)
     *   adj : adjacency list where adj[u] is a list of outgoing edges from u
     *   s   : source node
     *
     * Returns:
     *   dist : vector where dist[v] is the shortest distance from s to v,
     *          or INF if v is unreachable.
     *
     * Implementation details:
     *   - Uses a min-priority queue of (distance, node).
     *   - Because we don't do "decrease-key", we may push multiple entries for
     *     the same node. We discard outdated entries using:
     *       if (d > dist[u]) continue;
     */
    const int INF = numeric_limits<int>::max() / 2; // reduced max to lower overflow risk
    vector<int> dist(n, INF);

    // Distance from source to itself is 0
    dist[s] = 0;

    // Priority queue item type: (distance, node).
    // We'll use greater<> to make it a min-heap by distance.
    using P = pair<int,int>; // (dist, node)
    priority_queue<P, vector<P>, greater<P>> pq;

    // Start with source node at distance 0
    pq.push({0, s});

    // Main loop: always expand the currently known closest node
    while (!pq.empty()) {
        // Pop best (smallest distance) candidate
        auto [d, u] = pq.top();
        pq.pop();

        // If this (d,u) is stale (we've since found a better path), skip it.
        if (d > dist[u]) continue;

        // Relax all outgoing edges u -> v
        for (const auto& e : adj[u]) {
            int v = e.to;
            int nd = d + e.w; // candidate distance to v via u

            // If we found a better path to v, record it and push into the heap.
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }

    return dist;
}

// ==========================
// Helpers
// ==========================

void addEdge(vector<vector<Edge>>& adj, int u, int v, int w) {
    /*
     * Adds a DIRECTED edge u -> v with weight w to the graph.
     *
     * Parameters:
     *   adj : adjacency list (mutable)
     *   u   : source node
     *   v   : destination node
     *   w   : edge weight
     *
     * Note:
     *   - No bounds checking is performed for u/v.
     *   - For undirected graphs you would add both (u->v) and (v->u).
     */
    adj[u].push_back({v, w}); // directed edge
}

void printDist(const vector<int>& dist) {
    /*
     * Prints the distance array in a compact list form.
     *
     * Convention:
     *   - Distances that are still very large are printed as "INF"
     *     to indicate unreachable nodes.
     *
     * Note:
     *   - Uses numeric_limits<int>::max()/4 as a "large enough" threshold.
     *     This matches the INF style used in dijkstra() (max/2) and helps avoid
     *     printing enormous integers.
     */
    cout << "[";
    for (int i = 0; i < (int)dist.size(); i++) {
        if (dist[i] >= numeric_limits<int>::max() / 4)
            cout << "INF";
        else
            cout << dist[i];
        if (i + 1 < (int)dist.size()) cout << ", ";
    }
    cout << "]\n";
}

void runTest(const string& name, int n, const vector<vector<Edge>>& adj, int src) {
    /*
     * Runs Dijkstra from `src` and prints the resulting distance array.
     *
     * Parameters:
     *   name : test label printed first
     *   n    : number of nodes
     *   adj  : adjacency list
     *   src  : source node
     *
     * Note:
     *   - `n` is passed to mirror the signature of dijkstra(), even though
     *     `adj.size()` could also be used.
     */
    cout << name << "\n";
    cout << "Source = " << src << "\n";
    auto dist = dijkstra(n, adj, src);
    cout << "Distances: ";
    printDist(dist);
    cout << "\n";
}

// ==========================
// Main Test Harness
// ==========================

int main() {

    // -----------------------------------------
    // Test 1: Simple weighted directed graph
    // -----------------------------------------
    {
        cout << "=== Test 1: Simple Graph ===\n";
        int n = 5;
        vector<vector<Edge>> adj(n);

        // Graph (directed, weighted):
        // 0→1 (2), 0→2 (5)
        // 1→2 (1)
        // 1→3 (3)
        // 2→3 (1)
        // 3→4 (2)
        //
        // This test checks that:
        //   - Dijkstra chooses cheaper multi-hop paths over expensive direct edges
        //     (e.g., 0->1->2 can beat 0->2).
        addEdge(adj, 0, 1, 2);
        addEdge(adj, 0, 2, 5);
        addEdge(adj, 1, 2, 1);
        addEdge(adj, 1, 3, 3);
        addEdge(adj, 2, 3, 1);
        addEdge(adj, 3, 4, 2);

        runTest("Simple weighted graph", n, adj, 0);
    }

    // -----------------------------------------
    // Test 2: Multiple shortest paths
    // -----------------------------------------
    {
        cout << "=== Test 2: Multiple Shortest Paths ===\n";

        int n = 6;
        vector<vector<Edge>> adj(n);

        // Graph:
        // 0→1 (1), 0→2 (1)
        // 1→3 (2)
        // 2→3 (2)
        // 3→4 (1)
        // 4→5 (5)
        //
        // There are two equal-cost ways to reach node 3 from 0:
        //   0->1->3 and 0->2->3 both cost 3.
        // This test checks the algorithm remains correct when multiple
        // shortest paths exist.
        addEdge(adj, 0, 1, 1);
        addEdge(adj, 0, 2, 1);
        addEdge(adj, 1, 3, 2);
        addEdge(adj, 2, 3, 2);
        addEdge(adj, 3, 4, 1);
        addEdge(adj, 4, 5, 5);

        runTest("Branches with equal-cost paths", n, adj, 0);
    }

    // -----------------------------------------
    // Test 3: Disconnected graph
    // -----------------------------------------
    {
        cout << "=== Test 3: Disconnected Graph ===\n";

        int n = 5;
        vector<vector<Edge>> adj(n);

        // Only edges inside component {0,1,2}:
        //   0→1 (4)
        //   1→2 (6)
        //
        // Nodes 3 and 4 are unreachable from source 0, so their distances
        // should remain INF.
        addEdge(adj, 0, 1, 4);
        addEdge(adj, 1, 2, 6);

        runTest("Disconnected graph", n, adj, 0);
    }

    return 0; // successful termination
}
