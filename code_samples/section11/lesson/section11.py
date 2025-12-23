"""Section 11: Graph Algorithms"""

# This module demonstrates a small toolkit of graph algorithms and a DSU (union-find),
# along with a console-based test harness.
#
# Included algorithms / helpers:
#   - build_graph: build an undirected adjacency list from an edge list
#   - bfs_shortest: BFS shortest path distances for unweighted graphs
#   - _dfs_visit + count_components: DFS traversal and connected component counting
#   - topo_sort: Kahn's algorithm for topological sorting (directed graphs)
#   - dijkstra: Dijkstra's algorithm for weighted graphs with non-negative weights
#   - DSU: Disjoint Set Union (Union-Find) with path compression + union by rank
#
# Notes:
#   - Many functions assume nodes are labeled 0..n-1 and indices are valid.
#   - topo_sort expects a directed adjacency list (u -> v). build_graph creates undirected graphs.
#   - Dijkstra assumes all edge weights are non-negative.

from collections import deque  # deque provides O(1) pops from the left; ideal for BFS queues
import heapq                  # heapq implements a binary min-heap used by Dijkstra


def build_graph(n: int, edges: list[tuple[int, int]]) -> list[list[int]]:
    """Build an adjacency list for an undirected graph.

    Parameters:
        n: number of nodes (assumed labeled 0..n-1)
        edges: list of (u, v) edges (0-based indices)

    Returns:
        graph: adjacency list representation where graph[u] is a list of neighbors of u

    Behavior:
        - Because the graph is undirected, each edge is stored in both directions:
            u -> v and v -> u
        - No validation is performed (e.g., bounds checks or edge shape checks).
        - Parallel edges/self-loops are stored as provided.
    """
    # Create n empty neighbor lists.
    graph = [[] for _ in range(n)]

    # Add each undirected edge to both endpoints.
    for u, v in edges:
        graph[u].append(v)
        graph[v].append(u)

    return graph


def bfs_shortest(graph: list[list[int]], src: int) -> list[int]:
    """Breadth-first search for shortest path.

    Computes shortest-path distances from `src` in an UNWEIGHTED graph.

    Parameters:
        graph: adjacency list (graph[u] lists neighbors of u)
        src: source node index

    Returns:
        dist: list where dist[v] is the minimum number of edges from src to v,
              or INF if v is unreachable.

    Notes:
        - BFS is correct for unweighted graphs because it explores nodes in increasing
          distance layers.
        - INF is set to a large value; code uses equality check (dist[v] == INF)
          as the "unvisited" condition.
    """
    n = len(graph)

    # Large sentinel used to represent unreachable/unvisited nodes.
    # (Here it's far bigger than any possible shortest-path length in these tests.)
    INF = 10**15

    # Initialize all distances to INF.
    dist = [INF] * n

    # Queue for BFS frontier.
    q: deque[int] = deque()

    # Seed BFS from the source.
    dist[src] = 0
    q.append(src)

    while q:
        # Pop from the left to process nodes in FIFO order.
        u = q.popleft()

        # Expand neighbors.
        for v in graph[u]:
            # If v is unvisited, its first discovery is via the shortest path.
            if dist[v] == INF:
                dist[v] = dist[u] + 1
                q.append(v)

    return dist


def _dfs_visit(u: int, graph: list[list[int]], visited: list[bool]) -> None:
    # Internal DFS helper used by count_components().
    #
    # Parameters:
    #   u       : current node to visit
    #   graph   : adjacency list
    #   visited : boolean visitation markers, mutated in place
    #
    # Behavior:
    #   - Marks u visited, then recursively visits all unvisited neighbors.
    #
    # Caution:
    #   - Recursive DFS can exceed Python's recursion limit on large/deep graphs.
    visited[u] = True
    for v in graph[u]:
        if not visited[v]:
            _dfs_visit(v, graph, visited)


def count_components(graph: list[list[int]]) -> int:
    """Count connected components.

    Counts connected components in an UNDIRECTED graph.

    Parameters:
        graph: adjacency list for an undirected graph

    Returns:
        components: number of connected components

    Approach:
        - Iterate over all nodes; when an unvisited node is found, start a DFS.
        - DFS marks the entire connected component.
        - Each DFS start increments the component count.
    """
    n = len(graph)
    visited = [False] * n
    components = 0

    for u in range(n):
        if not visited[u]:
            components += 1
            _dfs_visit(u, graph, visited)

    return components


def topo_sort(graph: list[list[int]]) -> list[int]:
    """Topological sort.

    Performs topological sorting on a DIRECTED graph using Kahn's algorithm.

    Parameters:
        graph: directed adjacency list where edges are u -> v

    Returns:
        order: a topological ordering if the graph is a DAG
               If len(order) < n, the graph contains a cycle and the result is incomplete.

    Notes:
        - Topological sort is only defined for DAGs.
        - Kahn's algorithm repeatedly removes nodes with indegree 0.
    """
    n = len(graph)

    # indeg[v] = number of incoming edges to v.
    indeg = [0] * n
    for u in range(n):
        for v in graph[u]:
            indeg[v] += 1

    # Initialize queue with all nodes that have indegree 0.
    q: deque[int] = deque(i for i in range(n) if indeg[i] == 0)

    order: list[int] = []

    while q:
        u = q.popleft()
        order.append(u)

        # "Remove" u by decrementing indegree of its outgoing neighbors.
        for v in graph[u]:
            indeg[v] -= 1
            if indeg[v] == 0:
                q.append(v)

    # If len(order) < n, at least one cycle exists.
    return order


def dijkstra(graph: list[list[tuple[int, int]]], src: int) -> list[int]:
    """graph[u] = list of (v, w)

    Computes single-source shortest paths using Dijkstra's algorithm.

    Parameters:
        graph: adjacency list where graph[u] is a list of (v, w) edges for u -> v
        src: source node index

    Returns:
        dist: list where dist[v] is shortest path cost from src to v,
              or INF if unreachable.

    Correctness requirement:
        - All weights w must be >= 0 for Dijkstra to be valid.

    Implementation details:
        - Uses a min-heap of (distance, node).
        - Uses "lazy deletion": if the heap pops a pair (d,u) where d != dist[u],
          the entry is stale and skipped.
    """
    n = len(graph)
    INF = 10**18

    # Initialize distances to INF and set source distance to 0.
    dist = [INF] * n
    dist[src] = 0

    # Min-heap of (distance, node).
    heap: list[tuple[int, int]] = [(0, src)]

    while heap:
        d, u = heapq.heappop(heap)

        # Skip stale entries (not matching current best distance).
        if d != dist[u]:
            continue

        # Relax all outgoing edges u -> v.
        for v, w in graph[u]:
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(heap, (nd, v))

    return dist


class DSU:
    """Disjoint set union.

    Maintains a partition of elements into disjoint sets.

    Supports:
        - find(x): return representative (root) of x's set (with path compression)
        - union(a,b): merge sets containing a and b (with union by rank)

    Complexity:
        - Near-constant amortized time per operation in practice.
    """

    def __init__(self, n: int) -> None:
        # parent[i] = i indicates i is initially its own set representative.
        self.parent = list(range(n))

        # rank is a heuristic for tree height; used to keep unions balanced.
        self.rank = [0] * n

    def find(self, x: int) -> int:
        """Find the parent of x.

        Returns the representative/root of the set containing x.

        Uses path compression:
            - After find(x), parent[x] is updated to point directly to the root,
              flattening the structure for faster future queries.
        """
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def union(self, a: int, b: int) -> None:
        """Union a and b.

        Merges the sets containing a and b using union-by-rank:
            - Attach the smaller-rank root under the larger-rank root.
            - If ranks tie, pick one root and increment its rank.

        If a and b are already in the same set, does nothing.
        """
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


# ===========================
# Test Harness
# ===========================

def test_dsu():
    """Test DSU.

    Creates a DSU over 5 elements and unions:
        (0,1), (1,2), (3,4)
    Expected sets:
        {0,1,2} and {3,4}  => 2 sets total
    """
    print("=== Test 1: DSU ===")
    d = DSU(5)

    d.union(0, 1)
    d.union(1, 2)
    d.union(3, 4)

    # Collect representatives for each element.
    # Exact representative IDs may vary; grouping is what matters.
    reps = [d.find(i) for i in range(5)]
    print("Representatives:", reps)

    distinct = len(set(reps))
    print(f"Number of sets = {distinct} (expected 2)\n")


def test_bfs_and_components():
    """Test BFS shortest and components.

    Builds a connected undirected graph and verifies:
        - BFS distances from 0
        - connected component count (should be 1)
    """
    print("=== Test 2: BFS shortest + components ===")

    # Graph:
    # 0 - 1 - 2 - 4
    #     |
    #     3
    n = 5
    edges = [(0, 1), (1, 2), (1, 3), (2, 4)]
    g = build_graph(n, edges)

    dist = bfs_shortest(g, 0)
    print("Distances from 0:", dist)
    print("Expected: [0, 1, 2, 2, 3]")

    comps = count_components(g)
    print(f"Connected components = {comps} (expected 1)\n")


def test_disconnected_components():
    """Test disconnected components.

    Builds a graph with three components:
        0-1-2   3-4   5
    Expected component count: 3
    """
    print("=== Test 3: Disconnected components ===")

    # 0-1-2   3-4   5
    n = 6
    edges = [(0, 1), (1, 2), (3, 4)]
    g = build_graph(n, edges)

    comps = count_components(g)
    print(f"Components = {comps} (expected 3)\n")


def test_topo_sort():
    """Test topological sort.

    Constructs a directed acyclic graph (DAG) and prints one possible topo order.
    If the printed order has length < n, it indicates a cycle.
    """
    print("=== Test 4: Topological sort (DAG) ===")

    # DAG:
    # 5 -> 2, 5 -> 0
    # 4 -> 0, 4 -> 1
    # 2 -> 3
    # 3 -> 1
    n = 6
    dag: list[list[int]] = [[] for _ in range(n)]

    def add(u: int, v: int) -> None:
        # Helper to add directed edge u -> v.
        dag[u].append(v)

    add(5, 2)
    add(5, 0)
    add(4, 0)
    add(4, 1)
    add(2, 3)
    add(3, 1)

    order = topo_sort(dag)
    print("Topological order:", order)
    print("(If len(order) < 6, graph has a cycle)\n")


def test_dijkstra():
    """Dijkstra's algorithm.

    Builds a small weighted directed graph and prints shortest distances from 0.
    """
    print("=== Test 5: Dijkstra ===")

    # Weighted directed graph:
    # 0→1(10), 0→3(5)
    # 1→2(1), 1→3(2)
    # 2→4(4)
    # 3→1(3), 3→2(9), 3→4(2)
    # 4→0(7), 4→2(6)
    n = 5
    g: list[list[tuple[int, int]]] = [[] for _ in range(n)]

    def add(u: int, v: int, w: int) -> None:
        # Add directed weighted edge u -> v (weight w).
        g[u].append((v, w))

    add(0, 1, 10)
    add(0, 3, 5)

    add(1, 2, 1)
    add(1, 3, 2)

    add(2, 4, 4)

    add(3, 1, 3)
    add(3, 2, 9)
    add(3, 4, 2)

    add(4, 0, 7)
    add(4, 2, 6)

    dist = dijkstra(g, 0)
    print("Distances from 0:", dist)
    print("Expected: [0, 8, 9, 5, 7]\n")


if __name__ == "__main__":
    # Run all tests when executed as a script.
    test_dsu()
    test_bfs_and_components()
    test_disconnected_components()
    test_topo_sort()
    test_dijkstra()
