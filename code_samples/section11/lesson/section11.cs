static List<List<int>> BuildGraph(int n, int[][] edges) {
    var graph = new List<List<int>>(n);
    for (int i = 0; i < n; i++) {
        graph.Add(new List<int>());
    }
    foreach (var e in edges) {
        int u = e[0], v = e[1];
        graph[u].Add(v);
        graph[v].Add(u);
    }
    return graph;
}

static int[] BfsShortest(List<List<int>> graph, int src) {
    int n = graph.Count;
    int INF = int.MaxValue / 4;
    var dist = new int[n];
    Array.Fill(dist, INF);

    var q = new Queue<int>();
    dist[src] = 0;
    q.Enqueue(src);

    while (q.Count > 0) {
        int u = q.Dequeue();
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
    visited[u] = true;
    foreach (int v in graph[u]) {
        if (!visited[v]) {
            DfsVisit(v, graph, visited);
        }
    }
}

static int CountComponents(List<List<int>> graph) {
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
    int n = graph.Count;
    var indeg = new int[n];
    for (int u = 0; u < n; u++) {
        foreach (int v in graph[u]) {
            indeg[v]++;
        }
    }

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
        foreach (int v in graph[u]) {
            if (--indeg[v] == 0) {
                q.Enqueue(v);
            }
        }
    }
    // if order.Count < n, there was a cycle
    return order;
}

static long[] Dijkstra(List<List<Edge>> graph, int src) {
    int n = graph.Count;
    long INF = long.MaxValue / 4;
    var dist = new long[n];
    Array.Fill(dist, INF);

    var pq = new PriorityQueue<int, long>();
    dist[src] = 0;
    pq.Enqueue(src, 0);

    while (pq.Count > 0) {
        pq.TryDequeue(out int u, out long d);
        if (d != dist[u]) continue;
        foreach (var e in graph[u]) {
            int v = e.To;
            long nd = d + e.W;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.Enqueue(v, nd);
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
    var d = new DSU(5);
    d.Union(0, 1);
    d.Union(1, 2);
    d.Union(3, 4);

    var repr = new int[5];
    for (int i = 0; i < 5; i++)
        repr[i] = d.Find(i);

    Console.WriteLine("Representatives: [" + string.Join(",", repr) + "]");

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
    // Graph:
    // 0 - 1 - 2 - 4
    //     |
    //     3
    int[][] edges =
    [
        [0,1],
        [1,2],
        [1,3],
        [2,4],
    ];

    var g = BuildGraph(5, edges);
    var dist = BfsShortest(g, 0);

    Console.WriteLine("Distances from 0: [" + string.Join(",", dist) + "]");
    Console.WriteLine("Expected: [0,1,2,2,3]");

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
    // 0-1-2  3-4   5
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
    int n = 6;
    var dag = new List<List<int>>(n);
    for (int i = 0; i < n; i++) dag.Add(new List<int>());

    // Directed edges:
    // 5→2, 5→0
    // 4→0, 4→1
    // 2→3
    // 3→1
    dag[5].Add(2);
    dag[5].Add(0);
    dag[4].Add(0);
    dag[4].Add(1);
    dag[2].Add(3);
    dag[3].Add(1);

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
    int n = 5;
    var g = new List<List<Edge>>(n);
    for (int i = 0; i < n; i++) g.Add(new List<Edge>());

    void ADD(int u, int v, int w) => g[u].Add(new Edge(v, w));

    // Graph:
    // 0→1(10), 0→3(5)
    // 1→2(1), 1→3(2)
    // 2→4(4)
    // 3→1(3), 3→2(9), 3→4(2)
    // 4→0(7), 4→2(6)

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

    long[] dist = Dijkstra(g, 0);

    Console.WriteLine("Distances from 0: [" + string.Join(",", dist) + "]");
    Console.WriteLine("Expected: [0,8,9,5,7]\n");
}

Console.WriteLine("=== All tests complete ===");

class DSU {
    private readonly int[] parent;
    private readonly int[] rank;

    public DSU(int n) {
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    public int Find(int x) {
        if (parent[x] != x) {
            parent[x] = Find(parent[x]);
        }
        return parent[x];
    }

    public void Union(int a, int b) {
        int ra = Find(a);
        int rb = Find(b);
        if (ra == rb) return;
        if (rank[ra] < rank[rb]) {
            parent[ra] = rb;
        } else if (rank[ra] > rank[rb]) {
            parent[rb] = ra;
        } else {
            parent[rb] = ra;
            rank[ra]++;
        }
    }
}

class Edge(int to, int w)
{
    public int To = to;
    public int W = w;
}
