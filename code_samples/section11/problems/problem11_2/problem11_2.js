function dfsComp(u, cid, adj, comp) {
    // Depth-First Search (DFS) helper that labels all nodes reachable
    // from node `u` with the connected component id `cid`.
    //
    // Parameters:
    //   u    : current node being visited
    //   cid  : component id assigned to this connected component
    //   adj  : adjacency list of an UNDIRECTED graph
    //   comp : array where comp[x] == -1 means "unvisited",
    //          otherwise comp[x] stores the component id of node x

    // Mark the current node as belonging to component `cid`
    comp[u] = cid;

    // Visit all neighbors of u
    for (const v of adj[u]) {
        // If the neighbor has not yet been assigned a component,
        // it belongs to the same connected component
        if (comp[v] === -1) {
            dfsComp(v, cid, adj, comp);
        }
    }
}

function connectedComponents(n, adj) {
    // Computes connected components of an UNDIRECTED graph.
    //
    // Parameters:
    //   n   : number of nodes (assumed labeled 0..n-1)
    //   adj : adjacency list
    //
    // Returns:
    //   An object containing:
    //     - count : total number of connected components
    //     - comp  : array where comp[u] is the component id of node u

    // Initialize all nodes as unvisited
    const comp = new Array(n).fill(-1);

    // Component id counter
    let cid = 0;

    // Scan all nodes
    for (let u = 0; u < n; u++) {
        // If u has not been visited yet, it starts a new component
        if (comp[u] === -1) {
            dfsComp(u, cid, adj, comp);
            cid++;
        }
    }

    return { count: cid, comp };
}

// ===== Helpers for testing =====

function makeGraph(n) {
    // Creates an empty adjacency list for a graph with n nodes.
    //
    // Returns:
    //   adj : array of n empty arrays, one per node
    const adj = new Array(n);
    for (let i = 0; i < n; i++) adj[i] = [];
    return adj;
}

function addEdge(adj, u, v) {
    // Adds an UNDIRECTED edge between nodes u and v.
    //
    // Behavior:
    //   - Adds v to u's adjacency list
    //   - Adds u to v's adjacency list
    adj[u].push(v);
    adj[v].push(u);
}

function runTest(name, n, adj) {
    // Runs the connected components algorithm and prints results.
    //
    // Parameters:
    //   name : descriptive test name
    //   n    : number of nodes
    //   adj  : adjacency list

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
    // Complete graph (clique):
    // Every node is connected to every other node.
    // Expected number of components: 1
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
    // Graph structure:
    //   - component 0: 0-1-2
    //   - component 1: 3-4
    //   - component 2: {5} isolated
    // Expected number of components: 3
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
    // No edges at all.
    // Each node is its own connected component.
    // Expected number of components: n
    const n = 4;
    const adj = makeGraph(n);

    runTest("=== Test 3: All nodes isolated ===", n, adj);
}

// ============================
// Test 4: Mixed graph
// ============================

{
    // Graph structure:
    //   - component 0: 0-1-2
    //   - component 1: 3-4-5
    //   - component 2: {6} isolated
    // Expected number of components: 3
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
