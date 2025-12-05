"""Problem 11.2 Connected Components in an Undirected Graph (DFS)"""

def _dfs_comp(u: int, cid: int, adj: list[list[int]], comp: list[int]) -> None:
    comp[u] = cid
    for v in adj[u]:
        if comp[v] == -1:
            _dfs_comp(v, cid, adj, comp)

def connected_components(n: int, adj: list[list[int]]) -> tuple[int, list[int]]:
    """Identify connected components in an undirected graph (DFS)"""
    comp = [-1] * n
    cid = 0
    for u in range(n):
        if comp[u] == -1:
            _dfs_comp(u, cid, adj, comp)
            cid += 1
    return cid, comp

# ===========================
# Test Harness
# ===========================

def make_graph(n: int) -> list[list[int]]:
    """Make an empty graph with n nodes."""
    return [[] for _ in range(n)]

def add_edge(adj: list[list[int]], u: int, v: int) -> None:
    """Add an edge from u to v."""
    adj[u].append(v)
    adj[v].append(u)

def run_test(name: str, n: int, adj: list[list[int]]):
    """Run a test."""
    print(name)
    count, comp = connected_components(n, adj)
    print("Component count =", count)
    print("Component IDs   =", comp)
    print()

# ===========================
# Test 1: Fully connected graph
# ===========================
print("=== Test 1: Fully connected graph ===")
num = 5
adjac = make_graph(num)
for u_val in range(num):
    for v_val in range(u_val + 1, num):
        add_edge(adjac, u_val, v_val)

run_test("Complete graph", num, adjac)

# ===========================
# Test 2: Two components + isolated
# ===========================
print("=== Test 2: Two components + isolated ===")
num = 6
adjac = make_graph(num)
add_edge(adjac, 0, 1)
add_edge(adjac, 1, 2)
add_edge(adjac, 3, 4)
# node 5 isolated

run_test("Two components plus isolated", num, adjac)

# ===========================
# Test 3: All nodes isolated
# ===========================
print("=== Test 3: All nodes isolated ===")
num = 4
adjac = make_graph(num)  # no edges

run_test("Four isolated nodes", num, adjac)

# ===========================
# Test 4: Mixed graph
# ===========================
print("=== Test 4: Mixed graph ===")
num = 7
adjac = make_graph(num)
add_edge(adjac, 0, 1)
add_edge(adjac, 1, 2)
add_edge(adjac, 3, 4)
add_edge(adjac, 4, 5)
# node 6 isolated

run_test("Mixed graph", num, adjac)
