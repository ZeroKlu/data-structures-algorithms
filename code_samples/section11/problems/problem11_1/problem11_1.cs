static int[] BfsShortestPaths(int n, List<List<int>> adj, int s) {
    // Computes shortest path distances from a single source in an UNWEIGHTED graph
    // using Breadth-First Search (BFS).
    //
    // Parameters:
    //   n   : number of vertices (assumed labeled 0..n-1)
    //   adj : adjacency list; adj[u] contains neighbors of u
    //   s   : source vertex index
    //
    // Returns:
    //   dist[v] = minimum number of edges from s to v
    //   dist[v] = -1 if v is unreachable from s
    //
    // Notes:
    //   - BFS is correct for unweighted graphs because it explores vertices
    //     in increasing distance layers from the source.
    //   - -1 is used as a sentinel meaning "unvisited/unreachable".
    var dist = new int[n];
    Array.Fill(dist, -1);           // initialize all distances to -1 (unvisited/unreachable)

    var q = new Queue<int>();       // FIFO queue for BFS frontier

    dist[s] = 0;                    // distance from source to itself is 0
    q.Enqueue(s);                   // start BFS from the source

    while (q.Count > 0) {
        int u = q.Dequeue();        // dequeue next vertex to expand

        // Explore all neighbors of u.
        foreach (var v in adj[u]) {
            // If v hasn't been visited yet, the first time we discover it is
            // via the shortest path (in an unweighted graph).
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1; // neighbor is one edge farther than u
                q.Enqueue(v);          // enqueue v for later expansion
            }
        }
    }

    return dist;
}

// ===== Helper functions =====

List<List<int>> MakeGraph(int n)
{
    // Allocates an empty adjacency list for a graph with n vertices.
    //
    // Returns:
    //   g : List<List<int>> where each g[i] starts as an empty neighbor list.
    //
    // Note:
    //   - This creates the container; edges are added separately via AddEdge().
    var g = new List<List<int>>(n);

    // Pre-populate with n empty lists so g[i] is valid for all 0..n-1.
    for (int i = 0; i < n; i++)
        g.Add([]); // target-typed "new List<int>()" (C# collection expression / modern syntax)
    return g;
}

void AddEdge(List<List<int>> adj, int u, int v)
{
    // Adds an UNDIRECTED edge between u and v.
    //
    // Parameters:
    //   adj : adjacency list
    //   u,v : endpoints
    //
    // Behavior:
    //   - Adds v to adj[u] and u to adj[v].
    //   - No bounds checking is performed on u and v.
    adj[u].Add(v);
    adj[v].Add(u);
}

void PrintTest(string name, int[] dist, int[] expected)
{
    // Simple "print and compare" helper for tests.
    //
    // Parameters:
    //   name     : label for the test case
    //   dist     : computed distances to print
    //   expected : expected distances to print
    //
    // Note:
    //   - This does not assert correctness; it just prints both arrays for visual comparison.
    Console.WriteLine(name);
    Console.WriteLine("Distances: " + string.Join(", ", dist));
    Console.WriteLine("Expected : " + string.Join(", ", expected));
    Console.WriteLine();
}

// ===== Test 1: Connected graph =====

{
    Console.WriteLine("=== Test 1: Connected graph ===");

    // Graph (undirected):
    // 0 - 1 - 2 - 4
    //     |
    //     3
    //
    // From source 0, expected shortest distances:
    // [0, 1, 2, 2, 3]
    int n = 5;
    var adj = MakeGraph(n);

    // Add undirected edges to match the diagram.
    AddEdge(adj, 0, 1);
    AddEdge(adj, 1, 2);
    AddEdge(adj, 1, 3);
    AddEdge(adj, 2, 4);

    // Run BFS shortest paths from node 0.
    int[] dist = BfsShortestPaths(n, adj, 0);

    // Expected distance array for visual verification.
    int[] expected = [0, 1, 2, 2, 3];

    PrintTest("Connected graph", dist, expected);
}

// ===== Test 2: Disconnected graph =====

{
    Console.WriteLine("=== Test 2: Disconnected graph ===");

    // Graph (undirected):
    // 0 - 1    2    3 - 4
    //
    // From source 0:
    //   - nodes 0 and 1 are reachable
    //   - nodes 2, 3, 4 are unreachable => -1
    int n = 5;
    var adj = MakeGraph(n);

    // Two separate components: {0,1} and {3,4}; node 2 is isolated.
    AddEdge(adj, 0, 1);
    AddEdge(adj, 3, 4);

    int[] dist = BfsShortestPaths(n, adj, 0);
    int[] expected = [0, 1, -1, -1, -1];

    PrintTest("Disconnected graph", dist, expected);
}

// ===== Test 3: Single node graph =====

{
    Console.WriteLine("=== Test 3: Single node graph ===");

    // Graph with a single vertex 0 and no edges.
    // From source 0: dist should be [0].
    int n = 1;

    // Adjacency list has one empty neighbor list (no edges to add).
    var adj = MakeGraph(n);

    int[] dist = BfsShortestPaths(n, adj, 0);
    int[] expected = [0];

    PrintTest("Single node graph", dist, expected);
}
