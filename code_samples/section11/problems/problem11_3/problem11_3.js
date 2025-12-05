function topoSortKahn(n, adj) {
    const indeg = new Array(n).fill(0);
    for (let u = 0; u < n; u++) {
        for (const v of adj[u]) indeg[v]++;
    }
    const q = [];
    let head = 0;
    for (let u = 0; u < n; u++) {
        if (indeg[u] === 0) q.push(u);
    }
    const order = [];
    while (head < q.length) {
        const u = q[head++];
        order.push(u);
        for (const v of adj[u]) {
            indeg[v]--;
            if (indeg[v] === 0) q.push(v);
        }
    }
    if (order.length !== n) return []; // cycle
    return order;
}

// ===== Helper functions =====

function makeGraph(n) {
    return Array.from({ length: n }, () => []);
}

function add(adj, u, v) {
    adj[u].push(v); // directed edge u → v
}

function runTest(name, n, adj) {
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

    const n = 6;
    const adj = makeGraph(n);

    // DAG:
    // 5 → 2, 5 → 0
    // 4 → 0, 4 → 1
    // 2 → 3
    // 3 → 1
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

    const n = 4;
    const adj = makeGraph(n);

    // 0 → 1, 0 → 2
    // 1 → 3
    // 2 → 3
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

    const n = 3;
    const adj = makeGraph(n);

    // Cycle: 0 → 1 → 2 → 0
    add(adj, 0, 1);
    add(adj, 1, 2);
    add(adj, 2, 0);

    runTest("Cyclic graph", n, adj);
}
