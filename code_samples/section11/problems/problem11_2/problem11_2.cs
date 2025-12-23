static void DfsComp(int u, int cid, List<List<int>> adj, int[] comp) {
    // Depth-First Search (DFS) helper that labels all nodes reachable from `u`
    // with the connected-component id `cid`.
    //
    // Parameters:
    //   u    : current node being visited
    //   cid  : component id to assign to all nodes in this connected component
    //   adj  : adjacency list for an UNDIRECTED graph; adj[u] lists neighbors of u
    //   comp : output array; comp[x] == -1 means "unvisited", otherwise stores component id
    //
    // Behavior:
    //   - Marks comp[u] = cid
    //   - Recursively visits every neighbor v that has not yet been labeled
    //
    // Note:
    //   - This is recursive DFS; extremely large/deep graphs can cause stack overflow.
    comp[u] = cid;

    foreach (var v in adj[u]) {
        // If v is unvisited, it belongs to the same component.
        if (comp[v] == -1) {
            DfsComp(v, cid, adj, comp);
        }
    }
}

static int ConnectedComponents(int n, List<List<int>> adj, int[] comp) {
    // Computes connected components in an UNDIRECTED graph using DFS labeling.
    //
    // Parameters:
    //   n    : number of nodes (assumed labeled 0..n-1)
    //   adj  : adjacency list
    //   comp : caller-provided output array of length n; will be overwritten
    //
    // Returns:
    //   cid  : the number of connected components discovered
    //
    // Approach:
    //   - Initialize comp[] to -1 (meaning "unvisited")
    //   - For each node u:
    //       - if unvisited, start DFS from u and label the entire component with cid
    //       - increment cid after finishing that component
    Array.Fill(comp, -1);

    int cid = 0;

    for (int u = 0; u < n; u++) {
        if (comp[u] == -1) {
            DfsComp(u, cid, adj, comp);
            cid++;
        }
    }

    // When done, cid equals the total number of components.
    return cid;
}

// ===== Helper Functions =====

List<List<int>> MakeGraph(int n)
{
    // Allocates an empty adjacency list for a graph with n nodes.
    //
    // Returns:
    //   g : List<List<int>> where g[i] is the (initially empty) neighbor list for node i
    //
    // Note:
    //   - This sets up structure only; edges are added with AddEdge().
    var g = new List<List<int>>(n);

    // Pre-populate so g[i] is valid for every node.
    for (int i = 0; i < n; i++)
        g.Add([]); // target-typed empty list (modern C# collection expression)
    return g;
}

void AddEdge(List<List<int>> adj, int u, int v)
{
    // Adds an UNDIRECTED edge between u and v.
    //
    // Parameters:
    //   adj : adjacency list
    //   u,v : endpoints (assumed valid indices)
    //
    // Behavior:
    //   - Adds v to adj[u] and u to adj[v].
    //   - Parallel edges / self-loops are allowed if provided.
    adj[u].Add(v);
    adj[v].Add(u);
}

void RunTest(string name, int n, List<List<int>> adj)
{
    // Runs the connected-components algorithm on the provided graph and prints:
    //   - the number of components
    //   - the component id assigned to each node
    //
    // Parameters:
    //   name : label printed for the test case
    //   n    : number of nodes
    //   adj  : adjacency list
    //
    // Note:
    //   - This is a "print and inspect" harness; it does not assert correctness.
    Console.WriteLine(name);

    // Allocate comp[] for this run; ConnectedComponents will fill it.
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

    // Build a clique (complete graph) by adding an edge between every pair (u, v), u < v.
    // A complete graph is always a single connected component.
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

    // Graph intended:
    //   - component 0: {0,1,2}
    //   - component 1: {3,4}
    //   - component 2: {5} (isolated)
    // Total expected components: 3
    int n = 6;
    var adj = MakeGraph(n);

    // Component 0: 0-1-2
    AddEdge(adj, 0, 1);
    AddEdge(adj, 1, 2);

    // Component 1: 3-4
    AddEdge(adj, 3, 4);

    // Node 5 isolated (no edges added)

    RunTest("Two components plus isolated", n, adj);
}

// ===============================
// Test 3: All Nodes Isolated
// ===============================

{
    Console.WriteLine("=== Test 3: All nodes isolated ===");

    // No edges => each node is its own component.
    // With n=4, expected component count is 4.
    int n = 4;
    var adj = MakeGraph(n); // no edges

    RunTest("4 isolated nodes", n, adj);
}

// ===============================
// Test 4: Mixed Graph
// ===============================

{
    Console.WriteLine("=== Test 4: Mixed graph ===");

    // Graph intended:
    //   - component 0: {0,1,2}
    //   - component 1: {3,4,5}
    //   - component 2: {6} isolated
    // Total expected components: 3
    int n = 7;
    var adj = MakeGraph(n);

    // Component 0: 0-1-2
    AddEdge(adj, 0, 1);
    AddEdge(adj, 1, 2);

    // Component 1: 3-4-5
    AddEdge(adj, 3, 4);
    AddEdge(adj, 4, 5);

    // Node 6 isolated (no edges added)

    RunTest("Mixed graph", n, adj);
}
