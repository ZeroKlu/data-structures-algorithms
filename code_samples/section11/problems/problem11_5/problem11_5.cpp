#include <iostream>
#include <vector>
using namespace std;

struct DSU {
    vector<int> parent;
    vector<int> rnk;
    int count;

    DSU(int n) : parent(n), rnk(n, 0), count(n) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;

        if (rnk[ra] < rnk[rb]) {
            parent[ra] = rb;
        } else if (rnk[ra] > rnk[rb]) {
            parent[rb] = ra;
        } else {
            parent[rb] = ra;
            rnk[ra]++;
        }
        count--;
    }
};

int countComponents(int n, const vector<pair<int,int>>& edges) {
    DSU d(n);
    for (auto &e : edges)
        d.unite(e.first, e.second);
    return d.count;
}

// ==================================
// Test Helper
// ==================================
void runTest(const string& name, int n, const vector<pair<int,int>>& edges, int expected) {
    int result = countComponents(n, edges);
    cout << name << "\n";
    cout << "n = " << n << ", edges = { ";
    for (auto& e : edges) cout << "(" << e.first << "," << e.second << ") ";
    cout << "}\n";
    cout << "Connected components = " << result
         << " (expected " << expected << ")\n\n";
}

// ==================================
// Main test harness
// ==================================
int main() {

    // Test 1: Single connected component (chain)
    {
        int n = 4;
        vector<pair<int,int>> edges = {
            {0,1}, {1,2}, {2,3}
        };
        runTest("Test 1: Single component chain", n, edges, 1);
    }

    // Test 2: Two components + isolated node
    {
        int n = 6;
        vector<pair<int,int>> edges = {
            {0,1}, {1,2},  // component 1
            {3,4}          // component 2
            // 5 isolated → component 3
        };
        runTest("Test 2: Two components plus isolated", n, edges, 3);
    }

    // Test 3: No edges
    {
        int n = 5;
        vector<pair<int,int>> edges; // empty
        runTest("Test 3: No edges (all isolated)", n, edges, 5);
    }

    // Test 4: Fully connected via redundant edges
    {
        int n = 5;
        vector<pair<int,int>> edges = {
            {0,1}, {1,2}, {2,3}, {3,4}, {1,3}
        };
        runTest("Test 4: Fully connected with redundant edges", n, edges, 1);
    }

    return 0;
}
