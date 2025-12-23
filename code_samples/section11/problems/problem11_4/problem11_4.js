// Simple binary-heap-like priority queue can be imported or implemented.
// Here we assume a minimal interface:
//   pq.push({ dist, node });
//   const item = pq.pop(); // smallest dist
// For brevity, use a naive O(n) "priority queue" for teaching purposes.
//
// NOTE ABOUT THIS FILE:
// - The first dijkstra() definition below is immediately followed by a second
//   dijkstra() definition with the same name. In JavaScript, the latter
//   overwrites the former, so only the SECOND implementation is actually used
//   by the tests at the bottom.

function dijkstra(n, adj, s) {
    /*
     * Dijkstra's algorithm (naive O(n^2) implementation).
     *
     * Parameters:
     *   n   : number of nodes (0..n-1)
     *   adj : adjacency list where adj[u] is an array of [v, w] pairs
     *         representing directed edges u -> v with weight w
     *   s   : source node
     *
     * Returns:
     *   dist : array where dist[v] is the shortest distance from s to v,
     *          or INF if v is unreachable.
     *
     * Correctness requirements:
     *   - All edge weights w must be >= 0 for Dijkstra to be correct.
     *
     * Performance:
     *   - O(n^2 + m) time due to scanning all nodes to find the next closest
     *     unused node on each iteration.
     *   - Good for teaching and small graphs; for large graphs use a real
     *     priority queue (binary heap) for O((n + m) log n).
     */
    const INF = 1e15;                // "infinity" sentinel for unreachable nodes
    const dist = new Array(n).fill(INF);
    dist[s] = 0;

    // used[u] indicates whether node u has been "finalized" (its shortest
    // distance is known and won't change).
    const used = new Array(n).fill(false);

    // Repeat up to n times: each iteration finalizes one new node (if reachable).
    for (let it = 0; it < n; it++) {
        // Pick the unused node with the smallest current dist[] (O(n) scan).
        let u = -1;
        let best = INF;
        for (let i = 0; i < n; i++) {
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }

        // If no unused reachable node remains, we're done early.
        if (u === -1) break;

        // Finalize u: dist[u] is now the shortest possible distance from s to u.
        used[u] = true;

        // Relax all outgoing edges u -> v.
        for (const [v, w] of adj[u]) {
            // Candidate distance to v via u.
            const nd = dist[u] + w;

            // If we found a better path to v, update dist[v].
            if (nd < dist[v]) dist[v] = nd;
        }
    }

    return dist;
}

// ===============================
// Dijkstra Implementation (Given)
// ===============================
//
// The following function has the same name/signature as the one above.
// In JavaScript, this re-declaration overwrites the earlier definition.
// That means: the tests will call THIS version (the second one).

// Naive O(n^2) version using "used" array
function dijkstra(n, adj, s) {
    /*
     * Same algorithm as above, repeated as the "given" implementation.
     * Because it has the same name, it replaces the previous dijkstra().
     */
    const INF = 1e15;
    const dist = new Array(n).fill(INF);
    dist[s] = 0;

    const used = new Array(n).fill(false);

    for (let it = 0; it < n; it++) {
        // Select next node u with minimal dist among unused nodes (O(n)).
        let u = -1;
        let best = INF;
        for (let i = 0; i < n; i++) {
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }

        // If no reachable unused nodes remain, stop.
        if (u === -1) break;

        // Mark u finalized.
        used[u] = true;

        // Relax edges out of u.
        for (const [v, w] of adj[u]) {
            const nd = dist[u] + w;
            if (nd < dist[v]) dist[v] = nd;
        }
    }

    return dist;
}

// ===============================
// Helper Functions
// ===============================

function makeGraph(n) {
    /*
     * Creates an empty directed weighted graph with n nodes.
     *
     * Returns:
     *   adj : adjacency list where adj[u] is an array of [v, w] edges.
     */
    return Array.from({ length: n }, () => []);
}

function addEdge(adj, u, v, w) {
    /*
     * Adds a DIRECTED edge u -> v with weight w.
     *
     * Parameters:
     *   adj : adjacency list (mutated in-place)
     *   u   : source node
     *   v   : destination node
     *   w   : non-negative edge weight (required for Dijkstra correctness)
     *
     * Note:
     *   For an undirected graph, add both directions:
     *     addEdge(adj, u, v, w); addEdge(adj, v, u, w);
     */
    adj[u].push([v, w]);   // directed edge u -> v
}

function printDist(dist) {
    /*
     * Prints the distance array as a list.
     *
     * Convention:
     *   - Any distance that is still very large is printed as "INF"
     *     to indicate the node is unreachable from the chosen source.
     */
    console.log(
        "[" +
            dist
                .map(d => (d >= 1e14 ? "INF" : d))
                .join(", ") +
        "]"
    );
}

function runTest(name, n, adj, src) {
    /*
     * Runs Dijkstra from `src` on the given graph and prints the results.
     *
     * Parameters:
     *   name : test label
     *   n    : number of nodes
     *   adj  : adjacency list
     *   src  : source node
     */
    console.log(name);
    console.log("Source =", src);

    const dist = dijkstra(n, adj, src);

    // process.stdout.write used here to keep "Distances:" on same line as the array.
    process.stdout.write("Distances: ");
    printDist(dist);
    console.log();
}

// ===============================
// Test 1: Simple Weighted Graph
// ===============================

console.log("=== Test 1: Simple Weighted Graph ===");

{
    // Graph:
    // 0→1(2), 0→2(5)
    // 1→2(1)
    // 1→3(3)
    // 2→3(1)
    // 3→4(2)
    //
    // Purpose:
    // - Verifies Dijkstra chooses cheaper multi-hop paths over expensive direct paths.
    const n = 5;
    const adj = makeGraph(n);

    addEdge(adj, 0, 1, 2);
    addEdge(adj, 0, 2, 5);
    addEdge(adj, 1, 2, 1);
    addEdge(adj, 1, 3, 3);
    addEdge(adj, 2, 3, 1);
    addEdge(adj, 3, 4, 2);

    runTest("Simple weighted graph", n, adj, 0);
}

// ===============================
// Test 2: Multiple Shortest Paths
// ===============================

console.log("=== Test 2: Multiple Shortest Paths ===");

{
    // Graph:
    // 0→1(1), 0→2(1)
    // 1→3(2)
    // 2→3(2)
    // 3→4(1)
    // 4→5(5)
    //
    // Purpose:
    // - Confirms correctness when multiple equal-cost shortest paths exist.
    const n = 6;
    const adj = makeGraph(n);

    addEdge(adj, 0, 1, 1);
    addEdge(adj, 0, 2, 1);
    addEdge(adj, 1, 3, 2);
    addEdge(adj, 2, 3, 2);
    addEdge(adj, 3, 4, 1);
    addEdge(adj, 4, 5, 5);

    runTest("Branches with equal-cost paths", n, adj, 0);
}

// ===============================
// Test 3: Disconnected Graph
// ===============================

console.log("=== Test 3: Disconnected Graph ===");

{
    // Graph:
    // Component reachable from 0:
    //   0→1(4), 1→2(6)
    // Nodes 3 and 4 are unreachable from 0 and should print as "INF".
    const n = 5;
    const adj = makeGraph(n);

    addEdge(adj, 0, 1, 4);
    addEdge(adj, 1, 2, 6);

    // nodes 3 and 4 unreachable

    runTest("Disconnected graph", n, adj, 0);
}
