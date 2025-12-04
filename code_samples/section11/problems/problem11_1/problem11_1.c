#include <stdio.h>
#include <stdlib.h>

// ===========================
// Dynamic int vector (adjacency list)
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

VecInt *build_graph(int n, int m, int edges[][2]) {
    VecInt *graph = (VecInt *)malloc(sizeof(VecInt) * n);
    for (int i = 0; i < n; i++) {
        vec_init(&graph[i], 4);
    }
    for (int i = 0; i < m; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        vec_push(&graph[u], v);
        vec_push(&graph[v], u);  // undirected
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

void bfs_shortest(int n, VecInt *graph, int s, int *dist) {
    // initialize distances to -1 (unreachable)
    for (int i = 0; i < n; i++) {
        dist[i] = -1;
    }

    // simple queue using an array
    int *q = (int *)malloc(sizeof(int) * n);
    int head = 0, tail = 0;

    dist[s] = 0;
    q[tail++] = s;

    while (head < tail) {
        int u = q[head++];
        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].data[i];
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q[tail++] = v;
            }
        }
    }

    free(q);
}

// ===========================
// Tests
// ===========================

static void print_distances(const int *dist, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", dist[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]");
}

static void test_bfs_connected() {
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
    int edges[][2] = {
        {0, 1},
        {1, 2},
        {1, 3},
        {2, 4}
    };
    int m = (int)(sizeof(edges) / sizeof(edges[0]));

    VecInt *g = build_graph(n, m, edges);
    int *dist = (int *)malloc(sizeof(int) * n);

    bfs_shortest(n, g, 0, dist);

    printf("Distances from 0: ");
    print_distances(dist, n);
    printf("\nExpected:         [0, 1, 2, 2, 3]\n\n");

    free(dist);
    free_graph(g, n);
}

static void test_bfs_disconnected() {
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
    printf("=== Test 3: Single node ===\n");
    // Graph: single vertex 0, no edges
    // From source 0: dist[0] = 0

    int n = 1;
    int m = 0;

    VecInt *g = build_graph(n, m, NULL);
    int dist[1];

    bfs_shortest(n, g, 0, dist);

    printf("Distances from 0: ");
    print_distances(dist, n);
    printf("\nExpected:         [0]\n\n");

    free_graph(g, n);
}

int main(void) {
    test_bfs_connected();
    test_bfs_disconnected();
    test_bfs_single_node();
    return 0;
}
