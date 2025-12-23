#include <stdio.h>   // printf() for test output
#include <stdlib.h>  // malloc(), realloc(), free() for dynamic memory management

// ===========================
// Dynamic int vector (adjacency list)
// ===========================
//
// This file implements a minimal "dynamic array of int" (VecInt) and uses it
// to represent an adjacency list for an undirected, unweighted graph.
//
// VecInt is used like:
//   VecInt adj; vec_init(&adj, 4); vec_push(&adj, neighbor);
//
// The graph is represented as:
//   VecInt *graph = malloc(sizeof(VecInt) * n);
// where graph[u] is the dynamic list of neighbors of vertex u.

typedef struct {
    int *data;      // pointer to contiguous storage holding the elements
    int size;       // current number of valid elements in data[]
    int capacity;   // allocated capacity of data[] (in number of ints)
} VecInt;

static void vec_init(VecInt *v, int cap) {
    // Initialize a VecInt with an initial capacity 'cap'.
    //
    // Parameters:
    //   v   : pointer to VecInt to initialize
    //   cap : initial allocation size (number of ints)
    //
    // Behavior:
    //   - Allocates 'cap' ints via malloc.
    //   - Sets size = 0 and capacity = cap.
    //
    // Note:
    //   - This function does not check for malloc failure (data == NULL).
    //   - cap should be > 0; if cap == 0, malloc(0) behavior is implementation-defined.
    v->data = (int *)malloc(sizeof(int) * cap);
    v->size = 0;
    v->capacity = cap;
}

static void vec_push(VecInt *v, int x) {
    // Append an element to the end of the dynamic array.
    //
    // Parameters:
    //   v : vector to append to
    //   x : element value to append
    //
    // Behavior:
    //   - If the underlying buffer is full, it doubles capacity and reallocates.
    //   - Writes x at data[size] then increments size.
    //
    // Notes / assumptions:
    //   - Assumes v->capacity is positive. If capacity were 0, doubling keeps it 0.
    //   - Does not check realloc failure (realloc could return NULL).
    //   - Growth strategy: doubling gives amortized O(1) push-back.
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = (int *)realloc(v->data, sizeof(int) * v->capacity);
    }
    v->data[v->size++] = x;
}

static void free_graph(VecInt *g, int n) {
    // Free an adjacency-list graph allocated by build_graph().
    //
    // Parameters:
    //   g : pointer to array of VecInt (length n)
    //   n : number of vertices (size of g)
    //
    // Behavior:
    //   - Frees each vertex's adjacency list buffer (g[i].data).
    //   - Frees the array 'g' itself.
    //
    // Safe-guards:
    //   - If g is NULL, returns immediately.
    if (!g) return;
    for (int i = 0; i < n; i++) {
        free(g[i].data);
    }
    free(g);
}

// ===========================
// Build undirected graph
// edges[i][0], edges[i][1] are endpoints (0-based)
// ===========================
//
// This section constructs an undirected adjacency list from an edge list.
// Representation choices:
//   - n vertices, labeled 0..n-1
//   - m edges, each an (u, v) pair
//   - For undirected graphs, store both u->v and v->u in adjacency lists.

VecInt *build_graph(int n, int m, int edges[][2]) {
    // Build an UNDIRECTED graph adjacency list.
    //
    // Parameters:
    //   n     : number of vertices
    //   m     : number of edges
    //   edges : an array of m pairs, where edges[i][0]=u and edges[i][1]=v
    //
    // Returns:
    //   graph : pointer to a dynamically allocated array of VecInt of length n
    //
    // Behavior:
    //   - Initializes each adjacency list with capacity 4.
    //   - Adds each edge (u, v) in both directions.
    //
    // Notes:
    //   - No validation: u and v are assumed within 0..n-1.
    //   - Parallel edges/self-loops are stored as given.
    //   - This function assumes that if m == 0, it will not read from edges.
    //     (In this file, test_bfs_single_node passes edges==NULL with m==0;
    //      the loop does 0 iterations, so it is safe.)
    VecInt *graph = (VecInt *)malloc(sizeof(VecInt) * n);

    // Initialize each vertex's adjacency list.
    for (int i = 0; i < n; i++) {
        vec_init(&graph[i], 4);
    }

    // Add edges to adjacency lists.
    for (int i = 0; i < m; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        vec_push(&graph[u], v);
        vec_push(&graph[v], u);  // undirected: add reverse direction too
    }

    return graph;
}

// ===========================
// BFS shortest paths in unweighted graph
// - n: number of vertices [0..n-1]
// - graph: adjacency list
// - s: source vertex
// - dist: output array of length n
//   dist[v] = min number of edges from s to v, or -1 if unreachable
// ===========================
//
// This section implements BFS over the adjacency list to compute shortest distances
// in an unweighted graph from a single source s.
//
// Key properties:
//   - BFS explores nodes in "layers" of increasing distance from s.
//   - The first time a node is discovered is via the shortest path (in an unweighted graph).
//   - Using dist[v] == -1 as the "unvisited" marker is a common C pattern.

void bfs_shortest(int n, VecInt *graph, int s, int *dist) {
    // Initialize distances to -1 to indicate "unreachable/unvisited".
    for (int i = 0; i < n; i++) {
        dist[i] = -1;
    }

    // Simple queue implemented using a fixed-size array of length n.
    //
    // Why length n is enough:
    //   - In BFS, each node is enqueued at most once (when discovered).
    //   - Therefore the queue never needs to hold more than n entries total.
    int *q = (int *)malloc(sizeof(int) * n);
    int head = 0, tail = 0; // queue region is q[head..tail-1]

    // Seed BFS with the source vertex.
    dist[s] = 0;
    q[tail++] = s;

    while (head < tail) {
        // Dequeue next vertex.
        int u = q[head++];

        // Examine all neighbors of u.
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].data[i];

            // If v hasn't been visited, record its distance and enqueue it.
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q[tail++] = v;
            }
        }
    }

    // Free temporary queue storage.
    free(q);
}

// ===========================
// Tests
// ===========================
//
// The following tests are "print-and-compare" style demonstrations.
// Each test constructs a graph, runs bfs_shortest, prints the result,
// and prints an expected output for visual verification.

static void print_distances(const int *dist, int n) {
    // Print a distance array in a compact "[a, b, c]" format.
    // Parameters:
    //   dist : array of length n
    //   n    : number of elements to print
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", dist[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]");
}

static void test_bfs_connected() {
    // Test 1: a connected undirected graph with 5 nodes.
    // Verifies BFS distances match expected shortest path lengths.
    printf("=== Test 1: Connected graph ===\n");

    // Graph:
    // 0 - 1 - 2 - 4
    //     |
    //     3
    //
    // Shortest distances from 0:
    //   0: 0
    //   1: 1
    //   2: 2
    //   3: 2
    //   4: 3
    int n = 5;

    // Edge list (undirected); build_graph() will store both directions.
    int edges[][2] = {
        {0, 1},
        {1, 2},
        {1, 3},
        {2, 4}
    };

    // m = number of edges in the above static array.
    int m = (int)(sizeof(edges) / sizeof(edges[0]));

    // Build adjacency list and allocate distance array.
    VecInt *g = build_graph(n, m, edges);
    int *dist = (int *)malloc(sizeof(int) * n);

    // Compute BFS distances from source 0.
    bfs_shortest(n, g, 0, dist);

    // Print actual vs expected.
    printf("Distances from 0: ");
    print_distances(dist, n);
    printf("\nExpected:         [0, 1, 2, 2, 3]\n\n");

    // Clean up allocated memory.
    free(dist);
    free_graph(g, n);
}

static void test_bfs_disconnected() {
    // Test 2: a graph with disconnected components.
    // Verifies unreachable nodes remain at -1.
    printf("=== Test 2: Disconnected graph ===\n");

    // Graph:
    // 0 - 1     2     3 - 4
    //
    // From source 0:
    //   dist[0] = 0
    //   dist[1] = 1
    //   dist[2] = -1 (unreachable)
    //   dist[3] = -1
    //   dist[4] = -1
    int n = 5;

    // Only two edges; node 2 is isolated, and {3,4} is a separate component.
    int edges[][2] = {
        {0, 1},
        {3, 4}
    };
    int m = (int)(sizeof(edges) / sizeof(edges[0]));

    VecInt *g = build_graph(n, m, edges);
    int *dist = (int *)malloc(sizeof(int) * n);

    bfs_shortest(n, g, 0, dist);

    printf("Distances from 0: ");
    print_distances(dist, n);
    printf("\nExpected:         [0, 1, -1, -1, -1]\n\n");

    free(dist);
    free_graph(g, n);
}

static void test_bfs_single_node() {
    // Test 3: smallest possible graph.
    // Verifies BFS on a single node graph yields dist[0] = 0.
    printf("=== Test 3: Single node ===\n");

    // Graph: single vertex 0, no edges.
    // From source 0: dist[0] = 0
    int n = 1;
    int m = 0;

    // Passing edges==NULL is safe here because m==0, so build_graph's edge loop runs 0 times.
    VecInt *g = build_graph(n, m, NULL);

    // Use a stack-allocated distance array for n=1.
    int dist[1];

    bfs_shortest(n, g, 0, dist);

    printf("Distances from 0: ");
    print_distances(dist, n);
    printf("\nExpected:         [0]\n\n");

    free_graph(g, n);
}

int main(void) {
    // Program entry point: run all BFS tests.
    test_bfs_connected();
    test_bfs_disconnected();
    test_bfs_single_node();
    return 0; // indicate successful execution
}
