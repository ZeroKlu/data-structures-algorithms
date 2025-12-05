"""Problem 11.3. Topological Sort of a DAG (Kahn's Algorithm)"""

from collections import deque

def topo_sort_kahn(n: int, adj: list[list[int]]) -> list[int]:
    """Topological sort. Kahn's algorithm."""
    indeg = [0] * n
    for u in range(n):
        for v in adj[u]:
            indeg[v] += 1
    q: deque[int] = deque(u for u in range(n) if indeg[u] == 0)
    order: list[int] = []
    while q:
        u = q.popleft()
        order.append(u)
        for v in adj[u]:
            indeg[v] -= 1
            if indeg[v] == 0:
                q.append(v)
    if len(order) != n:
        return []  # cycle
    return order

# ===========================
# Test Harness for topo_sort_kahn
# ===========================

def make_graph(n: int) -> list[list[int]]:
    """Make an empty graph with n nodes."""
    return [[] for _ in range(n)]

def add_edge(adj: list[list[int]], u: int, v: int) -> None:
    """Add an edge from u to v."""
    adj[u].append(v)   # directed edge u -> v

def run_test(name: str, n: int, adj: list[list[int]]):
    """Run a test."""
    print(name)
    order = topo_sort_kahn(n, adj)
    if not order:
        print("Topological order: [] (cycle detected)\n")
    else:
        print("Topological order:", order, "\n")


# ===========================
# Test 1: Simple DAG
# ===========================
print("=== Test 1: Simple DAG ===")
num = 6
adjac = make_graph(num)

# DAG:
# 5 → 2, 5 → 0
# 4 → 0, 4 → 1
# 2 → 3
# 3 → 1
add_edge(adjac, 5, 2)
add_edge(adjac, 5, 0)
add_edge(adjac, 4, 0)
add_edge(adjac, 4, 1)
add_edge(adjac, 2, 3)
add_edge(adjac, 3, 1)

run_test("Simple DAG", num, adjac)

# ===========================
# Test 2: Another DAG
# ===========================
print("=== Test 2: Another DAG ===")
num = 4
adjac = make_graph(num)

# 0 → 1, 0 → 2
# 1 → 3
# 2 → 3
add_edge(adjac, 0, 1)
add_edge(adjac, 0, 2)
add_edge(adjac, 1, 3)
add_edge(adjac, 2, 3)

run_test("Another DAG", num, adjac)

# ===========================
# Test 3: Graph with a cycle
# ===========================
print("=== Test 3: Graph with cycle ===")
num = 3
adjac = make_graph(num)

# 0 → 1 → 2 → 0
add_edge(adjac, 0, 1)
add_edge(adjac, 1, 2)
add_edge(adjac, 2, 0)

run_test("Graph with cycle", num, adjac)
