#include <iostream>   // std::cout, std::endl for printing test output to the console
#include <vector>     // std::vector dynamic arrays used for adjacency lists and various work buffers
#include <queue>      // std::queue used for BFS and Kahn's algorithm in topo sort
#include <limits>     // numeric limits (note: this file uses INT_MAX from <climits>; many compilers still make it visible indirectly)
#include <numeric>    // std::iota used to initialize DSU parent array

using std::vector;    // bring just vector into the global namespace (in addition to the next line)
using namespace std;  // bring in the rest of std:: (generally discouraged in headers, but acceptable in small examples)

// ===========================
// Graph utilities
// ===========================

/*
 * build_graph
 * ----------
 * Builds an adjacency list representation of an UNDIRECTED, UNWEIGHTED graph.
 *
 * Parameters:
 *   n     : number of nodes (assumed labeled 0..n-1)
 *   edges : list of (u, v) pairs, 0-based indices
 *
 * Returns:
 *   graph : vector of size n, where graph[u] is the list of neighbors of node u
 *
 * Notes:
 *   - Because the graph is undirected, every edge (u, v) is added twice:
 *       u -> v and v -> u
 *   - No validation is performed (e.g., bounds checks on u/v).
 *   - Parallel edges and self-loops are allowed by this function (it will store them as provided).
 */
vector<vector<int>> build_graph(int n, const vector<pair<int,int>>& edges) {
    vector<vector<int>> graph(n);
    for (auto [u, v] : edges) {      // structured bindings: u and v are copies of the pair elements
        graph[u].push_back(v);       // add neighbor v to u's adjacency list
        graph[v].push_back(u);       // add neighbor u to v's adjacency list (undirected)
    }
    return graph;
}

/*
 * bfs_shortest
 * ------------
 * Computes shortest path distances in an UNWEIGHTED graph from a single source using BFS.
 *
 * Parameters:
 *   n     : number of nodes
 *   graph : adjacency list (graph[u] lists neighbors of u)
 *   src   : source node index
 *   dist  : output vector of distances; overwritten by this function
 *
 * Output:
 *   dist[v] = length (in edges) of the shortest path from src to v
 *   dist[v] = INF (a large sentinel) if v is unreachable from src
 *
 * Notes:
 *   - BFS is correct for unweighted graphs because it explores nodes in nondecreasing distance order.
 *   - INF chosen as INT_MAX/4 to avoid overflow when computing dist[u] + 1.
 *   - This works for directed graphs too, as long as `graph` encodes directed edges.
 */
void bfs_shortest(int n, const vector<vector<int>>& graph, int src, vector<int>& dist) {
    const int INF = INT_MAX / 4;     // "infinite" distance sentinel, reduced to avoid overflow on +1
    dist.assign(n, INF);             // initialize all distances to INF (unvisited/unreachable)
    queue<int> q;                    // BFS frontier queue

    dist[src] = 0;                   // distance to the source is 0
    q.push(src);                     // start BFS from src

    while (!q.empty()) {
        int u = q.front();           // next node to expand
        q.pop();
        for (int v : graph[u]) {     // consider each neighbor of u
            if (dist[v] == INF) {    // first time seeing v => shortest path to v found
                dist[v] = dist[u] + 1;
                q.push(v);           // push v for expansion of its neighbors
            }
        }
    }
}

/*
 * dfs_visit
 * ---------
 * Depth-first search helper to mark all nodes reachable from a starting node u.
 *
 * Parameters:
 *   u       : starting node to visit
 *   graph   : adjacency list
 *   visited : vector of 0/1 flags (mutated); visited[x]==1 means x has been visited
 *
 * Notes:
 *   - This is a recursive DFS; deep graphs may overflow the call stack in worst cases.
 *   - Used by count_components to traverse each connected component.
 */
void dfs_visit(int u, const vector<vector<int>>& graph, vector<int>& visited) {
    visited[u] = 1;                  // mark u as visited
    for (int v : graph[u]) {         // explore all neighbors
        if (!visited[v]) {           // only recurse if neighbor not yet visited
            dfs_visit(v, graph, visited);
        }
    }
}

/*
 * count_components
 * ----------------
 * Counts the number of connected components in an UNDIRECTED graph.
 *
 * Parameters:
 *   n     : number of nodes
 *   graph : adjacency list for an undirected graph
 *
 * Returns:
 *   components : number of connected components
 *
 * How it works:
 *   - Iterate all nodes; when an unvisited node is found, it starts a DFS.
 *   - That DFS marks the entire component reachable from that node.
 *   - Each DFS start corresponds to one connected component.
 *
 * Notes:
 *   - For directed graphs, this would count "weakly connected" components only if the graph were symmetrized.
 *   - Complexity: O(n + m) where m is the number of edges in adjacency lists.
 */
int count_components(int n, const vector<vector<int>>& graph) {
    vector<int> visited(n, 0);       // visited flags for each node
    int components = 0;              // component counter
    for (int u = 0; u < n; u++) {
        if (!visited[u]) {           // new component found
            components++;
            dfs_visit(u, graph, visited); // mark all nodes in this component
        }
    }
    return components;
}

/*
 * topo_sort
 * ---------
 * Performs topological sorting on a DIRECTED graph using Kahn's algorithm (BFS over indegree-0 nodes).
 *
 * Parameters:
 *   n         : number of nodes
 *   graph     : adjacency list for a directed graph (u -> v edges)
 *   out_order : output vector storing a topological order if one exists; cleared and filled here
 *
 * Returns:
 *   size of out_order:
 *     - If it equals n, out_order is a valid topological ordering (graph is a DAG).
 *     - If it's < n, the graph contains at least one cycle (no topo ordering covers all nodes).
 *
 * Notes:
 *   - Kahn's algorithm works by repeatedly removing indegree-0 nodes.
 *   - If a cycle exists, nodes in the cycle never reach indegree 0, so the result is partial.
 *   - Complexity: O(n + m).
 */
int topo_sort(int n, const vector<vector<int>>& graph, vector<int>& out_order) {
    vector<int> indeg(n, 0);         // indeg[v] = number of incoming edges to v

    // Compute indegrees by scanning all adjacency lists.
    for (int u = 0; u < n; u++) {
        for (int v : graph[u]) {
            indeg[v]++;              // edge u -> v contributes to v's indegree
        }
    }

    queue<int> q;                    // queue of nodes with current indegree 0
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) {         // nodes with no prerequisites
            q.push(i);
        }
    }

    out_order.clear();               // ensure output starts empty
    while (!q.empty()) {
        int u = q.front();           // take an available node
        q.pop();
        out_order.push_back(u);      // append to topo order

        // "Remove" u by decreasing indegree of its outgoing neighbors.
        for (int v : graph[u]) {
            if (--indeg[v] == 0) {   // if v now has no remaining incoming edges, it becomes available
                q.push(v);
            }
        }
    }

    return (int)out_order.size();    // if < n, there was a cycle preventing full ordering
}

/*
 * dijkstra_simple
 * ---------------
 * Computes single-source shortest paths in a WEIGHTED directed graph with NON-NEGATIVE edge weights.
 * This implementation uses the classic O(n^2) version of Dijkstra (no heap/priority queue).
 *
 * Parameters:
 *   n     : number of nodes
 *   neigh : adjacency list of neighbors; neigh[u][i] gives the i-th neighbor v of u
 *   w     : parallel adjacency list of weights; w[u][i] is the weight of edge u -> neigh[u][i]
 *   src   : source node
 *   dist  : output distances (long long); overwritten by this function
 *
 * Output:
 *   dist[v] = shortest distance from src to v, or INF if unreachable
 *
 * Notes / assumptions:
 *   - All edge weights must be >= 0 for Dijkstra to be correct.
 *   - neigh and w must be aligned: for each u, neigh[u].size() == w[u].size().
 *   - Complexity: O(n^2 + m) ~ O(n^2) for dense graphs; good enough for small n.
 *   - INF is set extremely large to represent unreachable nodes.
 */
void dijkstra_simple(int n,
                     const vector<vector<int>>& neigh,
                     const vector<vector<int>>& w,
                     int src,
                     vector<long long>& dist) {
    const long long INF = (long long)4e18; // large sentinel well beyond typical path sums
    vector<int> used(n, 0);                // used[u]=1 means u is finalized (shortest dist determined)
    dist.assign(n, INF);                   // initialize all distances to INF
    dist[src] = 0;                         // distance to source is 0

    // Repeat up to n times: each iteration finalizes one node (the closest unused node).
    for (int it = 0; it < n; it++) {
        int u = -1;                        // next node to finalize
        long long best = INF;              // best distance among unused nodes

        // Find the unused node with minimum tentative distance (O(n) scan).
        for (int i = 0; i < n; i++) {
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }

        // If no reachable unused node remains, we can stop early.
        if (u == -1) break;

        used[u] = 1;                       // finalize u

        // Relax all outgoing edges u -> v.
        for (int idx = 0; idx < (int)neigh[u].size(); idx++) {
            int v = neigh[u][idx];         // neighbor node
            int wt = w[u][idx];            // edge weight
            // If going through u improves dist[v], update it.
            if (dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
        }
    }
}

// ===========================
// DSU (Disjoint Set Union)
// ===========================

/*
 * DSU (Disjoint Set Union) / Union-Find
 * -------------------------------------
 * Maintains a collection of disjoint sets with efficient union and find operations.
 *
 * Typical operations:
 *   - find(x): returns representative (root) of the set containing x
 *   - unite(a, b): merges the sets containing a and b (if they are different)
 *
 * Implementation details:
 *   - parent[x] points to x's parent in the tree; roots satisfy parent[root] == root
 *   - rnk[root] is an upper bound on the tree height (rank heuristic)
 *   - path compression in find() flattens trees over time
 *   - union by rank keeps trees shallow
 *
 * Amortized performance:
 *   - Almost constant time per operation (inverse Ackermann).
 */
struct DSU {
    vector<int> parent;  // parent pointers for union-find forest
    vector<int> rnk;     // rank (approximate tree height) for union-by-rank heuristic

    /*
     * Constructor:
     *   Initializes n singleton sets: each element is its own parent (root).
     */
    DSU(int n) : parent(n), rnk(n, 0) {
        iota(parent.begin(), parent.end(), 0); // parent[i] = i for all i
    }

    /*
     * find
     * ----
     * Finds the representative/root of the set containing x.
     * Applies path compression by setting parent[x] directly to the root.
     */
    int find(int x) {
        if (parent[x] != x) {          // if x is not a root
            parent[x] = find(parent[x]); // recurse to root, compress path
        }
        return parent[x];              // return representative
    }

    /*
     * unite
     * -----
     * Merges the sets containing a and b.
     * Uses union by rank to attach the smaller-rank tree under the larger-rank root.
     *
     * If a and b are already in the same set, does nothing.
     */
    void unite(int a, int b) {
        int ra = find(a);              // root of a
        int rb = find(b);              // root of b
        if (ra == rb) return;          // already unified

        // Attach lower-rank root under higher-rank root.
        if (rnk[ra] < rnk[rb]) {
            parent[ra] = rb;
        } else if (rnk[ra] > rnk[rb]) {
            parent[rb] = ra;
        } else {
            // Same rank: choose one as new root and increment its rank.
            parent[rb] = ra;
            rnk[ra]++;
        }
    }
};

// ===========================
// Tests
// ===========================

/*
 * test_bfs_and_components
 * -----------------------
 * Builds a small connected undirected graph, runs:
 *   - BFS shortest paths from node 0
 *   - connected component count
 *
 * Prints computed distances and the component count.
 * Intended to demonstrate correctness of bfs_shortest() and count_components().
 */
void test_bfs_and_components() {
    cout << "=== Test 1: BFS shortest paths & components (undirected) ===\n";
    // Graph structure:
    // 0 - 1 - 2 - 4
    //     |
    //     3
    //
    // Expected distances from 0: [0,1,2,2,3], components = 1
    int n = 5;
    vector<pair<int,int>> edges = {
        {0,1},
        {1,2},
        {1,3},
        {2,4}
    };
    auto g = build_graph(n, edges);   // adjacency list for undirected graph

    vector<int> dist;                 // distance output buffer
    bfs_shortest(n, g, 0, dist);      // compute BFS distances from source 0

    // Print distances, using "INF" for unreachable nodes.
    cout << "Distances from node 0:\n";
    for (int i = 0; i < n; i++) {
        if (dist[i] >= INT_MAX / 4) { // matches BFS's INF sentinel
            cout << "  dist[" << i << "] = INF\n";
        } else {
            cout << "  dist[" << i << "] = " << dist[i] << "\n";
        }
    }

    int comps = count_components(n, g); // connected components in an undirected graph
    cout << "Connected components = " << comps << " (expected 1)\n\n";
}

/*
 * test_components_disconnected
 * ----------------------------
 * Builds an undirected graph with 3 components and verifies count_components().
 */
void test_components_disconnected() {
    cout << "=== Test 2: Components on disconnected graph ===\n";
    // Graph:
    // 0 - 1 - 2   3 - 4   5
    //
    // Expected components = 3 (one path on {0,1,2}, one edge on {3,4}, singleton {5})
    int n = 6;
    vector<pair<int,int>> edges = {
        {0,1},
        {1,2},
        {3,4}
    };
    auto g = build_graph(n, edges);   // adjacency list for undirected graph

    int comps = count_components(n, g);
    cout << "Connected components = " << comps << " (expected 3)\n\n";
}

/*
 * test_topo_sort
 * --------------
 * Constructs a classic directed acyclic graph (DAG) and runs topo_sort().
 * Prints one possible topological order or indicates a cycle if found.
 */
void test_topo_sort() {
    cout << "=== Test 3: Topological sort on DAG ===\n";
    // Classic DAG edges:
    // 5 -> 2, 5 -> 0
    // 4 -> 0, 4 -> 1
    // 2 -> 3
    // 3 -> 1
    //
    // A valid topological order must place each u before its outgoing neighbors v.
    int n = 6;
    vector<vector<int>> dag(n);       // directed adjacency list

    // Local helper lambda to add a directed edge u -> v into dag.
    auto add_edge = [&](int u, int v) {
        dag[u].push_back(v);
    };

    // Add directed edges as per the example.
    add_edge(5, 2);
    add_edge(5, 0);
    add_edge(4, 0);
    add_edge(4, 1);
    add_edge(2, 3);
    add_edge(3, 1);

    vector<int> order;                // topo order output buffer
    int got = topo_sort(n, dag, order);

    // If we didn't output all nodes, there's a cycle.
    if (got < n) {
        cout << "Graph has a cycle (topo_sort returned " << got << " < " << n << ")\n\n";
    } else {
        cout << "Topological order (one possible): ";
        for (int i = 0; i < n; i++) {
            cout << order[i] << (i + 1 == n ? "" : " ");
        }
        cout << "\n\n";
    }
}

/*
 * test_dijkstra_simple
 * --------------------
 * Builds a small directed, weighted graph and runs the O(n^2) Dijkstra.
 * Prints distances and an expected vector for comparison.
 *
 * Important:
 *   Dijkstra requires non-negative edge weights; all weights here are positive.
 */
void test_dijkstra_simple() {
    cout << "=== Test 4: Dijkstra (simple O(n^2)) ===\n";
    // CLRS-style example (directed edges with weights):
    // 0 -> 1 (10), 0 -> 3 (5)
    // 1 -> 2 (1), 1 -> 3 (2)
    // 2 -> 4 (4)
    // 3 -> 1 (3), 3 -> 2 (9), 3 -> 4 (2)
    // 4 -> 0 (7), 4 -> 2 (6)
    //
    // Expected shortest distances from 0:
    //   0: 0
    //   1: 8
    //   2: 9
    //   3: 5
    //   4: 7
    int n = 5;
    vector<vector<int>> neigh(n), w(n); // parallel arrays: neighbors and their edge weights

    // Local helper lambda to add a directed edge u -> v with weight wt.
    // Maintains alignment between neigh[u] and w[u].
    auto add_edge = [&](int u, int v, int wt) {
        neigh[u].push_back(v);
        w[u].push_back(wt);
    };

    // Add all edges from the example.
    add_edge(0, 1, 10);
    add_edge(0, 3, 5);
    add_edge(1, 2, 1);
    add_edge(1, 3, 2);
    add_edge(2, 4, 4);
    add_edge(3, 1, 3);
    add_edge(3, 2, 9);
    add_edge(3, 4, 2);
    add_edge(4, 0, 7);
    add_edge(4, 2, 6);

    vector<long long> dist;            // output buffer for distances
    dijkstra_simple(n, neigh, w, 0, dist);

    // Print computed distances from node 0.
    cout << "Distances from node 0:\n";
    for (int i = 0; i < n; i++) {
        cout << "  dist[" << i << "] = " << dist[i] << "\n";
    }
    cout << "Expected: [0, 8, 9, 5, 7]\n\n";
}

/*
 * test_dsu
 * --------
 * Demonstrates DSU operations by:
 *   - creating 5 singleton sets {0},{1},{2},{3},{4}
 *   - union(0,1), union(1,2), union(3,4)
 *   - printing find(i) for each element
 *   - counting distinct representatives to confirm there are 2 sets total
 *
 * Notes:
 *   - The representative values can vary depending on union order and rank ties.
 *     The important property is which elements share a representative.
 */
void test_dsu() {
    cout << "=== Test 5: Disjoint Set Union (DSU) ===\n";
    // Elements: 0,1,2,3,4
    // unions: (0,1), (1,2), (3,4)
    // Expected partition: {0,1,2}, {3,4}
    int n = 5;
    DSU d(n);

    d.unite(0, 1);                     // merge sets containing 0 and 1
    d.unite(1, 2);                     // merge set {0,1} with {2} => {0,1,2}
    d.unite(3, 4);                     // merge sets containing 3 and 4 => {3,4}

    vector<int> repr(n);               // store representatives for printing and counting
    for (int i = 0; i < n; i++) {
        repr[i] = d.find(i);           // find with path compression may alter internal parent structure
    }

    cout << "Representatives after unions:\n";
    for (int i = 0; i < n; i++) {
        cout << "  find(" << i << ") = " << repr[i] << "\n";
    }

    // Count distinct sets by marking which representatives appear.
    int count = 0;
    vector<int> seen(n, 0);            // seen[r]=1 once representative r has been observed
    for (int i = 0; i < n; i++) {
        int r = repr[i];
        if (!seen[r]) {
            seen[r] = 1;
            count++;
        }
    }
    cout << "Number of disjoint sets = " << count << " (expected 2)\n\n";
}

// ===========================
// main
// ===========================

/*
 * main
 * ----
 * Runs all tests in sequence. This file is structured as a self-contained demo:
 * graph utilities + DSU + small, readable tests that print expected results.
 */
int main() {
    test_bfs_and_components();
    test_components_disconnected();
    test_topo_sort();
    test_dijkstra_simple();
    test_dsu();
    return 0;                           // conventional successful exit code
}
