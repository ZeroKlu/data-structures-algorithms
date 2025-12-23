package code_samples.section11.problems.problem11_2;

import java.util.*;

/*
 * problem11_2
 * -----------
 * This class demonstrates how to find / label connected components in an
 * UNDIRECTED graph using Depth-First Search (DFS).
 *
 * High-level idea:
 *   - Maintain an array comp[] where comp[u] is the component id for node u.
 *   - Initialize comp[] to -1 (meaning "unvisited").
 *   - For each node u:
 *       - if comp[u] == -1, start a DFS from u and label every reachable node
 *         with the current component id.
 *       - increment component id and continue scanning.
 *
 * Graph representation:
 *   - The graph is an adjacency list: List<List<Integer>>
 *   - Nodes are assumed labeled 0..n-1.
 *
 * Test harness:
 *   - Builds several graphs (clique, two components + isolated, isolated nodes, mixed)
 *   - Prints component count and the component id assigned to each node.
 */
public class problem11_2 {

    /*
     * dfsComp
     * -------
     * Recursive DFS helper that labels all nodes reachable from `u`
     * with a given component id `cid`.
     *
     * Parameters:
     *   u    : current node being visited
     *   cid  : component id to assign to all nodes in this connected component
     *   adj  : adjacency list for an UNDIRECTED graph (adj.get(u) lists neighbors)
     *   comp : component id array; comp[x] == -1 indicates "unvisited"
     *
     * Behavior:
     *   - Sets comp[u] = cid
     *   - Recursively visits any neighbor v that is still unvisited (comp[v] == -1)
     *
     * Note:
     *   - This implementation is recursive. On very large/deep graphs it could
     *     overflow the call stack; an explicit stack can be used instead if needed.
     */
    void dfsComp(int u, int cid, List<List<Integer>> adj, int[] comp) {
        comp[u] = cid; // mark u as belonging to component cid

        // Explore all neighbors of u.
        for (int v : adj.get(u)) {
            // If neighbor has not been labeled yet, visit it.
            if (comp[v] == -1) {
                dfsComp(v, cid, adj, comp);
            }
        }
    }

    /*
     * connectedComponents
     * -------------------
     * Labels and counts connected components in an UNDIRECTED graph.
     *
     * Parameters:
     *   n    : number of nodes
     *   adj  : adjacency list
     *   comp : output array of length n; will be overwritten with component ids
     *
     * Returns:
     *   cid  : number of connected components found
     *
     * Approach:
     *   - Fill comp[] with -1 to indicate all nodes are initially unvisited.
     *   - Scan nodes 0..n-1:
     *       - if a node is unvisited, start a DFS to label its entire component.
     *       - increment cid after finishing each component.
     */
    int connectedComponents(int n, List<List<Integer>> adj, int[] comp) {
        Arrays.fill(comp, -1); // mark all nodes as unvisited

        int cid = 0; // current component id; also ends as the component count

        for (int u = 0; u < n; u++) {
            // If u has not been assigned a component yet, it begins a new component.
            if (comp[u] == -1) {
                dfsComp(u, cid, adj, comp);
                cid++;
            }
        }
        return cid;
    }

    /*
     * main
     * ----
     * Runs the provided test cases.
     */
    public static void main(String[] args) {
        problem11_2 solver = new problem11_2();

        testFullyConnected(solver);
        testTwoComponents(solver);
        testIsolatedNodes(solver);
        testMixedGraph(solver);
    }

    // Helper: create adjacency list
    static List<List<Integer>> makeGraph(int n) {
        /*
         * Allocates an empty adjacency list for a graph with n nodes.
         *
         * Returns:
         *   g : List<List<Integer>> where each g.get(i) is an initially empty list
         *       of neighbors for node i.
         */
        List<List<Integer>> g = new ArrayList<>(n);
        for (int i = 0; i < n; i++) g.add(new ArrayList<>());
        return g;
    }

    // Helper: undirected edge
    static void addEdge(List<List<Integer>> adj, int u, int v) {
        /*
         * Adds an UNDIRECTED edge between u and v.
         *
         * Behavior:
         *   - Adds v to u's neighbor list
         *   - Adds u to v's neighbor list
         *
         * Notes:
         *   - No bounds checking is performed on u and v.
         *   - Parallel edges/self-loops are allowed if provided.
         */
        adj.get(u).add(v);
        adj.get(v).add(u);
    }

    static void runTest(String name, problem11_2 solver, int n, List<List<Integer>> adj) {
        /*
         * Runs connected component labeling on the provided graph and prints:
         *   - test name
         *   - component count
         *   - component id array
         *
         * Note:
         *   - This is a "print and inspect" test harness (no assertions).
         */
        System.out.println(name);

        int[] comp = new int[n];
        int count = solver.connectedComponents(n, adj, comp);

        System.out.println("Component count = " + count);
        System.out.println("Component IDs   = " + Arrays.toString(comp));
        System.out.println();
    }

    // ======================
    // Test cases
    // ======================

    static void testFullyConnected(problem11_2 solver) {
        /*
         * Test 1: Fully connected graph (clique).
         *
         * A clique on n nodes has exactly 1 connected component,
         * because every node is reachable from every other node.
         */
        int n = 5;
        List<List<Integer>> adj = makeGraph(n);

        // clique: add every edge (u, v) for u < v
        for (int u = 0; u < n; u++)
            for (int v = u + 1; v < n; v++)
                addEdge(adj, u, v);

        runTest("=== Test 1: Fully connected graph ===", solver, n, adj);
    }

    static void testTwoComponents(problem11_2 solver) {
        /*
         * Test 2: Two components + isolated node.
         *
         * Graph structure:
         *   - component A: 0-1-2
         *   - component B: 3-4
         *   - component C: {5} isolated
         *
         * Expected component count: 3.
         */
        int n = 6;
        List<List<Integer>> adj = makeGraph(n);

        // component A: 0-1-2
        addEdge(adj, 0, 1);
        addEdge(adj, 1, 2);

        // component B: 3-4
        addEdge(adj, 3, 4);

        // node 5 isolated (no edges added)

        runTest("=== Test 2: Two components + isolated ===", solver, n, adj);
    }

    static void testIsolatedNodes(problem11_2 solver) {
        /*
         * Test 3: All nodes isolated.
         *
         * No edges => each node is its own component.
         * For n = 4, expected component count: 4.
         */
        int n = 4;
        List<List<Integer>> adj = makeGraph(n); // no edges

        runTest("=== Test 3: All nodes isolated ===", solver, n, adj);
    }

    static void testMixedGraph(problem11_2 solver) {
        /*
         * Test 4: Mixed graph.
         *
         * Graph structure:
         *   - component A: 0-1-2
         *   - component B: 3-4-5
         *   - component C: {6} isolated
         *
         * Expected component count: 3.
         */
        int n = 7;
        List<List<Integer>> adj = makeGraph(n);

        // Component A: 0-1-2
        addEdge(adj, 0, 1);
        addEdge(adj, 1, 2);

        // Component B: 3-4-5
        addEdge(adj, 3, 4);
        addEdge(adj, 4, 5);

        // Node 6 isolated

        runTest("=== Test 4: Mixed graph ===", solver, n, adj);
    }
}
