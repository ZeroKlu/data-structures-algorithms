#include <stdio.h>
#include <stdlib.h>

// ===========================
// Dynamic adjacency list (VecInt)
// ===========================

typedef struct {
    int *data;
    int size;
    int capacity;
} VecInt;

static void vec_init(VecInt *v, int cap) {
    v->data = (int *)malloc(sizeof(int) * cap);
    v->size = 0;
    v->capacity = cap;
}

static void vec_push(VecInt *v, int x) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = (int *)realloc(v->data, sizeof(int) * v->capacity);
    }
    v->data[v->size++] = x;
}

static void free_graph(VecInt *g, int n) {
    for (int i = 0; i < n; i++)
        free(g[i].data);
    free(g);
}

// ===========================
// Build undirected graph
// edges[i][0], edges[i][1] are endpoints (0-based)
// ===========================

VecInt *build_graph(int n, int m, int edges[][2]) {
    VecInt *adj = (VecInt *)malloc(sizeof(VecInt) * n);
    for (int i = 0; i < n; i++)
        vec_init(&adj[i], 4);

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

void dfs_comp(int u, int cid, VecInt *adj, int *comp) {
    comp[u] = cid;
    for (int i = 0; i < adj[u].size; i++) {
        int v = adj[u].data[i];
        if (comp[v] == -1) {
            dfs_comp(v, cid, adj, comp);
        }
    }
}

// ===========================
// Count connected components
// Returns: number of components
// ===========================

int connected_components(int n, VecInt *adj, int *comp) {
    for (int i = 0; i < n; i++)
        comp[i] = -1;

    int cid = 0;
    for (int u = 0; u < n; u++) {
        if (comp[u] == -1) {
            dfs_comp(u, cid, adj, comp);
            cid++;
        }
    }
    return cid;
}

// ===========================
// Helpers for testing
// ===========================

void print_array(const int *arr, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]");
}

void run_test(const char *name, int n, VecInt *adj) {
    printf("%s\n", name);

    int *comp = (int *)malloc(sizeof(int) * n);
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
        int edges[10][2];
        int m = 0;

        // Build clique
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

        int n = 4;
        VecInt *adj = build_graph(n, 0, NULL);
        run_test("4 isolated nodes", n, adj);
        free_graph(adj, n);
    }

    return 0;
}
