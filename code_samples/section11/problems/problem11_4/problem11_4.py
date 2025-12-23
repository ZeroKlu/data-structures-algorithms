"""Problem 4 - Dijkstra's Algorithm (Non-Negative Weights)"""

import heapq  # heapq provides a binary min-heap implementation for priority queues

def dijkstra(n: int, adj: list[list[tuple[int, int]]], s: int) -> list[int]:
    """
    Dijkstra's algorithm (single-source shortest paths).

    Purpose:
        Compute the shortest distance from a source node `s` to every node in a
        directed, weighted graph with NON-NEGATIVE edge weights.

    Parameters:
        n   : Number of vertices (assumed labeled 0..n-1).
        adj : Adjacency list where adj[u] is a list of (v, w) edges representing
              a directed edge u -> v with weight w.
        s   : Source vertex.

    Returns:
        dist : List of length n where dist[v] is the shortest distance from s to v.
               If v is unreachable, dist[v] remains INF.

    Correctness requirement:
        Dijkstra's algorithm assumes all weights w >= 0. If negative edge weights
        exist, the algorithm may produce incorrect results.

    Implementation notes:
        - Uses a min-heap priority queue storing (distance, node).
        - Because we do not "decrease-key" in place, the heap may contain multiple
          entries for the same node. We skip outdated (stale) entries with:
              if d > dist[u]: continue
    """
    INF = 10**15               # "infinity" sentinel for unreachable nodes
    dist = [INF] * n           # initialize all distances to INF
    dist[s] = 0                # distance from source to itself is 0

    # Priority queue (min-heap) of (current_best_distance, node)
    pq: list[tuple[int, int]] = [(0, s)]

    # Main loop: repeatedly expand the node with smallest tentative distance
    while pq:
        d, u = heapq.heappop(pq)  # pop node u with current smallest candidate distance d

        # If this heap entry is stale (we already found a better distance), skip it.
        if d > dist[u]:
            continue

        # Relax all outgoing edges u -> v
        for v, w in adj[u]:
            nd = d + w  # candidate distance to v via u
            if nd < dist[v]:
                dist[v] = nd
                # Push the updated best-known distance for v onto the heap.
                # Any older heap entries for v will be discarded later.
                heapq.heappush(pq, (nd, v))

    return dist

# ===========================
# Test Harness for Dijkstra
# ===========================

def make_graph(n: int) -> list[list[tuple[int, int]]]:
    """
    Create an empty adjacency list for a directed weighted graph.

    Returns:
        A list of n lists, where adj[u] will hold (v, w) pairs for edges u -> v.
    """
    return [[] for _ in range(n)]

def add_edge(adj: list[list[tuple[int, int]]], u: int, v: int, w: int) -> None:
    """
    Add a directed edge u -> v with weight w to the adjacency list.

    Note:
        - This graph is directed. For an undirected graph, you would add both
          (u -> v) and (v -> u).
        - Dijkstra requires w >= 0 for correctness.
    """
    adj[u].append((v, w))  # directed edge u → v

def print_dist(dist: list[int]) -> None:
    """
    Print a list of distances in a compact format.

    Convention:
        Distances that are still very large are printed as "INF" to indicate
        unreachable nodes.
    """
    out = ["INF" if d >= 10**14 else str(d) for d in dist]
    print("[" + ", ".join(out) + "]")

def run_test(name: str, n: int, adj: list[list[tuple[int, int]]], src: int):
    """
    Run Dijkstra on the provided graph and print the results.

    Parameters:
        name : Label for the test output.
        n    : Number of nodes.
        adj  : Adjacency list.
        src  : Source vertex.
    """
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
#
# This test checks that multi-hop cheaper routes are discovered correctly
# (e.g., 0->1->2 has cost 3, which is cheaper than 0->2 with cost 5).
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

# Graph:
# 0→1 (1), 0→2 (1)
# 1→3 (2)
# 2→3 (2)
# 3→4 (1)
# 4→5 (5)
#
# Node 3 can be reached by two equal-cost paths:
#   0->1->3 and 0->2->3 both cost 3.
# This test ensures correctness even when multiple shortest paths exist.
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
#   0→1 (4), 1→2 (6)
#
# Nodes 3 and 4 are unreachable from 0, so they should print as "INF".
add_edge(adjac, 0, 1, 4)
add_edge(adjac, 1, 2, 6)

# nodes 3 and 4 unreachable

run_test("Disconnected graph", num, adjac, 0)
