// -----------------------------
// Graph utilities + algorithm demos (C#)
// -----------------------------
//
// This file contains:
//   1) Helpers to build graphs (adjacency lists)
//   2) Core graph algorithms:
//        - BFS shortest paths (unweighted)
//        - DFS traversal + connected component counting (undirected)
//        - Topological sort (directed, Kahn's algorithm)
//        - Dijkstra (weighted directed graph, min-heap priority queue)
//   3) A small battery of console "tests" that exercise DSU + each algorithm
//   4) Supporting data structures:
//        - DSU (Union-Find / Disjoint Set Union)
//        - Edge record-like class for weighted adjacency lists
//
// Notes:
//   - Most helper functions are static and operate on adjacency list structures.
//   - The tests print output and expected results for quick visual verification.
//   - No input validation is performed (e.g., checking node indices are in range).

static List<List<int>> BuildGraph(int n, int[][] edges) {
    // Build an adjacency list for an UNDIRECTED, UNWEIGHTED graph with n nodes.
    //
    // Parameters:
    //   n     : number of nodes (assumed 0..n-1)
    //   edges : array of edges, each edge is an int[2] = {u, v}
    //
    // Returns:
    //   graph : List<List<int>> where graph[u] contains all neighbors of u
    //
    // Behavior:
    //   - For undirected graphs, each edge (u, v) is added in both directions:
    //       u -> v and v -> u
    //   - Parallel edges and self-loops are stored as given (no dedup).
    var graph = new List<List<int>>(n);

    // Initialize adjacency list with n empty neighbor lists.
    for (int i = 0; i < n; i++) {
        graph.Add(new List<int>());
    }

    // Add each undirected edge to both adjacency lists.
    foreach (var e in edges) {
        int u = e[0], v = e[1];
        graph[u].Add(v);
        graph[v].Add(u);
    }

    return graph;
}

static int[] BfsShortest(List<List<int>> graph, int src) {
    // BFS shortest paths for an UNWEIGHTED graph from a single source.
    //
    // Parameters:
    //   graph : adjacency list (graph[u] enumerates neighbors)
    //   src   : source node index
    //
    // Returns:
    //   dist[v] = number of edges in shortest path from src to v
    //   dist[v] = INF sentinel if unreachable
    //
    // Why BFS works:
    //   - In an unweighted graph, exploring in layers guarantees the first time
    //     we visit a node is via the shortest path.
    int n = graph.Count;

    // Use a reduced "infinite" sentinel to avoid overflow on dist[u] + 1.
    int INF = int.MaxValue / 4;

    // Distance array; initialize all distances to INF (unvisited/unreachable).
    var dist = new int[n];
    Array.Fill(dist, INF);

    // Standard BFS queue.
    var q = new Queue<int>();

    // Start from source.
    dist[src] = 0;
    q.Enqueue(src);

    while (q.Count > 0) {
        int u = q.Dequeue();

        // Relax edges in BFS sense: if v is unvisited, assign distance and enqueue.
        foreach (int v in graph[u]) {
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                q.Enqueue(v);
            }
        }
    }

    return dist;
}

static void DfsVisit(int u, List<List<int>> graph, bool[] visited) {
    // Recursive DFS visit helper.
    //
    // Parameters:
    //   u       : current node to visit
    //   graph   : adjacency list
    //   visited : boolean visitation marks, mutated in-place
    //
    // Behavior:
    //   - Marks u visited, then recursively visits all unvisited neighbors.
    //
    // Caution:
    //   - Recursive DFS can overflow the stack on very deep graphs; for large inputs,
    //     an explicit stack-based DFS is safer.
    visited[u] = true;

    foreach (int v in graph[u]) {
        if (!visited[v]) {
            DfsVisit(v, graph, visited);
        }
    }
}

static int CountComponents(List<List<int>> graph) {
    // Counts connected components in an UNDIRECTED graph.
    //
    // Parameters:
    //   graph : adjacency list representing an undirected graph
    //
    // Returns:
    //   Number of connected components.
    //
    // Approach:
    //   - Iterate all nodes; when encountering an unvisited node, it starts a DFS.
    //   - That DFS marks the entire connected component.
    //   - Each DFS start increments the component count.
    int n = graph.Count;
    var visited = new bool[n];
    int components = 0;

    for (int u = 0; u < n; u++) {
        if (!visited[u]) {
            components++;
            DfsVisit(u, graph, visited);
        }
    }

    return components;
}

static List<int> TopoSort(List<List<int>> graph) {
    // Topological sort of a DIRECTED graph using Kahn's algorithm.
    //
    // Parameters:
    //   graph : directed adjacency list (u -> v edges)
    //
    // Returns:
    //   order : a list of nodes in topological order IF the graph is a DAG.
    //           If order.Count < n, then the graph contains at least one cycle,
    //           and a complete topo ordering does not exist.
    //
    // How Kahn's algorithm works:
    //   1) Compute indegree of every node.
    //   2) Enqueue all nodes with indegree 0.
    //   3) Pop from queue, append to ordering, decrement indegree of outgoing neighbors.
    //   4) Any neighbor that reaches indegree 0 is enqueued.
    int n = graph.Count;

    // indeg[v] = number of incoming edges to v.
    var indeg = new int[n];
    for (int u = 0; u < n; u++) {
        foreach (int v in graph[u]) {
            indeg[v]++;
        }
    }

    // Queue of nodes currently with indegree 0 (no remaining prerequisites).
    var q = new Queue<int>();
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) {
            q.Enqueue(i);
        }
    }

    var order = new List<int>();

    while (q.Count > 0) {
        int u = q.Dequeue();
        order.Add(u);

        // "Remove" u from the graph by decrementing indegree of its neighbors.
        foreach (int v in graph[u]) {
            if (--indeg[v] == 0) {
                q.Enqueue(v);
            }
        }
    }

    // If order.Count < n, there was a cycle: nodes in the cycle never reached indegree 0.
    return order;
}

static long[] Dijkstra(List<List<Edge>> graph, int src) {
    // Dijkstra's algorithm for shortest paths in a WEIGHTED directed graph
    // with NON-NEGATIVE edge weights.
    //
    // Parameters:
    //   graph : adjacency list where graph[u] is a list of Edge (u -> v with weight w)
    //   src   : source node
    //
    // Returns:
    //   dist[v] = shortest path distance from src to v (long), or INF if unreachable.
    //
    // Implementation details:
    //   - Uses .NET PriorityQueue<TElement, TPriority> as a min-heap.
    //   - "Lazy" deletion: we may push multiple (node, distance) pairs; when popped,
    //     we skip if it doesn't match the current dist[u].
    //
    // Correctness requirement:
    //   - All edge weights must be >= 0. (Negative weights break Dijkstra.)
    int n = graph.Count;

    // Large INF sentinel; /4 leaves room for adding weights without overflow.
    long INF = long.MaxValue / 4;

    // Distance array initialized to INF.
    var dist = new long[n];
    Array.Fill(dist, INF);

    // Min-priority queue storing nodes keyed by their tentative distance.
    var pq = new PriorityQueue<int, long>();

    // Initialize with the source.
    dist[src] = 0;
    pq.Enqueue(src, 0);

    while (pq.Count > 0) {
        // TryDequeue pops the element with the smallest priority (distance).
        pq.TryDequeue(out int u, out long d);

        // If this entry is stale (not equal to current best dist[u]), skip it.
        if (d != dist[u]) continue;

        // Relax outgoing edges u -> v.
        foreach (var e in graph[u]) {
            int v = e.To;
            long nd = d + e.W;   // tentative distance to v via u

            if (nd < dist[v]) {
                dist[v] = nd;
                pq.Enqueue(v, nd); // push updated distance; stale older entries will be ignored later
            }
        }
    }

    return dist;
}

//
// =======================
// 1. Test DSU
// =======================
//
Console.WriteLine("=== Test 1: DSU ===");

{
    // This block tests the Disjoint Set Union structure by performing unions
    // and then inspecting representatives.

    var d = new DSU(5);

    // Unions:
    //   union(0,1), union(1,2) => {0,1,2}
    //   union(3,4)             => {3,4}
    d.Union(0, 1);
    d.Union(1, 2);
    d.Union(3, 4);

    // Collect the representative (root) of each element.
    // Note: exact representative IDs may vary based on union-by-rank tie breaks.
    var repr = new int[5];
    for (int i = 0; i < 5; i++)
        repr[i] = d.Find(i);

    Console.WriteLine("Representatives: [" + string.Join(",", repr) + "]");

    // Count unique representatives to determine number of disjoint sets.
    var seen = new HashSet<int>(repr);
    Console.WriteLine($"Number of sets = {seen.Count} (expected 2)\n");
}

//
// =======================
// 2. Test BFS + Components
// =======================
//
Console.WriteLine("=== Test 2: BFS shortest + components ===");

{
    // Build and test an undirected graph:
    // 0 - 1 - 2 - 4
    //     |
    //     3
    //
    // Expected BFS distances from 0: [0,1,2,2,3]
    // Expected components: 1

    int[][] edges =
    [
        [0,1],
        [1,2],
        [1,3],
        [2,4],
    ];

    var g = BuildGraph(5, edges);

    // BFS shortest paths from source 0.
    var dist = BfsShortest(g, 0);

    Console.WriteLine("Distances from 0: [" + string.Join(",", dist) + "]");
    Console.WriteLine("Expected: [0,1,2,2,3]");

    // Count connected components via DFS.
    int comps = CountComponents(g);
    Console.WriteLine($"Connected components = {comps} (expected 1)\n");
}

//
// =======================
// 3. Disconnected Components
// =======================
//
Console.WriteLine("=== Test 3: Disconnected components ===");

{
    // Undirected graph with three components:
    // 0-1-2   3-4   5
    //
    // Expected components = 3

    int[][] edges =
    [
        [0,1],
        [1,2],
        [3,4],
    ];

    var g = BuildGraph(6, edges);

    int comps = CountComponents(g);
    Console.WriteLine($"Components = {comps} (expected 3)\n");
}

//
// =======================
// 4. Topological Sort (DAG)
// =======================
//
Console.WriteLine("=== Test 4: Topological sort (DAG) ===");

{
    // Topological sort is defined for directed acyclic graphs (DAGs).
    // This builds a classic example DAG with 6 nodes and prints a topo order.
    //
    // Directed edges:
    // 5→2, 5→0
    // 4→0, 4→1
    // 2→3
    // 3→1

    int n = 6;

    // Initialize directed adjacency list.
    var dag = new List<List<int>>(n);
    for (int i = 0; i < n; i++) dag.Add(new List<int>());

    // Add directed edges.
    dag[5].Add(2);
    dag[5].Add(0);
    dag[4].Add(0);
    dag[4].Add(1);
    dag[2].Add(3);
    dag[3].Add(1);

    // Run Kahn topo sort; if order.Count < n, it indicates a cycle.
    var order = TopoSort(dag);

    Console.WriteLine("Topological order: [" + string.Join(",", order) + "]");
    Console.WriteLine("(If length < 6, graph contains a cycle)\n");
}

//
// =======================
// 5. Dijkstra (weighted directed graph)
// =======================
//
Console.WriteLine("=== Test 5: Dijkstra ===");

{
    // Builds a weighted directed graph and runs Dijkstra from node 0.
    //
    // Graph:
    // 0→1(10), 0→3(5)
    // 1→2(1), 1→3(2)
    // 2→4(4)
    // 3→1(3), 3→2(9), 3→4(2)
    // 4→0(7), 4→2(6)
    //
    // Expected distances from 0: [0,8,9,5,7]

    int n = 5;

    // Initialize adjacency list for weighted edges.
    var g = new List<List<Edge>>(n);
    for (int i = 0; i < n; i++) g.Add(new List<Edge>());

    // Local helper to add a weighted directed edge u -> v.
    // Note: weight parameter is int here; distances are computed in long.
    void ADD(int u, int v, int w) => g[u].Add(new Edge(v, w));

    // Add edges as described above.
    ADD(0, 1, 10);
    ADD(0, 3, 5);

    ADD(1, 2, 1);
    ADD(1, 3, 2);

    ADD(2, 4, 4);

    ADD(3, 1, 3);
    ADD(3, 2, 9);
    ADD(3, 4, 2);

    ADD(4, 0, 7);
    ADD(4, 2, 6);

    // Run Dijkstra from source 0.
    long[] dist = Dijkstra(g, 0);

    Console.WriteLine("Distances from 0: [" + string.Join(",", dist) + "]");
    Console.WriteLine("Expected: [0,8,9,5,7]\n");
}

// Final marker indicating tests finished.
Console.WriteLine("=== All tests complete ===");

class DSU {
    // Disjoint Set Union (Union-Find) data structure.
    //
    // Supports:
    //   - Find(x): returns representative/root of the set containing x (with path compression)
    //   - Union(a,b): merges sets containing a and b (with union-by-rank)
    //
    // Fields:
    //   parent[i] : parent pointer; roots satisfy parent[i] == i
    //   rank[i]   : rank heuristic (approx tree height upper bound) used to keep trees shallow
    private readonly int[] parent;
    private readonly int[] rank;

    public DSU(int n) {
        // Initialize n singleton sets: each element is its own parent/root.
        parent = new int[n];
        rank = new int[n];

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    public int Find(int x) {
        // Finds representative/root of x's set.
        // Path compression: flattens the structure by pointing x directly to the root.
        if (parent[x] != x) {
            parent[x] = Find(parent[x]);
        }
        return parent[x];
    }

    public void Union(int a, int b) {
        // Unites the sets containing a and b.
        // Uses union-by-rank to attach smaller-rank root under larger-rank root.
        int ra = Find(a);
        int rb = Find(b);

        if (ra == rb) return; // already in the same set; nothing to do

        if (rank[ra] < rank[rb]) {
            parent[ra] = rb;
        } else if (rank[ra] > rank[rb]) {
            parent[rb] = ra;
        } else {
            // Same rank: choose ra as new root and increment its rank.
            parent[rb] = ra;
            rank[ra]++;
        }
    }
}

// Simple Edge container for weighted adjacency lists.
class Edge(int to, int w)
{
    // Destination node of this directed edge.
    public int To = to;

    // Weight/cost of this edge (assumed non-negative for Dijkstra).
    public int W = w;
}
