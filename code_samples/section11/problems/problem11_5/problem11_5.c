#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *parent;
    int *rank;
    int count;
} DSU;

static DSU *dsu_create(int n) {
    DSU *d = (DSU *)malloc(sizeof(DSU));
    d->parent = (int *)malloc(n * sizeof(int));
    d->rank   = (int *)calloc(n, sizeof(int));
    d->count  = n;
    for (int i = 0; i < n; i++) {
        d->parent[i] = i;
    }
    return d;
}

static int dsu_find(DSU *d, int x) {
    if (d->parent[x] != x) {
        d->parent[x] = dsu_find(d, d->parent[x]);
    }
    return d->parent[x];
}

static void dsu_union(DSU *d, int a, int b) {
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
    d->count--;
}

static void dsu_free(DSU *d) {
    free(d->parent);
    free(d->rank);
    free(d);
}

// edges: array of pairs (u, v) given as int** (each edges[i] points to 2 ints)
int countComponents(int n, int **edges, int edgesSize) {
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
    // Build int** view over edges[][2], or NULL if edgesSize == 0
    int **ptrs = NULL;
    if (edgesSize > 0) {
        ptrs = (int **)malloc(edgesSize * sizeof(int *));
        for (int i = 0; i < edgesSize; i++) {
            ptrs[i] = edges[i];  // edges[i] decays to int*
        }
    }

    int result = countComponents(n, ptrs, edgesSize);
    printf("%s\n", name);
    printf("n = %d, edgesSize = %d\n", n, edgesSize);
    printf("Connected components = %d (expected %d)\n\n", result, expected);

    free(ptrs);
}

int main(void) {
    // Test 1: Single connected component (0-1-2-3)
    {
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
        // Component A: 0-1-2
        // Component B: 3-4
        // Node 5 isolated
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
        int n = 5;
        int (*edges3)[2] = NULL;  // no edges
        int m3 = 0;
        run_test("Test 3: No edges (all isolated)", n, edges3, m3, 5);
    }

    // Test 4: Fully connected with extra edges
    {
        int n = 5;
        int edges4[][2] = {
            {0, 1},
            {1, 2},
            {2, 3},
            {3, 4},
            {1, 3}  // redundant edge
        };
        int m4 = (int)(sizeof(edges4) / sizeof(edges4[0]));
        run_test("Test 4: Fully connected with extra edges", n, edges4, m4, 1);
    }

    return 0;
}
