static IList<int> TopoSortKahn(int n, List<List<int>> adj) {
    var indeg = new int[n];
    for (int u = 0; u < n; u++) {
        foreach (var v in adj[u]) indeg[v]++;
    }
    var q = new Queue<int>();
    for (int u = 0; u < n; u++) {
        if (indeg[u] == 0) q.Enqueue(u);
    }
    var order = new List<int>();
    while (q.Count > 0) {
        int u = q.Dequeue();
        order.Add(u);
        foreach (var v in adj[u]) {
            indeg[v]--;
            if (indeg[v] == 0) q.Enqueue(v);
        }
    }
    if (order.Count != n) return Array.Empty<int>();
    return order;
}

// ===== Helper Functions =====

List<List<int>> MakeGraph(int n)
{
    var g = new List<List<int>>(n);
    for (int i = 0; i < n; i++)
        g.Add(new List<int>());
    return g;
}

void AddEdge(List<List<int>> adj, int u, int v)
{
    adj[u].Add(v); // directed edge u -> v
}

void RunTest(string name, int n, List<List<int>> adj)
{
    Console.WriteLine(name);
    var result = TopoSortKahn(n, adj);

    if (result.Count == 0)
        Console.WriteLine("Topological order: [] (cycle detected)\n");
    else
        Console.WriteLine("Topological order: " + string.Join(", ", result) + "\n");
}

// ===============================
// Test 1: Simple DAG
// ===============================

{
    Console.WriteLine("=== Test 1: Simple DAG ===");

    int n = 6;
    var adj = MakeGraph(n);

    // DAG:
    // 5 → 2, 5 → 0
    // 4 → 0, 4 → 1
    // 2 → 3
    // 3 → 1
    AddEdge(adj, 5, 2);
    AddEdge(adj, 5, 0);
    AddEdge(adj, 4, 0);
    AddEdge(adj, 4, 1);
    AddEdge(adj, 2, 3);
    AddEdge(adj, 3, 1);

    RunTest("Simple DAG", n, adj);
}

// ===============================
// Test 2: Another DAG
// ===============================

{
    Console.WriteLine("=== Test 2: Another DAG ===");

    int n = 4;
    var adj = MakeGraph(n);

    // 0 → 1, 0 → 2
    // 1 → 3
    // 2 → 3
    AddEdge(adj, 0, 1);
    AddEdge(adj, 0, 2);
    AddEdge(adj, 1, 3);
    AddEdge(adj, 2, 3);

    RunTest("Another DAG", n, adj);
}

// ===============================
// Test 3: Graph with a cycle
// ===============================

{
    Console.WriteLine("=== Test 3: Graph with cycle ===");

    int n = 3;
    var adj = MakeGraph(n);

    // 0 → 1 → 2 → 0
    AddEdge(adj, 0, 1);
    AddEdge(adj, 1, 2);
    AddEdge(adj, 2, 0);

    RunTest("Cyclic graph", n, adj);
}
