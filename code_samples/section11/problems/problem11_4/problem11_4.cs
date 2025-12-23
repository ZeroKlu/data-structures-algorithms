static int[] Dijkstra(int n, List<List<Edge>> adj, int s) {
    // Computes single-source shortest paths using Dijkstra's algorithm on a
    // DIRECTED graph with NON-NEGATIVE edge weights.
    //
    // Parameters:
    //   n   : number of nodes (assumed labeled 0..n-1)
    //   adj : adjacency list where adj[u] contains outgoing edges from u
    //   s   : source node
    //
    // Returns:
    //   dist : array where dist[v] is the shortest distance from s to v.
    //          Unreachable nodes remain at INF (a very large sentinel value).
    //
    // Important correctness requirement:
    //   - Dijkstra's algorithm assumes all edge weights are >= 0. If negative
    //     weights exist, the results may be incorrect.
    //
    // Implementation notes:
    //   - Uses .NET's PriorityQueue<TElement, TPriority>.
    //   - Since PriorityQueue does not support decrease-key, the algorithm
    //     may enqueue multiple entries for the same node. Outdated entries
    //     are ignored via: if (d > dist[u]) continue;
    int INF = int.MaxValue / 2; // "infinity" (reduced to help avoid overflow on addition)
    var dist = new int[n];
    Array.Fill(dist, INF);

    // Distance from source to itself is 0
    dist[s] = 0;

    // Min-priority queue keyed by current best-known distance.
    // Element = node id, Priority = tentative distance.
    var pq = new PriorityQueue<int, int>(); // (node, priority = dist)
    pq.Enqueue(s, 0);

    // Main loop: repeatedly expand the node with the smallest tentative distance.
    while (pq.Count > 0) {
        // TryDequeue returns:
        //   u = node with smallest priority
        //   d = that priority (a candidate distance for u)
        pq.TryDequeue(out int u, out int d);

        // If this entry is stale (we already found a shorter path to u), skip it.
        if (d > dist[u]) continue;

        // Relax edges u -> v
        foreach (var e in adj[u]) {
            int v = e.To;

            // Candidate distance to v going through u
            int nd = d + e.W;

            // If this path improves the best-known distance to v, update and enqueue.
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
    // Allocates an empty adjacency list for a directed weighted graph.
    //
    // Returns:
    //   g : List<List<Edge>> where each g[u] starts as an empty list of outgoing edges.
    var g = new List<List<Edge>>(n);
    for (int i = 0; i < n; i++) g.Add([]);
    return g;
}

void AddEdge(List<List<Edge>> adj, int u, int v, int w)
{
    // Adds a DIRECTED weighted edge u -> v with weight w.
    //
    // Parameters:
    //   adj : adjacency list
    //   u   : source node
    //   v   : destination node
    //   w   : edge weight (should be >= 0 for Dijkstra)
    //
    // Note:
    //   - For an undirected graph, you would add both (u->v) and (v->u).
    adj[u].Add(new Edge(v, w));   // directed
}

void PrintDist(int[] dist)
{
    // Prints the distance array in a compact list format.
    //
    // Convention:
    //   - Values that are still "very large" are printed as "INF" to indicate
    //     unreachable nodes.
    //
    // Note:
    //   - The threshold uses int.MaxValue / 4 to avoid printing huge numbers
    //     while still distinguishing from normal path lengths.
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
    // Builds a small directed weighted graph and runs Dijkstra from node 0.
    // Graph:
    //   0→1(2), 0→2(5)
    //   1→2(1)
    //   1→3(3)
    //   2→3(1)
    //   3→4(2)
    //
    // This test checks that the algorithm chooses cheaper multi-hop routes,
    // e.g., 0->1->2 (cost 3) instead of 0->2 (cost 5).
    int n = 5;
    var adj = MakeGraph(n);

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
    // Graph:
    //   0→1(1), 0→2(1)
    //   1→3(2)
    //   2→3(2)
    //   3→4(1)
    //   4→5(5)
    //
    // There are equal-cost ways to reach node 3:
    //   0->1->3 (1+2) and 0->2->3 (1+2).
    // The resulting shortest distances should be correct regardless of which
    // equal-cost path is discovered first.
    int n = 6;
    var adj = MakeGraph(n);

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
    // Graph with 5 nodes but only a reachable component {0,1,2} from source 0:
    //   0→1(4), 1→2(6)
    //
    // Nodes 3 and 4 have no path from 0 and should print as "INF".
    int n = 5;
    var adj = MakeGraph(n);

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
    // Minimal edge container for adjacency lists.
    //
    // Fields:
    //   To : destination node id
    //   W  : edge weight
    //
    // Note:
    //   - We store fields publicly for simplicity in a teaching/sample setting.
    public int To = to;
    public int W = w;
}
