package code_samples.section11.problems.problem11_3;

import java.util.*;

public class problem11_3 {
    List<Integer> topoSortKahn(int n, List<List<Integer>> adj) {
        int[] indeg = new int[n];
        for (int u = 0; u < n; u++) {
            for (int v : adj.get(u)) {
                indeg[v]++;
            }
        }
        Queue<Integer> q = new ArrayDeque<>();
        for (int u = 0; u < n; u++) {
            if (indeg[u] == 0) q.offer(u);
        }
        List<Integer> order = new ArrayList<>();
        while (!q.isEmpty()) {
            int u = q.poll();
            order.add(u);
            for (int v : adj.get(u)) {
                if (--indeg[v] == 0) {
                    q.offer(v);
                }
            }
        }
        if (order.size() != n) {
            return Collections.emptyList(); // cycle detected
        }
        return order;
    }
    
    public static void main(String[] args) {
        problem11_3 solver = new problem11_3();

        testSimpleDAG(solver);
        testAnotherDAG(solver);
        testGraphWithCycle(solver);
    }

    // Helper to create adjacency list
    static List<List<Integer>> makeGraph(int n) {
        List<List<Integer>> g = new ArrayList<>(n);
        for (int i = 0; i < n; i++) g.add(new ArrayList<>());
        return g;
    }

    // Directed edge
    static void add(List<List<Integer>> g, int u, int v) {
        g.get(u).add(v);
    }

    static void runTest(String name, problem11_3 solver, int n, List<List<Integer>> adj) {
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
        System.out.println("=== Test 1: Simple DAG ===");

        int n = 6;
        List<List<Integer>> adj = makeGraph(n);

        // DAG:
        // 5 → 2, 5 → 0
        // 4 → 0, 4 → 1
        // 2 → 3
        // 3 → 1
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
        System.out.println("=== Test 2: Another DAG ===");

        int n = 4;
        List<List<Integer>> adj = makeGraph(n);

        // 0 → 1, 0 → 2
        // 1 → 3
        // 2 → 3
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
        System.out.println("=== Test 3: Graph with cycle ===");

        int n = 3;
        List<List<Integer>> adj = makeGraph(n);

        // 0 → 1 → 2 → 0 (cycle)
        add(adj, 0, 1);
        add(adj, 1, 2);
        add(adj, 2, 0);

        runTest("Graph with cycle", solver, n, adj);
    }
}
