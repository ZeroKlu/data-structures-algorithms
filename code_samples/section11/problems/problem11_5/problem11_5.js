class DSU {
    constructor(n) {
        this.parent = Array.from({ length: n }, (_, i) => i);
        this.rank = new Array(n).fill(0);
        this.count = n;
    }
    find(x) {
        if (this.parent[x] !== x) {
            this.parent[x] = this.find(this.parent[x]);
        }
        return this.parent[x];
    }
    unite(a, b) {
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
    const n = 5;
    const edges = [];
    runTest("Test 3: No edges (all isolated)", n, edges, 5);
}

// ===============================
// Test 4: Fully connected (redundant edges)
// ===============================
{
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
