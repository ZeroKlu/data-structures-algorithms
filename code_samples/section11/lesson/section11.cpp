#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <numeric>

using std::vector;
using namespace std;

// ===========================
// Graph utilities
// ===========================

// Build adjacency list for undirected graph with n nodes
// edges: list of (u, v) pairs, 0-based
vector<vector<int>> build_graph(int n, const vector<pair<int,int>>& edges) {
    vector<vector<int>> graph(n);
    for (auto [u, v] : edges) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    return graph;
}

void bfs_shortest(int n, const vector<vector<int>>& graph, int src, vector<int>& dist) {
    const int INF = INT_MAX / 4;
    dist.assign(n, INF);
    queue<int> q;

    dist[src] = 0;
    q.push(src);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : graph[u]) {
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}

void dfs_visit(int u, const vector<vector<int>>& graph, vector<int>& visited) {
    visited[u] = 1;
    for (int v : graph[u]) {
        if (!visited[v]) {
            dfs_visit(v, graph, visited);
        }
    }
}

int count_components(int n, const vector<vector<int>>& graph) {
    vector<int> visited(n, 0);
    int components = 0;
    for (int u = 0; u < n; u++) {
        if (!visited[u]) {
            components++;
            dfs_visit(u, graph, visited);
        }
    }
    return components;
}

// Topological sort on a directed graph
// Returns number of nodes in out_order; if < n, graph has a cycle
int topo_sort(int n, const vector<vector<int>>& graph, vector<int>& out_order) {
    vector<int> indeg(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : graph[u]) {
            indeg[v]++;
        }
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) {
            q.push(i);
        }
    }

    out_order.clear();
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        out_order.push_back(u);
        for (int v : graph[u]) {
            if (--indeg[v] == 0) {
                q.push(v);
            }
        }
    }

    return (int)out_order.size(); // if < n, there was a cycle
}

// Simple O(n^2) Dijkstra without heap
// neigh[u][i] = neighbor v, w[u][i] = corresponding weight
void dijkstra_simple(int n,
                     const vector<vector<int>>& neigh,
                     const vector<vector<int>>& w,
                     int src,
                     vector<long long>& dist) {
    const long long INF = (long long)4e18;
    vector<int> used(n, 0);
    dist.assign(n, INF);
    dist[src] = 0;

    for (int it = 0; it < n; it++) {
        int u = -1;
        long long best = INF;
        for (int i = 0; i < n; i++) {
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        used[u] = 1;

        for (int idx = 0; idx < (int)neigh[u].size(); idx++) {
            int v = neigh[u][idx];
            int wt = w[u][idx];
            if (dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
        }
    }
}

// ===========================
// DSU (Disjoint Set Union)
// ===========================

struct DSU {
    vector<int> parent;
    vector<int> rnk;

    DSU(int n) : parent(n), rnk(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return;
        if (rnk[ra] < rnk[rb]) {
            parent[ra] = rb;
        } else if (rnk[ra] > rnk[rb]) {
            parent[rb] = ra;
        } else {
            parent[rb] = ra;
            rnk[ra]++;
        }
    }
};

// ===========================
// Tests
// ===========================

void test_bfs_and_components() {
    cout << "=== Test 1: BFS shortest paths & components (undirected) ===\n";
    // Graph:
    // 0 - 1 - 2 - 4
    //     |
    //     3
    //
    // Distances from 0: [0,1,2,2,3], components = 1
    int n = 5;
    vector<pair<int,int>> edges = {
        {0,1},
        {1,2},
        {1,3},
        {2,4}
    };
    auto g = build_graph(n, edges);

    vector<int> dist;
    bfs_shortest(n, g, 0, dist);

    cout << "Distances from node 0:\n";
    for (int i = 0; i < n; i++) {
        if (dist[i] >= INT_MAX / 4) {
            cout << "  dist[" << i << "] = INF\n";
        } else {
            cout << "  dist[" << i << "] = " << dist[i] << "\n";
        }
    }

    int comps = count_components(n, g);
    cout << "Connected components = " << comps << " (expected 1)\n\n";
}

void test_components_disconnected() {
    cout << "=== Test 2: Components on disconnected graph ===\n";
    // Graph:
    // 0 - 1 - 2   3 - 4   5
    //
    // Components = 3
    int n = 6;
    vector<pair<int,int>> edges = {
        {0,1},
        {1,2},
        {3,4}
    };
    auto g = build_graph(n, edges);

    int comps = count_components(n, g);
    cout << "Connected components = " << comps << " (expected 3)\n\n";
}

void test_topo_sort() {
    cout << "=== Test 3: Topological sort on DAG ===\n";
    // Classic DAG:
    // 5 -> 2, 5 -> 0
    // 4 -> 0, 4 -> 1
    // 2 -> 3
    // 3 -> 1
    //
    // One valid topological order: 5,4,2,3,1,0 (or similar)
    int n = 6;
    vector<vector<int>> dag(n);
    auto add_edge = [&](int u, int v) {
        dag[u].push_back(v);
    };

    add_edge(5, 2);
    add_edge(5, 0);
    add_edge(4, 0);
    add_edge(4, 1);
    add_edge(2, 3);
    add_edge(3, 1);

    vector<int> order;
    int got = topo_sort(n, dag, order);

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

void test_dijkstra_simple() {
    cout << "=== Test 4: Dijkstra (simple O(n^2)) ===\n";
    // CLRS-style example:
    // 0 -> 1 (10), 0 -> 3 (5)
    // 1 -> 2 (1), 1 -> 3 (2)
    // 2 -> 4 (4)
    // 3 -> 1 (3), 3 -> 2 (9), 3 -> 4 (2)
    // 4 -> 0 (7), 4 -> 2 (6)
    //
    // Shortest distances from 0:
    //   0: 0
    //   1: 8
    //   2: 9
    //   3: 5
    //   4: 7
    int n = 5;
    vector<vector<int>> neigh(n), w(n);

    auto add_edge = [&](int u, int v, int wt) {
        neigh[u].push_back(v);
        w[u].push_back(wt);
    };

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

    vector<long long> dist;
    dijkstra_simple(n, neigh, w, 0, dist);

    cout << "Distances from node 0:\n";
    for (int i = 0; i < n; i++) {
        cout << "  dist[" << i << "] = " << dist[i] << "\n";
    }
    cout << "Expected: [0, 8, 9, 5, 7]\n\n";
}

void test_dsu() {
    cout << "=== Test 5: Disjoint Set Union (DSU) ===\n";
    // Elements: 0,1,2,3,4
    // unions: (0,1), (1,2), (3,4)
    // Sets: {0,1,2}, {3,4}
    int n = 5;
    DSU d(n);

    d.unite(0, 1);
    d.unite(1, 2);
    d.unite(3, 4);

    vector<int> repr(n);
    for (int i = 0; i < n; i++) {
        repr[i] = d.find(i);
    }

    cout << "Representatives after unions:\n";
    for (int i = 0; i < n; i++) {
        cout << "  find(" << i << ") = " << repr[i] << "\n";
    }

    // Count distinct sets
    int count = 0;
    vector<int> seen(n, 0);
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
int main() {
    test_bfs_and_components();
    test_components_disconnected();
    test_topo_sort();
    test_dijkstra_simple();
    test_dsu();
    return 0;
}
