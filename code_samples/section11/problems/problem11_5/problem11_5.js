class DSU {
    /*
     * DSU (Disjoint Set Union) / Union-Find
     *
     * Purpose:
     *   Maintain disjoint sets over elements {0..n-1} while supporting:
     *     - find(x):    get the representative (root) of x's set
     *     - unite(a,b): merge the sets containing a and b
     *
     * Key ideas:
     *   - parent[] forms a forest of trees; each set is one tree.
     *   - rank[] is a heuristic to keep trees shallow (union by rank).
     *   - path compression in find() flattens trees for faster future queries.
     *
     * Extra convenience:
     *   - count tracks how many disjoint sets currently exist.
     *     This makes "number of connected components" easy to compute.
     */
    constructor(n) {
        /*
         * Initialize n singleton sets:
         *   parent[i] = i means every node starts as its own root.
         *   rank[i] = 0 because all trees are size/height 1 initially.
         *   count = n because there are n separate components at the start.
         */
        this.parent = Array.from({ length: n }, (_, i) => i);
        this.rank = new Array(n).fill(0);
        this.count = n;
    }

    find(x) {
        /*
         * Find the root representative of x's set.
         *
         * Path compression:
         *   If x is not a root, recursively find the root and rewrite parent[x]
         *   to point directly to it. This drastically speeds up future finds.
         */
        if (this.parent[x] !== x) {
            this.parent[x] = this.find(this.parent[x]);
        }
        return this.parent[x];
    }

    unite(a, b) {
        /*
         * Union/merge the sets that contain a and b.
         *
         * Steps:
         *   1) Find roots ra and rb.
         *   2) If they match, already in same set → nothing to do.
         *   3) Otherwise, attach the lower-rank root under the higher-rank root.
         *   4) If ranks are equal, pick one as new root and increment its rank.
         *   5) Decrement count exactly once for a successful merge (one fewer set).
         */
        let ra = this.find(a), rb = this.find(b);
        if (ra === rb) return;

        if (this.rank[ra] < this.rank[rb]) {
            this.parent[ra] = rb;
        } else if (this.rank[ra] > this.rank[rb]) {
            this.parent[rb] = ra;
        } else {
            this.parent[rb] = ra;
            this.rank[ra]++;
        }
        this.count--;
    }
}

// edges: array of [u, v]
function countComponents(n, edges) {
    /*
     * Count connected components in an undirected graph.
     *
     * Parameters:
     *   n     : number of nodes labeled 0..n-1
     *   edges : list of undirected edges, each as [u, v]
     *
     * Approach (DSU):
     *   - Start with n components (each node alone).
     *   - For each edge (u, v), union their sets (connect their components).
     *   - At the end, DSU.count equals the number of connected components.
     *
     * Complexity:
     *   Roughly O((n + m) α(n)) where m = edges.length and α is very small.
     */
    const d = new DSU(n);
    for (const [u, v] of edges) {
        d.unite(u, v);
    }
    return d.count;
}

// ===============================
// Test Harness for countComponents
// ===============================

function runTest(name, n, edges, expected) {
    /*
     * Runs a single test case and prints:
     *   - the test name
     *   - input n and edge list
     *   - computed number of connected components
     *   - expected answer for verification
     */
    const result = countComponents(n, edges);
    console.log(name);
    console.log("n =", n, "edges =", edges);
    console.log("Connected components =", result, "(expected", expected + ")");
    console.log();
}

// ===============================
// Test 1: Single connected chain (0-1-2-3)
// ===============================
{
    /*
     * Graph:
     *   0 - 1 - 2 - 3
     * All nodes connected → exactly 1 component.
     */
    const n = 4;
    const edges = [
        [0, 1],
        [1, 2],
        [2, 3]
    ];
    runTest("Test 1: Single connected component (chain)", n, edges, 1);
}

// ===============================
// Test 2: Two components + isolated node
// ===============================
{
    /*
     * Graph:
     *   Component A: 0 - 1 - 2
     *   Component B: 3 - 4
     *   Node 5 isolated
     * Total components: 3
     */
    // Component A: 0-1-2
    // Component B: 3-4
    // Node 5 isolated → own component
    const n = 6;
    const edges = [
        [0, 1],
        [1, 2],
        [3, 4]
    ];
    runTest("Test 2: Two components plus isolated node", n, edges, 3);
}

// ===============================
// Test 3: No edges at all
// ===============================
{
    /*
     * Graph:
     *   n = 5, edges = []
     * Every node is isolated → 5 components.
     */
    const n = 5;
    const edges = [];
    runTest("Test 3: No edges (all isolated)", n, edges, 5);
}

// ===============================
// Test 4: Fully connected (redundant edges)
// ===============================
{
    /*
     * Graph:
     *   0 - 1 - 2 - 3 - 4 plus an extra edge 1 - 3
     * Still all nodes are connected → 1 component.
     * The edge [1,3] is redundant (it adds an alternate route / cycle).
     */
    // All 5 nodes should collapse into one component
    const n = 5;
    const edges = [
        [0, 1],
        [1, 2],
        [2, 3],
        [3, 4],
        [1, 3] // redundant
    ];
    runTest("Test 4: Fully connected with redundant edges", n, edges, 1);
}
