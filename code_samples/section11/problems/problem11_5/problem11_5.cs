static int CountComponents(int n, int[][] edges) {
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
    int result = CountComponents(n, edges);
    Console.WriteLine(name);
    Console.Write("Edges = ");
    PrintEdges(edges);
    Console.WriteLine($"Connected components = {result} (expected {expected})\n");
}

void PrintEdges(int[][] edges)
{
    Console.Write("{ ");
    foreach (var e in edges)
        Console.Write($"({e[0]},{e[1]}) ");
    Console.WriteLine("}");
}

// =======================================
// Test 1: Single connected chain 0-1-2-3
// =======================================
{
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
    // Component A: 0-1-2
    // Component B: 3-4
    // Node 5 is isolated → 3 components total
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
    int n = 5;
    int[][] edges = [];
    RunTest("Test 3: No edges (all isolated)", n, edges, 5);
}

// =======================================
// Test 4: Fully connected with redundancy
// =======================================
{
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
    public int[] Parent;
    public int[] Rank;
    public int Count;

    public DSU(int n) {
        Parent = new int[n];
        Rank = new int[n];
        Count = n;
        for (int i = 0; i < n; i++) Parent[i] = i;
    }

    public int Find(int x) {
        if (Parent[x] != x) {
            Parent[x] = Find(Parent[x]);
        }
        return Parent[x];
    }

    public void Unite(int a, int b) {
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
