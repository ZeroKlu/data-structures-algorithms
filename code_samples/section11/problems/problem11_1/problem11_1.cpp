#include <iostream>
#include <vector>
#include <queue>
using namespace std;

vector<int> bfsShortestPaths(int n, const vector<vector<int> > &adj, int s) {
    vector<int> dist(n, -1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
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

void printVec(const vector<int>& v) {
    cout << "[";
    for (int i = 0; i < (int)v.size(); i++) {
        cout << v[i];
        if (i + 1 < (int)v.size()) cout << ", ";
    }
    cout << "]";
}

void test(const string& name, int n, const vector<vector<int>>& adj, int src, const vector<int>& expected) {
    cout << name << "\n";
    cout << "Source = " << src << "\n";

    auto dist = bfsShortestPaths(n, adj, src);

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
    // 0 - 1 - 2 - 4
    //     |
    //     3
    //
    // From source 0:
    // dist = [0,1,2,2,3]

    {
        int n = 5;
        vector<vector<int>> adj(n);
        auto add = [&](int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        };

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
    // 0 - 1     2     3 - 4
    //
    // From 0:
    // dist = [0,1,-1,-1,-1]

    {
        int n = 5;
        vector<vector<int>> adj(n);
        auto add = [&](int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        };

        add(0,1);
        add(3,4);

        test("Test 2: Disconnected graph", n, adj, 0, {0,1,-1,-1,-1});
    }

    // ------------------------------
    // Test 3: Single node graph
    // ------------------------------
    //
    // n=1, no edges
    // dist = [0]

    {
        int n = 1;
        vector<vector<int>> adj(1);  // no edges
        test("Test 3: Single node", n, adj, 0, {0});
    }

    return 0;
}
