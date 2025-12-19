// ==========================
// BINARY TREE NODE DEFINITION
// ==========================

class TreeNode {
    // Creates a binary tree node
    // value: value stored in the node
    // left / right: child references (default to null)
    constructor(value, left = null, right = null) {
        this.value = value;
        this.left = left;
        this.right = right;
    }
}

// ==========================
// BINARY TREE TRAVERSALS
// ==========================

// Preorder traversal of a binary tree
// Order: Root -> Left -> Right
function preorder(root) {
    // Base case: if node is null, stop recursion
    if (root === null) return;

    // Visit the current node
    console.log(root.value);

    // Recursively traverse left subtree
    preorder(root.left);

    // Recursively traverse right subtree
    preorder(root.right);
}

// Level-order traversal (Breadth-First Search) of a binary tree
function levelOrder(root) {
    // If tree is empty, nothing to traverse
    if (root === null) return;

    // Queue implemented using an array
    const queue = [root];

    // Head index avoids expensive shift() operations
    let head = 0;

    // Process nodes level by level
    while (head < queue.length) {
        // Dequeue next node
        const node = queue[head++];

        // Visit the node
        console.log(node.value);

        // Enqueue left child if it exists
        if (node.left) queue.push(node.left);

        // Enqueue right child if it exists
        if (node.right) queue.push(node.right);
    }
}

// ==========================
// GRAPH DEPTH-FIRST SEARCH
// ==========================

// Helper function for recursive DFS traversal
function dfsUtil(node, graph, visited) {
    // Mark current node as visited
    visited[node] = true;

    // Visit the node
    console.log(node);

    // Explore all adjacent nodes
    for (const neighbor of graph[node]) {
        // Recurse only if the neighbor has not been visited
        if (!visited[neighbor]) {
            dfsUtil(neighbor, graph, visited);
        }
    }
}

// Entry point for DFS
function dfs(start, graph) {
    // Number of nodes in the graph
    const n = graph.length;

    // Track visited nodes to avoid cycles
    const visited = new Array(n).fill(false);

    // Start DFS from the given node
    dfsUtil(start, graph, visited);
}

// ==========================
// SHORTEST PATHS (BFS)
// ==========================

// Computes shortest distances from a start node
// in an unweighted graph using BFS
function shortestDistances(start, graph) {
    const n = graph.length;

    // Distance array initialized to -1 (unreachable)
    const dist = new Array(n).fill(-1);

    // Queue for BFS traversal
    const queue = [];

    // Distance to start node is zero
    dist[start] = 0;
    queue.push(start);

    // Head index for efficient dequeue
    let head = 0;

    // Standard BFS loop
    while (head < queue.length) {
        const node = queue[head++];

        // Visit all neighbors
        for (const neighbor of graph[node]) {
            // If neighbor has not been visited yet
            if (dist[neighbor] === -1) {
                // Update distance and enqueue neighbor
                dist[neighbor] = dist[node] + 1;
                queue.push(neighbor);
            }
        }
    }

    // Return shortest distances from start node
    return dist;
}

// ==========================
// TEST TREE (preorder + levelOrder)
// ==========================

console.log("==== TEST TREE (preorder + levelOrder) ====");

// Build this binary tree:
//
//        1
//       / \
//      2   3
//     / \   \
//    4   5   6
//
const node4 = new TreeNode(4);
const node5 = new TreeNode(5);
const node6 = new TreeNode(6);
const node2 = new TreeNode(2, node4, node5);
const node3 = new TreeNode(3, null, node6);
const root = new TreeNode(1, node2, node3);

console.log("Preorder (expect 1 2 4 5 3 6):");
preorder(root);

console.log("\nLevel-order (expect 1 2 3 4 5 6):");
levelOrder(root);

// ==========================
// TEST DFS
// ==========================

console.log("\n==== TEST DFS (graph) ====");

// Undirected graph:
//
// 0 -- 1
// |  /
// 2
// |
// 3
//
const graph = [
    [1, 2],      // neighbors of 0
    [0, 2],      // neighbors of 1
    [0, 1, 3],   // neighbors of 2
    [2]          // neighbors of 3
];

console.log("DFS from node 0 (expect 0 1 2 3):");
dfs(0, graph);

// ==========================
// TEST shortestDistances
// ==========================

console.log("\n==== TEST shortestDistances (BFS) ====");
const dist = shortestDistances(0, graph);

console.log("Shortest distances from node 0:");
dist.forEach((d, i) => {
    console.log(`dist to ${i} = ${d}`);
});

// Expected:
// dist[0] = 0
// dist[1] = 1
// dist[2] = 1
// dist[3] = 2

console.log("\n==== ALL TESTS COMPLETE ====");
