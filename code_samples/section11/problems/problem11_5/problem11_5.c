#include <stdio.h>
#include <stdlib.h>

/*
 * DSU (Disjoint Set Union) / Union-Find
 *
 * Purpose:
 *   Efficiently maintains a partition of {0..n-1} into disjoint sets under:
 *     - find(x):    get a representative (root) for x's set
 *     - union(a,b): merge the sets containing a and b
 *
 * This implementation uses:
 *   - Path compression in find() to flatten the tree structure over time
 *   - Union by rank in union() to keep trees shallow
 *
 * Amortized complexity:
 *   Near O(1) per operation (inverse Ackermann), very fast in practice.
 */

typedef struct {
    int *parent;  // parent[i] points to i's parent; roots have parent[i] == i
    int *rank;    // rank approximates tree height for union-by-rank heuristic
    int count;    // number of disjoint sets currently tracked
} DSU;

static DSU *dsu_create(int n) {
    /*
     * Allocate and initialize a DSU for n elements (0..n-1).
     *
     * Initialization:
     *   - Each element is its own parent (n singleton sets)
     *   - rank[] starts at 0 for all roots
     *   - count starts at n and decreases with each successful union
     *
     * Memory:
     *   - Caller must eventually call dsu_free() to release allocations.
     */
    DSU *d = (DSU *)malloc(sizeof(DSU));
    d->parent = (int *)malloc(n * sizeof(int));
    d->rank   = (int *)calloc(n, sizeof(int)); // zero-initialized ranks
    d->count  = n;

    for (int i = 0; i < n; i++) {
        d->parent[i] = i; // each node starts as its own root
    }
    return d;
}

static int dsu_find(DSU *d, int x) {
    /*
     * Find the representative (root) for element x.
     *
     * Path compression:
     *   If x is not a root, recursively find its root and set parent[x]
     *   directly to that root. This dramatically speeds up future finds.
     *
     * Returns:
     *   The root index representing x's set.
     */
    if (d->parent[x] != x) {
        d->parent[x] = dsu_find(d, d->parent[x]);
    }
    return d->parent[x];
}

static void dsu_union(DSU *d, int a, int b) {
    /*
     * Union (merge) the sets containing a and b.
     *
     * Steps:
     *   1) Find roots ra, rb.
     *   2) If already same root, nothing to do.
     *   3) Otherwise, attach the shorter tree under the taller tree (union by rank).
     *      If ranks are equal, pick one root and increment its rank.
     *   4) Decrement d->count because two distinct components became one.
     *
     * Note:
     *   Union by rank + path compression keeps operations very fast.
     */
    int ra = dsu_find(d, a);
    int rb = dsu_find(d, b);
    if (ra == rb) return; // already connected; count does not change

    if (d->rank[ra] < d->rank[rb]) {
        d->parent[ra] = rb;
    } else if (d->rank[ra] > d->rank[rb]) {
        d->parent[rb] = ra;
    } else {
        d->parent[rb] = ra;
        d->rank[ra]++; // only increase rank when both ranks were equal
    }

    d->count--; // successful merge reduces number of components by 1
}

static void dsu_free(DSU *d) {
    /*
     * Release all memory owned by the DSU.
     *
     * Safe usage:
     *   Only call this on a DSU pointer returned by dsu_create().
     */
    free(d->parent);
    free(d->rank);
    free(d);
}

// edges: array of pairs (u, v) given as int** (each edges[i] points to 2 ints)
int countComponents(int n, int **edges, int edgesSize) {
    /*
     * Counts connected components in an undirected graph of n nodes (0..n-1),
     * using DSU/Union-Find.
     *
     * Parameters:
     *   n         : number of nodes
     *   edges     : pointer to an array of pointers, where edges[i] points to
     *               an int[2] storing {u, v}
     *   edgesSize : number of edges
     *
     * Interpretation:
     *   Each edge (u, v) indicates that u and v are connected (undirected),
     *   so we union(u, v).
     *
     * Returns:
     *   The number of connected components after processing all edges.
     *
     * Notes:
     *   - This function assumes node ids are valid (0 <= u,v < n).
     *   - If edgesSize == 0, edges may be NULL and the answer is n.
     */
    DSU *d = dsu_create(n);

    for (int i = 0; i < edgesSize; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        dsu_union(d, u, v);
    }

    int ans = d->count;
    dsu_free(d);
    return ans;
}

/* =======================
   Test helpers
   ======================= */

static void run_test(const char *name,
                     int n,
                     int edges[][2],  // may be NULL if edgesSize == 0
                     int edgesSize,
                     int expected)
{
    /*
     * Adapts a stack-allocated edges[][2] array (or NULL) into the int** format
     * expected by countComponents().
     *
     * Why this exists:
     *   - The solver expects edges as int** with each entry pointing to 2 ints.
     *   - In tests, it's convenient to write edges as int edges[][2].
     *
     * Implementation detail:
     *   - We allocate an array of int* pointers (ptrs).
     *   - Each ptrs[i] points to edges[i] (which decays to int*).
     *   - If edgesSize == 0, ptrs remains NULL and is safe to pass along
     *     because countComponents won't dereference it.
     */
    int **ptrs = NULL;

    if (edgesSize > 0) {
        ptrs = (int **)malloc(edgesSize * sizeof(int *));
        for (int i = 0; i < edgesSize; i++) {
            ptrs[i] = edges[i];  // edges[i] decays to int* (points to 2 ints)
        }
    }

    int result = countComponents(n, ptrs, edgesSize);

    printf("%s\n", name);
    printf("n = %d, edgesSize = %d\n", n, edgesSize);
    printf("Connected components = %d (expected %d)\n\n", result, expected);

    // free(NULL) is safe in C, so this is fine even when edgesSize == 0.
    free(ptrs);
}

int main(void) {
    /*
     * Test driver for counting connected components via DSU.
     *
     * Each test constructs an undirected graph on nodes 0..n-1 using an edge list,
     * calls countComponents(), and prints the result alongside the expected value.
     */

    // Test 1: Single connected component (0-1-2-3)
    {
        /*
         * Graph: a chain 0--1--2--3
         * All nodes are connected, so components = 1.
         */
        int n = 4;
        int edges1[][2] = {
            {0, 1},
            {1, 2},
            {2, 3}
        };
        int m1 = (int)(sizeof(edges1) / sizeof(edges1[0]));
        run_test("Test 1: Single component chain", n, edges1, m1, 1);
    }

    // Test 2: Two components + isolated
    {
        /*
         * Graph:
         *   Component A: 0--1--2
         *   Component B: 3--4
         *   Node 5 isolated
         * Total components = 3.
         */
        int n = 6;
        int edges2[][2] = {
            {0, 1},
            {1, 2},
            {3, 4}
        };
        int m2 = (int)(sizeof(edges2) / sizeof(edges2[0]));
        run_test("Test 2: Two components plus isolated node", n, edges2, m2, 3);
    }

    // Test 3: No edges (all isolated)
    {
        /*
         * Graph: n=5 nodes, no edges.
         * Each node is its own component, so components = 5.
         */
        int n = 5;
        int (*edges3)[2] = NULL;  // no edges provided
        int m3 = 0;
        run_test("Test 3: No edges (all isolated)", n, edges3, m3, 5);
    }

    // Test 4: Fully connected with extra edges
    {
        /*
         * Graph: 0--1--2--3--4 plus an extra redundant edge 1--3.
         * Still fully connected, so components = 1.
         * Redundant edges should not change the component count.
         */
        int n = 5;
        int edges4[][2] = {
            {0, 1},
            {1, 2},
            {2, 3},
            {3, 4},
            {1, 3}  // redundant edge (creates a cycle)
        };
        int m4 = (int)(sizeof(edges4) / sizeof(edges4[0]));
        run_test("Test 4: Fully connected with extra edges", n, edges4, m4, 1);
    }

    return 0;
}
