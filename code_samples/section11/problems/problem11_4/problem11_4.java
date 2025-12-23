package code_samples.section11.problems.problem11_4;

import java.util.*; // Collections, Arrays, PriorityQueue, Comparator, List, ArrayList

/*
 * Edge:
 * Simple container for a directed weighted edge u -> to with weight w.
 *
 * Fields:
 *   to : destination vertex id
 *   w  : edge weight (must be non-negative for Dijkstra to be correct)
 *
 * Note:
 *   This class is package-private (no 'public') so it can live in the same
 *   file as problem11_4 for a compact example.
 */
class Edge {
    int to; // destination node
    int w;  // edge weight

    Edge(int t, int w) {
        this.to = t;
        this.w = w;
    }
}

public class problem11_4 {

    int[] dijkstra(int n, List<List<Edge>> adj, int s) {
        /*
         * Dijkstra's algorithm (single-source shortest paths).
         *
         * Preconditions / correctness requirements:
         *   - Graph is directed (as constructed by addEdge()).
         *   - All edge weights must be >= 0. Negative weights invalidate Dijkstra.
         *
         * Parameters:
         *   n   : number of nodes (assumed 0..n-1)
         *   adj : adjacency list where adj.get(u) contains outgoing edges from u
         *   s   : source node
         *
         * Returns:
         *   dist : dist[v] is the shortest distance from s to v.
         *          If v is unreachable, dist[v] remains INF.
         *
         * Implementation details:
         *   - Uses a PriorityQueue of int[] where:
         *       cur[0] = distance, cur[1] = node
         *   - The queue may contain multiple entries per node because we don't
         *     implement decrease-key. We discard "stale" entries with:
         *       if (d > dist[u]) continue;
         */
        int INF = Integer.MAX_VALUE / 2; // "infinity" reduced to limit overflow on addition
        int[] dist = new int[n];
        Arrays.fill(dist, INF);
        dist[s] = 0;

        // Priority queue ordered by smallest distance first.
        // Each element is int[]{distance, node}.
        PriorityQueue<int[]> pq =
            new PriorityQueue<>(Comparator.comparingInt(a -> a[0]));

        // Seed with the source at distance 0.
        pq.offer(new int[]{0, s});

        // Main loop: repeatedly expand the node with smallest known tentative distance.
        while (!pq.isEmpty()) {
            int[] cur = pq.poll();
            int d = cur[0], u = cur[1];

            // Skip outdated entries: we already found a better distance to u.
            if (d > dist[u]) continue;

            // Relax all outgoing edges u -> v.
            for (Edge e : adj.get(u)) {
                int v = e.to;
                int nd = d + e.w; // candidate distance via u

                // If going through u improves the best-known distance to v, update.
                if (nd < dist[v]) {
                    dist[v] = nd;
                    // Push the new best distance for v.
                    // Older entries for v (if any) will be skipped later.
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
        /*
         * Runs a small suite of tests:
         *   1) Simple weighted directed graph
         *   2) Graph with multiple equal-cost shortest paths
         *   3) Disconnected graph (unreachable nodes)
         */
        testSimpleGraph();
        testMultiplePaths();
        testDisconnected();
    }

    private List<List<Edge>> makeGraph(int n) {
        /*
         * Allocates an empty adjacency list representation.
         *
         * Returns:
         *   g where g.get(u) is initially an empty list of outgoing edges.
         */
        List<List<Edge>> g = new ArrayList<>(n);
        for (int i = 0; i < n; i++) g.add(new ArrayList<>());
        return g;
    }

    private void addEdge(List<List<Edge>> g, int u, int v, int w) {
        /*
         * Adds a DIRECTED edge u -> v with weight w.
         *
         * Note:
         *   For an undirected graph, you'd add both u->v and v->u.
         */
        g.get(u).add(new Edge(v, w));
    }

    private void printDist(int[] dist) {
        /*
         * Prints distances in a compact list form.
         *
         * Convention:
         *   - Values that remain "very large" are printed as "INF" to indicate
         *     unreachable nodes.
         */
        System.out.print("[");
        for (int i = 0; i < dist.length; i++) {
            if (dist[i] >= Integer.MAX_VALUE / 4) System.out.print("INF");
            else System.out.print(dist[i]);
            if (i + 1 < dist.length) System.out.print(", ");
        }
        System.out.println("]");
    }

    private void testSimpleGraph() {
        /*
         * Test 1: Simple weighted directed graph.
         *
         * Graph:
         *   0→1(2), 0→2(5)
         *   1→2(1)
         *   1→3(3)
         *   2→3(1)
         *   3→4(2)
         *
         * Purpose:
         *   - Verifies that multi-hop cheaper paths are discovered correctly
         *     (e.g., 0->1->2 is cheaper than 0->2 directly).
         */
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
        /*
         * Test 2: Multiple equal-cost shortest paths.
         *
         * Graph:
         *   0→1(1), 0→2(1)
         *   1→3(2)
         *   2→3(2)
         *   3→4(1)
         *   4→5(5)
         *
         * Purpose:
         *   - Checks correctness when there are multiple shortest ways to reach
         *     the same node (e.g., node 3 can be reached via 1 or 2 at equal cost).
         */
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
        /*
         * Test 3: Disconnected graph.
         *
         * Graph:
         *   0→1(4), 1→2(6)
         * Nodes 3 and 4 are unreachable from source 0.
         *
         * Purpose:
         *   - Confirms unreachable nodes remain INF and are printed as such.
         */
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
        /*
         * Entry point: constructs the solver instance and runs the test suite.
         */
        new problem11_4().runTests();
    }
}
