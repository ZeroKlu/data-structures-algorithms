static void DfsComp(int u, int cid, List<List<int>> adj, int[] comp) {
    comp[u] = cid;
    foreach (var v in adj[u]) {
        if (comp[v] == -1) {
            DfsComp(v, cid, adj, comp);
        }
    }
}

static int ConnectedComponents(int n, List<List<int>> adj, int[] comp) {
    Array.Fill(comp, -1);
    int cid = 0;
    for (int u = 0; u < n; u++) {
        if (comp[u] == -1) {
            DfsComp(u, cid, adj, comp);
            cid++;
        }
    }
    return cid;
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
    adj[u].Add(v);
    adj[v].Add(u);
}

void RunTest(string name, int n, List<List<int>> adj)
{
    Console.WriteLine(name);
    int[] comp = new int[n];
    int count = ConnectedComponents(n, adj, comp);

    Console.WriteLine($"Component count = {count}");
    Console.WriteLine("Component IDs   = " + string.Join(", ", comp));
    Console.WriteLine();
}

// ===============================
// Test 1: Fully Connected Graph
// ===============================

{
    Console.WriteLine("=== Test 1: Fully connected graph ===");

    int n = 5;
    var adj = MakeGraph(n);

    // Build a clique
    for (int u = 0; u < n; u++)
        for (int v = u + 1; v < n; v++)
            AddEdge(adj, u, v);

    RunTest("Complete graph", n, adj);
}

// ===============================
// Test 2: Two Components + Isolated Node
// ===============================

{
    Console.WriteLine("=== Test 2: Two components + isolated ===");

    int n = 6;
    var adj = MakeGraph(n);

    // Component 0: 0-1-2
    AddEdge(adj, 0, 1);
    AddEdge(adj, 1, 2);

    // Component 1: 3-4
    AddEdge(adj, 3, 4);

    // Node 5 isolated
    RunTest("Two components plus isolated", n, adj);
}

// ===============================
// Test 3: All Nodes Isolated
// ===============================

{
    Console.WriteLine("=== Test 3: All nodes isolated ===");

    int n = 4;
    var adj = MakeGraph(n); // no edges

    RunTest("4 isolated nodes", n, adj);
}

// ===============================
// Test 4: Mixed Graph
// ===============================

{
    Console.WriteLine("=== Test 4: Mixed graph ===");

    int n = 7;
    var adj = MakeGraph(n);

    // Component 0: 0-1-2
    AddEdge(adj, 0, 1);
    AddEdge(adj, 1, 2);

    // Component 1: 3-4-5
    AddEdge(adj, 3, 4);
    AddEdge(adj, 4, 5);

    // Node 6 isolated

    RunTest("Mixed graph", n, adj);
}
