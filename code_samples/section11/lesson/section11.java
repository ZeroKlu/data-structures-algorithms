package code_samples.section11.lesson;

import java.util.*; // Pulls in common utilities used throughout: List/ArrayList, Queue/ArrayDeque,
                    // Arrays, Set/HashSet, PriorityQueue, Comparator, etc.

/*
 * DSU (Disjoint Set Union) / Union-Find
 * ------------------------------------
 * Maintains a partition of {0..n-1} into disjoint sets and supports:
 *   - find(x): returns the representative/root for x's set (with path compression)
 *   - unionSet(a,b): merges the sets containing a and b (with union by rank)
 *
 * Typical use cases:
 *   - Connectivity queries (e.g., "are u and v in the same component?")
 *   - Kruskal's algorithm for MST
 *   - Dynamic connectivity (offline) problems
 *
 * Performance:
 *   - Amortized near-constant per operation (inverse Ackermann).
 */
class DSU {
    private int[] parent; // parent[i] is i's parent in the union-find forest; roots have parent[i] == i
    private int[] rank;   // rank heuristic (approximate tree height upper bound) to keep trees shallow

    DSU(int n) {
        // Initialize n singleton sets: each element is its own parent/root.
        parent = new int[n];
        rank = new int[n];           // defaults to 0, representing height 0 trees initially
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        // Returns representative/root of x's set.
        // Path compression: flattens structure by linking x directly to the root.
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // recursive compression
        }
        return parent[x];
    }

    void unionSet(int a, int b) {
        // Merges sets containing a and b using union-by-rank.
        // If already same set, does nothing.
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return;

        // Attach lower-rank tree under higher-rank tree.
        if (rank[ra] < rank[rb]) {
            parent[ra] = rb;
        } else if (rank[ra] > rank[rb]) {
            parent[rb] = ra;
        } else {
            // Tie: choose ra as new root and increment its rank.
            parent[rb] = ra;
            rank[ra]++;
        }
    }
}

/*
 * section11
 * ---------
 * Demonstration container class holding multiple graph algorithms and small tests.
 *
 * Contents:
 *   - buildGraph: undirected adjacency list builder for int nodes
 *   - bfsShortest: BFS shortest paths for unweighted graphs
 *   - dfsVisit + countComponents: DFS-based connected components in undirected graphs
 *   - topoSort: Kahn's algorithm topological sort for directed graphs
 *   - Edge + dijkstra: Dijkstra shortest paths for weighted directed graphs (non-negative weights)
 *
 * Notes:
 *   - Naming convention in Java would typically be "Section11" (PascalCase).
 *   - The nested "Main" class contains the program entry point and runs tests.
 */
public class section11 {

    List<List<Integer>> buildGraph(int n, int[][] edges) {
        // Builds adjacency list for an UNDIRECTED, UNWEIGHTED graph.
        //
        // Parameters:
        //   n     : number of nodes (assumed labeled 0..n-1)
        //   edges : int[][] where each edge is {u, v}
        //
        // Returns:
        //   graph : List<List<Integer>> with graph[u] listing u's neighbors
        //
        // Behavior:
        //   - For undirected graphs, add edge both ways.
        //   - No bounds checking / validation on u and v.
        List<List<Integer>> graph = new ArrayList<>(n);

        // Initialize n empty adjacency lists.
        for (int i = 0; i < n; i++) {
            graph.add(new ArrayList<>());
        }

        // Add each undirected edge to both endpoints' adjacency lists.
        for (int[] e : edges) {
            int u = e[0], v = e[1];
            graph.get(u).add(v);
            graph.get(v).add(u);
        }

        return graph;
    }

    int[] bfsShortest(List<List<Integer>> graph, int src) {
        // Computes shortest path distances from src in an UNWEIGHTED graph via BFS.
        //
        // Parameters:
        //   graph : adjacency list
        //   src   : source node
        //
        // Returns:
        //   dist[v] = shortest distance from src to v in number of edges,
        //             or INF sentinel if unreachable.
        //
        // Notes:
        //   - INF is Integer.MAX_VALUE/4 to reduce overflow risk when adding 1.
        int n = graph.size();
        final int INF = Integer.MAX_VALUE / 4;

        int[] dist = new int[n];
        Arrays.fill(dist, INF);         // mark all nodes initially unreachable/unvisited

        // ArrayDeque is a fast queue implementation for BFS.
        Queue<Integer> q = new ArrayDeque<>();

        dist[src] = 0;                  // distance to itself is 0
        q.add(src);                     // enqueue source

        while (!q.isEmpty()) {
            int u = q.remove();         // dequeue next frontier node

            for (int v : graph.get(u)) {
                // If v not yet visited (still INF), its first discovery is the shortest.
                if (dist[v] == INF) {
                    dist[v] = dist[u] + 1;
                    q.add(v);
                }
            }
        }

        return dist;
    }

    void dfsVisit(int u, List<List<Integer>> graph, boolean[] visited) {
        // Recursive DFS helper that marks all nodes reachable from u.
        //
        // Parameters:
        //   u       : node to visit
        //   graph   : adjacency list
        //   visited : visitation flags (mutated in-place)
        //
        // Caution:
        //   - Recursive DFS can overflow the stack on very large/deep graphs.
        visited[u] = true;

        for (int v : graph.get(u)) {
            if (!visited[v]) {
                dfsVisit(v, graph, visited);
            }
        }
    }

    int countComponents(List<List<Integer>> graph) {
        // Counts connected components in an UNDIRECTED graph using DFS.
        //
        // Approach:
        //   - Iterate each node; if unvisited, start DFS and increment component count.
        int n = graph.size();
        boolean[] visited = new boolean[n]; // defaults to false
        int components = 0;

        for (int u = 0; u < n; u++) {
            if (!visited[u]) {
                components++;
                dfsVisit(u, graph, visited);
            }
        }

        return components;
    }

    List<Integer> topoSort(List<List<Integer>> graph) {
        // Performs topological sort on a DIRECTED graph using Kahn's algorithm.
        //
        // Parameters:
        //   graph : directed adjacency list (u -> v edges)
        //
        // Returns:
        //   order : a topological ordering if graph is a DAG.
        //           If order.size() < n, then the graph has a cycle (ordering incomplete).
        int n = graph.size();

        // indeg[v] counts incoming edges to v.
        int[] indeg = new int[n];
        for (int u = 0; u < n; u++) {
            for (int v : graph.get(u)) {
                indeg[v]++;
            }
        }

        // Queue starts with nodes with indegree 0 (no prerequisites).
        Queue<Integer> q = new ArrayDeque<>();
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                q.add(i);
            }
        }

        List<Integer> order = new ArrayList<>();

        while (!q.isEmpty()) {
            int u = q.remove();
            order.add(u);

            // "Remove" u by decrementing indegree of its outgoing neighbors.
            for (int v : graph.get(u)) {
                if (--indeg[v] == 0) {
                    q.add(v);
                }
            }
        }

        // If order.size() < n, at least one directed cycle exists.
        return order;
    }

    static class Edge {
        // Represents one directed, weighted edge u -> to with weight w.
        // Used by Dijkstra's adjacency list.
        int to, w;

        Edge(int t, int w) {
            this.to = t;
            this.w = w;
        }
    }

    long[] dijkstra(List<List<Edge>> graph, int src) {
        // Dijkstra's algorithm (min-heap) for shortest paths in a weighted directed graph.
        //
        // Parameters:
        //   graph : adjacency list of Edge objects
        //   src   : source node
        //
        // Returns:
        //   dist[v] = shortest distance from src to v (long), or INF if unreachable.
        //
        // Correctness requirement:
        //   - Edge weights must be NON-NEGATIVE. Negative weights invalidate Dijkstra.
        //
        // Implementation details:
        //   - Uses a PriorityQueue of long[] where:
        //       cur[0] = distance, cur[1] = node
        //   - Uses "lazy" removal: multiple entries per node may exist; stale ones are skipped
        //     by checking (d != dist[u]).
        int n = graph.size();
        final long INF = Long.MAX_VALUE / 4;

        long[] dist = new long[n];
        Arrays.fill(dist, INF);

        // PriorityQueue ordered by the distance element (cur[0]).
        PriorityQueue<long[]> pq =
            new PriorityQueue<>(Comparator.comparingLong(a -> a[0]));

        dist[src] = 0;
        pq.offer(new long[]{0, src}); // push (distance=0, node=src)

        while (!pq.isEmpty()) {
            long[] cur = pq.poll();
            long d = cur[0];
            int u = (int)cur[1];

            // Skip if this entry is stale (not the current best distance).
            if (d != dist[u]) continue;

            // Relax all outgoing edges u -> v.
            for (Edge e : graph.get(u)) {
                int v = e.to;
                long nd = d + e.w;
                if (nd < dist[v]) {
                    dist[v] = nd;
                    pq.offer(new long[]{nd, v});
                }
            }
        }

        return dist;
    }

    /*
     * Main
     * ----
     * Nested class providing the program entry point (main) and running all tests.
     *
     * Note:
     *   - In typical Java structure, you'd usually have a top-level Main class or
     *     keep main() in the public class itself, but this works for demos.
     */
    public class Main {
        public static void main(String[] args) {
            // Instantiate the algorithms container.
            section11 s = new section11();

            // Run tests in sequence.
            testDSU();
            testBFSAndComponents(s);
            testDisconnectedComponents(s);
            testTopoSort(s);
            testDijkstra(s);
        }

        // ===========================
        // DSU TEST
        // ===========================
        static void testDSU() {
            System.out.println("=== Test 1: DSU ===");

            // Create DSU for elements {0,1,2,3,4}.
            DSU d = new DSU(5);

            // Merge sets: {0,1,2} and {3,4}.
            d.unionSet(0, 1);
            d.unionSet(1, 2);
            d.unionSet(3, 4);

            // Gather representative for each element.
            // Representative IDs are not fixed; what matters is grouping.
            int[] repr = new int[5];
            for (int i = 0; i < 5; i++) repr[i] = d.find(i);

            System.out.println("Representatives:");
            System.out.println(Arrays.toString(repr));

            // Count distinct reps to determine number of disjoint sets.
            Set<Integer> distinct = new HashSet<>();
            for (int r : repr) distinct.add(r);

            System.out.println("Number of sets = " + distinct.size() + " (expected 2)\n");
        }

        // ===========================
        // BFS + Connected Components
        // ===========================
        static void testBFSAndComponents(section11 s) {
            System.out.println("=== Test 2: BFS shortest + components ===");

            // Undirected graph:
            // 0 - 1 - 2 - 4
            //     |
            //     3
            int n = 5;
            int[][] edges = {
                {0,1},
                {1,2},
                {1,3},
                {2,4}
            };

            // Build adjacency list and compute BFS distances from node 0.
            List<List<Integer>> g = s.buildGraph(n, edges);

            int[] dist = s.bfsShortest(g, 0);
            System.out.println("Distances from 0: " + Arrays.toString(dist));

            // Count connected components (should be 1 for this connected graph).
            int comps = s.countComponents(g);
            System.out.println("Connected components = " + comps + " (expected 1)\n");
        }

        static void testDisconnectedComponents(section11 s) {
            System.out.println("=== Test 3: Disconnected components ===");

            // Graph with three components:
            // {0,1,2} connected as a chain, {3,4} connected, {5} isolated.
            int n = 6;
            int[][] edges = {
                {0,1},
                {1,2},
                {3,4}
            };

            List<List<Integer>> g = s.buildGraph(n, edges);

            int comps = s.countComponents(g);
            System.out.println("Components = " + comps + " (expected 3)\n");
        }

        // ===========================
        // Topological Sort
        // ===========================
        static void testTopoSort(section11 s) {
            System.out.println("=== Test 4: Topological sort (DAG) ===");

            int n = 6;

            // Build directed adjacency list.
            List<List<Integer>> dag = new ArrayList<>();
            for (int i = 0; i < n; i++) dag.add(new ArrayList<>());

            // Directed edges:
            // 5→2, 5→0
            // 4→0, 4→1
            // 2→3
            // 3→1
            dag.get(5).add(2);
            dag.get(5).add(0);
            dag.get(4).add(0);
            dag.get(4).add(1);
            dag.get(2).add(3);
            dag.get(3).add(1);

            // Run topo sort.
            List<Integer> order = s.topoSort(dag);

            // If not all nodes are produced, the graph contains a cycle.
            if (order.size() < n)
                System.out.println("Graph has a cycle, result incomplete");
            else
                System.out.println("Topological order: " + order);

            System.out.println();
        }

        // ===========================
        // DIJKSTRA
        // ===========================
        static void testDijkstra(section11 s) {
            System.out.println("=== Test 5: Dijkstra shortest paths ===");

            int n = 5;

            // Weighted directed graph adjacency list.
            List<List<section11.Edge>> g = new ArrayList<>();
            for (int i = 0; i < n; i++) g.add(new ArrayList<>());

            // Helper lambda for adding edges. It accepts:
            //   pair = {u, v} and weight w, then adds edge u -> v of weight w.
            // This is just a compact way to mimic "ADD(u,v,w)" without a separate method.
            java.util.function.BiConsumer<int[], Integer> ADD = (pair, w) ->
                g.get(pair[0]).add(new section11.Edge(pair[1], w));

            // Add directed weighted edges for the CLRS-style example.
            ADD.accept(new int[]{0,1}, 10);
            ADD.accept(new int[]{0,3}, 5);

            ADD.accept(new int[]{1,2}, 1);
            ADD.accept(new int[]{1,3}, 2);

            ADD.accept(new int[]{2,4}, 4);

            ADD.accept(new int[]{3,1}, 3);
            ADD.accept(new int[]{3,2}, 9);
            ADD.accept(new int[]{3,4}, 2);

            ADD.accept(new int[]{4,0}, 7);
            ADD.accept(new int[]{4,2}, 6);

            // Run Dijkstra from source 0 and print results.
            long[] dist = s.dijkstra(g, 0);
            System.out.println("Distances from 0: " + Arrays.toString(dist));
            System.out.println("Expected: [0, 8, 9, 5, 7]\n");
        }
    }
}
