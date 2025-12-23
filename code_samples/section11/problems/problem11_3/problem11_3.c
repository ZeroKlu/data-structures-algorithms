#include <stdio.h>   // printf() for output
#include <stdlib.h>  // malloc(), realloc(), calloc(), free()

// ===============================
// Simple dynamic integer vector
// ===============================
//
// VecInt is a minimal "dynamic array of int" used to store adjacency lists.
// Each VecInt holds:
//   - data: pointer to allocated storage
//   - size: number of valid elements stored
//   - capacity: allocated capacity (in number of ints)
//
// This is used to represent a directed graph as an adjacency list:
//   VecInt adj[n];
// where adj[u] contains all outgoing neighbors v for edges u -> v.
typedef struct {
    int *data;      // pointer to contiguous storage of ints
    int size;       // number of elements currently stored
    int capacity;   // allocated capacity of data[] in ints
} VecInt;

void vec_init(VecInt *v, int cap) {
    // Initialize a VecInt with an initial capacity.
    //
    // Parameters:
    //   v   : vector to initialize
    //   cap : initial capacity (number of ints to allocate)
    //
    // Behavior:
    //   - Allocates an array of 'cap' ints.
    //   - Sets size to 0 and capacity to cap.
    //
    // Notes:
    //   - No error checking on malloc().
    //   - cap should be > 0; malloc(0) behavior is implementation-defined.
    v->data = malloc(sizeof(int) * cap);
    v->size = 0;
    v->capacity = cap;
}

void vec_push(VecInt *v, int x) {
    // Append a value to the vector, growing capacity as needed.
    //
    // Parameters:
    //   v : vector to append into
    //   x : value to append
    //
    // Behavior:
    //   - If size == capacity, doubles capacity and reallocates.
    //   - Stores x at data[size] and increments size.
    //
    // Notes:
    //   - No error checking on realloc().
    //   - Assumes capacity is positive. If capacity were 0, doubling keeps it 0.
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, sizeof(int) * v->capacity);
    }
    v->data[v->size++] = x;
}

void vec_free(VecInt *v) {
    // Free the internal buffer for a VecInt.
    //
    // Parameters:
    //   v : vector whose data buffer should be freed
    //
    // Notes:
    //   - This frees only v->data. The VecInt struct itself is not freed
    //     because in this file VecInt objects are stack-allocated (adj1[6], etc.).
    //   - Does not reset fields; the VecInt should not be reused unless re-initialized.
    free(v->data);
}

// ===============================
// Queue for Kahn's algorithm
// ===============================
//
// This queue is used in Kahn's topological sort to process nodes with indegree 0.
//
// Implementation:
//   - Simple array-backed queue with head/tail indices.
//   - Intended usage here enqueues each node at most once, so capacity n is sufficient.
//   - Not a circular buffer: head/tail only move forward.
//     (That's fine for this algorithm because we do at most n enqueues/dequeues.)
typedef struct {
    int *data;          // underlying storage for queued ints
    int head, tail;     // dequeue from head, enqueue at tail
    int capacity;       // maximum number of elements the buffer can hold
} Queue;

void queue_init(Queue *q, int cap) {
    // Initialize the queue with capacity cap.
    //
    // Parameters:
    //   q   : queue to initialize
    //   cap : maximum number of enqueued elements
    //
    // Behavior:
    //   - Allocates an array of cap ints.
    //   - Sets head and tail to 0 (empty queue).
    //
    // Notes:
    //   - No error checking on malloc().
    q->data = malloc(sizeof(int) * cap);
    q->head = q->tail = 0;
    q->capacity = cap;
}

int queue_empty(Queue *q) {
    // Return 1 if queue is empty, 0 otherwise.
    //
    // Empty condition for this simple implementation:
    //   head == tail
    return q->head == q->tail;
}

void enqueue(Queue *q, int x) {
    // Add an item to the end of the queue.
    //
    // Parameters:
    //   q : queue
    //   x : value to enqueue
    //
    // Notes:
    //   - No bounds checking; assumes q has enough capacity.
    //   - In Kahn's algorithm with cap = n, this is safe because each node is
    //     enqueued at most once (when its indegree becomes 0).
    q->data[q->tail++] = x;
}

int dequeue(Queue *q) {
    // Remove and return the item at the front of the queue.
    //
    // Parameters:
    //   q : queue
    //
    // Notes:
    //   - No empty-check here; caller must ensure queue is not empty.
    return q->data[q->head++];
}

void queue_free(Queue *q) {
    // Free the underlying queue buffer.
    //
    // Parameters:
    //   q : queue whose data buffer should be freed
    free(q->data);
}

// ===============================
// Topological Sort (Kahn's Algorithm)
// Returns length of result array in *outSize
// Returns NULL if cycle exists
// ===============================
//
// Topological sort is defined for DAGs (Directed Acyclic Graphs).
// Kahn's algorithm:
//   1) Compute indegree of every node.
//   2) Initialize a queue with all nodes of indegree 0.
//   3) Repeatedly dequeue a node u, append it to output order,
//      and decrement indegree of u's outgoing neighbors.
//      If a neighbor's indegree becomes 0, enqueue it.
//   4) If output contains all nodes, it's a valid topo order.
//      Otherwise, the graph has at least one cycle.
//
// Memory conventions in this function:
//   - Returns a newly allocated int[] of length n on success (caller frees).
//   - Returns NULL on cycle; sets *outSize to 0.
//
// Parameters:
//   n       : number of nodes
//   adj     : adjacency list (directed edges u -> v stored in adj[u])
//   outSize : output parameter; set to n on success, 0 on failure
int* topoSortKahn(int n, VecInt *adj, int *outSize) {
    // indeg[v] = number of incoming edges to v
    int *indeg = calloc(n, sizeof(int)); // zero-initialized
    int *order = malloc(sizeof(int) * n); // output order
    int idx = 0; // number of nodes placed into order so far

    // Compute indegree for each node by scanning all adjacency lists.
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < adj[u].size; i++) {
            int v = adj[u].data[i];
            indeg[v]++;
        }
    }

    // Push all nodes with indegree 0 into the queue.
    Queue q;
    queue_init(&q, n);
    for (int u = 0; u < n; u++)
        if (indeg[u] == 0)
            enqueue(&q, u);

    // Kahn's BFS-like processing:
    // Each node is output once; each directed edge is processed once when
    // decrementing indegrees.
    while (!queue_empty(&q)) {
        int u = dequeue(&q);
        order[idx++] = u;

        // "Remove" u's outgoing edges by decrementing neighbors' indegree.
        for (int i = 0; i < adj[u].size; i++) {
            int v = adj[u].data[i];
            if (--indeg[v] == 0)
                enqueue(&q, v);
        }
    }

    // Clean up temporary structures.
    queue_free(&q);
    free(indeg);

    // If we couldn't output all nodes, there is a cycle.
    // In a cyclic graph, some nodes never reach indegree 0.
    if (idx != n) {
        free(order);
        *outSize = 0;
        return NULL;
    }

    // Success: order contains a valid topological ordering of all nodes.
    *outSize = n;
    return order;
}

// ===============================
// Helper to print arrays
// ===============================
void print_array(int *arr, int n) {
    // Prints an integer array in "[a, b, c]" format.
    //
    // Special-case:
    //   - If arr is NULL, prints an empty representation and indicates a cycle.
    //   - Used by tests to display topo sort results.
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
    // Adds a directed edge u -> v by appending v to adj[u].
    //
    // Parameters:
    //   adj : adjacency list array
    //   u   : source node
    //   v   : destination node
    //
    // Note:
    //   - This is for a directed graph (unlike many earlier examples which were undirected).
    vec_push(&adj[u], v);
}

// ===============================
// Test Harness
// ===============================
int main(void) {

    // ============ Test 1 ============
    printf("=== Test 1: Simple DAG ===\n");

    // This test uses a classic DAG example:
    // 5→2, 5→0
    // 4→0, 4→1
    // 2→3
    // 3→1
    //
    // A valid topological order exists; many are possible.
    int n1 = 6;

    // Stack-allocated adjacency list array of length 6.
    // Each VecInt is initialized with a small capacity.
    VecInt adj1[6];
    for (int i = 0; i < n1; i++) vec_init(&adj1[i], 4);

    // Add directed edges as listed above.
    addEdge(adj1, 5, 2);
    addEdge(adj1, 5, 0);
    addEdge(adj1, 4, 0);
    addEdge(adj1, 4, 1);
    addEdge(adj1, 2, 3);
    addEdge(adj1, 3, 1);

    // Run topo sort and print result.
    int size1;
    int *order1 = topoSortKahn(n1, adj1, &size1);
    printf("Topological order: ");
    print_array(order1, size1);
    printf("\n");

    // Free returned topo order (allocated in topoSortKahn).
    free(order1);

    // Free adjacency lists for this test.
    for (int i = 0; i < n1; i++) vec_free(&adj1[i]);

    // ============ Test 2 ============
    printf("=== Test 2: Another DAG ===\n");

    // Graph:
    // 0→1, 0→2
    // 1→3
    // 2→3
    //
    // Valid topological orders include [0,1,2,3] and [0,2,1,3].
    int n2 = 4;
    VecInt adj2[4];
    for (int i = 0; i < n2; i++) vec_init(&adj2[i], 4);

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

    // Cycle:
    // 0→1→2→0
    //
    // A topological ordering does not exist for graphs with cycles.
    // topoSortKahn should return NULL and outSize should be set to 0.
    int n3 = 3;
    VecInt adj3[3];
    for (int i = 0; i < n3; i++) vec_init(&adj3[i], 4);

    addEdge(adj3, 0, 1);
    addEdge(adj3, 1, 2);
    addEdge(adj3, 2, 0);

    int size3;
    int *order3 = topoSortKahn(n3, adj3, &size3);
    printf("Result: ");
    print_array(order3, size3);
    printf("\n");

    // No free(order3) here because order3 is NULL when a cycle is detected.
    // (free(NULL) is safe, but this code simply doesn't call free.)
    for (int i = 0; i < n3; i++) vec_free(&adj3[i]);

    return 0; // success
}
