package code_samples.section11.lesson;

import java.util.*;

class DSU {
    private int[] parent;
    private int[] rank;

    DSU(int n) {
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unionSet(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return;
        if (rank[ra] < rank[rb]) {
            parent[ra] = rb;
        } else if (rank[ra] > rank[rb]) {
            parent[rb] = ra;
        } else {
            parent[rb] = ra;
            rank[ra]++;
        }
    }
}

public class section11 {
    List<List<Integer>> buildGraph(int n, int[][] edges) {
        List<List<Integer>> graph = new ArrayList<>(n);
        for (int i = 0; i < n; i++) {
            graph.add(new ArrayList<>());
        }
        for (int[] e : edges) {
            int u = e[0], v = e[1];
            graph.get(u).add(v);
            graph.get(v).add(u);
        }
        return graph;
    }

    int[] bfsShortest(List<List<Integer>> graph, int src) {
        int n = graph.size();
        final int INF = Integer.MAX_VALUE / 4;
        int[] dist = new int[n];
        Arrays.fill(dist, INF);

        Queue<Integer> q = new ArrayDeque<>();
        dist[src] = 0;
        q.add(src);

        while (!q.isEmpty()) {
            int u = q.remove();
            for (int v : graph.get(u)) {
                if (dist[v] == INF) {
                    dist[v] = dist[u] + 1;
                    q.add(v);
                }
            }
        }
        return dist;
    }

    void dfsVisit(int u, List<List<Integer>> graph, boolean[] visited) {
        visited[u] = true;
        for (int v : graph.get(u)) {
            if (!visited[v]) {
                dfsVisit(v, graph, visited);
            }
        }
    }

    int countComponents(List<List<Integer>> graph) {
        int n = graph.size();
        boolean[] visited = new boolean[n];
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
        int n = graph.size();
        int[] indeg = new int[n];
        for (int u = 0; u < n; u++) {
            for (int v : graph.get(u)) {
                indeg[v]++;
            }
        }

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
            for (int v : graph.get(u)) {
                if (--indeg[v] == 0) {
                    q.add(v);
                }
            }
        }
        // if order.size() < n, there was a cycle
        return order;
    }

    static class Edge {
        int to, w;
        Edge(int t, int w) { this.to = t; this.w = w; }
    }

    long[] dijkstra(List<List<Edge>> graph, int src) {
        int n = graph.size();
        final long INF = Long.MAX_VALUE / 4;
        long[] dist = new long[n];
        Arrays.fill(dist, INF);

        PriorityQueue<long[]> pq =
            new PriorityQueue<>(Comparator.comparingLong(a -> a[0]));
        dist[src] = 0;
        pq.offer(new long[]{0, src});

        while (!pq.isEmpty()) {
            long[] cur = pq.poll();
            long d = cur[0];
            int u = (int)cur[1];
            if (d != dist[u]) continue;

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

    public class Main {
        public static void main(String[] args) {
            section11 s = new section11();

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

            DSU d = new DSU(5);
            d.unionSet(0, 1);
            d.unionSet(1, 2);
            d.unionSet(3, 4);

            int[] repr = new int[5];
            for (int i = 0; i < 5; i++) repr[i] = d.find(i);

            System.out.println("Representatives:");
            System.out.println(Arrays.toString(repr));

            Set<Integer> distinct = new HashSet<>();
            for (int r : repr) distinct.add(r);

            System.out.println("Number of sets = " + distinct.size() + " (expected 2)\n");
        }

        // ===========================
        // BFS + Connected Components
        // ===========================
        static void testBFSAndComponents(section11 s) {
            System.out.println("=== Test 2: BFS shortest + components ===");

            int n = 5;
            int[][] edges = {
                {0,1},
                {1,2},
                {1,3},
                {2,4}
            };

            List<List<Integer>> g = s.buildGraph(n, edges);

            int[] dist = s.bfsShortest(g, 0);
            System.out.println("Distances from 0: " + Arrays.toString(dist));

            int comps = s.countComponents(g);
            System.out.println("Connected components = " + comps + " (expected 1)\n");
        }

        static void testDisconnectedComponents(section11 s) {
            System.out.println("=== Test 3: Disconnected components ===");

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
            List<List<Integer>> dag = new ArrayList<>();
            for (int i = 0; i < n; i++) dag.add(new ArrayList<>());

            // Directed edges
            dag.get(5).add(2);
            dag.get(5).add(0);
            dag.get(4).add(0);
            dag.get(4).add(1);
            dag.get(2).add(3);
            dag.get(3).add(1);

            List<Integer> order = s.topoSort(dag);

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
            List<List<section11.Edge>> g = new ArrayList<>();
            for (int i = 0; i < n; i++) g.add(new ArrayList<>());

            // helper lambda:
            java.util.function.BiConsumer<int[], Integer> ADD = (pair, w) ->
                g.get(pair[0]).add(new section11.Edge(pair[1], w));

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

            long[] dist = s.dijkstra(g, 0);
            System.out.println("Distances from 0: " + Arrays.toString(dist));
            System.out.println("Expected: [0, 8, 9, 5, 7]\n");
        }
    }
}
