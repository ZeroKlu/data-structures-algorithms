#include <stdio.h>   // printf() for printing test output
#include <stdlib.h>  // malloc(), realloc(), free() for dynamic memory allocation

// ===========================
// Dynamic adjacency list (VecInt)
// ===========================
//
// This file implements a minimal dynamic array of ints (VecInt) and uses an
// array of VecInt as an adjacency list representation of a graph.
//
// Graph representation:
//   - The graph has n nodes labeled 0..n-1
//   - The adjacency list is VecInt *adj of length n
//   - adj[u] stores all neighbors v of node u in adj[u].data[0..size-1]
//
// This is a lightweight substitute for std::vector<int> in C.

typedef struct {
    int *data;      // pointer to allocated array of neighbors
    int size;       // number of neighbors currently stored
    int capacity;   // allocated capacity of data[] (in number of ints)
} VecInt;

static void vec_init(VecInt *v, int cap) {
    // Initialize a VecInt with an initial capacity.
    //
    // Parameters:
    //   v   : vector object to initialize
    //   cap : initial number of int slots to allocate
    //
    // Behavior:
    //   - Allocates cap ints via malloc.
    //   - Sets size to 0 and capacity to cap.
    //
    // Notes:
    //   - No error checking on malloc() result.
    //   - cap should be > 0; malloc(0) is implementation-defined.
    v->data = (int *)malloc(sizeof(int) * cap);
    v->size = 0;
    v->capacity = cap;
}

static void vec_push(VecInt *v, int x) {
    // Append x to the end of the vector, resizing if needed.
    //
    // Parameters:
    //   v : vector to append to
    //   x : value to append
    //
    // Behavior:
    //   - If size == capacity, doubles capacity and reallocates.
    //   - Stores x at data[size] and increments size.
    //
    // Notes:
    //   - Assumes capacity is positive. If capacity were 0, doubling keeps it 0.
    //   - No error checking on realloc() result.
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = (int *)realloc(v->data, sizeof(int) * v->capacity);
    }
    v->data[v->size++] = x;
}

static void free_graph(VecInt *g, int n) {
    // Free a graph adjacency list created by build_graph().
    //
    // Parameters:
    //   g : pointer to array of VecInt (length n)
    //   n : number of nodes / length of g
    //
    // Behavior:
    //   - Frees each adjacency list buffer g[i].data
    //   - Frees the outer array g
    //
    // Notes:
    //   - This function assumes g is non-NULL.
    //   - free(NULL) is safe, but g itself being NULL would be unsafe to index;
    //     this implementation does not guard against g == NULL.
    for (int i = 0; i < n; i++)
        free(g[i].data);
    free(g);
}

// ===========================
// Build undirected graph
// edges[i][0], edges[i][1] are endpoints (0-based)
// ===========================
//
// Builds an UNDIRECTED adjacency list from an edge list.
// For each edge (u, v) we store both u->v and v->u.

VecInt *build_graph(int n, int m, int edges[][2]) {
    // Parameters:
    //   n     : number of nodes
    //   m     : number of edges
    //   edges : array of m pairs {u, v}; can be NULL if m == 0
    //
    // Returns:
    //   adj : adjacency list (VecInt array of length n)
    //
    // Notes:
    //   - No bounds checking for u and v.
    //   - Parallel edges and self-loops are stored as provided.
    //   - Passing edges == NULL is safe only when m == 0 because the edge loop
    //     will perform 0 iterations (as used in Test 3).
    VecInt *adj = (VecInt *)malloc(sizeof(VecInt) * n);

    // Initialize each node's adjacency vector with a small starting capacity.
    for (int i = 0; i < n; i++)
        vec_init(&adj[i], 4);

    // Insert edges into adjacency lists (both directions for undirected graph).
    for (int i = 0; i < m; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        vec_push(&adj[u], v);
        vec_push(&adj[v], u);
    }

    return adj;
}

// ===========================
// DFS for connected components
// comp[u] = component id of node u
// ===========================
//
// This section implements a depth-first search (DFS) used to label connected
// components in an undirected graph.
//
// The array comp[] acts as both:
//   - a visited marker (comp[u] == -1 means unvisited)
//   - the output component label (comp[u] == cid means u is in component cid)

void dfs_comp(int u, int cid, VecInt *adj, int *comp) {
    // Label node u as belonging to component cid.
    comp[u] = cid;

    // Visit all neighbors; recursively label any unvisited neighbors.
    for (int i = 0; i < adj[u].size; i++) {
        int v = adj[u].data[i];
        if (comp[v] == -1) {
            dfs_comp(v, cid, adj, comp);
        }
    }

    // Note:
    //   - This is a recursive DFS. Very large or deep graphs may overflow
    //     the call stack in C.
}

// ===========================
// Count connected components
// Returns: number of components
// ===========================
//
// Walks all nodes, starting a new DFS whenever an unvisited node is found.
// Each DFS labels exactly one connected component.

int connected_components(int n, VecInt *adj, int *comp) {
    // Initialize all nodes as unvisited by setting comp[u] = -1.
    for (int i = 0; i < n; i++)
        comp[i] = -1;

    int cid = 0; // component id counter; also ends as the total component count

    for (int u = 0; u < n; u++) {
        // If node u is unvisited, it starts a new connected component.
        if (comp[u] == -1) {
            dfs_comp(u, cid, adj, comp);
            cid++;
        }
    }

    // After processing all nodes, cid equals the number of connected components.
    return cid;
}

// ===========================
// Helpers for testing
// ===========================

void print_array(const int *arr, int n) {
    // Print an integer array in "[a, b, c]" format.
    //
    // Parameters:
    //   arr : pointer to array
    //   n   : number of elements to print
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]");
}

void run_test(const char *name, int n, VecInt *adj) {
    // Runs connected component labeling and prints:
    //   - component count
    //   - component id assigned to each node
    //
    // Parameters:
    //   name : test name printed to console
    //   n    : number of nodes
    //   adj  : adjacency list
    printf("%s\n", name);

    // Allocate component id array (output).
    int *comp = (int *)malloc(sizeof(int) * n);

    // Compute connected components and fill comp[] with component ids.
    int count = connected_components(n, adj, comp);

    printf("Component count = %d\n", count);
    printf("Component IDs   = ");
    print_array(comp, n);
    printf("\n\n");

    free(comp);
}

// ===========================
// Main test driver
// ===========================

int main(void) {

    // -----------------------------------------
    // Test 1: Fully connected graph
    // -----------------------------------------
    {
        printf("=== Test 1: Fully connected graph ===\n");

        int n = 5;

        // edges array sized for up to 10 edges (which matches C(5,2) = 10).
        // This test constructs a clique (complete graph) on 5 nodes.
        int edges[10][2];
        int m = 0;

        // Build clique by adding every pair (u, v) with u < v.
        // A clique should have exactly 1 connected component.
        for (int u = 0; u < n; u++)
            for (int v = u + 1; v < n; v++)
                edges[m][0] = u, edges[m][1] = v, m++;

        VecInt *adj = build_graph(n, m, edges);
        run_test("Complete graph", n, adj);
        free_graph(adj, n);
    }

    // -----------------------------------------
    // Test 2: Two components + isolated node
    // -----------------------------------------
    {
        printf("=== Test 2: Two components ===\n");

        // Graph with:
        //   - component 0: 0-1-2
        //   - component 1: 3-4
        //   - node 5 isolated (component 2)
        //
        // Expected total components: 3
        int n = 6;

        int edges[][2] = {
            {0,1}, {1,2},   // component 0
            {3,4}           // component 1
                            // node 5 isolated
        };

        int m = sizeof(edges) / sizeof(edges[0]);

        VecInt *adj = build_graph(n, m, edges);
        run_test("Two components plus isolated", n, adj);
        free_graph(adj, n);
    }

    // -----------------------------------------
    // Test 3: All nodes isolated
    // -----------------------------------------
    {
        printf("=== Test 3: Isolated nodes ===\n");

        // No edges: every node is its own connected component.
        // Expected total components: n (here, 4).
        int n = 4;

        // Passing edges == NULL is safe because m == 0.
        VecInt *adj = build_graph(n, 0, NULL);
        run_test("4 isolated nodes", n, adj);
        free_graph(adj, n);
    }

    return 0; // success
}
