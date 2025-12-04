"""Section 11: Graph Algorithms"""

from collections import deque
import heapq

def build_graph(n: int, edges: list[tuple[int, int]]) -> list[list[int]]:
    """Build an adjacency list for an undirected graph."""
    graph = [[] for _ in range(n)]
    for u, v in edges:
        graph[u].append(v)
        graph[v].append(u)
    return graph

def bfs_shortest(graph: list[list[int]], src: int) -> list[int]:
    """Breadth-first search for shortest path."""
    n = len(graph)
    INF = 10**15
    dist = [INF] * n
    q: deque[int] = deque()

    dist[src] = 0
    q.append(src)

    while q:
        u = q.popleft()
        for v in graph[u]:
            if dist[v] == INF:
                dist[v] = dist[u] + 1
                q.append(v)
    return dist

def _dfs_visit(u: int, graph: list[list[int]], visited: list[bool]) -> None:
    visited[u] = True
    for v in graph[u]:
        if not visited[v]:
            _dfs_visit(v, graph, visited)

def count_components(graph: list[list[int]]) -> int:
    """Count connected components."""
    n = len(graph)
    visited = [False] * n
    components = 0
    for u in range(n):
        if not visited[u]:
            components += 1
            _dfs_visit(u, graph, visited)
    return components

def topo_sort(graph: list[list[int]]) -> list[int]:
    """Topological sort."""
    n = len(graph)
    indeg = [0] * n
    for u in range(n):
        for v in graph[u]:
            indeg[v] += 1

    q: deque[int] = deque(i for i in range(n) if indeg[i] == 0)
    order: list[int] = []

    while q:
        u = q.popleft()
        order.append(u)
        for v in graph[u]:
            indeg[v] -= 1
            if indeg[v] == 0:
                q.append(v)

    # if len(order) < n, there was a cycle
    return order

def dijkstra(graph: list[list[tuple[int, int]]], src: int) -> list[int]:
    """graph[u] = list of (v, w)"""
    n = len(graph)
    INF = 10**18
    dist = [INF] * n
    dist[src] = 0
    heap: list[tuple[int, int]] = [(0, src)]

    while heap:
        d, u = heapq.heappop(heap)
        if d != dist[u]:
            continue
        for v, w in graph[u]:
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(heap, (nd, v))
    return dist

class DSU:
    """Disjoint set union."""
    def __init__(self, n: int) -> None:
        self.parent = list(range(n))
        self.rank = [0] * n

    def find(self, x: int) -> int:
        """Find the parent of x."""
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def union(self, a: int, b: int) -> None:
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

# ===========================
# Test Harness
# ===========================

def test_dsu():
    """Test DSU."""
    print("=== Test 1: DSU ===")
    d = DSU(5)
    d.union(0, 1)
    d.union(1, 2)
    d.union(3, 4)

    reps = [d.find(i) for i in range(5)]
    print("Representatives:", reps)

    distinct = len(set(reps))
    print(f"Number of sets = {distinct} (expected 2)\n")


def test_bfs_and_components():
    """Test BFS shortest and components."""
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
    """Test disconnected components."""
    print("=== Test 3: Disconnected components ===")
    # 0-1-2   3-4   5
    n = 6
    edges = [(0, 1), (1, 2), (3, 4)]
    g = build_graph(n, edges)

    comps = count_components(g)
    print(f"Components = {comps} (expected 3)\n")


def test_topo_sort():
    """Test topological sort."""
    print("=== Test 4: Topological sort (DAG) ===")
    # DAG:
    # 5 -> 2, 5 -> 0
    # 4 -> 0, 4 -> 1
    # 2 -> 3
    # 3 -> 1

    n = 6
    dag: list[list[int]] = [[] for _ in range(n)]

    def add(u: int, v: int) -> None:
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
    """Dijkstra's algorithm."""
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
    test_dsu()
    test_bfs_and_components()
    test_disconnected_components()
    test_topo_sort()
    test_dijkstra()
