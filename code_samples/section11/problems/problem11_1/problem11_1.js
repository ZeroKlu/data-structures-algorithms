// adj: adjacency list as array of arrays, nodes 0..n-1
function bfsShortestPaths(n, adj, s) {
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

function bfsShortestPaths(n, adj, s) {
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
    return Array.from({ length: n }, () => []);
}

function addEdge(adj, u, v) {
    adj[u].push(v);
    adj[v].push(u);
}

function runTest(name, n, adj, src, expected) {
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
    addEdge(adj, 0, 1);
    addEdge(adj, 1, 2);
    addEdge(adj, 1, 3);
    addEdge(adj, 2, 4);

    runTest("=== Test 1: Connected Graph ===", n, adj, 0, [0, 1, 2, 2, 3]);
}

function testDisconnected() {
    // 0 - 1    2    3 - 4
    //
    // From 0 → distances: [0,1,-1,-1,-1]

    const n = 5;
    const adj = makeGraph(n);
    addEdge(adj, 0, 1);
    addEdge(adj, 3, 4);

    runTest("=== Test 2: Disconnected Graph ===", n, adj, 0, [0, 1, -1, -1, -1]);
}

function testSingleNode() {
    // n = 1, no edges
    // From 0 → [0]

    const n = 1;
    const adj = makeGraph(n);

    runTest("=== Test 3: Single Node Graph ===", n, adj, 0, [0]);
}

// ====================================
// Run Tests
// ====================================

testConnected();
testDisconnected();
testSingleNode();
