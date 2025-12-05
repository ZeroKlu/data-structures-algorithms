package code_samples.section11.problems.problem11_2;

import java.util.*;

public class problem11_2 {
    void dfsComp(int u, int cid, List<List<Integer>> adj, int[] comp) {
        comp[u] = cid;
        for (int v : adj.get(u)) {
            if (comp[v] == -1) {
                dfsComp(v, cid, adj, comp);
            }
        }
    }

    int connectedComponents(int n, List<List<Integer>> adj, int[] comp) {
        Arrays.fill(comp, -1);
        int cid = 0;
        for (int u = 0; u < n; u++) {
            if (comp[u] == -1) {
                dfsComp(u, cid, adj, comp);
                cid++;
            }
        }
        return cid;
    }
    
    public static void main(String[] args) {
        problem11_2 solver = new problem11_2();

        testFullyConnected(solver);
        testTwoComponents(solver);
        testIsolatedNodes(solver);
        testMixedGraph(solver);
    }

    // Helper: create adjacency list
    static List<List<Integer>> makeGraph(int n) {
        List<List<Integer>> g = new ArrayList<>(n);
        for (int i = 0; i < n; i++) g.add(new ArrayList<>());
        return g;
    }

    // Helper: undirected edge
    static void addEdge(List<List<Integer>> adj, int u, int v) {
        adj.get(u).add(v);
        adj.get(v).add(u);
    }

    static void runTest(String name, problem11_2 solver, int n, List<List<Integer>> adj) {
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
        int n = 5;
        List<List<Integer>> adj = makeGraph(n);

        // clique
        for (int u = 0; u < n; u++)
            for (int v = u + 1; v < n; v++)
                addEdge(adj, u, v);

        runTest("=== Test 1: Fully connected graph ===", solver, n, adj);
    }

    static void testTwoComponents(problem11_2 solver) {
        int n = 6;
        List<List<Integer>> adj = makeGraph(n);

        // component 0: 0-1-2
        addEdge(adj, 0, 1);
        addEdge(adj, 1, 2);

        // component 1: 3-4
        addEdge(adj, 3, 4);

        // node 5 isolated

        runTest("=== Test 2: Two components + isolated ===", solver, n, adj);
    }

    static void testIsolatedNodes(problem11_2 solver) {
        int n = 4;
        List<List<Integer>> adj = makeGraph(n); // no edges

        runTest("=== Test 3: All nodes isolated ===", solver, n, adj);
    }

    static void testMixedGraph(problem11_2 solver) {
        int n = 7;
        List<List<Integer>> adj = makeGraph(n);

        // Component 0: 0-1-2
        addEdge(adj, 0, 1);
        addEdge(adj, 1, 2);

        // Component 1: 3-4-5
        addEdge(adj, 3, 4);
        addEdge(adj, 4, 5);

        // Node 6 isolated

        runTest("=== Test 4: Mixed graph ===", solver, n, adj);
    }
}
