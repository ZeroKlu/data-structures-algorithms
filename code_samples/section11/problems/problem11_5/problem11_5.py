"""Problem 5 - Number of Connected Components via Union-Find"""

class DSU:
    """
    Disjoint Set Union (DSU) / Union-Find.

    Purpose:
        Maintain a partition of the elements {0..n-1} into disjoint sets,
        supporting efficient:
          - find(x):  get the representative (root) of x's set
          - unite(a, b): merge the sets containing a and b

    Why it works well for connected components:
        If you start with each node in its own set and union(u, v) for every
        undirected edge (u, v), then each resulting DSU set corresponds to
        one connected component.

    Optimizations used:
        - Path compression (in find): flattens trees for faster future queries
        - Union by rank (in unite): keeps trees shallow

    Extra field:
        - count: number of sets currently present (i.e., components remaining)
    """
    def __init__(self, n: int) -> None:
        """
        Initialize n singleton sets.

        Args:
            n: number of elements (0..n-1)

        State after initialization:
            parent[i] = i for all i (each node is its own root)
            rank[i]   = 0 for all i (tree "height" heuristic starts at 0)
            count     = n (n disjoint sets/components)
        """
        self.parent = list(range(n))
        self.rank = [0] * n
        self.count = n

    def find(self, x: int) -> int:
        """
        Find the representative (root) of the set containing x.

        Path compression:
            If x is not a root, recursively find the root and update parent[x]
            to point directly to the root. This makes subsequent finds faster.

        Returns:
            The root representative of x's set.
        """
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def unite(self, a: int, b: int) -> None:
        """
        Merge the sets containing a and b.

        Steps:
            1) Find roots ra and rb.
            2) If ra == rb, already connected -> nothing changes.
            3) Otherwise, attach smaller-rank tree under larger-rank tree.
            4) If ranks equal, pick one root and increment its rank.
            5) Decrement count exactly once for a successful merge (fewer sets).

        Args:
            a, b: elements to union
        """
        ra = self.find(a)
        rb = self.find(b)
        if ra == rb:
            return

        # Union by rank: attach shorter tree under taller tree
        if self.rank[ra] < self.rank[rb]:
            self.parent[ra] = rb
        elif self.rank[ra] > self.rank[rb]:
            self.parent[rb] = ra
        else:
            # Same rank: choose one as new root and bump its rank
            self.parent[rb] = ra
            self.rank[ra] += 1

        # Successful merge reduces the number of disjoint sets by 1
        self.count -= 1

def count_components(n: int, edges: list[list[int]]) -> int:
    """
    Count the number of connected components in an undirected graph using DSU.

    Args:
        n: number of nodes labeled 0..n-1
        edges: list of undirected edges, each edge as [u, v]

    Algorithm:
        - Start with n components (each node alone).
        - For every edge (u, v), union(u, v).
        - The DSU's `count` is the number of connected components remaining.

    Returns:
        Number of connected components.

    Complexity:
        Approximately O((n + m) * α(n)), where m = len(edges) and α is tiny.
    """
    d = DSU(n)
    for u, v in edges:
        d.unite(u, v)
    return d.count

# ===========================
# Test Harness for count_components
# ===========================

def run_test(name: str, n: int, edges: list[list[int]], expected: int) -> None:
    """
    Run a single test case and print input + output.

    Args:
        name: label to print for the test
        n: number of nodes
        edges: edge list for the test
        expected: expected number of connected components
    """
    print(name)
    print("n =", n, "edges =", edges)
    result = count_components(n, edges)
    print(f"Connected components = {result} (expected {expected})\n")


# ===========================
# Test 1: Single component chain
# ===========================
print("=== Test 1: Single component chain ===")
num = 4
edge_list = [
    [0, 1],
    [1, 2],
    [2, 3]
]
# Graph: 0-1-2-3 forms one connected component
run_test("Single chain 0-1-2-3", num, edge_list, expected=1)


# ===========================
# Test 2: Two components + isolated node
# ===========================
print("=== Test 2: Two components plus isolated node ===")
num = 6
edge_list = [
    [0, 1],
    [1, 2],   # component A: {0,1,2}
    [3, 4]    # component B: {3,4}
    # node 5 is isolated -> its own component
]
# Components: {0,1,2}, {3,4}, {5} => 3
run_test("Two components and one isolated", num, edge_list, expected=3)


# ===========================
# Test 3: No edges (all isolated)
# ===========================
print("=== Test 3: No edges ===")
num = 5
edge_list = []
# With no edges, every node is isolated => 5 components
run_test("All nodes isolated", num, edge_list, expected=5)


# ===========================
# Test 4: Fully connected with redundancy
# ===========================
print("=== Test 4: Fully connected with redundant edges ===")
num = 5
edge_list = [
    [0, 1],
    [1, 2],
    [2, 3],
    [3, 4],
    [1, 3]   # redundant edge: creates an alternate path/cycle but doesn't add components
]
# All nodes connected => 1 component
run_test("Fully connected graph", num, edge_list, expected=1)
