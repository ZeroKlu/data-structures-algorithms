#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

struct Edge {
    int to;
    int w;
};

vector<int> dijkstra(int n, const vector<vector<Edge>>& adj, int s) {
    const int INF = numeric_limits<int>::max() / 2;
    vector<int> dist(n, INF);
    dist[s] = 0;

    using P = pair<int,int>; // (dist, node)
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.push({0, s});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;

        for (const auto& e : adj[u]) {
            int v = e.to;
            int nd = d + e.w;
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
    adj[u].push_back({v, w}); // directed edge
}

void printDist(const vector<int>& dist) {
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

        // Graph:
        // 0→1 (2), 0→2 (5)
        // 1→2 (1)
        // 1→3 (3)
        // 2→3 (1)
        // 3→4 (2)

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

        // Only edges inside component {0,1,2}
        addEdge(adj, 0, 1, 4);
        addEdge(adj, 1, 2, 6);

        // Nodes 3 and 4 unreachable from 0

        runTest("Disconnected graph", n, adj, 0);
    }

    return 0;
}
