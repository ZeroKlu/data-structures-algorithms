/*
 * This file contains a small collection of graph utilities and classic algorithms,
 * plus a simple console-based test harness to demonstrate correctness.
 *
 * Included:
 *   - buildGraph: adjacency list builder for undirected, unweighted graphs
 *   - bfsShortest: BFS shortest path distances in unweighted graphs
 *   - dfsVisit + countComponents: DFS-based connected component counting (undirected)
 *   - topoSort: Kahn's algorithm topological sorting (directed graphs)
 *   - dijkstraSimple: O(n^2) Dijkstra (weighted directed graphs, non-negative weights)
 *   - DSU: Disjoint Set Union / Union-Find with path compression + union by rank
 *   - Test functions for each algorithm + "run all tests" section
 *
 * Notes:
 *   - This is written for clarity over maximum performance.
 *   - There is no argument validation (e.g., node indices assumed in range).
 *   - Some functions assume a specific graph representation (documented below).
 */

function buildGraph(n, edges) {
    // Build an adjacency list for an UNDIRECTED, UNWEIGHTED graph.
    //
    // Parameters:
    //   n     : number of nodes, assumed labeled 0..n-1
    //   edges : array of [u, v] pairs (0-based indices)
    //
    // Returns:
    //   graph : array of length n; graph[u] is an array of neighbors of u
    //
    // Behavior:
    //   - Because the graph is undirected, each edge is added in both directions:
    //       u -> v and v -> u
    //   - Parallel edges and self-loops are allowed as provided.
    const graph = Array.from({ length: n }, () => []);
    for (const [u, v] of edges) {
        graph[u].push(v);
        graph[v].push(u);
    }
    return graph;
}

function bfsShortest(graph, src) {
    // Compute shortest path distances from src in an UNWEIGHTED graph using BFS.
    //
    // Parameters:
    //   graph : adjacency list; graph[u] is an array of neighbors of u
    //   src   : source node index
    //
    // Returns:
    //   dist : array where dist[v] is the minimum number of edges from src to v
    //          or INF if v is unreachable.
    //
    // Implementation details:
    //   - Uses a plain JS array as a queue and a moving "head" index to avoid O(n)
    //     shift() operations.
    const n = graph.length;

    // Use a large integer sentinel for "unreachable/unvisited".
    // Note: In JS, Number is a floating-point type; MAX_SAFE_INTEGER is the largest
    // integer that can be represented exactly.
    const INF = Number.MAX_SAFE_INTEGER;

    const dist = new Array(n).fill(INF);

    // Queue implemented as an array + head pointer.
    const q = [];

    dist[src] = 0;
    q.push(src);

    let head = 0;
    while (head < q.length) {
        const u = q[head++];

        // For each neighbor, if it's unvisited, set distance and enqueue.
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
    // Recursive DFS visit helper.
    //
    // Parameters:
    //   u       : node to visit
    //   graph   : adjacency list
    //   visited : boolean array marking visited nodes; mutated in-place
    //
    // Behavior:
    //   - Marks u visited, then recursively visits all unvisited neighbors.
    //
    // Caution:
    //   - For very large/deep graphs, recursion can exceed the JS call stack.
    visited[u] = true;
    for (const v of graph[u]) {
        if (!visited[v]) {
            dfsVisit(v, graph, visited);
        }
    }
}

function countComponents(graph) {
    // Count connected components in an UNDIRECTED graph using DFS.
    //
    // Parameters:
    //   graph : adjacency list representing an undirected graph
    //
    // Returns:
    //   components : number of connected components
    //
    // How it works:
    //   - Iterate all nodes; if a node is unvisited, it's the start of a new component.
    //   - DFS from that node marks the entire component.
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
    // Topological sort for a DIRECTED graph using Kahn's algorithm.
    //
    // Parameters:
    //   graph : directed adjacency list; graph[u] contains nodes v for edges u -> v
    //
    // Returns:
    //   order : array of nodes in topological order if graph is a DAG
    //           If order.length < n, the graph contains a cycle.
    //
    // Notes:
    //   - Topological sort is not defined for graphs with cycles; Kahn's algorithm
    //     will return a partial ordering when cycles exist.
    const n = graph.length;

    // indeg[v] counts number of incoming edges to v.
    const indeg = new Array(n).fill(0);
    for (let u = 0; u < n; u++) {
        for (const v of graph[u]) {
            indeg[v]++;
        }
    }

    // Queue (array + head index) of all nodes with indegree 0.
    const q = [];
    for (let i = 0; i < n; i++) {
        if (indeg[i] === 0) q.push(i);
    }

    const order = [];
    let head = 0;

    while (head < q.length) {
        const u = q[head++];
        order.push(u);

        // "Remove" u by decrementing indegree of its outgoing neighbors.
        for (const v of graph[u]) {
            indeg[v]--;
            if (indeg[v] === 0) {
                q.push(v);
            }
        }
    }

    // If order.length < n, remaining nodes are part of or reachable only through a cycle.
    return order;
}

function dijkstraSimple(graph, src) {
    // Dijkstra's algorithm (simple O(n^2) version) for shortest paths
    // in a WEIGHTED directed graph with NON-NEGATIVE edge weights.
    //
    // Parameters:
    //   graph : adjacency list where graph[u] is an array of [v, w] pairs
    //           representing directed edges u -> v with weight w
    //   src   : source node index
    //
    // Returns:
    //   dist : array where dist[v] is shortest distance from src to v, or INF if unreachable
    //
    // Implementation details:
    //   - No heap is used; each iteration scans all nodes to find the unused node
    //     with the smallest tentative distance.
    //   - Complexity: O(n^2 + m), which is fine for small graphs.
    //
    // Correctness requirement:
    //   - Edge weights must be >= 0. Negative weights break Dijkstra.
    const n = graph.length;
    const INF = Number.MAX_SAFE_INTEGER;

    const dist = new Array(n).fill(INF);
    const used = new Array(n).fill(false);

    dist[src] = 0;

    for (let iter = 0; iter < n; iter++) {
        // Select the unused node with smallest tentative distance.
        let u = -1;
        let best = INF;

        for (let i = 0; i < n; i++) {
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }

        // If we didn't find any reachable unused node, we're done.
        if (u === -1) break;

        used[u] = true; // finalize u

        // Relax outgoing edges u -> v.
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
    // Disjoint Set Union / Union-Find structure.
    //
    // Fields:
    //   parent[i] : parent pointer; roots satisfy parent[i] === i
    //   rank[i]   : rank heuristic (approx tree height upper bound)
    //
    // Methods:
    //   find(x)  : returns representative/root of x's set (with path compression)
    //   union(a,b): merges sets containing a and b (with union by rank)
    constructor(n) {
        // Initialize n singleton sets.
        this.parent = Array.from({ length: n }, (_, i) => i);
        this.rank = new Array(n).fill(0);
    }

    find(x) {
        // Path-compressed find.
        if (this.parent[x] !== x) {
            this.parent[x] = this.find(this.parent[x]);
        }
        return this.parent[x];
    }

    union(a, b) {
        // Union by rank.
        let ra = this.find(a);
        let rb = this.find(b);

        if (ra === rb) return; // already in the same set

        if (this.rank[ra] < this.rank[rb]) {
            this.parent[ra] = rb;
        } else if (this.rank[ra] > this.rank[rb]) {
            this.parent[rb] = ra;
        } else {
            // Same rank: choose ra as new root and increment its rank.
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
    // Verifies DSU unions create the expected partition.
    console.log("=== Test 1: DSU ===");

    const d = new DSU(5);

    // Unions produce sets: {0,1,2} and {3,4}.
    d.union(0, 1);
    d.union(1, 2);
    d.union(3, 4);

    // Gather representatives; exact representative IDs may vary,
    // but elements in the same set should share the same find() result.
    const repr = [];
    for (let i = 0; i < 5; i++) repr.push(d.find(i));
    console.log("Representatives:", repr);

    // Count distinct representatives to count sets.
    const distinct = new Set(repr);
    console.log("Number of sets =", distinct.size, "(expected 2)\n");
}

function testBFSAndComponents() {
    // Tests BFS shortest paths and connected component counting on a connected graph.
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

    // BFS from 0 should yield distances [0,1,2,2,3].
    const dist = bfsShortest(g, 0);
    console.log("Distances from 0:", dist);
    console.log("Expected: [0,1,2,2,3]");

    // Graph is connected, so components should be 1.
    const comps = countComponents(g);
    console.log("Connected components =", comps, "(expected 1)\n");
}

function testDisconnectedComponents() {
    // Tests component counting on a graph with multiple components.
    console.log("=== Test 3: Disconnected components ===");

    // 0 - 1 - 2    3 - 4     5
    const edges = [
        [0, 1],
        [1, 2],
        [3, 4]
    ];

    const g = buildGraph(6, edges);

    // Expected components: {0,1,2}, {3,4}, {5} => 3 total.
    const comps = countComponents(g);
    console.log("Components =", comps, "(expected 3)\n");
}

function testTopoSort() {
    // Tests topological sorting on a DAG.
    console.log("=== Test 4: Topological Sort (DAG) ===");

    // DAG (directed):
    // 5 → 2, 5 → 0
    // 4 → 0, 4 → 1
    // 2 → 3
    // 3 → 1
    //
    // Note: topoSort expects a DIRECTED adjacency list.
    const n = 6;
    const dag = Array.from({ length: n }, () => []);

    // Helper to add directed edge u -> v.
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
    // Tests O(n^2) Dijkstra on a classic small directed weighted graph.
    console.log("=== Test 5: Dijkstra simple ===");

    // Weighted directed graph adjacency format expected by dijkstraSimple:
    //   g[u] is an array of [v, w] pairs.
    //
    // Graph:
    // 0→1(10), 0→3(5)
    // 1→2(1), 1→3(2)
    // 2→4(4)
    // 3→1(3), 3→2(9), 3→4(2)
    // 4→0(7), 4→2(6)

    const n = 5;
    const g = Array.from({ length: n }, () => []);

    // Helper to add directed weighted edge u -> v with weight w.
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

    // Expected shortest distances from 0: [0, 8, 9, 5, 7]
    const dist = dijkstraSimple(g, 0);
    console.log("Distances from 0:", dist);
    console.log("Expected: [0,8,9,5,7]\n");
}


// ===========================
// Run all tests
// ===========================

// Execute each test in a fixed order.
testDSU();
testBFSAndComponents();
testDisconnectedComponents();
testTopoSort();
testDijkstra();
