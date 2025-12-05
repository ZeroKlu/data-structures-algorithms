package code_samples.section11.problems.problem11_5;

class DSU {
    int[] parent;
    int[] rank;
    int count;

    DSU(int n) {
        parent = new int[n];
        rank = new int[n];
        count = n;
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;
        if (rank[ra] < rank[rb]) {
            parent[ra] = rb;
        } else if (rank[ra] > rank[rb]) {
            parent[rb] = ra;
        } else {
            parent[rb] = ra;
            rank[ra]++;
        }
        count--;
    }
}

public class problem11_5 {

    int countComponents(int n, int[][] edges) {
        DSU d = new DSU(n);
        for (int[] e : edges) {
            d.unite(e[0], e[1]);
        }
        return d.count;
    }

    // ==========================================
    // TEST HARNESS (inside the same class)
    // ==========================================
    public void runTests() {
        testSingleComponent();
        testMultipleComponents();
        testNoEdges();
        testFullyConnected();
    }

    private void runTest(String name, int n, int[][] edges, int expected) {
        int result = countComponents(n, edges);
        System.out.println(name);
        System.out.print("edges = ");
        printEdges(edges);
        System.out.println("Result: " + result + " (expected " + expected + ")");
        System.out.println();
    }

    private void printEdges(int[][] edges) {
        System.out.print("{ ");
        for (int[] e : edges) {
            System.out.print("(" + e[0] + "," + e[1] + ") ");
        }
        System.out.println("}");
    }

    // =============== Test Cases ===============

    // Test 1: Single chain (0-1-2-3)
    private void testSingleComponent() {
        int n = 4;
        int[][] edges = {
            {0, 1},
            {1, 2},
            {2, 3}
        };
        runTest("Test 1: Single component chain", n, edges, 1);
    }

    // Test 2: Two components + isolated node
    private void testMultipleComponents() {
        int n = 6;
        int[][] edges = {
            {0, 1}, {1, 2}, // Component 1
            {3, 4}          // Component 2
            // Node 5 isolated
        };
        runTest("Test 2: Two components plus isolated node", n, edges, 3);
    }

    // Test 3: No edges
    private void testNoEdges() {
        int n = 5;
        int[][] edges = {}; // empty
        runTest("Test 3: No edges (all isolated)", n, edges, 5);
    }

    // Test 4: Fully connected (redundant edges)
    private void testFullyConnected() {
        int n = 5;
        int[][] edges = {
            {0, 1},
            {1, 2},
            {2, 3},
            {3, 4},
            {1, 3} // extra redundant link
        };
        runTest("Test 4: Fully connected with redundant edges", n, edges, 1);
    }
}
