#include <stdio.h>    // printf() for console output
#include <stdlib.h>   // malloc(), realloc(), free()
#include <limits.h>   // INT_MAX for "infinity" sentinel

/* ============================
   Adjacency List Structures
   ============================ */

/*
 * Edge:
 * Represents a directed weighted edge u -> to with weight w.
 * This is the per-neighbor item stored in an adjacency list.
 */
typedef struct {
    int to;  // destination vertex id
    int w;   // edge weight (assumed non-negative for Dijkstra)
} Edge;

/*
 * VecEdge:
 * A minimal dynamic array (vector) of Edge values.
 * Used to store adjacency lists:
 *   adj[u] is a VecEdge holding all outgoing edges from node u.
 */
typedef struct {
    Edge *data;     // pointer to allocated edge storage
    int size;       // number of valid edges stored
    int capacity;   // allocated capacity (# of Edge slots)
} VecEdge;

void vec_init(VecEdge *v, int cap) {
    /*
     * Initialize a VecEdge with an initial capacity.
     *
     * Parameters:
     *   v   : vector to initialize
     *   cap : initial capacity (number of Edge slots to allocate)
     *
     * Behavior:
     *   - Allocates memory for `cap` Edge entries
     *   - Sets size = 0 and capacity = cap
     *
     * Notes:
     *   - No error handling for malloc failure.
     *   - cap should be > 0; malloc(0) is implementation-defined.
     */
    v->data = (Edge *)malloc(sizeof(Edge) * cap);
    v->size = 0;
    v->capacity = cap;
}

void vec_push(VecEdge *v, int to, int w) {
    /*
     * Append a new edge (to, w) to the vector, resizing if needed.
     *
     * Parameters:
     *   v  : vector to append into
     *   to : destination node
     *   w  : weight
     *
     * Behavior:
     *   - If size == capacity, doubles capacity and reallocates storage.
     *   - Writes the edge at data[size], then increments size.
     *
     * Notes:
     *   - No error handling for realloc failure.
     *   - Doubling strategy provides amortized O(1) append.
     */
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = (Edge *)realloc(v->data, sizeof(Edge) * v->capacity);
    }
    v->data[v->size].to = to;
    v->data[v->size].w = w;
    v->size++;
}

void free_adj(VecEdge *adj, int n) {
    /*
     * Frees the internal buffers for an adjacency list array.
     *
     * Parameters:
     *   adj : pointer to an array of VecEdge of length n
     *   n   : number of nodes (length of adj array)
     *
     * Behavior:
     *   - Frees each adj[i].data
     *
     * Notes:
     *   - This does NOT free `adj` itself; callers do that separately when
     *     `adj` was allocated with malloc().
     */
    for (int i = 0; i < n; i++) {
        free(adj[i].data);
    }
}

/* ============================
   Min-Heap for (dist, node)
   ============================ */

/*
 * HeapItem:
 * A single entry in the priority queue used by Dijkstra:
 *   - dist: tentative distance value
 *   - node: which node that distance corresponds to
 *
 * Important detail:
 *   This heap does not support decrease-key. Instead, whenever a better
 *   distance is found we push a new (dist,node) entry. Outdated entries
 *   are discarded during pop by comparing against dist[node].
 */
typedef struct {
    int dist;  // tentative distance to node
    int node;  // node id
} HeapItem;

/*
 * MinHeap:
 * Binary min-heap storing HeapItem entries, ordered by smallest dist.
 */
typedef struct {
    HeapItem *data;  // heap array storage
    int size;        // number of elements currently in heap
    int capacity;    // allocated capacity
} MinHeap;

void heap_init(MinHeap *h, int cap) {
    /*
     * Initialize a min-heap with an initial capacity.
     *
     * Parameters:
     *   h   : heap to initialize
     *   cap : initial heap capacity
     */
    h->data = (HeapItem *)malloc(sizeof(HeapItem) * cap);
    h->size = 0;
    h->capacity = cap;
}

void heap_swap(HeapItem *a, HeapItem *b) {
    /*
     * Swap two HeapItem values (used during heapify operations).
     */
    HeapItem tmp = *a;
    *a = *b;
    *b = tmp;
}

void heap_ensure_capacity(MinHeap *h) {
    /*
     * Ensures the heap has room for at least one more element.
     * Doubles capacity when needed.
     */
    if (h->size >= h->capacity) {
        h->capacity *= 2;
        h->data = (HeapItem *)realloc(h->data, sizeof(HeapItem) * h->capacity);
    }
}

void heap_push(MinHeap *h, int dist, int node) {
    /*
     * Push a new (dist, node) entry into the min-heap.
     *
     * Parameters:
     *   h    : heap
     *   dist : priority key (smaller dist == higher priority)
     *   node : node id associated with dist
     *
     * Implementation:
     *   - Append at end
     *   - Sift up until heap property is restored
     */
    heap_ensure_capacity(h);

    // Insert at end
    int i = h->size++;
    h->data[i].dist = dist;
    h->data[i].node = node;

    // Sift up: compare with parent until parent is <= current
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[p].dist <= h->data[i].dist) break;
        heap_swap(&h->data[p], &h->data[i]);
        i = p;
    }
}

int heap_pop_min(MinHeap *h, int *outDist, int *outNode) {
    /*
     * Pop the minimum (dist, node) entry from the heap.
     *
     * Parameters:
     *   h       : heap
     *   outDist : output parameter for popped dist
     *   outNode : output parameter for popped node
     *
     * Returns:
     *   1 if an element was popped successfully
     *   0 if the heap was empty
     *
     * Implementation:
     *   - Save root
     *   - Move last element to root
     *   - Sift down until heap property is restored
     */
    if (h->size == 0) return 0;

    // Extract min at root
    *outDist = h->data[0].dist;
    *outNode = h->data[0].node;

    // Replace root with last element and reduce size
    h->data[0] = h->data[--h->size];

    // Sift down to restore heap order
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < h->size && h->data[left].dist < h->data[smallest].dist)
            smallest = left;
        if (right < h->size && h->data[right].dist < h->data[smallest].dist)
            smallest = right;

        if (smallest == i) break;

        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return 1;
}

void heap_free(MinHeap *h) {
    /*
     * Free heap storage.
     */
    free(h->data);
}

/* ============================
   Dijkstra (Non-Negative Weights)
   adj[u] = list of edges (u -> v, w)
   ============================ */

int* dijkstra(int n, VecEdge *adj, int s) {
    /*
     * Computes single-source shortest paths using Dijkstra's algorithm.
     *
     * Requirements:
     *   - Edge weights must be non-negative for correctness.
     *
     * Parameters:
     *   n   : number of nodes (0..n-1)
     *   adj : adjacency list, where adj[u] is a VecEdge of outgoing edges from u
     *   s   : source node
     *
     * Returns:
     *   dist : dynamically allocated array of length n, where dist[v] is the
     *          shortest distance from s to v (or INF if unreachable).
     *          Caller is responsible for free(dist).
     *
     * Priority queue strategy:
     *   - Because this heap doesn't support decrease-key, we push a new entry
     *     each time we improve dist[v].
     *   - When popping (d,u), if d > dist[u], it's an outdated entry and is skipped.
     */
    const int INF = INT_MAX / 2; // use a reduced INT_MAX to lower overflow risk
    int *dist = (int *)malloc(sizeof(int) * n);

    // Initialize all distances to INF (meaning "unreachable so far")
    for (int i = 0; i < n; i++) dist[i] = INF;
    dist[s] = 0;

    // Min-heap of (distance, node) pairs
    MinHeap h;
    heap_init(&h, 16);
    heap_push(&h, 0, s);

    // Main loop: repeatedly expand the node with smallest tentative distance
    while (h.size > 0) {
        int d, u;

        // Pop best candidate from heap
        if (!heap_pop_min(&h, &d, &u)) break;

        // If this heap entry is stale, skip it.
        // (A better distance to u was found after this entry was pushed.)
        if (d > dist[u]) continue;

        // Relax outgoing edges u -> v
        for (int i = 0; i < adj[u].size; i++) {
            int v = adj[u].data[i].to;
            int w = adj[u].data[i].w;

            // Candidate distance through u
            int nd = d + w;

            // If shorter, update and push new heap entry
            if (nd < dist[v]) {
                dist[v] = nd;
                heap_push(&h, nd, v);
            }
        }
    }

    // Clean up heap memory; dist is returned to caller
    heap_free(&h);
    return dist;
}

/* ============================
   Helpers & Tests
   ============================ */

void add_edge(VecEdge *adj, int u, int v, int w) {
    /*
     * Add a directed weighted edge u -> v with weight w.
     *
     * Parameters:
     *   adj : adjacency list
     *   u   : source node
     *   v   : destination node
     *   w   : edge weight (should be non-negative for Dijkstra)
     */
    vec_push(&adj[u], v, w);  // directed edge u -> v
}

void print_distances(int *dist, int n) {
    /*
     * Print distances in a compact list format.
     *
     * Convention:
     *   - Any value that is still "very large" is printed as INF.
     *
     * Note:
     *   This uses the same INF definition style as dijkstra() so the printed
     *   "INF" matches what "unreachable" looks like in the dist array.
     */
    int INF = INT_MAX / 2;
    printf("[");
    for (int i = 0; i < n; i++) {
        // If it's close to INF, treat it as unreachable for printing purposes.
        if (dist[i] >= INF / 2) printf("INF");
        else printf("%d", dist[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]\n");
}

void run_test(const char *name, int n, VecEdge *adj, int src) {
    /*
     * Runs Dijkstra from a given source and prints the resulting distance array.
     *
     * Parameters:
     *   name : test label
     *   n    : number of nodes
     *   adj  : adjacency list
     *   src  : source node for shortest paths
     */
    printf("%s\n", name);
    printf("Source = %d\n", src);

    int *dist = dijkstra(n, adj, src);

    printf("Distances: ");
    print_distances(dist, n);
    printf("\n");

    free(dist);
}

int main(void) {

    // ============================
    // Test 1: Simple weighted graph
    // ============================
    {
        printf("=== Test 1: Simple Graph ===\n");

        // Build a directed weighted graph with 5 nodes.
        // Edges:
        //   0→1 (2), 0→2 (5)
        //   1→2 (1)
        //   1→3 (3)
        //   2→3 (1)
        //   3→4 (2)
        //
        // Expected behavior:
        //   Dijkstra from 0 should find shortest paths through intermediate nodes
        //   (e.g., 0->1->2 is cheaper than 0->2 directly).
        int n = 5;
        VecEdge *adj = (VecEdge *)malloc(sizeof(VecEdge) * n);
        for (int i = 0; i < n; i++) vec_init(&adj[i], 4);

        add_edge(adj, 0, 1, 2);
        add_edge(adj, 0, 2, 5);
        add_edge(adj, 1, 2, 1);
        add_edge(adj, 1, 3, 3);
        add_edge(adj, 2, 3, 1);
        add_edge(adj, 3, 4, 2);

        run_test("Simple weighted graph", n, adj, 0);

        // Free adjacency list buffers and the adj array itself.
        free_adj(adj, n);
        free(adj);
    }

    // ============================
    // Test 2: Multiple shortest paths
    // ============================
    {
        printf("=== Test 2: Multiple Shortest Paths ===\n");

        // This test builds a graph where there are two equal-cost routes to node 3:
        //   0->1->3 (1+2) and 0->2->3 (1+2)
        //
        // Dijkstra should produce the same shortest distance regardless of which
        // equal-cost path is discovered first.
        int n = 6;
        VecEdge *adj = (VecEdge *)malloc(sizeof(VecEdge) * n);
        for (int i = 0; i < n; i++) vec_init(&adj[i], 4);

        add_edge(adj, 0, 1, 1);
        add_edge(adj, 0, 2, 1);
        add_edge(adj, 1, 3, 2);
        add_edge(adj, 2, 3, 2);
        add_edge(adj, 3, 4, 1);
        add_edge(adj, 4, 5, 5);

        run_test("Branches with equal-cost paths", n, adj, 0);

        free_adj(adj, n);
        free(adj);
    }

    // ============================
    // Test 3: Disconnected graph
    // ============================
    {
        printf("=== Test 3: Disconnected Graph ===\n");

        // Graph with 5 nodes where nodes 3 and 4 are unreachable from source 0.
        // Edges:
        //   0→1 (4), 1→2 (6)
        //
        // Expected behavior:
        //   dist[0]=0, dist[1]=4, dist[2]=10, and dist[3],dist[4] remain INF.
        int n = 5;
        VecEdge *adj = (VecEdge *)malloc(sizeof(VecEdge) * n);
        for (int i = 0; i < n; i++) vec_init(&adj[i], 4);

        add_edge(adj, 0, 1, 4);
        add_edge(adj, 1, 2, 6);
        // Nodes 3 and 4 have no incoming path from 0 in this graph.

        run_test("Disconnected graph", n, adj, 0);

        free_adj(adj, n);
        free(adj);
    }

    return 0;
}
