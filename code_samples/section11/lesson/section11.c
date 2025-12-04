#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Dynamic array of integers
typedef struct {
    int* data;
    int size;
    int capacity;
} VecInt;

// Initialize vector
static void vec_init(VecInt* v, int cap) {
    // allocate memory
    v->data = (int*)malloc(sizeof(int) * cap);
    v->size = 0;
    v->capacity = cap;
}

static void vec_push(VecInt* v, int x) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        // reallocate memory
        v->data = (int*)realloc(v->data, sizeof(int) * v->capacity);
    }
    // add element at the end
    v->data[v->size++] = x;
}

// Build adjacency list for undirected graph with n nodes and m edges
// edges[i][0], edges[i][1] are endpoints (0-based)
VecInt* build_graph(int n, int m, int edges[][2]) {
    VecInt* graph = (VecInt*)malloc(sizeof(VecInt) * n);

    // initialize graph
    for (int i = 0; i < n; i++) {
        vec_init(&graph[i], 4);
    }

    // add edges
    for (int i = 0; i < m; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        vec_push(&graph[u], v);
        vec_push(&graph[v], u);
    }
    return graph;
}

// Build adjacency list for undirected graph with n nodes and m edges
void bfs_shortest(int n, VecInt* graph, int src, int* dist) {
    const int INF = INT_MAX / 4;
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }
    int* q = (int*)malloc(sizeof(int) * n);
    int head = 0, tail = 0;

    dist[src] = 0;
    q[tail++] = src;

    while (head < tail) {
        int u = q[head++];
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].data[i];
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                q[tail++] = v;
            }
        }
    }
    free(q);
}

void dfs_visit(int u, VecInt* graph, int* visited) {
    visited[u] = 1;
    for (int i = 0; i < graph[u].size; i++) {
        int v = graph[u].data[i];
        if (!visited[v]) {
            dfs_visit(v, graph, visited);
        }
    }
}

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

int topo_sort(int n, VecInt* graph, int* out_order) {
    int* indeg = (int*)calloc(n, sizeof(int));
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].data[i];
            indeg[v]++;
        }
    }

    int* q = (int*)malloc(sizeof(int) * n);
    int head = 0, tail = 0;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) {
            q[tail++] = i;
        }
    }

    int idx = 0;
    while (head < tail) {
        int u = q[head++];
        out_order[idx++] = u;
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].data[i];
            if (--indeg[v] == 0) {
                q[tail++] = v;
            }
        }
    }

    free(indeg);
    free(q);
    return idx; // if idx < n, there was a cycle
}

// C (simple O(n²) version without a heap)
// graph: for each node u, we have arrays neigh[u], weight[u], with sizes size[u]
void dijkstra_simple(int n, VecInt* neigh, VecInt* w, int src, long long* dist) {
    const long long INF = (long long)4e18;
    int* used = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;

    for (int it = 0; it < n; it++) {
        int u = -1;
        long long best = INF;
        for (int i = 0; i < n; i++) {
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        used[u] = 1;
        for (int i = 0; i < neigh[u].size; i++) {
            int v = neigh[u].data[i];
            int wt = w[u].data[i];
            if (dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
        }
    }
    free(used);
}

typedef struct {
    int* parent;
    int* rank;
    int n;
} DSU;

DSU* dsu_create(int n) {
    DSU* d = (DSU*)malloc(sizeof(DSU));
    d->n = n;
    d->parent = (int*)malloc(sizeof(int) * n);
    d->rank = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        d->parent[i] = i;
    }
    return d;
}

int dsu_find(DSU* d, int x) {
    if (d->parent[x] != x) {
        d->parent[x] = dsu_find(d, d->parent[x]);
    }
    return d->parent[x];
}

void dsu_union(DSU* d, int a, int b) {
    int ra = dsu_find(d, a);
    int rb = dsu_find(d, b);
    if (ra == rb) return;
    if (d->rank[ra] < d->rank[rb]) {
        d->parent[ra] = rb;
    } else if (d->rank[ra] > d->rank[rb]) {
        d->parent[rb] = ra;
    } else {
        d->parent[rb] = ra;
        d->rank[ra]++;
    }
}

void dsu_free(DSU* d) {
    free(d->parent);
    free(d->rank);
    free(d);
}

// ===========================
// Helpers for freeing graphs
// ===========================
static void free_graph(VecInt* g, int n) {
    if (!g) return;
    for (int i = 0; i < n; i++) {
        free(g[i].data);
    }
    free(g);
}

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
    VecInt* g = build_graph(n, m, edges);

    int dist[5];
    bfs_shortest(n, g, 0, dist);

    printf("Distances from node 0:\n");
    for (int i = 0; i < n; i++) {
        if (dist[i] >= INT_MAX / 4) {
            printf("  dist[%d] = INF\n", i);
        } else {
            printf("  dist[%d] = %d\n", i, dist[i]);
        }
    }

    int comps = count_components(n, g);
    printf("Connected components = %d (expected 1)\n\n", comps);

    free_graph(g, n);
}

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

static void test_topo_sort() {
    printf("=== Test 3: Topological sort on DAG ===\n");
    // Classic DAG:
    // 5 -> 2, 5 -> 0
    // 4 -> 0, 4 -> 1
    // 2 -> 3
    // 3 -> 1
    //
    // One valid topological order: 5,4,2,3,1,0 or 4,5,2,3,1,0, etc.
    int n = 6;
    VecInt* dag = (VecInt*)malloc(sizeof(VecInt) * n);
    for (int i = 0; i < n; i++) {
        vec_init(&dag[i], 4);
    }

    // Add directed edges
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
    VecInt* neigh = (VecInt*)malloc(sizeof(VecInt) * n);
    VecInt* w     = (VecInt*)malloc(sizeof(VecInt) * n);
    for (int i = 0; i < n; i++) {
        vec_init(&neigh[i], 4);
        vec_init(&w[i], 4);
    }

    // helper macro to add directed edge u -> v with weight wt
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

    int repr[5];
    for (int i = 0; i < n; i++) {
        repr[i] = dsu_find(d, i);
    }

    printf("Representatives after unions:\n");
    for (int i = 0; i < n; i++) {
        printf("  find(%d) = %d\n", i, repr[i]);
    }

    // Count distinct sets
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
int main(void) {
    test_bfs_and_components();
    test_components_disconnected();
    test_topo_sort();
    test_dijkstra_simple();
    test_dsu();
    return 0;
}
