// Simple binary-heap-like priority queue can be imported or implemented.
// Here we assume a minimal interface:
//   pq.push({ dist, node });
//   const item = pq.pop(); // smallest dist
// For brevity, use a naive O(n) "priority queue" for teaching purposes.

function dijkstra(n, adj, s) {
    const INF = 1e15;
    const dist = new Array(n).fill(INF);
    dist[s] = 0;

    const used = new Array(n).fill(false);
    for (let it = 0; it < n; it++) {
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
        for (const [v, w] of adj[u]) {
            const nd = dist[u] + w;
            if (nd < dist[v]) dist[v] = nd;
        }
    }
    return dist;
}

// ===============================
// Dijkstra Implementation (Given)
// ===============================

// Naive O(n^2) version using "used" array
function dijkstra(n, adj, s) {
    const INF = 1e15;
    const dist = new Array(n).fill(INF);
    dist[s] = 0;

    const used = new Array(n).fill(false);

    for (let it = 0; it < n; it++) {
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
    return Array.from({ length: n }, () => []);
}

function addEdge(adj, u, v, w) {
    adj[u].push([v, w]);   // directed edge u -> v
}

function printDist(dist) {
    console.log(
        "[" +
            dist
                .map(d => (d >= 1e14 ? "INF" : d))
                .join(", ") +
        "]"
    );
}

function runTest(name, n, adj, src) {
    console.log(name);
    console.log("Source =", src);
    const dist = dijkstra(n, adj, src);
    process.stdout.write("Distances: ");
    printDist(dist);
    console.log();
}

// ===============================
// Test 1: Simple Weighted Graph
// ===============================

console.log("=== Test 1: Simple Weighted Graph ===");

{
    const n = 5;
    const adj = makeGraph(n);

    // 0→1(2), 0→2(5)
    // 1→2(1)
    // 1→3(3)
    // 2→3(1)
    // 3→4(2)
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
    const n = 6;
    const adj = makeGraph(n);

    // 0→1(1), 0→2(1)
    // 1→3(2)
    // 2→3(2)
    // 3→4(1)
    // 4→5(5)
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
    const n = 5;
    const adj = makeGraph(n);

    // Component 0→1(4), 1→2(6)
    addEdge(adj, 0, 1, 4);
    addEdge(adj, 1, 2, 6);

    // nodes 3 and 4 unreachable

    runTest("Disconnected graph", n, adj, 0);
}
