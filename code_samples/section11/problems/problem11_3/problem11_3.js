function topoSortKahn(n, adj) {
    // Computes a topological ordering of a DIRECTED graph using Kahn's algorithm.
    //
    // Parameters:
    //   n   : number of nodes (assumed labeled 0..n-1)
    //   adj : adjacency list for a directed graph; edges are u -> v stored in adj[u]
    //
    // Returns:
    //   - An array `order` of length n containing a valid topological order if the graph is a DAG.
    //   - An empty array [] if a cycle is detected (no topological ordering exists).
    //
    // Kahn's algorithm overview:
    //   1) Compute indeg[v] = number of incoming edges to v.
    //   2) Add all nodes with indeg == 0 to a queue.
    //   3) Repeatedly pop a node u, append it to `order`, and decrement indeg of neighbors v.
    //      If any neighbor reaches indeg == 0, add it to the queue.
    //   4) If `order` doesn't contain all nodes, the graph has a cycle.
    const indeg = new Array(n).fill(0);

    // Step 1: Compute indegree of each node by scanning all edges u -> v.
    for (let u = 0; u < n; u++) {
        for (const v of adj[u]) indeg[v]++;
    }

    // Step 2: Initialize the queue with all nodes having indegree 0.
    // This implementation uses an array `q` plus a moving `head` index
    // to avoid O(n) cost of shift().
    const q = [];
    let head = 0;
    for (let u = 0; u < n; u++) {
        if (indeg[u] === 0) q.push(u);
    }

    // Step 3: Process nodes in BFS-like order.
    const order = [];
    while (head < q.length) {
        const u = q[head++];

        // u currently has no remaining prerequisites, so it can come next.
        order.push(u);

        // "Remove" outgoing edges u -> v by decrementing indeg[v].
        for (const v of adj[u]) {
            indeg[v]--;
            if (indeg[v] === 0) q.push(v); // v is now ready to be processed
        }
    }

    // Step 4: If we didn't output all nodes, at least one cycle exists.
    // Nodes in cycles never reach indegree 0 after processing begins.
    if (order.length !== n) return []; // cycle detected

    return order;
}

// ===== Helper functions =====

function makeGraph(n) {
    // Creates an empty adjacency list for a directed graph with n nodes.
    //
    // Returns:
    //   adj : array of n empty arrays, where adj[u] will hold outgoing neighbors of u
    return Array.from({ length: n }, () => []);
}

function add(adj, u, v) {
    // Adds a DIRECTED edge u -> v to the adjacency list.
    //
    // Parameters:
    //   adj : adjacency list
    //   u   : source node
    //   v   : destination node
    adj[u].push(v); // directed edge u → v
}

function runTest(name, n, adj) {
    // Runs topoSortKahn on the given graph and prints results.
    //
    // Parameters:
    //   name : test label
    //   n    : number of nodes
    //   adj  : adjacency list
    //
    // Output convention:
    //   - [] indicates "cycle detected" (no valid topological ordering).
    console.log(name);

    const result = topoSortKahn(n, adj);

    if (result.length === 0) {
        console.log("Topological order: [] (cycle detected)\n");
    } else {
        console.log("Topological order:", JSON.stringify(result));
        console.log();
    }
}

// ============================
// Test 1: Simple DAG
// ============================

{
    console.log("=== Test 1: Simple DAG ===");

    // Classic DAG example:
    // 5 → 2, 5 → 0
    // 4 → 0, 4 → 1
    // 2 → 3
    // 3 → 1
    //
    // Many valid topological orders exist; output may vary based on queue order,
    // but must satisfy all precedence constraints implied by edges.
    const n = 6;
    const adj = makeGraph(n);

    add(adj, 5, 2);
    add(adj, 5, 0);
    add(adj, 4, 0);
    add(adj, 4, 1);
    add(adj, 2, 3);
    add(adj, 3, 1);

    runTest("Simple DAG", n, adj);
}

// ============================
// Test 2: Another DAG
// ============================

{
    console.log("=== Test 2: Another DAG ===");

    // DAG:
    // 0 → 1, 0 → 2
    // 1 → 3
    // 2 → 3
    //
    // Valid topological orders include:
    //   [0, 1, 2, 3] and [0, 2, 1, 3]
    const n = 4;
    const adj = makeGraph(n);

    add(adj, 0, 1);
    add(adj, 0, 2);
    add(adj, 1, 3);
    add(adj, 2, 3);

    runTest("Another DAG", n, adj);
}

// ============================
// Test 3: Graph with a cycle
// ============================

{
    console.log("=== Test 3: Graph with cycle ===");

    // Cycle:
    // 0 → 1 → 2 → 0
    //
    // Cyclic graphs do not have a topological ordering.
    // The algorithm should return [] here.
    const n = 3;
    const adj = makeGraph(n);

    add(adj, 0, 1);
    add(adj, 1, 2);
    add(adj, 2, 0);

    runTest("Cyclic graph", n, adj);
}
