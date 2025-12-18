#include <stdio.h>
#include <stdlib.h>

// ===============================
// Simple dynamic integer vector
// ===============================
typedef struct {
    int *data;
    int size;
    int capacity;
} VecInt;

void vec_init(VecInt *v, int cap) {
    v->data = malloc(sizeof(int) * cap);
    v->size = 0;
    v->capacity = cap;
}

void vec_push(VecInt *v, int x) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, sizeof(int) * v->capacity);
    }
    v->data[v->size++] = x;
}

void vec_free(VecInt *v) {
    free(v->data);
}

// ===============================
// Queue for Kahn's algorithm
// ===============================
typedef struct {
    int *data;
    int head, tail, capacity;
} Queue;

void queue_init(Queue *q, int cap) {
    q->data = malloc(sizeof(int) * cap);
    q->head = q->tail = 0;
    q->capacity = cap;
}

int queue_empty(Queue *q) {
    return q->head == q->tail;
}

void enqueue(Queue *q, int x) {
    q->data[q->tail++] = x;
}

int dequeue(Queue *q) {
    return q->data[q->head++];
}

void queue_free(Queue *q) {
    free(q->data);
}

// ===============================
// Topological Sort (Kahn's Algorithm)
// Returns length of result array in *outSize
// Returns NULL if cycle exists
// ===============================
int* topoSortKahn(int n, VecInt *adj, int *outSize) {
    int *indeg = calloc(n, sizeof(int));
    int *order = malloc(sizeof(int) * n);
    int idx = 0;

    // Compute indegree
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < adj[u].size; i++) {
            int v = adj[u].data[i];
            indeg[v]++;
        }
    }

    // Push all nodes with indegree 0
    Queue q;
    queue_init(&q, n);
    for (int u = 0; u < n; u++)
        if (indeg[u] == 0)
            enqueue(&q, u);

    // Kahn's BFS
    while (!queue_empty(&q)) {
        int u = dequeue(&q);
        order[idx++] = u;

        for (int i = 0; i < adj[u].size; i++) {
            int v = adj[u].data[i];
            if (--indeg[v] == 0)
                enqueue(&q, v);
        }
    }

    queue_free(&q);
    free(indeg);

    // If not all nodes processed → cycle
    if (idx != n) {
        free(order);
        *outSize = 0;
        return NULL;
    }

    *outSize = n;
    return order;
}

// ===============================
// Helper to print arrays
// ===============================
void print_array(int *arr, int n) {
    if (!arr) {
        printf("[] (cycle detected)\n");
        return;
    }
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]\n");
}

// Add directed edge u -> v
void addEdge(VecInt *adj, int u, int v) {
    vec_push(&adj[u], v);
}

// ===============================
// Test Harness
// ===============================
int main(void) {

    // ============ Test 1 ============
    printf("=== Test 1: Simple DAG ===\n");
    int n1 = 6;
    VecInt adj1[6];
    for (int i = 0; i < n1; i++) vec_init(&adj1[i], 4);

    // 5→2, 5→0
    // 4→0, 4→1
    // 2→3
    // 3→1
    addEdge(adj1, 5, 2);
    addEdge(adj1, 5, 0);
    addEdge(adj1, 4, 0);
    addEdge(adj1, 4, 1);
    addEdge(adj1, 2, 3);
    addEdge(adj1, 3, 1);

    int size1;
    int *order1 = topoSortKahn(n1, adj1, &size1);
    printf("Topological order: ");
    print_array(order1, size1);
    printf("\n");

    free(order1);
    for (int i = 0; i < n1; i++) vec_free(&adj1[i]);

    // ============ Test 2 ============
    printf("=== Test 2: Another DAG ===\n");
    int n2 = 4;
    VecInt adj2[4];
    for (int i = 0; i < n2; i++) vec_init(&adj2[i], 4);

    // 0→1, 0→2
    // 1→3
    // 2→3
    addEdge(adj2, 0, 1);
    addEdge(adj2, 0, 2);
    addEdge(adj2, 1, 3);
    addEdge(adj2, 2, 3);

    int size2;
    int *order2 = topoSortKahn(n2, adj2, &size2);
    printf("Topological order: ");
    print_array(order2, size2);
    printf("\n");

    free(order2);
    for (int i = 0; i < n2; i++) vec_free(&adj2[i]);

    // ============ Test 3 ============
    printf("=== Test 3: Graph with cycle ===\n");
    int n3 = 3;
    VecInt adj3[3];
    for (int i = 0; i < n3; i++) vec_init(&adj3[i], 4);

    // Cycle: 0→1→2→0
    addEdge(adj3, 0, 1);
    addEdge(adj3, 1, 2);
    addEdge(adj3, 2, 0);

    int size3;
    int *order3 = topoSortKahn(n3, adj3, &size3);
    printf("Result: ");
    print_array(order3, size3);
    printf("\n");

    for (int i = 0; i < n3; i++) vec_free(&adj3[i]);

    return 0;
}
