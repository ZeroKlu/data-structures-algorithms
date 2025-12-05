"""Problem 5 - Number of Connected Components via Union-Find"""

class DSU:
    """Disjoint Set Union."""
    def __init__(self, n: int) -> None:
        self.parent = list(range(n))
        self.rank = [0] * n
        self.count = n

    def find(self, x: int) -> int:
        """Find the parent of x."""
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def unite(self, a: int, b: int) -> None:
        """Union a and b."""
        ra = self.find(a)
        rb = self.find(b)
        if ra == rb:
            return
        if self.rank[ra] < self.rank[rb]:
            self.parent[ra] = rb
        elif self.rank[ra] > self.rank[rb]:
            self.parent[rb] = ra
        else:
            self.parent[rb] = ra
            self.rank[ra] += 1
        self.count -= 1

def count_components(n: int, edges: list[list[int]]) -> int:
    """Count the number of connected components."""
    d = DSU(n)
    for u, v in edges:
        d.unite(u, v)
    return d.count

# ===========================
# Test Harness for count_components
# ===========================

def run_test(name: str, n: int, edges: list[list[int]], expected: int) -> None:
    """Run a test."""
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
run_test("Single chain 0–1–2–3", num, edge_list, expected=1)


# ===========================
# Test 2: Two components + isolated node
# ===========================
print("=== Test 2: Two components plus isolated node ===")
num = 6
edge_list = [
    [0, 1],
    [1, 2],   # component A
    [3, 4]    # component B
    # node 5 is isolated
]
run_test("Two components and one isolated", num, edge_list, expected=3)


# ===========================
# Test 3: No edges (all isolated)
# ===========================
print("=== Test 3: No edges ===")
num = 5
edge_list = []
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
    [1, 3]   # redundant
]
run_test("Fully connected graph", num, edge_list, expected=1)
