static IList<int> TopoSortKahn(int n, List<List<int>> adj) {
    // Computes a topological ordering of a DIRECTED graph using Kahn's algorithm.
    //
    // Parameters:
    //   n   : number of nodes (assumed labeled 0..n-1)
    //   adj : adjacency list for a directed graph; edges are stored as u -> v in adj[u]
    //
    // Returns:
    //   - A list containing a valid topological order if the graph is a DAG.
    //   - An empty array (Array.Empty<int>()) if a cycle is detected (no topo order exists).
    //
    // Kahn's algorithm (high level):
    //   1) Compute indegree[v] for each node v.
    //   2) Enqueue all nodes with indegree 0 (no prerequisites).
    //   3) Repeatedly dequeue a node u, append it to the output ordering, and
    //      decrement indegree of all outgoing neighbors v.
    //      If indegree[v] becomes 0, enqueue v.
    //   4) If output contains all nodes, success; otherwise the graph has a cycle.
    var indeg = new int[n];

    // Step 1: Compute indegree for each node by scanning all outgoing edges u -> v.
    for (int u = 0; u < n; u++) {
        foreach (var v in adj[u]) indeg[v]++;
    }

    // Step 2: Initialize queue with all nodes that have indegree 0.
    var q = new Queue<int>();
    for (int u = 0; u < n; u++) {
        if (indeg[u] == 0) q.Enqueue(u);
    }

    // Step 3: Process nodes in a BFS-like manner.
    var order = new List<int>();
    while (q.Count > 0) {
        int u = q.Dequeue();

        // u currently has no remaining prerequisites, so it can appear next.
        order.Add(u);

        // "Remove" outgoing edges u -> v by decrementing indegree[v].
        foreach (var v in adj[u]) {
            indeg[v]--;
            if (indeg[v] == 0) q.Enqueue(v);
        }
    }

    // Step 4: If we didn't output all nodes, a cycle prevented some nodes from
    // ever reaching indegree 0.
    if (order.Count != n) return Array.Empty<int>();

    return order;
}

// ===== Helper Functions =====

List<List<int>> MakeGraph(int n)
{
    // Allocates an empty adjacency list for a directed graph with n nodes.
    //
    // Returns:
    //   g : List<List<int>> where each g[i] is an initially empty list of
    //       outgoing neighbors for node i.
    var g = new List<List<int>>(n);
    for (int i = 0; i < n; i++)
        g.Add(new List<int>());
    return g;
}

void AddEdge(List<List<int>> adj, int u, int v)
{
    // Adds a DIRECTED edge u -> v to the adjacency list.
    //
    // Parameters:
    //   adj : adjacency list
    //   u   : source node
    //   v   : destination node
    //
    // Note:
    //   - No bounds checking is performed for u and v.
    //   - Parallel edges are allowed if called repeatedly with the same (u, v).
    adj[u].Add(v); // directed edge u -> v
}

void RunTest(string name, int n, List<List<int>> adj)
{
    // Runs TopoSortKahn on the provided graph and prints the result.
    //
    // Parameters:
    //   name : test label printed to console
    //   n    : number of nodes
    //   adj  : adjacency list
    //
    // Output convention:
    //   - If result.Count == 0, treat as "cycle detected" (no topological ordering).
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
    //
    // Multiple valid topological orders exist; the exact output may vary
    // depending on queue insertion order, but it must respect all edges.
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

    // DAG:
    // 0 → 1, 0 → 2
    // 1 → 3
    // 2 → 3
    //
    // Valid topological orders include:
    //   [0, 1, 2, 3] or [0, 2, 1, 3]
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

    // Cycle:
    // 0 → 1 → 2 → 0
    //
    // Cyclic graphs do not have a topological order, so TopoSortKahn should
    // return an empty result here.
    AddEdge(adj, 0, 1);
    AddEdge(adj, 1, 2);
    AddEdge(adj, 2, 0);

    RunTest("Cyclic graph", n, adj);
}
