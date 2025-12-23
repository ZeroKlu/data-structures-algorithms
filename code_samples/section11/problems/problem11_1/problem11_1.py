"""Problem 11.1: Breadth-first search for shortest path."""

# This module implements Breadth-First Search (BFS) to compute shortest-path
# distances in an UNWEIGHTED graph, along with a small test harness.
#
# Key assumptions:
#   - Nodes are labeled 0..n-1.
#   - The graph is represented as an adjacency list.
#   - All edges are unweighted (each edge has equal cost = 1).
#
# For unweighted graphs, BFS guarantees shortest paths because it explores
# vertices in increasing order of distance from the source.

from collections import deque  # deque provides O(1) pops from the left for BFS queues


def bfs_shortest_paths(n: int, adj: list[list[int]], s: int) -> list[int]:
    """Breadth-first search for shortest path.

    Computes shortest-path distances from a source node `s` in an unweighted graph.

    Parameters:
        n   : number of nodes in the graph (assumed labeled 0..n-1)
        adj : adjacency list where adj[u] contains neighbors of node u
        s   : source node index

    Returns:
        dist : list of length n where
               - dist[v] is the minimum number of edges from s to v
               - dist[v] == -1 if v is unreachable from s

    Why BFS works:
        - In an unweighted graph, BFS explores nodes level by level.
        - The first time a node is discovered, the path used is guaranteed
          to be the shortest possible path (in terms of number of edges).
    """
    # Initialize all distances to -1, which serves as:
    #   - an "unvisited" marker
    #   - a signal that the node is unreachable from the source
    dist = [-1] * n

    # Distance from the source to itself is always 0
    dist[s] = 0

    # Queue for BFS frontier; start with the source node
    q: deque[int] = deque([s])

    # Standard BFS loop
    while q:
        # Pop the next node to expand
        u = q.popleft()

        # Explore all neighbors of u
        for v in adj[u]:
            # If v has not been visited yet, record its distance
            # and enqueue it for future expansion
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                q.append(v)

    return dist


# ===========================
# Test Harness for bfs_shortest_paths
# ===========================

def make_graph(n: int) -> list[list[int]]:
    """Make an empty graph with n nodes.

    Parameters:
        n : number of nodes

    Returns:
        adj : adjacency list with n empty neighbor lists
    """
    return [[] for _ in range(n)]


def add_edge(adj: list[list[int]], u: int, v: int) -> None:
    """Add an edge from u to v.

    Parameters:
        adj : adjacency list
        u,v : endpoints of the edge

    Behavior:
        - Adds an UNDIRECTED edge by inserting:
            v into adj[u]
            u into adj[v]
        - No bounds checking is performed.
    """
    adj[u].append(v)
    adj[v].append(u)


def run_test(
    name: str,
    n: int,
    adj: list[list[int]],
    src: int,
    expected: list[int]
) -> None:
    """Run a test.

    Parameters:
        name     : descriptive test name
        n        : number of nodes
        adj      : adjacency list
        src      : source node for BFS
        expected : expected distance array

    Behavior:
        - Runs bfs_shortest_paths
        - Prints computed distances and expected result
        - Intended for visual verification (no assertions)
    """
    print(name)
    print(f"Source = {src}")

    # Run BFS shortest paths
    dist = bfs_shortest_paths(n, adj, src)

    # Print results for comparison
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
#
# Expected distances from 0: [0, 1, 2, 2, 3]

num = 5
adjac = make_graph(num)

# Build the undirected graph
add_edge(adjac, 0, 1)
add_edge(adjac, 1, 2)
add_edge(adjac, 1, 3)
add_edge(adjac, 2, 4)

# Run and print test
run_test(
    "=== Test 1: Connected graph ===",
    num,
    adjac,
    0,
    [0, 1, 2, 2, 3]
)

# ===========================
# Test 2: Disconnected graph
# ===========================

# Graph:
# 0 - 1    2    3 - 4
#
# Expected distances from 0: [0, 1, -1, -1, -1]
# Nodes 2, 3, and 4 are unreachable from source 0.

num = 5
adjac = make_graph(num)

# Build two separate components
add_edge(adjac, 0, 1)
add_edge(adjac, 3, 4)

# Run and print test
run_test(
    "=== Test 2: Disconnected graph ===",
    num,
    adjac,
    0,
    [0, 1, -1, -1, -1]
)

# ===========================
# Test 3: Single node
# ===========================

# Graph:
# n = 1, no edges
#
# Expected distances from 0: [0]

num = 1
adjac = make_graph(num)  # single node with no edges

# Run and print test
run_test(
    "=== Test 3: Single node graph ===",
    num,
    adjac,
    0,
    [0]
)
