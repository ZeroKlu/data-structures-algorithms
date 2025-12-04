package code_samples.section11.problems.problem11_1;

import java.util.*;

public class problem11_1 {
    int[] bfsShortestPaths(int n, List<List<Integer>> adj, int s) {
        int[] dist = new int[n];
        Arrays.fill(dist, -1);
        Queue<Integer> q = new ArrayDeque<>();
        dist[s] = 0;
        q.offer(s);

        while (!q.isEmpty()) {
            int u = q.poll();
            for (int v : adj.get(u)) {
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
    public static void main(String[] args) {
        problem11_1 solver = new problem11_1();

        testConnected(solver);
        testDisconnected(solver);
        testSingleNode(solver);
    }

    private static void testConnected(problem11_1 solver) {
        System.out.println("=== Test 1: Connected graph ===");

        // 0 - 1 - 2 - 4
        //     |
        //     3
        int n = 5;
        List<List<Integer>> adj = makeGraph(n);
        add(adj, 0, 1);
        add(adj, 1, 2);
        add(adj, 1, 3);
        add(adj, 2, 4);

        int[] dist = solver.bfsShortestPaths(n, adj, 0);
        System.out.println("Distances from 0: " + Arrays.toString(dist));
        System.out.println("Expected:          [0, 1, 2, 2, 3]\n");
    }

    private static void testDisconnected(problem11_1 solver) {
        System.out.println("=== Test 2: Disconnected graph ===");

        // 0 - 1    2    3 - 4
        int n = 5;
        List<List<Integer>> adj = makeGraph(n);
        add(adj, 0, 1);
        add(adj, 3, 4);

        int[] dist = solver.bfsShortestPaths(n, adj, 0);
        System.out.println("Distances from 0: " + Arrays.toString(dist));
        System.out.println("Expected:          [0, 1, -1, -1, -1]\n");
    }

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
    private static List<List<Integer>> makeGraph(int n) {
        List<List<Integer>> g = new ArrayList<>(n);
        for (int i = 0; i < n; i++)
            g.add(new ArrayList<>());
        return g;
    }

    private static void add(List<List<Integer>> adj, int u, int v) {
        adj.get(u).add(v);
        adj.get(v).add(u);
    }
}
