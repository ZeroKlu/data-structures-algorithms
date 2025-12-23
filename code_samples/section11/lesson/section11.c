#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Dynamic array of integers
typedef struct {
    int* data;
    int size;
    int capacity;
} VecInt;

/**
 * vec_init
 * --------
 * Initializes a dynamic integer vector with an initial capacity.
 *
 * The vector tracks:
 *   - data: pointer to heap-allocated array
 *   - size: number of valid elements currently stored
 *   - capacity: total allocated slots in data[]
 *
 * @param v   Pointer to the vector to initialize
 * @param cap Initial capacity (number of ints to allocate space for)
 *
 * Notes:
 *   - If cap is small, the vector will grow as needed in vec_push().
 *   - This function does not handle cap == 0 explicitly; if cap == 0,
 *     malloc(0) is implementation-defined (might return NULL or a unique pointer).
 */
static void vec_init(VecInt* v, int cap) {
    // allocate memory
    v->data = (int*)malloc(sizeof(int) * cap);
    v->size = 0;
    v->capacity = cap;
}

/**
 * vec_push
 * --------
 * Appends an element to the end of the dynamic vector, resizing if needed.
 *
 * Resizing strategy:
 *   - When size == capacity, double the capacity and realloc() the backing array.
 *
 * @param v Pointer to the vector
 * @param x Value to append
 *
 * Notes:
 *   - realloc() may move memory; v->data is updated accordingly.
 *   - No explicit error handling is included if realloc() returns NULL.
 */
static void vec_push(VecInt* v, int x) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        // reallocate memory
        v->data = (int*)realloc(v->data, sizeof(int) * v->capacity);
    }
    // add element at the end
    v->data[v->size++] = x;
}

/**
 * build_graph
 * -----------
 * Builds an adjacency list representation for an undirected, unweighted graph.
 *
 * Representation:
 *   graph[u] is a VecInt containing all neighbors of node u.
 *
 * Input edges:
 *   edges[i][0], edges[i][1] are endpoints (0-based indices).
 *
 * @param n     Number of nodes
 * @param m     Number of edges
 * @param edges Edge list of length m, each edge is a pair (u, v)
 *
 * @return Pointer to heap-allocated array of VecInt of length n.
 *         Caller must free it using free_graph().
 */
VecInt* build_graph(int n, int m, int edges[][2]) {
    VecInt* graph = (VecInt*)malloc(sizeof(VecInt) * n);

    // initialize graph (each adjacency list starts with capacity 4)
    for (int i = 0; i < n; i++) {
        vec_init(&graph[i], 4);
    }

    // add edges (undirected: add both directions)
    for (int i = 0; i < m; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        vec_push(&graph[u], v);
        vec_push(&graph[v], u);
    }
    return graph;
}

/**
 * bfs_shortest
 * ------------
 * Computes shortest path distances (in number of edges) from a source node
 * in an unweighted graph using Breadth-First Search (BFS).
 *
 * Input graph is assumed to be:
 *   - adjacency list in `graph`
 *   - nodes are 0..n-1
 *
 * Output:
 *   dist[v] will be:
 *     - the minimum number of edges from src to v if reachable
 *     - INF if v is unreachable from src
 *
 * Implementation details:
 *   - Uses an integer array as a simple queue:
 *       q[head..tail-1] are the current queued nodes.
 *   - Each node is enqueued at most once because we only enqueue when dist[v] == INF.
 *
 * @param n    Number of nodes
 * @param graph Adjacency list array of length n
 * @param src  Source node
 * @param dist Output array of length n (written by this function)
 */
void bfs_shortest(int n, VecInt* graph, int src, int* dist) {
    const int INF = INT_MAX / 4;

    // Initialize all distances to INF (meaning "unvisited/unreachable")
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }

    // Simple queue for BFS; max size needed is n
    int* q = (int*)malloc(sizeof(int) * n);
    int head = 0, tail = 0;

    // Start BFS from src
    dist[src] = 0;
    q[tail++] = src;

    // Standard BFS loop
    while (head < tail) {
        int u = q[head++];

        // Explore all neighbors of u
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].data[i];

            // First time we see v: set distance and enqueue
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                q[tail++] = v;
            }
        }
    }

    // Clean up queue memory
    free(q);
}

/**
 * dfs_visit
 * ---------
 * Depth-First Search (DFS) helper used for connected components counting.
 *
 * Marks all vertices reachable from u as visited.
 *
 * @param u       Current node
 * @param graph   Adjacency list
 * @param visited visited[v] != 0 indicates node v has been visited
 *
 * Notes:
 *   - Recursive implementation; for very deep graphs, recursion could overflow
 *     the call stack. For typical small educational graphs, this is fine.
 */
void dfs_visit(int u, VecInt* graph, int* visited) {
    visited[u] = 1;
    for (int i = 0; i < graph[u].size; i++) {
        int v = graph[u].data[i];
        if (!visited[v]) {
            dfs_visit(v, graph, visited);
        }
    }
}

/**
 * count_components
 * ----------------
 * Counts the number of connected components in an undirected graph.
 *
 * Approach:
 *   - Maintain a visited array.
 *   - For each node not yet visited, run DFS to mark its whole component,
 *     and increment the component count.
 *
 * @param n     Number of nodes
 * @param graph Adjacency list (undirected)
 * @return Number of connected components
 */
int count_components(int n, VecInt* graph) {
    int* visited = (int*)calloc(n, sizeof(int));
    int components = 0;

    for (int u = 0; u < n; u++) {
        if (!visited[u]) {
            components++;
            dfs_visit(u, graph, visited);
        }
    }

    free(visited);
    return components;
}

/**
 * topo_sort
 * ---------
 * Computes a topological ordering using Kahn's algorithm (BFS on indegrees).
 *
 * IMPORTANT NOTE ABOUT GRAPH TYPE:
 *   Topological sorting is defined for directed graphs (DAGs).
 *   This function assumes `graph` contains directed edges u -> v.
 *
 * DP/Algorithm:
 *   1. Compute indegree[v] for each vertex v.
 *   2. Enqueue all nodes with indegree 0.
 *   3. Repeatedly pop from queue, append to output ordering, and decrement
 *      indegree of outgoing neighbors. If a neighbor's indegree hits 0, enqueue it.
 *
 * Output:
 *   out_order will contain the ordering for the nodes processed.
 *
 * Return value:
 *   Returns idx, the number of nodes placed into out_order.
 *   - If idx == n: success (graph is a DAG).
 *   - If idx < n: graph contains a cycle (no valid topological ordering).
 *
 * @param n         Number of nodes
 * @param graph     Adjacency list representing a directed graph
 * @param out_order Output array of length n (filled up to returned idx)
 * @return          Number of nodes placed in out_order (n if DAG, < n if cycle)
 */
int topo_sort(int n, VecInt* graph, int* out_order) {
    // Compute indegrees
    int* indeg = (int*)calloc(n, sizeof(int));
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].data[i];
            indeg[v]++;
        }
    }

    // Queue of nodes with indegree 0
    int* q = (int*)malloc(sizeof(int) * n);
    int head = 0, tail = 0;

    // Initialize queue with all indegree-0 nodes
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) {
            q[tail++] = i;
        }
    }

    // Process queue
    int idx = 0;
    while (head < tail) {
        int u = q[head++];
        out_order[idx++] = u;

        // "Remove" outgoing edges u -> v by decrementing indegree[v]
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].data[i];
            if (--indeg[v] == 0) {
                q[tail++] = v;
            }
        }
    }

    // Cleanup
    free(indeg);
    free(q);

    // If idx < n, some nodes were not processed => cycle exists
    return idx; // if idx < n, there was a cycle
}

/**
 * dijkstra_simple
 * ---------------
 * Computes single-source shortest path distances using Dijkstra's algorithm,
 * implemented in a simple O(n^2) way (no heap / priority queue).
 *
 * Graph representation:
 *   - neigh[u] is a VecInt listing neighbors v of u
 *   - w[u] is a VecInt listing corresponding weights (parallel arrays):
 *       neigh[u].data[i] is the i-th neighbor
 *       w[u].data[i] is the weight of edge u -> neigh[u].data[i]
 *
 * Assumptions:
 *   - Edge weights are non-negative (required by Dijkstra).
 *   - Graph is directed or undirected depending on how edges are inserted.
 *
 * Algorithm:
 *   - dist[src] = 0, others = INF
 *   - Repeat up to n times:
 *       - choose unused node u with smallest dist[u]
 *       - mark u used
 *       - relax all edges u -> v
 *
 * Complexity:
 *   Time:  O(n^2 + E)  (n selections each scanning all n nodes, plus relaxations)
 *   Space: O(n)
 *
 * @param n     Number of nodes
 * @param neigh Neighbor lists (length n)
 * @param w     Weight lists (length n), aligned with neigh
 * @param src   Source node
 * @param dist  Output distances array (length n)
 */
void dijkstra_simple(int n, VecInt* neigh, VecInt* w, int src, long long* dist) {
    const long long INF = (long long)4e18;

    // used[u] == 1 means shortest path to u has been finalized
    int* used = (int*)calloc(n, sizeof(int));

    // Initialize distances
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;

    // Run n iterations (or stop early if remaining nodes are unreachable)
    for (int it = 0; it < n; it++) {
        int u = -1;
        long long best = INF;

        // Select the unused node with minimal tentative distance
        for (int i = 0; i < n; i++) {
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }

        // If no node can be selected, remaining nodes are unreachable
        if (u == -1) break;

        // Finalize u
        used[u] = 1;

        // Relax outgoing edges from u
        for (int i = 0; i < neigh[u].size; i++) {
            int v = neigh[u].data[i];
            int wt = w[u].data[i];

            // Standard Dijkstra relaxation
            if (dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
        }
    }

    free(used);
}

/**
 * DSU (Disjoint Set Union) / Union-Find
 * -------------------------------------
 * Maintains a partition of {0..n-1} into disjoint sets, supporting:
 *   - find(x): return representative (root) of x's set
 *   - union(a,b): merge the sets containing a and b
 *
 * Optimizations used:
 *   - Path compression in find()
 *   - Union by rank in union()
 */
typedef struct {
    int* parent;
    int* rank;
    int n;
} DSU;

/**
 * dsu_create
 * ----------
 * Allocates and initializes a DSU with n singleton sets.
 *
 * @param n Number of elements
 * @return Pointer to heap-allocated DSU (must be freed with dsu_free)
 */
DSU* dsu_create(int n) {
    DSU* d = (DSU*)malloc(sizeof(DSU));
    d->n = n;
    d->parent = (int*)malloc(sizeof(int) * n);
    d->rank = (int*)calloc(n, sizeof(int));

    // Initially each element is its own parent (each is a root)
    for (int i = 0; i < n; i++) {
        d->parent[i] = i;
    }
    return d;
}

/**
 * dsu_find
 * --------
 * Finds the representative (root) of element x.
 *
 * Uses path compression:
 *   - After finding the root, parent[x] is set directly to the root,
 *     flattening the tree and speeding up future operations.
 *
 * @param d DSU pointer
 * @param x Element to find
 * @return Representative/root of x's set
 */
int dsu_find(DSU* d, int x) {
    if (d->parent[x] != x) {
        d->parent[x] = dsu_find(d, d->parent[x]);
    }
    return d->parent[x];
}

/**
 * dsu_union
 * ---------
 * Unites the sets containing a and b.
 *
 * Uses union by rank:
 *   - Attach the smaller-rank tree under the larger-rank tree
 *   - If ranks are equal, choose one root and increment its rank
 *
 * @param d DSU pointer
 * @param a Element a
 * @param b Element b
 */
void dsu_union(DSU* d, int a, int b) {
    int ra = dsu_find(d, a);
    int rb = dsu_find(d, b);
    if (ra == rb) return; // already in same set

    if (d->rank[ra] < d->rank[rb]) {
        d->parent[ra] = rb;
    } else if (d->rank[ra] > d->rank[rb]) {
        d->parent[rb] = ra;
    } else {
        d->parent[rb] = ra;
        d->rank[ra]++;
    }
}

/**
 * dsu_free
 * --------
 * Frees all memory associated with a DSU.
 *
 * @param d DSU pointer (may not be NULL in this implementation)
 */
void dsu_free(DSU* d) {
    free(d->parent);
    free(d->rank);
    free(d);
}

// ===========================
// Helpers for freeing graphs
// ===========================

/**
 * free_graph
 * ----------
 * Frees an adjacency list graph created by build_graph().
 *
 * @param g Pointer to VecInt array of length n
 * @param n Number of nodes
 */
static void free_graph(VecInt* g, int n) {
    if (!g) return;
    for (int i = 0; i < n; i++) {
        free(g[i].data);
    }
    free(g);
}

/**
 * free_vec_array
 * --------------
 * Frees an array of VecInt where each VecInt owns a heap-allocated data array.
 *
 * Used for:
 *   - The directed DAG adjacency list used in topo_sort tests
 *   - The neighbor and weight arrays used in dijkstra_simple tests
 *
 * @param arr Pointer to VecInt array of length n
 * @param n   Number of vectors in the array
 */
static void free_vec_array(VecInt* arr, int n) {
    if (!arr) return;
    for (int i = 0; i < n; i++) {
        free(arr[i].data);
    }
    free(arr);
}

// ===========================
// Tests
// ===========================

/**
 * test_bfs_and_components
 * -----------------------
 * Builds a connected undirected graph, runs BFS from node 0, prints distances,
 * and verifies number of connected components is 1.
 */
static void test_bfs_and_components() {
    printf("=== Test 1: BFS shortest paths & components (undirected) ===\n");

    // Graph:
    // 0 - 1 - 2 - 4
    //     |
    //     3
    //
    // Distances from 0: [0,1,2,2,3], components = 1
    int n = 5;
    int edges[][2] = {
        {0,1},
        {1,2},
        {1,3},
        {2,4}
    };
    int m = sizeof(edges) / sizeof(edges[0]);

    // Build adjacency list
    VecInt* g = build_graph(n, m, edges);

    // Run BFS shortest paths from node 0
    int dist[5];
    bfs_shortest(n, g, 0, dist);

    // Print distances
    printf("Distances from node 0:\n");
    for (int i = 0; i < n; i++) {
        if (dist[i] >= INT_MAX / 4) {
            printf("  dist[%d] = INF\n", i);
        } else {
            printf("  dist[%d] = %d\n", i, dist[i]);
        }
    }

    // Count connected components using DFS
    int comps = count_components(n, g);
    printf("Connected components = %d (expected 1)\n\n", comps);

    // Free graph memory
    free_graph(g, n);
}

/**
 * test_components_disconnected
 * ----------------------------
 * Builds a disconnected undirected graph and verifies the component count.
 */
static void test_components_disconnected() {
    printf("=== Test 2: Components on disconnected graph ===\n");

    // Graph:
    // 0 - 1 - 2   3 - 4   5
    //
    // Components = 3
    int n = 6;
    int edges[][2] = {
        {0,1},
        {1,2},
        {3,4}
    };
    int m = sizeof(edges) / sizeof(edges[0]);

    VecInt* g = build_graph(n, m, edges);

    int comps = count_components(n, g);
    printf("Connected components = %d (expected 3)\n\n", comps);

    free_graph(g, n);
}

/**
 * test_topo_sort
 * --------------
 * Constructs a small DAG and runs topo_sort() (Kahn's algorithm).
 *
 * Prints one possible topological order if the graph is acyclic, otherwise
 * reports a cycle.
 *
 * Note:
 *   The adjacency lists here are used as directed edges.
 */
static void test_topo_sort() {
    printf("=== Test 3: Topological sort on DAG ===\n");

    // Classic DAG:
    // 5 -> 2, 5 -> 0
    // 4 -> 0, 4 -> 1
    // 2 -> 3
    // 3 -> 1
    //
    // One valid topological order: 5,4,2,3,1,0 (among others).
    int n = 6;

    // Allocate adjacency list for directed graph
    VecInt* dag = (VecInt*)malloc(sizeof(VecInt) * n);
    for (int i = 0; i < n; i++) {
        vec_init(&dag[i], 4);
    }

    // Add directed edges (u -> v)
    vec_push(&dag[5], 2);
    vec_push(&dag[5], 0);
    vec_push(&dag[4], 0);
    vec_push(&dag[4], 1);
    vec_push(&dag[2], 3);
    vec_push(&dag[3], 1);

    int* order = (int*)malloc(sizeof(int) * n);
    int got = topo_sort(n, dag, order);

    if (got < n) {
        printf("Graph has a cycle (topo_sort returned %d < %d)\n\n", got, n);
    } else {
        printf("Topological order (one possible): ");
        for (int i = 0; i < n; i++) {
            printf("%d%s", order[i], (i + 1 == n ? "" : " "));
        }
        printf("\n\n");
    }

    free(order);
    free_vec_array(dag, n);
}

/**
 * test_dijkstra_simple
 * --------------------
 * Builds a weighted directed graph using parallel adjacency/weight arrays and
 * runs the O(n^2) Dijkstra implementation from node 0.
 */
static void test_dijkstra_simple() {
    printf("=== Test 4: Dijkstra (simple O(n^2)) ===\n");

    // CLRS-style example:
    // 0 -> 1 (10), 0 -> 3 (5)
    // 1 -> 2 (1), 1 -> 3 (2)
    // 2 -> 4 (4)
    // 3 -> 1 (3), 3 -> 2 (9), 3 -> 4 (2)
    // 4 -> 0 (7), 4 -> 2 (6)
    //
    // Shortest distances from 0:
    //   0: 0
    //   1: 8
    //   2: 9
    //   3: 5
    //   4: 7
    int n = 5;

    // neigh[u] lists outgoing neighbors from u
    // w[u] lists corresponding edge weights (parallel to neigh[u])
    VecInt* neigh = (VecInt*)malloc(sizeof(VecInt) * n);
    VecInt* w     = (VecInt*)malloc(sizeof(VecInt) * n);

    for (int i = 0; i < n; i++) {
        vec_init(&neigh[i], 4);
        vec_init(&w[i], 4);
    }

    // helper macro to add directed edge u -> v with weight wt
    // Adds v to neigh[u] and wt to w[u] at the same index.
    #define ADD_EDGE(u,v,wt) \
        vec_push(&neigh[u], (v)); \
        vec_push(&w[u], (wt));

    ADD_EDGE(0, 1, 10);
    ADD_EDGE(0, 3, 5);
    ADD_EDGE(1, 2, 1);
    ADD_EDGE(1, 3, 2);
    ADD_EDGE(2, 4, 4);
    ADD_EDGE(3, 1, 3);
    ADD_EDGE(3, 2, 9);
    ADD_EDGE(3, 4, 2);
    ADD_EDGE(4, 0, 7);
    ADD_EDGE(4, 2, 6);

    #undef ADD_EDGE

    long long dist[5];
    dijkstra_simple(n, neigh, w, 0, dist);

    printf("Distances from node 0:\n");
    for (int i = 0; i < n; i++) {
        printf("  dist[%d] = %lld\n", i, dist[i]);
    }
    printf("Expected: [0, 8, 9, 5, 7]\n\n");

    free_vec_array(neigh, n);
    free_vec_array(w, n);
}

/**
 * test_dsu
 * --------
 * Demonstrates DSU operations:
 *   - create sets {0},{1},{2},{3},{4}
 *   - union (0,1), (1,2), (3,4)
 *   - verify there are 2 disjoint sets: {0,1,2} and {3,4}
 */
static void test_dsu() {
    printf("=== Test 5: Disjoint Set Union (DSU) ===\n");

    // Elements: 0,1,2,3,4
    // unions: (0,1), (1,2), (3,4)
    // Sets: {0,1,2}, {3,4}
    int n = 5;
    DSU* d = dsu_create(n);

    dsu_union(d, 0, 1);
    dsu_union(d, 1, 2);
    dsu_union(d, 3, 4);

    // Compute representative for each element
    int repr[5];
    for (int i = 0; i < n; i++) {
        repr[i] = dsu_find(d, i);
    }

    printf("Representatives after unions:\n");
    for (int i = 0; i < n; i++) {
        printf("  find(%d) = %d\n", i, repr[i]);
    }

    // Count distinct representatives to estimate number of sets
    // (Note: representatives might not be 0..n-1 in a compact way,
    //  but they are valid indices here because parents store indices.)
    int count = 0;
    int* seen = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        int r = repr[i];
        if (!seen[r]) {
            seen[r] = 1;
            count++;
        }
    }
    printf("Number of disjoint sets = %d (expected 2)\n\n", count);

    free(seen);
    dsu_free(d);
}

// ===========================
// main
// ===========================

/**
 * main
 * ----
 * Runs all test routines for:
 *   1) BFS shortest paths + connected components
 *   2) Connected components on disconnected graph
 *   3) Topological sort (DAG)
 *   4) Dijkstra shortest paths (simple O(n^2))
 *   5) DSU / Union-Find
 */
int main(void) {
    test_bfs_and_components();
    test_components_disconnected();
    test_topo_sort();
    test_dijkstra_simple();
    test_dsu();
    return 0;
}
