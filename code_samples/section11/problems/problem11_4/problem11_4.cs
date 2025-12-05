static int[] Dijkstra(int n, List<List<Edge>> adj, int s) {
    int INF = int.MaxValue / 2;
    var dist = new int[n];
    Array.Fill(dist, INF);
    dist[s] = 0;

    var pq = new PriorityQueue<int, int>(); // (node, priority = dist)
    pq.Enqueue(s, 0);

    while (pq.Count > 0) {
        pq.TryDequeue(out int u, out int d);
        if (d > dist[u]) continue;
        foreach (var e in adj[u]) {
            int v = e.To;
            int nd = d + e.W;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.Enqueue(v, nd);
            }
        }
    }
    return dist;
}

// ===============================
// Helper Functions
// ===============================

List<List<Edge>> MakeGraph(int n)
{
    var g = new List<List<Edge>>(n);
    for (int i = 0; i < n; i++) g.Add(new List<Edge>());
    return g;
}

void AddEdge(List<List<Edge>> adj, int u, int v, int w)
{
    adj[u].Add(new Edge(v, w));   // directed
}

void PrintDist(int[] dist)
{
    Console.Write("[");
    for (int i = 0; i < dist.Length; i++)
    {
        Console.Write(dist[i] >= int.MaxValue / 4 ? "INF" : dist[i]);
        if (i + 1 < dist.Length) Console.Write(", ");
    }
    Console.WriteLine("]");
}

// ===============================
// Test 1: Simple Weighted Graph
// ===============================

Console.WriteLine("=== Test 1: Simple Weighted Graph ===");

{
    int n = 5;
    var adj = MakeGraph(n);

    // 0→1(2), 0→2(5)
    // 1→2(1)
    // 1→3(3)
    // 2→3(1)
    // 3→4(2)
    AddEdge(adj, 0, 1, 2);
    AddEdge(adj, 0, 2, 5);
    AddEdge(adj, 1, 2, 1);
    AddEdge(adj, 1, 3, 3);
    AddEdge(adj, 2, 3, 1);
    AddEdge(adj, 3, 4, 2);

    int[] dist = Dijkstra(n, adj, 0);

    Console.Write("Distances: ");
    PrintDist(dist);
    Console.WriteLine();
}

// ===============================
// Test 2: Multiple Shortest Paths
// ===============================

Console.WriteLine("=== Test 2: Multiple Shortest Paths ===");

{
    int n = 6;
    var adj = MakeGraph(n);

    // 0→1(1), 0→2(1)
    // 1→3(2)
    // 2→3(2)
    // 3→4(1)
    // 4→5(5)
    AddEdge(adj, 0, 1, 1);
    AddEdge(adj, 0, 2, 1);
    AddEdge(adj, 1, 3, 2);
    AddEdge(adj, 2, 3, 2);
    AddEdge(adj, 3, 4, 1);
    AddEdge(adj, 4, 5, 5);

    int[] dist = Dijkstra(n, adj, 0);

    Console.Write("Distances: ");
    PrintDist(dist);
    Console.WriteLine();
}

// ===============================
// Test 3: Disconnected Graph
// ===============================

Console.WriteLine("=== Test 3: Disconnected Graph ===");

{
    int n = 5;
    var adj = MakeGraph(n);

    // 0→1(4), 1→2(6)
    AddEdge(adj, 0, 1, 4);
    AddEdge(adj, 1, 2, 6);
    // Nodes 3 and 4 unreachable

    int[] dist = Dijkstra(n, adj, 0);

    Console.Write("Distances: ");
    PrintDist(dist);
    Console.WriteLine();
}

class Edge(int to, int w)
{
    public int To = to;
    public int W = w;
}
