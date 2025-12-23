#include <iostream>
#include <vector>
using namespace std;

/*
 * DSU (Disjoint Set Union) / Union-Find
 *
 * Purpose:
 *   Maintain a partition of {0..n-1} into disjoint sets supporting:
 *     - find(x):    return a representative (root) for x's set
 *     - unite(a,b): merge the sets containing a and b
 *
 * Heuristics used:
 *   - Path compression in find(): flattens trees to speed up future queries
 *   - Union by rank in unite(): attaches smaller-rank tree under larger-rank tree
 *
 * Extra field:
 *   - count: tracks the current number of connected components (disjoint sets).
 *            Starts at n and decrements on each successful merge.
 */
struct DSU {
    vector<int> parent; // parent[i] is i's parent; roots have parent[i] == i
    vector<int> rnk;    // rank heuristic (approx. tree height)
    int count;          // number of disjoint sets currently present

    DSU(int n) : parent(n), rnk(n, 0), count(n) {
        /*
         * Initialize DSU with n singleton sets:
         *   - each node is its own parent (root)
         *   - all ranks start at 0
         *   - component count starts at n
         */
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        /*
         * Find the representative (root) of x's set.
         *
         * Path compression:
         *   If x is not a root, recursively find the root and rewrite parent[x]
         *   to point directly to that root. This greatly improves performance.
         */
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        /*
         * Merge the sets containing a and b.
         *
         * Steps:
         *   1) Find roots ra and rb.
         *   2) If they are already equal, a and b are already connected; do nothing.
         *   3) Otherwise, attach the lower-rank root under the higher-rank root.
         *      If ranks are equal, choose one as new root and increment its rank.
         *   4) Decrement count because two components merged into one.
         */
        int ra = find(a), rb = find(b);
        if (ra == rb) return; // already in the same set; component count unchanged

        if (rnk[ra] < rnk[rb]) {
            parent[ra] = rb;  // attach ra under rb
        } else if (rnk[ra] > rnk[rb]) {
            parent[rb] = ra;  // attach rb under ra
        } else {
            // equal ranks: pick ra as root and bump its rank
            parent[rb] = ra;
            rnk[ra]++;
        }
        count--; // successful union reduces number of components by 1
    }
};

int countComponents(int n, const vector<pair<int,int>>& edges) {
    /*
     * Counts connected components in an undirected graph on nodes 0..n-1,
     * using DSU.
     *
     * Parameters:
     *   n     : number of nodes
     *   edges : list of undirected edges (u, v)
     *
     * Approach:
     *   - Start with n singleton components.
     *   - For each edge (u, v), union(u, v).
     *   - The DSU's 'count' ends up as the number of components.
     *
     * Assumptions:
     *   - Node indices in edges are valid: 0 <= u,v < n.
     */
    DSU d(n);
    for (auto &e : edges)
        d.unite(e.first, e.second);
    return d.count;
}

// ==================================
// Test Helper
// ==================================
void runTest(const string& name, int n, const vector<pair<int,int>>& edges, int expected) {
    /*
     * Runs a single test case:
     *   - Prints the test name and the edge list
     *   - Computes component count via countComponents()
     *   - Prints the result alongside an expected value
     */
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
        /*
         * Graph: 0--1--2--3
         * All nodes are connected in one component.
         * Expected components: 1
         */
        int n = 4;
        vector<pair<int,int>> edges = {
            {0,1}, {1,2}, {2,3}
        };
        runTest("Test 1: Single component chain", n, edges, 1);
    }

    // Test 2: Two components + isolated node
    {
        /*
         * Graph:
         *   Component A: 0--1--2
         *   Component B: 3--4
         *   Node 5 is isolated
         * Expected components: 3
         */
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
        /*
         * Graph: n=5, no edges.
         * Every node is isolated, so each is its own component.
         * Expected components: 5
         */
        int n = 5;
        vector<pair<int,int>> edges; // empty
        runTest("Test 3: No edges (all isolated)", n, edges, 5);
    }

    // Test 4: Fully connected via redundant edges
    {
        /*
         * Graph:
         *   0--1--2--3--4 plus an extra edge 1--3
         * Still a single connected component. Extra edges are redundant.
         * Expected components: 1
         */
        int n = 5;
        vector<pair<int,int>> edges = {
            {0,1}, {1,2}, {2,3}, {3,4}, {1,3}
        };
        runTest("Test 4: Fully connected with redundant edges", n, edges, 1);
    }

    return 0;
}
