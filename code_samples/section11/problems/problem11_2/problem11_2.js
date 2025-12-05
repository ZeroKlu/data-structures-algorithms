function dfsComp(u, cid, adj, comp) {
    comp[u] = cid;
    for (const v of adj[u]) {
        if (comp[v] === -1) {
            dfsComp(v, cid, adj, comp);
        }
    }
}

function connectedComponents(n, adj) {
    const comp = new Array(n).fill(-1);
    let cid = 0;
    for (let u = 0; u < n; u++) {
        if (comp[u] === -1) {
            dfsComp(u, cid, adj, comp);
            cid++;
        }
    }
    return { count: cid, comp };
}

// ===== Helpers for testing =====

function makeGraph(n) {
    const adj = new Array(n);
    for (let i = 0; i < n; i++) adj[i] = [];
    return adj;
}

function addEdge(adj, u, v) {
    adj[u].push(v);
    adj[v].push(u);
}

function runTest(name, n, adj) {
    console.log(name);

    const { count, comp } = connectedComponents(n, adj);

    console.log("Component count =", count);
    console.log("Component IDs   =", JSON.stringify(comp));
    console.log();
}

// ============================
// Test 1: Fully connected graph
// ============================

{
    const n = 5;
    const adj = makeGraph(n);

    // Build clique
    for (let u = 0; u < n; u++) {
        for (let v = u + 1; v < n; v++) {
            addEdge(adj, u, v);
        }
    }

    runTest("=== Test 1: Fully connected graph ===", n, adj);
}

// ============================
// Test 2: Two components + isolated
// ============================

{
    const n = 6;
    const adj = makeGraph(n);

    // Component 0: 0-1-2
    addEdge(adj, 0, 1);
    addEdge(adj, 1, 2);

    // Component 1: 3-4
    addEdge(adj, 3, 4);

    // Node 5 is isolated

    runTest("=== Test 2: Two components + isolated ===", n, adj);
}

// ============================
// Test 3: All nodes isolated
// ============================

{
    const n = 4;
    const adj = makeGraph(n);

    runTest("=== Test 3: All nodes isolated ===", n, adj);
}

// ============================
// Test 4: Mixed graph
// ============================

{
    const n = 7;
    const adj = makeGraph(n);

    // Component 0: 0-1-2
    addEdge(adj, 0, 1);
    addEdge(adj, 1, 2);

    // Component 1: 3-4-5
    addEdge(adj, 3, 4);
    addEdge(adj, 4, 5);

    // Node 6 isolated

    runTest("=== Test 4: Mixed graph ===", n, adj);
}
