"""Problem 4 - Dijkstra's Algorithm (Non-Negative Weights)"""

import heapq

def dijkstra(n: int, adj: list[list[tuple[int, int]]], s: int) -> list[int]:
    """Dijkstra's algorithm."""
    INF = 10**15
    dist = [INF] * n
    dist[s] = 0
    pq: list[tuple[int, int]] = [(0, s)]
    while pq:
        d, u = heapq.heappop(pq)
        if d > dist[u]:
            continue
        for v, w in adj[u]:
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(pq, (nd, v))
    return dist

# ===========================
# Test Harness for Dijkstra
# ===========================

def make_graph(n: int) -> list[list[tuple[int, int]]]:
    """Create an empty graph with n vertices."""
    return [[] for _ in range(n)]

def add_edge(adj: list[list[tuple[int, int]]], u: int, v: int, w: int) -> None:
    """Add an edge from u to v with weight w."""
    adj[u].append((v, w))  # directed edge u → v

def print_dist(dist: list[int]) -> None:
    """Print a list of distances."""
    out = ["INF" if d >= 10**14 else str(d) for d in dist]
    print("[" + ", ".join(out) + "]")

def run_test(name: str, n: int, adj: list[list[tuple[int, int]]], src: int):
    """Run a test."""
    print(name)
    print("Source =", src)
    dist = dijkstra(n, adj, src)
    print("Distances: ", end="")
    print_dist(dist)
    print()

# ===========================
# Test 1: Simple Weighted Graph
# ===========================
print("=== Test 1: Simple Weighted Graph ===")

num = 5
adjac = make_graph(num)

# Graph:
# 0→1 (2), 0→2 (5)
# 1→2 (1)
# 1→3 (3)
# 2→3 (1)
# 3→4 (2)

add_edge(adjac, 0, 1, 2)
add_edge(adjac, 0, 2, 5)
add_edge(adjac, 1, 2, 1)
add_edge(adjac, 1, 3, 3)
add_edge(adjac, 2, 3, 1)
add_edge(adjac, 3, 4, 2)

run_test("Simple weighted graph", num, adjac, 0)


# ===========================
# Test 2: Multiple Shortest Paths
# ===========================
print("=== Test 2: Multiple Shortest Paths ===")

num = 6
adjac = make_graph(num)

# 0→1 (1), 0→2 (1)
# 1→3 (2)
# 2→3 (2)
# 3→4 (1)
# 4→5 (5)

add_edge(adjac, 0, 1, 1)
add_edge(adjac, 0, 2, 1)
add_edge(adjac, 1, 3, 2)
add_edge(adjac, 2, 3, 2)
add_edge(adjac, 3, 4, 1)
add_edge(adjac, 4, 5, 5)

run_test("Branches with equal-cost paths", num, adjac, 0)


# ===========================
# Test 3: Disconnected Graph
# ===========================
print("=== Test 3: Disconnected Graph ===")

num = 5
adjac = make_graph(num)

# Component reachable from 0:
# 0→1 (4), 1→2 (6)
add_edge(adjac, 0, 1, 4)
add_edge(adjac, 1, 2, 6)

# nodes 3 and 4 unreachable

run_test("Disconnected graph", num, adjac, 0)
