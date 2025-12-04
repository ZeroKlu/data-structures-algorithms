function buildGraph(n, edges) {
    const graph = Array.from({ length: n }, () => []);
    for (const [u, v] of edges) {
        graph[u].push(v);
        graph[v].push(u);
    }
    return graph;
}

function bfsShortest(graph, src) {
    const n = graph.length;
    const INF = Number.MAX_SAFE_INTEGER;
    const dist = new Array(n).fill(INF);
    const q = [];

    dist[src] = 0;
    q.push(src);

    let head = 0;
    while (head < q.length) {
        const u = q[head++];
        for (const v of graph[u]) {
            if (dist[v] === INF) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

function dfsVisit(u, graph, visited) {
    visited[u] = true;
    for (const v of graph[u]) {
        if (!visited[v]) {
            dfsVisit(v, graph, visited);
        }
    }
}

function countComponents(graph) {
    const n = graph.length;
    const visited = new Array(n).fill(false);
    let components = 0;

    for (let u = 0; u < n; u++) {
        if (!visited[u]) {
            components++;
            dfsVisit(u, graph, visited);
        }
    }
    return components;
}

function topoSort(graph) {
    const n = graph.length;
    const indeg = new Array(n).fill(0);
    for (let u = 0; u < n; u++) {
        for (const v of graph[u]) {
            indeg[v]++;
        }
    }

    const q = [];
    for (let i = 0; i < n; i++) {
        if (indeg[i] === 0) q.push(i);
    }

    const order = [];
    let head = 0;
    while (head < q.length) {
        const u = q[head++];
        order.push(u);
        for (const v of graph[u]) {
            indeg[v]--;
            if (indeg[v] === 0) {
                q.push(v);
            }
        }
    }
    // if order.length < n, there was a cycle
    return order;
}

function dijkstraSimple(graph, src) {
    const n = graph.length;
    const INF = Number.MAX_SAFE_INTEGER;
    const dist = new Array(n).fill(INF);
    const used = new Array(n).fill(false);

    dist[src] = 0;

    for (let iter = 0; iter < n; iter++) {
        let u = -1;
        let best = INF;
        for (let i = 0; i < n; i++) {
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if (u === -1) break;
        used[u] = true;
        for (const [v, w] of graph[u]) {
            const nd = dist[u] + w;
            if (nd < dist[v]) {
                dist[v] = nd;
            }
        }
    }
    return dist;
}

class DSU {
    constructor(n) {
        this.parent = Array.from({ length: n }, (_, i) => i);
        this.rank = new Array(n).fill(0);
    }

    find(x) {
        if (this.parent[x] !== x) {
            this.parent[x] = this.find(this.parent[x]);
        }
        return this.parent[x];
    }

    union(a, b) {
        let ra = this.find(a);
        let rb = this.find(b);
        if (ra === rb) return;
        if (this.rank[ra] < this.rank[rb]) {
            this.parent[ra] = rb;
        } else if (this.rank[ra] > this.rank[rb]) {
            this.parent[rb] = ra;
        } else {
            this.parent[rb] = ra;
            this.rank[ra]++;
        }
    }
}

//
// ===========================
// Test Harness
// ===========================
//

function testDSU() {
    console.log("=== Test 1: DSU ===");
    const d = new DSU(5);

    d.union(0, 1);
    d.union(1, 2);
    d.union(3, 4);

    const repr = [];
    for (let i = 0; i < 5; i++) repr.push(d.find(i));
    console.log("Representatives:", repr);

    const distinct = new Set(repr);
    console.log("Number of sets =", distinct.size, "(expected 2)\n");
}

function testBFSAndComponents() {
    console.log("=== Test 2: BFS shortest + components ===");

    // Graph:
    // 0 - 1 - 2 - 4
    //     |
    //     3
    const edges = [
        [0, 1],
        [1, 2],
        [1, 3],
        [2, 4]
    ];
    const g = buildGraph(5, edges);

    const dist = bfsShortest(g, 0);
    console.log("Distances from 0:", dist);
    console.log("Expected: [0,1,2,2,3]");

    const comps = countComponents(g);
    console.log("Connected components =", comps, "(expected 1)\n");
}

function testDisconnectedComponents() {
    console.log("=== Test 3: Disconnected components ===");

    // 0 - 1 - 2    3 - 4     5
    const edges = [
        [0, 1],
        [1, 2],
        [3, 4]
    ];
    const g = buildGraph(6, edges);

    const comps = countComponents(g);
    console.log("Components =", comps, "(expected 3)\n");
}

function testTopoSort() {
    console.log("=== Test 4: Topological Sort (DAG) ===");

    // DAG (directed):
    // 5 → 2, 5 → 0
    // 4 → 0, 4 → 1
    // 2 → 3
    // 3 → 1

    const n = 6;
    const dag = Array.from({ length: n }, () => []);

    const add = (u, v) => dag[u].push(v);

    add(5, 2);
    add(5, 0);
    add(4, 0);
    add(4, 1);
    add(2, 3);
    add(3, 1);

    const order = topoSort(dag);
    console.log("Topological order:", order);
    console.log("(if size < 6, graph has a cycle)\n");
}

function testDijkstra() {
    console.log("=== Test 5: Dijkstra simple ===");

    // Weighted directed graph:
    // 0→1(10), 0→3(5)
    // 1→2(1), 1→3(2)
    // 2→4(4)
    // 3→1(3), 3→2(9), 3→4(2)
    // 4→0(7), 4→2(6)

    const n = 5;
    const g = Array.from({ length: n }, () => []);

    const add = (u, v, w) => g[u].push([v, w]);

    add(0, 1, 10);
    add(0, 3, 5);

    add(1, 2, 1);
    add(1, 3, 2);

    add(2, 4, 4);

    add(3, 1, 3);
    add(3, 2, 9);
    add(3, 4, 2);

    add(4, 0, 7);
    add(4, 2, 6);

    const dist = dijkstraSimple(g, 0);
    console.log("Distances from 0:", dist);
    console.log("Expected: [0,8,9,5,7]\n");
}


// ===========================
// Run all tests
// ===========================

testDSU();
testBFSAndComponents();
testDisconnectedComponents();
testTopoSort();
testDijkstra();
