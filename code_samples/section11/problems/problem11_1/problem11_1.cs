static int[] BfsShortestPaths(int n, List<List<int>> adj, int s) {
    var dist = new int[n];
    Array.Fill(dist, -1);
    var q = new Queue<int>();
    dist[s] = 0;
    q.Enqueue(s);

    while (q.Count > 0) {
        int u = q.Dequeue();
        foreach (var v in adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.Enqueue(v);
            }
        }
    }
    return dist;
}

// ===== Helper functions =====

List<List<int>> MakeGraph(int n)
{
    var g = new List<List<int>>(n);
    for (int i = 0; i < n; i++)
        g.Add(new List<int>());
    return g;
}

void AddEdge(List<List<int>> adj, int u, int v)
{
    adj[u].Add(v);
    adj[v].Add(u);
}

void PrintTest(string name, int[] dist, int[] expected)
{
    Console.WriteLine(name);
    Console.WriteLine("Distances: " + string.Join(", ", dist));
    Console.WriteLine("Expected : " + string.Join(", ", expected));
    Console.WriteLine();
}

// ===== Test 1: Connected graph =====

{
    Console.WriteLine("=== Test 1: Connected graph ===");

    // Graph:
    // 0 - 1 - 2 - 4
    //     |
    //     3
    int n = 5;
    var adj = MakeGraph(n);

    AddEdge(adj, 0, 1);
    AddEdge(adj, 1, 2);
    AddEdge(adj, 1, 3);
    AddEdge(adj, 2, 4);

    int[] dist = BfsShortestPaths(n, adj, 0);
    int[] expected = { 0, 1, 2, 2, 3 };

    PrintTest("Connected graph", dist, expected);
}

// ===== Test 2: Disconnected graph =====

{
    Console.WriteLine("=== Test 2: Disconnected graph ===");

    // 0 - 1    2    3 - 4
    int n = 5;
    var adj = MakeGraph(n);

    AddEdge(adj, 0, 1);
    AddEdge(adj, 3, 4);

    int[] dist = BfsShortestPaths(n, adj, 0);
    int[] expected = { 0, 1, -1, -1, -1 };

    PrintTest("Disconnected graph", dist, expected);
}

// ===== Test 3: Single node graph =====

{
    Console.WriteLine("=== Test 3: Single node graph ===");

    int n = 1;
    var adj = MakeGraph(n); // no edges

    int[] dist = BfsShortestPaths(n, adj, 0);
    int[] expected = { 0 };

    PrintTest("Single node graph", dist, expected);
}
