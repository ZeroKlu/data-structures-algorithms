static int CountComponents(int n, int[][] edges) {
    /*
     * CountComponents
     *
     * Purpose:
     *   Count how many connected components exist in an undirected graph with:
     *     - n nodes labeled [0..n-1]
     *     - edges given as pairs [u, v]
     *
     * Approach:
     *   Uses DSU (Disjoint Set Union / Union-Find):
     *     - Start with n singleton sets (each node is its own component).
     *     - For each undirected edge (u, v), union their sets.
     *     - The DSU's Count tracks how many components remain.
     *
     * Complexity:
     *   ~O((n + m) α(n)) where m = edges.Length, α is inverse Ackermann (tiny).
     *
     * Assumptions:
     *   - Each e in edges is an int[2] where e[0], e[1] are valid node indices.
     */
    var d = new DSU(n);
    foreach (var e in edges) {
        d.Unite(e[0], e[1]);
    }
    return d.Count;
}

// ===========================
// Test Harness for CountComponents
// ===========================

void RunTest(string name, int n, int[][] edges, int expected)
{
    /*
     * Runs a single test case:
     *   - Calls CountComponents on the provided (n, edges)
     *   - Prints the edges and result alongside the expected answer
     */
    int result = CountComponents(n, edges);
    Console.WriteLine(name);
    Console.Write("Edges = ");
    PrintEdges(edges);
    Console.WriteLine($"Connected components = {result} (expected {expected})\n");
}

void PrintEdges(int[][] edges)
{
    /*
     * Pretty-prints the edge list as:
     *   { (u,v) (u,v) ... }
     *
     * Note:
     *   This is only for test output readability; it does not affect logic.
     */
    Console.Write("{ ");
    foreach (var e in edges)
        Console.Write($"({e[0]},{e[1]}) ");
    Console.WriteLine("}");
}

// =======================================
// Test 1: Single connected chain 0-1-2-3
// =======================================
{
    /*
     * Graph:
     *   0 - 1 - 2 - 3
     * All nodes connected, so there is exactly 1 component.
     */
    int n = 4;
    int[][] edges = [
        [0, 1],
        [1, 2],
        [2, 3]
    ];
    RunTest("Test 1: Single component chain", n, edges, 1);
}

// =======================================
// Test 2: Two components + isolated node
// =======================================
{
    /*
     * Graph:
     *   Component A: 0 - 1 - 2
     *   Component B: 3 - 4
     *   Node 5: isolated
     *
     * Total components = 3.
     */
    int n = 6;
    int[][] edges = [
        [0, 1],
        [1, 2],
        [3, 4]
    ];
    RunTest("Test 2: Two components plus isolated node", n, edges, 3);
}

// =======================================
// Test 3: No edges → all nodes isolated
// =======================================
{
    /*
     * Graph:
     *   n = 5, edges = []
     * No edges means every node is isolated → 5 components.
     */
    int n = 5;
    int[][] edges = [];
    RunTest("Test 3: No edges (all isolated)", n, edges, 5);
}

// =======================================
// Test 4: Fully connected with redundancy
// =======================================
{
    /*
     * Graph:
     *   0 - 1 - 2 - 3 - 4 plus an extra edge 1 - 3
     * Still all nodes are connected, so components = 1.
     * The edge (1,3) is redundant (it creates a cycle).
     */
    int n = 5;
    int[][] edges = [
        [0, 1],
        [1, 2],
        [2, 3],
        [3, 4],
        [1, 3] // redundant link
    ];
    RunTest("Test 4: Fully connected with redundant edges", n, edges, 1);
}


class DSU {
    /*
     * DSU (Disjoint Set Union) / Union-Find
     *
     * Data:
     *   Parent[i] : parent pointer for node i; roots satisfy Parent[i] == i
     *   Rank[i]   : heuristic upper-bound-ish measure of tree height (for union-by-rank)
     *   Count     : number of disjoint sets currently represented
     *
     * Key operations:
     *   - Find(x): returns the root representative of x's set (with path compression)
     *   - Unite(a,b): merges sets (union by rank) and decrements Count if merge happens
     */
    public int[] Parent;
    public int[] Rank;
    public int Count;

    public DSU(int n) {
        /*
         * Initialize n singleton sets:
         *   Parent[i] = i, Rank[i] = 0, Count = n
         */
        Parent = new int[n];
        Rank = new int[n];
        Count = n;
        for (int i = 0; i < n; i++) Parent[i] = i;
    }

    public int Find(int x) {
        /*
         * Find with path compression:
         *   If x is not a root, recursively find the root and rewrite Parent[x]
         *   to point directly to it. This flattens the structure over time.
         */
        if (Parent[x] != x) {
            Parent[x] = Find(Parent[x]);
        }
        return Parent[x];
    }

    public void Unite(int a, int b) {
        /*
         * Union by rank:
         *   - Find roots ra, rb
         *   - If same root, already connected → no change
         *   - Otherwise, attach smaller-rank tree under larger-rank tree
         *   - If ranks equal, pick one root and increment its rank
         *   - Decrement Count exactly once per successful merge
         */
        int ra = Find(a);
        int rb = Find(b);
        if (ra == rb) return;

        if (Rank[ra] < Rank[rb]) {
            Parent[ra] = rb;
        } else if (Rank[ra] > Rank[rb]) {
            Parent[rb] = ra;
        } else {
            Parent[rb] = ra;
            Rank[ra]++;
        }
        Count--;
    }
}
