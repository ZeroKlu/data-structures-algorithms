package code_samples.section11.problems.problem11_3;

import java.util.*;

/*
 * Problem 11.3: Topological Sort (Kahn's Algorithm)
 * -------------------------------------------------
 * This file implements topological sorting for a DIRECTED graph using
 * Kahn's algorithm (a BFS-like approach based on indegrees).
 *
 * Topological sort basics:
 *   - A topological ordering of a directed graph is an ordering of nodes
 *     such that for every directed edge u -> v, u appears before v.
 *   - A topological ordering exists IFF the graph is a DAG (Directed Acyclic Graph).
 *   - If the graph contains a cycle, no topological ordering exists.
 *
 * Graph representation:
 *   - The graph is represented as an adjacency list: List<List<Integer>>
 *   - Nodes are assumed to be labeled 0..n-1
 *   - For each node u, adj.get(u) contains all nodes v such that there is an edge u -> v
 *
 * This class includes a small test harness that constructs a few graphs and prints:
 *   - A valid topological order for DAGs (one of many possible)
 *   - An empty result indicating a detected cycle
 */
public class problem11_3 {

    /*
     * topoSortKahn
     * ------------
     * Computes a topological ordering using Kahn's algorithm.
     *
     * Parameters:
     *   n   : number of nodes
     *   adj : adjacency list for a directed graph (u -> v stored in adj.get(u))
     *
     * Returns:
     *   - A List<Integer> containing a topological order if the graph is acyclic.
     *   - Collections.emptyList() if a cycle is detected (order cannot include all nodes).
     *
     * Kahn's algorithm steps:
     *   1) Compute indegree[v] for each node v (number of incoming edges).
     *   2) Enqueue all nodes with indegree 0 (no prerequisites).
     *   3) While the queue is not empty:
     *        - Dequeue a node u
     *        - Append u to the output order
     *        - For each edge u -> v:
     *            - decrement indegree[v]
     *            - if indegree[v] becomes 0, enqueue v
     *   4) If output order contains all n nodes, success; otherwise, a cycle exists.
     */
    List<Integer> topoSortKahn(int n, List<List<Integer>> adj) {
        // indeg[v] = number of incoming edges to v
        int[] indeg = new int[n];

        // Step 1: compute indegrees by scanning all adjacency lists
        for (int u = 0; u < n; u++) {
            for (int v : adj.get(u)) {
                indeg[v]++;
            }
        }

        // Step 2: queue all nodes that currently have indegree 0
        Queue<Integer> q = new ArrayDeque<>();
        for (int u = 0; u < n; u++) {
            if (indeg[u] == 0) q.offer(u);
        }

        // Step 3: process nodes in a BFS-like order
        List<Integer> order = new ArrayList<>();
        while (!q.isEmpty()) {
            int u = q.poll();

            // u has no remaining prerequisites, so it can be output next
            order.add(u);

            // "Remove" outgoing edges u -> v by decrementing indegree[v]
            for (int v : adj.get(u)) {
                if (--indeg[v] == 0) {
                    // v now has no incoming edges remaining, so it becomes available
                    q.offer(v);
                }
            }
        }

        // Step 4: if we didn't output all nodes, a cycle prevented completion
        if (order.size() != n) {
            return Collections.emptyList(); // cycle detected
        }

        return order; // success
    }

    public static void main(String[] args) {
        // Entry point for the test harness.
        // Creates an instance of the solver and runs all test cases.
        problem11_3 solver = new problem11_3();

        testSimpleDAG(solver);
        testAnotherDAG(solver);
        testGraphWithCycle(solver);
    }

    // Helper to create adjacency list
    static List<List<Integer>> makeGraph(int n) {
        /*
         * Allocates an empty adjacency list for a directed graph with n nodes.
         *
         * Returns:
         *   g : List<List<Integer>> where each g.get(i) is an empty neighbor list.
         */
        List<List<Integer>> g = new ArrayList<>(n);
        for (int i = 0; i < n; i++) g.add(new ArrayList<>());
        return g;
    }

    // Directed edge
    static void add(List<List<Integer>> g, int u, int v) {
        /*
         * Adds a DIRECTED edge u -> v to the graph.
         *
         * Parameters:
         *   g : adjacency list
         *   u : source node
         *   v : destination node
         *
         * Note:
         *   - No bounds checking is performed.
         *   - Parallel edges are allowed if added multiple times.
         */
        g.get(u).add(v);
    }

    static void runTest(String name, problem11_3 solver, int n, List<List<Integer>> adj) {
        /*
         * Runs topoSortKahn on the provided graph and prints the result.
         *
         * Parameters:
         *   name   : test label printed to console
         *   solver : instance containing topoSortKahn()
         *   n      : number of nodes
         *   adj    : adjacency list
         *
         * Output convention:
         *   - If the returned list is empty, we interpret it as "cycle detected".
         *     (This is correct for these tests because n > 0 and a valid topo order
         *      would otherwise have length n.)
         */
        System.out.println(name);
        List<Integer> order = solver.topoSortKahn(n, adj);

        if (order.isEmpty()) {
            System.out.println("Topological order: [] (cycle detected)\n");
        } else {
            System.out.println("Topological order: " + order + "\n");
        }
    }

    // ===============================
    // Test 1: Simple DAG
    // ===============================
    static void testSimpleDAG(problem11_3 solver) {
        /*
         * Constructs a classic DAG example:
         *   5 → 2, 5 → 0
         *   4 → 0, 4 → 1
         *   2 → 3
         *   3 → 1
         *
         * Many valid topological orders exist; the exact ordering may vary depending
         * on queue behavior, but it must respect all directed edges.
         */
        System.out.println("=== Test 1: Simple DAG ===");

        int n = 6;
        List<List<Integer>> adj = makeGraph(n);

        add(adj, 5, 2);
        add(adj, 5, 0);
        add(adj, 4, 0);
        add(adj, 4, 1);
        add(adj, 2, 3);
        add(adj, 3, 1);

        runTest("Simple DAG", solver, n, adj);
    }

    // ===============================
    // Test 2: Another DAG
    // ===============================
    static void testAnotherDAG(problem11_3 solver) {
        /*
         * DAG:
         *   0 → 1, 0 → 2
         *   1 → 3
         *   2 → 3
         *
         * Valid topological orders include:
         *   [0, 1, 2, 3] and [0, 2, 1, 3]
         */
        System.out.println("=== Test 2: Another DAG ===");

        int n = 4;
        List<List<Integer>> adj = makeGraph(n);

        add(adj, 0, 1);
        add(adj, 0, 2);
        add(adj, 1, 3);
        add(adj, 2, 3);

        runTest("Another DAG", solver, n, adj);
    }

    // ===============================
    // Test 3: Graph with Cycle
    // ===============================
    static void testGraphWithCycle(problem11_3 solver) {
        /*
         * Cyclic graph:
         *   0 → 1 → 2 → 0
         *
         * Topological ordering does not exist. The algorithm should detect
         * the cycle by failing to output all nodes, and return an empty list.
         */
        System.out.println("=== Test 3: Graph with cycle ===");

        int n = 3;
        List<List<Integer>> adj = makeGraph(n);

        add(adj, 0, 1);
        add(adj, 1, 2);
        add(adj, 2, 0);

        runTest("Graph with cycle", solver, n, adj);
    }
}
