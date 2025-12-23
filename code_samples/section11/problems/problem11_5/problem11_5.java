package code_samples.section11.problems.problem11_5;

/*
 * DSU (Disjoint Set Union) / Union-Find data structure.
 *
 * Purpose:
 *   Efficiently maintain a collection of disjoint sets over elements {0..n-1},
 *   supporting:
 *     - find(x):    find the representative (root) of x's set
 *     - unite(a,b): merge the sets containing a and b
 *
 * Optimizations used:
 *   - Path compression in find(): flattens the tree structure over time
 *   - Union by rank in unite(): keeps trees shallow
 *
 * Additional field:
 *   - count: tracks the current number of connected components
 */
class DSU {
    int[] parent; // parent[i] is the parent of i; roots satisfy parent[i] == i
    int[] rank;   // rank heuristic (approximate tree height)
    int count;    // number of disjoint sets currently present

    DSU(int n) {
        /*
         * Initialize DSU with n singleton sets:
         *   - each node is its own parent
         *   - all ranks start at 0
         *   - component count starts at n
         */
        parent = new int[n];
        rank = new int[n];
        count = n;
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        /*
         * Find the representative (root) of x's set.
         *
         * Path compression:
         *   If x is not a root, recursively find the root and
         *   update parent[x] to point directly to it.
         *   This makes future find operations faster.
         */
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int a, int b) {
        /*
         * Merge the sets containing elements a and b.
         *
         * Steps:
         *   1) Find roots ra and rb.
         *   2) If ra == rb, the elements are already connected; do nothing.
         *   3) Otherwise, attach the smaller-rank tree under the larger-rank tree.
         *   4) If ranks are equal, choose one root and increment its rank.
         *   5) Decrement the component count after a successful merge.
         */
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
        count--; // one fewer connected component after a merge
    }
}

public class problem11_5 {

    int countComponents(int n, int[][] edges) {
        /*
         * Count the number of connected components in an undirected graph.
         *
         * Parameters:
         *   n     : number of nodes labeled 0..n-1
         *   edges : list of undirected edges [u, v]
         *
         * Approach:
         *   - Initialize DSU with n singleton components
         *   - For each edge (u, v), union their sets
         *   - The DSU's count field tracks remaining components
         *
         * Time complexity:
         *   ~O((n + m) α(n)), where m = edges.length and α is inverse Ackermann
         */
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
        /*
         * Executes all predefined test cases to validate correctness.
         */
        testSingleComponent();
        testMultipleComponents();
        testNoEdges();
        testFullyConnected();
    }

    private void runTest(String name, int n, int[][] edges, int expected) {
        /*
         * Helper to run a single test:
         *   - Calls countComponents
         *   - Prints the edge list
         *   - Prints the result and expected value
         */
        int result = countComponents(n, edges);
        System.out.println(name);
        System.out.print("edges = ");
        printEdges(edges);
        System.out.println("Result: " + result + " (expected " + expected + ")");
        System.out.println();
    }

    private void printEdges(int[][] edges) {
        /*
         * Pretty-print the list of edges in the form:
         *   { (u,v) (u,v) ... }
         *
         * Used only for readable test output.
         */
        System.out.print("{ ");
        for (int[] e : edges) {
            System.out.print("(" + e[0] + "," + e[1] + ") ");
        }
        System.out.println("}");
    }

    // =============== Test Cases ===============

    // Test 1: Single chain (0-1-2-3)
    private void testSingleComponent() {
        /*
         * Graph:
         *   0 - 1 - 2 - 3
         * All nodes are connected.
         * Expected number of components: 1
         */
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
        /*
         * Graph:
         *   Component A: 0 - 1 - 2
         *   Component B: 3 - 4
         *   Node 5 is isolated
         * Expected number of components: 3
         */
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
        /*
         * Graph:
         *   n = 5, no edges
         * Every node is isolated.
         * Expected number of components: 5
         */
        int n = 5;
        int[][] edges = {}; // empty
        runTest("Test 3: No edges (all isolated)", n, edges, 5);
    }

    // Test 4: Fully connected (redundant edges)
    private void testFullyConnected() {
        /*
         * Graph:
         *   0 - 1 - 2 - 3 - 4
         *   plus an extra edge 1 - 3
         * All nodes remain in a single connected component.
         * Expected number of components: 1
         */
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
