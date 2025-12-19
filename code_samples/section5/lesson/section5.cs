#nullable enable   // Enable nullable reference type warnings for safer null handling

// ==========================
// BINARY TREE TRAVERSALS
// ==========================

// Preorder traversal of a binary tree
// Order: Root -> Left -> Right
static void Preorder(TreeNode? root)
{
    // Base case: if the current node is null, stop recursion
    if (root == null) return;

    // Visit the current node
    Console.WriteLine(root.Value);

    // Recursively traverse the left subtree
    Preorder(root.Left);

    // Recursively traverse the right subtree
    Preorder(root.Right);
}

// Level-order traversal (Breadth-First Search) of a binary tree
static void LevelOrder(TreeNode? root)
{
    // If the tree is empty, nothing to traverse
    if (root == null) return;

    // Queue is used to process nodes level by level
    var queue = new Queue<TreeNode>();

    // Start with the root node
    queue.Enqueue(root);

    // Continue until all nodes have been processed
    while (queue.Count > 0)
    {
        // Remove the next node from the queue
        var node = queue.Dequeue();

        // Visit the node
        Console.WriteLine(node.Value);

        // Enqueue left child if it exists
        if (node.Left != null) queue.Enqueue(node.Left);

        // Enqueue right child if it exists
        if (node.Right != null) queue.Enqueue(node.Right);
    }
}

// ==========================
// GRAPH DEPTH-FIRST SEARCH
// ==========================

// Helper method for DFS that performs the recursive traversal
static void DfsUtil(int node, List<List<int>> graph, bool[] visited)
{
    // Mark the current node as visited
    visited[node] = true;

    // Visit the node
    Console.WriteLine(node);

    // Explore all adjacent nodes
    foreach (var neighbor in graph[node])
    {
        // Recurse only if the neighbor has not been visited
        if (!visited[neighbor])
        {
            DfsUtil(neighbor, graph, visited);
        }
    }
}

// Entry point for DFS
static void Dfs(int start, List<List<int>> graph)
{
    // Number of nodes in the graph
    int n = graph.Count;

    // Track visited nodes to avoid cycles
    var visited = new bool[n];

    // Start DFS from the given node
    DfsUtil(start, graph, visited);
}

// ==========================
// SHORTEST PATHS (BFS)
// ==========================

// Computes shortest distances from a start node in an unweighted graph
// Uses Breadth-First Search (BFS)
static List<int> ShortestDistances(int start, List<List<int>> graph)
{
    int n = graph.Count;

    // Distance list initialized to -1 (unreachable)
    var dist = new List<int>(n);
    for (int i = 0; i < n; i++)
    {
        dist.Add(-1);
    }

    // Queue for BFS traversal
    var queue = new Queue<int>();

    // Distance to the start node is zero
    dist[start] = 0;
    queue.Enqueue(start);

    // Standard BFS loop
    while (queue.Count > 0)
    {
        int node = queue.Dequeue();

        // Visit all neighbors
        foreach (var neighbor in graph[node])
        {
            // If neighbor has not been visited yet
            if (dist[neighbor] == -1)
            {
                // Set distance and enqueue
                dist[neighbor] = dist[node] + 1;
                queue.Enqueue(neighbor);
            }
        }
    }

    // Return shortest distances from the start node
    return dist;
}

// ==========================
// TEST TREE (Preorder + LevelOrder)
// ==========================

Console.WriteLine("==== TEST TREE (Preorder + LevelOrder) ====");

// Build this binary tree:
//
//        1
//       / \
//      2   3
//     / \   \
//    4   5   6
//
var node4 = new TreeNode(4);
var node5 = new TreeNode(5);
var node6 = new TreeNode(6);

// Attach children to node 2
var node2 = new TreeNode(2) { Left = node4, Right = node5 };

// Attach child to node 3
var node3 = new TreeNode(3) { Right = node6 };

// Root node
var root = new TreeNode(1) { Left = node2, Right = node3 };

Console.WriteLine("Preorder (expect 1 2 4 5 3 6):");
Preorder(root);

Console.WriteLine("\nLevelOrder (expect 1 2 3 4 5 6):");
LevelOrder(root);

// ==========================
// TEST DFS
// ==========================

Console.WriteLine("\n==== TEST DFS (graph) ====");

// Undirected graph represented as adjacency lists:
//
// 0 -- 1
// |  /
// 2
// |
// 3
//
var graph = new List<List<int>> {
    new() { 1, 2 },      // Node 0
    new() { 0, 2 },      // Node 1
    new() { 0, 1, 3 },   // Node 2
    new() { 2 }          // Node 3
};

Console.WriteLine("DFS starting at node 0 (expect 0 1 2 3):");
Dfs(0, graph);

// ==========================
// TEST SHORTEST DISTANCES
// ==========================

Console.WriteLine("\n==== TEST ShortestDistances (BFS) ====");
var dist = ShortestDistances(0, graph);

Console.WriteLine("Shortest distances from node 0:");
for (int i = 0; i < dist.Count; i++)
{
    Console.WriteLine($"dist to {i} = {dist[i]}");
}

// Expected output:
// dist[0] = 0
// dist[1] = 1
// dist[2] = 1
// dist[3] = 2

Console.WriteLine("\n==== ALL TESTS COMPLETE ====");

// ==========================
// TREE NODE DEFINITION
// ==========================

// Simple binary tree node class
class TreeNode(int value)
{
    // Value stored in the node
    public int Value = value;

    // Left and right child references (nullable)
    public TreeNode? Left;
    public TreeNode? Right;
}
