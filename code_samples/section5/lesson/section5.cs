#nullable enable

static void Preorder(TreeNode? root) {
    if (root == null) return;
    System.Console.WriteLine(root.Value);
    Preorder(root.Left);
    Preorder(root.Right);
}

static void LevelOrder(TreeNode? root) {
    if (root == null) return;
    var queue = new Queue<TreeNode>();
    queue.Enqueue(root);

    while (queue.Count > 0) {
        var node = queue.Dequeue();
        System.Console.WriteLine(node.Value);
        if (node.Left != null) queue.Enqueue(node.Left);
        if (node.Right != null) queue.Enqueue(node.Right);
    }
}

static void DfsUtil(int node, List<List<int>> graph, bool[] visited) {
    visited[node] = true;
    System.Console.WriteLine(node);
    foreach (var neighbor in graph[node]) {
        if (!visited[neighbor]) {
            DfsUtil(neighbor, graph, visited);
        }
    }
}

static void Dfs(int start, List<List<int>> graph) {
    int n = graph.Count;
    var visited = new bool[n];
    DfsUtil(start, graph, visited);
}

static List<int> ShortestDistances(int start, List<List<int>> graph) {
    int n = graph.Count;
    var dist = new List<int>(n);
    for (int i = 0; i < n; i++) {
        dist.Add(-1);
    }

    var queue = new Queue<int>();
    dist[start] = 0;
    queue.Enqueue(start);

    while (queue.Count > 0) {
        int node = queue.Dequeue();
        foreach (var neighbor in graph[node]) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[node] + 1;
                queue.Enqueue(neighbor);
            }
        }
    }
    return dist;
}

// ==== TEST TREE (Preorder + LevelOrder) ====
Console.WriteLine("==== TEST TREE (Preorder + LevelOrder) ====");

// Build this tree:
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
var node2 = new TreeNode(2) { Left = node4, Right = node5 };
var node3 = new TreeNode(3) { Right = node6 };
var root  = new TreeNode(1) { Left = node2, Right = node3 };

Console.WriteLine("Preorder (expect 1 2 4 5 3 6):");
Preorder(root);

Console.WriteLine("\nLevelOrder (expect 1 2 3 4 5 6):");
LevelOrder(root);


// ==== TEST DFS ====
Console.WriteLine("\n==== TEST DFS (graph) ====");

// Graph (undirected):
//
// 0 -- 1
// |  /
// 2
// |
// 3
//
var graph = new List<List<int>> {
    new() {1, 2},       // 0
    new() {0, 2},       // 1
    new() {0, 1, 3},    // 2
    new() {2}           // 3
};

Console.WriteLine("DFS starting at node 0 (expect 0 1 2 3):");
Dfs(0, graph);


// ==== TEST ShortestDistances ====
Console.WriteLine("\n==== TEST ShortestDistances (BFS) ====");
var dist = ShortestDistances(0, graph);

Console.WriteLine("Shortest distances from node 0:");
for (int i = 0; i < dist.Count; i++)
{
    Console.WriteLine($"dist to {i} = {dist[i]}");
}

// Expected:
// dist[0] = 0
// dist[1] = 1
// dist[2] = 1
// dist[3] = 2

Console.WriteLine("\n==== ALL TESTS COMPLETE ====");

class TreeNode(int value)
{
    public int Value = value;
    public TreeNode? Left;
    public TreeNode? Right;
}
