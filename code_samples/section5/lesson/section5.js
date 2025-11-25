class TreeNode {
    constructor(value, left = null, right = null) {
        this.value = value;
        this.left = left;
        this.right = right;
    }
}

function preorder(root) {
    if (root === null) return;
    console.log(root.value);
    preorder(root.left);
    preorder(root.right);
}

function levelOrder(root) {
    if (root === null) return;
    const queue = [root];
    let head = 0;

    while (head < queue.length) {
        const node = queue[head++];
        console.log(node.value);
        if (node.left) queue.push(node.left);
        if (node.right) queue.push(node.right);
    }
}

function dfsUtil(node, graph, visited) {
    visited[node] = true;
    console.log(node);
    for (const neighbor of graph[node]) {
        if (!visited[neighbor]) {
            dfsUtil(neighbor, graph, visited);
        }
    }
}

function dfs(start, graph) {
    const n = graph.length;
    const visited = new Array(n).fill(false);
    dfsUtil(start, graph, visited);
}

function shortestDistances(start, graph) {
    const n = graph.length;
    const dist = new Array(n).fill(-1);
    const queue = [];

    dist[start] = 0;
    queue.push(start);
    let head = 0;

    while (head < queue.length) {
        const node = queue[head++];
        for (const neighbor of graph[node]) {
            if (dist[neighbor] === -1) {
                dist[neighbor] = dist[node] + 1;
                queue.push(neighbor);
            }
        }
    }
    return dist;
}

// ==== TEST TREE (preorder + levelOrder) ====
console.log("==== TEST TREE (preorder + levelOrder) ====");

// Build this tree:
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

// ==== TEST DFS ====
console.log("\n==== TEST DFS (graph) ====");

// Graph (undirected):
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

// ==== TEST shortestDistances ====
console.log("\n==== TEST shortestDistances (BFS) ====");
const dist = shortestDistances(0, graph);

console.log("Shortest distances from node 0:");
dist.forEach((d, i) => {
    console.log(`dist to ${i} = ${d}`);
});
//
// Expected:
// dist[0] = 0
// dist[1] = 1
// dist[2] = 1
// dist[3] = 2
//

console.log("\n==== ALL TESTS COMPLETE ====");
