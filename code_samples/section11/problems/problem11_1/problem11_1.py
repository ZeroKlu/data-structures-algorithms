"""Problem 11.1: Breadth-first search for shortest path."""

from collections import deque

def bfs_shortest_paths(n: int, adj: list[list[int]], s: int) -> list[int]:
    """Breadth-first search for shortest path."""
    dist = [-1] * n
    dist[s] = 0
    q: deque[int] = deque([s])
    while q:
        u = q.popleft()
        for v in adj[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                q.append(v)
    return dist

# ===========================
# Test Harness for bfs_shortest_paths
# ===========================

def make_graph(n: int) -> list[list[int]]:
    """Make an empty graph with n nodes."""
    return [[] for _ in range(n)]

def add_edge(adj: list[list[int]], u: int, v: int) -> None:
    """Add an edge from u to v."""
    adj[u].append(v)
    adj[v].append(u)

def run_test(name: str, n: int, adj: list[list[int]], src: int, expected: list[int]) -> None:
    """Run a test."""
    print(name)
    print(f"Source = {src}")
    dist = bfs_shortest_paths(n, adj, src)
    print("Distances:", dist)
    print("Expected :", expected)
    print()

# ===========================
# Test 1: Connected graph
# ===========================

# Graph:
# 0 - 1 - 2 - 4
#     |
#     3
# Expected distances from 0: [0, 1, 2, 2, 3]

num = 5
adjac = make_graph(num)
add_edge(adjac, 0, 1)
add_edge(adjac, 1, 2)
add_edge(adjac, 1, 3)
add_edge(adjac, 2, 4)

run_test("=== Test 1: Connected graph ===", num, adjac, 0, [0, 1, 2, 2, 3])

# ===========================
# Test 2: Disconnected graph
# ===========================

# Graph:
# 0 - 1    2    3 - 4
# Expected distances from 0: [0, 1, -1, -1, -1]

num = 5
adjac = make_graph(num)
add_edge(adjac, 0, 1)
add_edge(adjac, 3, 4)

run_test("=== Test 2: Disconnected graph ===", num, adjac, 0, [0, 1, -1, -1, -1])

# ===========================
# Test 3: Single node
# ===========================

# Expected: [0]

num = 1
adjac = make_graph(num)

run_test("=== Test 3: Single node graph ===", num, adjac, 0, [0])
