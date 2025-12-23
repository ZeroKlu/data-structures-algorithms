// adj: adjacency list as array of arrays, nodes 0..n-1
function bfsShortestPaths(n, adj, s) {
    // Computes shortest path distances from a single source in an UNWEIGHTED graph
    // using Breadth-First Search (BFS).
    //
    // Parameters:
    //   n   : number of nodes (assumed labeled 0..n-1)
    //   adj : adjacency list; adj[u] is an array of neighbors of node u
    //   s   : source node index
    //
    // Returns:
    //   dist : array of length n where:
    //          - dist[v] is the minimum number of edges from s to v
    //          - dist[v] is -1 if v is unreachable from s
    //
    // Implementation detail:
    //   - Uses a plain array as a queue plus a moving head index so we avoid O(n)
    //     shift() operations.
    const dist = new Array(n).fill(-1); // -1 marks "unvisited/unreachable"
    const q = [];                       // queue storage
    let head = 0;                       // index of next element to dequeue

    // Initialize BFS from the source.
    dist[s] = 0;
    q.push(s);

    // Process the queue in FIFO order.
    while (head < q.length) {
        const u = q[head++];            // dequeue next node to expand

        // Explore all neighbors of u.
        for (const v of adj[u]) {
            // If v has not yet been discovered, record its distance and enqueue it.
            // In an unweighted graph, first discovery gives the shortest distance.
            if (dist[v] === -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist;
}

function bfsShortestPaths(n, adj, s) {
    // NOTE: This is a duplicate definition of bfsShortestPaths.
    //
    // In JavaScript, the second function with the same name overrides/replaces
    // the earlier one in the same scope. That means *this* version is the one
    // that will actually be used by the test harness below.
    //
    // The logic is identical to the first definition: BFS for unweighted shortest paths.
    const dist = new Array(n).fill(-1);
    const q = [];
    let head = 0;

    dist[s] = 0;
    q.push(s);

    while (head < q.length) {
        const u = q[head++];
        for (const v of adj[u]) {
            if (dist[v] === -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

// ====================================
// Helpers
// ====================================

function makeGraph(n) {
    // Allocates an empty adjacency list for a graph with n nodes.
    //
    // Returns:
    //   adj : array of length n, where each adj[i] is an empty array of neighbors.
    return Array.from({ length: n }, () => []);
}

function addEdge(adj, u, v) {
    // Adds an UNDIRECTED edge between u and v in an adjacency list.
    //
    // Parameters:
    //   adj : adjacency list
    //   u,v : endpoints
    //
    // Behavior:
    //   - Adds v to u's neighbor list and u to v's neighbor list.
    //   - No bounds checking is performed.
    adj[u].push(v);
    adj[v].push(u);
}

function runTest(name, n, adj, src, expected) {
    // Runs bfsShortestPaths on a test graph and prints:
    //   - test name
    //   - source node
    //   - computed distances
    //   - expected distances
    //
    // This is a "print and compare" harness (no assertions).
    console.log(name);
    console.log(`Source = ${src}`);

    const dist = bfsShortestPaths(n, adj, src);

    console.log("Distances:", JSON.stringify(dist));
    console.log("Expected :", JSON.stringify(expected));
    console.log();
}

// ====================================
// Test Cases
// ====================================

function testConnected() {
    // Graph:
    // 0 - 1 - 2 - 4
    //     |
    //     3
    //
    // From 0 → distances: [0,1,2,2,3]
    const n = 5;
    const adj = makeGraph(n);

    // Add undirected edges to match the diagram.
    addEdge(adj, 0, 1);
    addEdge(adj, 1, 2);
    addEdge(adj, 1, 3);
    addEdge(adj, 2, 4);

    runTest("=== Test 1: Connected Graph ===", n, adj, 0, [0, 1, 2, 2, 3]);
}

function testDisconnected() {
    // Graph:
    // 0 - 1    2    3 - 4
    //
    // From 0 → distances: [0,1,-1,-1,-1]
    const n = 5;
    const adj = makeGraph(n);

    // Two separate components {0,1} and {3,4}; node 2 is isolated.
    addEdge(adj, 0, 1);
    addEdge(adj, 3, 4);

    runTest("=== Test 2: Disconnected Graph ===", n, adj, 0, [0, 1, -1, -1, -1]);
}

function testSingleNode() {
    // Graph:
    // n = 1, no edges
    //
    // From 0 → [0]
    const n = 1;
    const adj = makeGraph(n); // adjacency list with one empty neighbor list

    runTest("=== Test 3: Single Node Graph ===", n, adj, 0, [0]);
}

// ====================================
// Run Tests
// ====================================

// Execute tests in order.
testConnected();
testDisconnected();
testSingleNode();
