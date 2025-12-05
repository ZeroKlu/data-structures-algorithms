package code_samples.section11.problems.problem11_4;

import java.util.*;

class Edge {
    int to;
    int w;
    Edge(int t, int w) { this.to = t; this.w = w; }
}

public class problem11_4 {

    int[] dijkstra(int n, List<List<Edge>> adj, int s) {
        int INF = Integer.MAX_VALUE / 2;
        int[] dist = new int[n];
        Arrays.fill(dist, INF);
        dist[s] = 0;

        PriorityQueue<int[]> pq =
            new PriorityQueue<>(Comparator.comparingInt(a -> a[0]));

        pq.offer(new int[]{0, s});

        while (!pq.isEmpty()) {
            int[] cur = pq.poll();
            int d = cur[0], u = cur[1];
            if (d > dist[u]) continue;

            for (Edge e : adj.get(u)) {
                int v = e.to;
                int nd = d + e.w;
                if (nd < dist[v]) {
                    dist[v] = nd;
                    pq.offer(new int[]{nd, v});
                }
            }
        }
        return dist;
    }

    // ======================================================
    // Test Harness
    // ======================================================
    public void runTests() {
        testSimpleGraph();
        testMultiplePaths();
        testDisconnected();
    }

    private List<List<Edge>> makeGraph(int n) {
        List<List<Edge>> g = new ArrayList<>(n);
        for (int i = 0; i < n; i++) g.add(new ArrayList<>());
        return g;
    }

    private void addEdge(List<List<Edge>> g, int u, int v, int w) {
        g.get(u).add(new Edge(v, w));
    }

    private void printDist(int[] dist) {
        System.out.print("[");
        for (int i = 0; i < dist.length; i++) {
            if (dist[i] >= Integer.MAX_VALUE / 4) System.out.print("INF");
            else System.out.print(dist[i]);
            if (i + 1 < dist.length) System.out.print(", ");
        }
        System.out.println("]");
    }

    private void testSimpleGraph() {
        System.out.println("=== Test 1: Simple Weighted Graph ===");

        int n = 5;
        List<List<Edge>> adj = makeGraph(n);

        addEdge(adj, 0, 1, 2);
        addEdge(adj, 0, 2, 5);
        addEdge(adj, 1, 2, 1);
        addEdge(adj, 1, 3, 3);
        addEdge(adj, 2, 3, 1);
        addEdge(adj, 3, 4, 2);

        int[] dist = dijkstra(n, adj, 0);

        System.out.print("Distances: ");
        printDist(dist);
        System.out.println();
    }

    private void testMultiplePaths() {
        System.out.println("=== Test 2: Multiple Shortest Paths ===");

        int n = 6;
        List<List<Edge>> adj = makeGraph(n);

        addEdge(adj, 0, 1, 1);
        addEdge(adj, 0, 2, 1);
        addEdge(adj, 1, 3, 2);
        addEdge(adj, 2, 3, 2);
        addEdge(adj, 3, 4, 1);
        addEdge(adj, 4, 5, 5);

        int[] dist = dijkstra(n, adj, 0);

        System.out.print("Distances: ");
        printDist(dist);
        System.out.println();
    }

    private void testDisconnected() {
        System.out.println("=== Test 3: Disconnected Graph ===");

        int n = 5;
        List<List<Edge>> adj = makeGraph(n);

        addEdge(adj, 0, 1, 4);
        addEdge(adj, 1, 2, 6);

        int[] dist = dijkstra(n, adj, 0);

        System.out.print("Distances: ");
        printDist(dist);
        System.out.println();
    }

    // ======================================================
    // MAIN FUNCTION (requested)
    // ======================================================
    public static void main(String[] args) {
        new problem11_4().runTests();
    }
}
