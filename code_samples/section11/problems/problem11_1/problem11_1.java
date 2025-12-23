package code_samples.section11.problems.problem11_1;

import java.util.*;

/*
 * problem11_1
 * -----------
 * This class demonstrates how to compute shortest-path distances in an
 * UNWEIGHTED graph using Breadth-First Search (BFS).
 *
 * The file also contains a small test harness that exercises the BFS logic
 * on several graphs:
 *   1) A connected graph
 *   2) A disconnected graph
 *   3) A single-node graph
 *
 * Graph representation:
 *   - The graph is represented as an adjacency list:
 *       List<List<Integer>>
 *   - Vertices are assumed to be labeled 0..n-1.
 */
public class problem11_1 {

    /*
     * bfsShortestPaths
     * ----------------
     * Computes shortest-path distances from a single source vertex `s`
     * in an UNWEIGHTED graph using BFS.
     *
     * Parameters:
     *   n   : number of vertices (assumed labeled 0..n-1)
     *   adj : adjacency list; adj.get(u) contains neighbors of vertex u
     *   s   : source vertex
     *
     * Returns:
     *   dist : int array of length n where
     *          - dist[v] is the minimum number of edges from s to v
     *          - dist[v] == -1 if v is unreachable from s
     *
     * Why BFS works:
     *   - In unweighted graphs, BFS explores vertices in layers of increasing
     *     distance from the source.
     *   - The first time a vertex is discovered, the path used is guaranteed
     *     to be a shortest path.
     */
    int[] bfsShortestPaths(int n, List<List<Integer>> adj, int s) {
        // Distance array initialized to -1 (meaning "unvisited/unreachable").
        int[] dist = new int[n];
        Arrays.fill(dist, -1);

        // Queue used to process vertices in FIFO order (BFS frontier).
        Queue<Integer> q = new ArrayDeque<>();

        // Initialize BFS with the source vertex.
        dist[s] = 0;   // distance from source to itself is 0
        q.offer(s);    // enqueue the source

        // Standard BFS loop.
        while (!q.isEmpty()) {
            int u = q.poll();  // dequeue next vertex to expand

            // Examine all neighbors of u.
            for (int v : adj.get(u)) {
                // If v has not been visited yet, assign distance and enqueue it.
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.offer(v);
                }
            }
        }

        return dist;
    }

    // ===============================
    // Test Harness
    // ===============================

    /*
     * main
     * ----
     * Entry point for running the BFS tests.
     * Each test builds a graph, runs bfsShortestPaths, and prints
     * the computed distances alongside the expected result.
     */
    public static void main(String[] args) {
        problem11_1 solver = new problem11_1();

        testConnected(solver);
        testDisconnected(solver);
        testSingleNode(solver);
    }

    /*
     * testConnected
     * -------------
     * Tests BFS on a connected undirected graph.
     *
     * Graph:
     *   0 - 1 - 2 - 4
     *       |
     *       3
     *
     * From source 0, expected distances:
     *   [0, 1, 2, 2, 3]
     */
    private static void testConnected(problem11_1 solver) {
        System.out.println("=== Test 1: Connected graph ===");

        int n = 5;
        List<List<Integer>> adj = makeGraph(n);

        // Add undirected edges.
        add(adj, 0, 1);
        add(adj, 1, 2);
        add(adj, 1, 3);
        add(adj, 2, 4);

        int[] dist = solver.bfsShortestPaths(n, adj, 0);
        System.out.println("Distances from 0: " + Arrays.toString(dist));
        System.out.println("Expected:          [0, 1, 2, 2, 3]\n");
    }

    /*
     * testDisconnected
     * ----------------
     * Tests BFS on a graph with multiple disconnected components.
     *
     * Graph:
     *   0 - 1    2    3 - 4
     *
     * From source 0:
     *   - Nodes 0 and 1 are reachable
     *   - Nodes 2, 3, and 4 are unreachable
     *
     * Expected distances:
     *   [0, 1, -1, -1, -1]
     */
    private static void testDisconnected(problem11_1 solver) {
        System.out.println("=== Test 2: Disconnected graph ===");

        int n = 5;
        List<List<Integer>> adj = makeGraph(n);

        // Add edges for two separate components.
        add(adj, 0, 1);
        add(adj, 3, 4);

        int[] dist = solver.bfsShortestPaths(n, adj, 0);
        System.out.println("Distances from 0: " + Arrays.toString(dist));
        System.out.println("Expected:          [0, 1, -1, -1, -1]\n");
    }

    /*
     * testSingleNode
     * --------------
     * Tests BFS on the smallest possible graph: a single node with no edges.
     *
     * Graph:
     *   n = 1
     *
     * From source 0:
     *   dist = [0]
     */
    private static void testSingleNode(problem11_1 solver) {
        System.out.println("=== Test 3: Single node ===");

        int n = 1;
        List<List<Integer>> adj = makeGraph(n); // no edges

        int[] dist = solver.bfsShortestPaths(n, adj, 0);
        System.out.println("Distances from 0: " + Arrays.toString(dist));
        System.out.println("Expected:          [0]\n");
    }

    // ===============================
    // Helper functions
    // ===============================

    /*
     * makeGraph
     * ---------
     * Allocates an empty adjacency list for a graph with n vertices.
     *
     * Parameters:
     *   n : number of vertices
     *
     * Returns:
     *   A List<List<Integer>> where each inner list initially contains no neighbors.
     */
    private static List<List<Integer>> makeGraph(int n) {
        List<List<Integer>> g = new ArrayList<>(n);
        for (int i = 0; i < n; i++)
            g.add(new ArrayList<>());
        return g;
    }

    /*
     * add
     * ---
     * Adds an UNDIRECTED edge between vertices u and v.
     *
     * Parameters:
     *   adj : adjacency list
     *   u,v : endpoints of the edge
     *
     * Behavior:
     *   - Adds v to adj[u] and u to adj[v].
     *   - No bounds checking is performed.
     */
    private static void add(List<List<Integer>> adj, int u, int v) {
        adj.get(u).add(v);
        adj.get(v).add(u);
    }
}
