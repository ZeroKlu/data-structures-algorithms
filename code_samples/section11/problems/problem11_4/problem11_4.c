#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* ============================
   Adjacency List Structures
   ============================ */

typedef struct {
    int to;
    int w;
} Edge;

typedef struct {
    Edge *data;
    int size;
    int capacity;
} VecEdge;

void vec_init(VecEdge *v, int cap) {
    v->data = (Edge *)malloc(sizeof(Edge) * cap);
    v->size = 0;
    v->capacity = cap;
}

void vec_push(VecEdge *v, int to, int w) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = (Edge *)realloc(v->data, sizeof(Edge) * v->capacity);
    }
    v->data[v->size].to = to;
    v->data[v->size].w = w;
    v->size++;
}

void free_adj(VecEdge *adj, int n) {
    for (int i = 0; i < n; i++) {
        free(adj[i].data);
    }
}

/* ============================
   Min-Heap for (dist, node)
   ============================ */

typedef struct {
    int dist;
    int node;
} HeapItem;

typedef struct {
    HeapItem *data;
    int size;
    int capacity;
} MinHeap;

void heap_init(MinHeap *h, int cap) {
    h->data = (HeapItem *)malloc(sizeof(HeapItem) * cap);
    h->size = 0;
    h->capacity = cap;
}

void heap_swap(HeapItem *a, HeapItem *b) {
    HeapItem tmp = *a;
    *a = *b;
    *b = tmp;
}

void heap_ensure_capacity(MinHeap *h) {
    if (h->size >= h->capacity) {
        h->capacity *= 2;
        h->data = (HeapItem *)realloc(h->data, sizeof(HeapItem) * h->capacity);
    }
}

void heap_push(MinHeap *h, int dist, int node) {
    heap_ensure_capacity(h);
    int i = h->size++;
    h->data[i].dist = dist;
    h->data[i].node = node;

    // sift up
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[p].dist <= h->data[i].dist) break;
        heap_swap(&h->data[p], &h->data[i]);
        i = p;
    }
}

int heap_pop_min(MinHeap *h, int *outDist, int *outNode) {
    if (h->size == 0) return 0;
    *outDist = h->data[0].dist;
    *outNode = h->data[0].node;

    h->data[0] = h->data[--h->size];

    // sift down
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
    free(h->data);
}

/* ============================
   Dijkstra (Non-Negative Weights)
   adj[u] = list of edges (u -> v, w)
   ============================ */

int* dijkstra(int n, VecEdge *adj, int s) {
    const int INF = INT_MAX / 2;
    int *dist = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) dist[i] = INF;
    dist[s] = 0;

    MinHeap h;
    heap_init(&h, 16);
    heap_push(&h, 0, s);

    while (h.size > 0) {
        int d, u;
        if (!heap_pop_min(&h, &d, &u)) break;
        if (d > dist[u]) continue;  // outdated entry

        for (int i = 0; i < adj[u].size; i++) {
            int v = adj[u].data[i].to;
            int w = adj[u].data[i].w;
            int nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                heap_push(&h, nd, v);
            }
        }
    }

    heap_free(&h);
    return dist;
}

/* ============================
   Helpers & Tests
   ============================ */

void add_edge(VecEdge *adj, int u, int v, int w) {
    vec_push(&adj[u], v, w);  // directed edge u -> v
}

void print_distances(int *dist, int n) {
    int INF = INT_MAX / 2;
    printf("[");
    for (int i = 0; i < n; i++) {
        if (dist[i] >= INF / 2) printf("INF");
        else printf("%d", dist[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]\n");
}

void run_test(const char *name, int n, VecEdge *adj, int src) {
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
        int n = 5;
        VecEdge *adj = (VecEdge *)malloc(sizeof(VecEdge) * n);
        for (int i = 0; i < n; i++) vec_init(&adj[i], 4);

        // 0→1 (2), 0→2 (5)
        // 1→2 (1)
        // 1→3 (3)
        // 2→3 (1)
        // 3→4 (2)
        add_edge(adj, 0, 1, 2);
        add_edge(adj, 0, 2, 5);
        add_edge(adj, 1, 2, 1);
        add_edge(adj, 1, 3, 3);
        add_edge(adj, 2, 3, 1);
        add_edge(adj, 3, 4, 2);

        run_test("Simple weighted graph", n, adj, 0);

        free_adj(adj, n);
        free(adj);
    }

    // ============================
    // Test 2: Multiple shortest paths
    // ============================
    {
        printf("=== Test 2: Multiple Shortest Paths ===\n");
        int n = 6;
        VecEdge *adj = (VecEdge *)malloc(sizeof(VecEdge) * n);
        for (int i = 0; i < n; i++) vec_init(&adj[i], 4);

        // 0→1 (1), 0→2 (1)
        // 1→3 (2)
        // 2→3 (2)
        // 3→4 (1)
        // 4→5 (5)
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
        int n = 5;
        VecEdge *adj = (VecEdge *)malloc(sizeof(VecEdge) * n);
        for (int i = 0; i < n; i++) vec_init(&adj[i], 4);

        // Component: 0→1 (4), 1→2 (6)
        add_edge(adj, 0, 1, 4);
        add_edge(adj, 1, 2, 6);
        // Nodes 3 and 4 are unreachable from 0

        run_test("Disconnected graph", n, adj, 0);

        free_adj(adj, n);
        free(adj);
    }

    return 0;
}
