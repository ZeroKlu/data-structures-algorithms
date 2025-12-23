"""Problem 11.3. Topological Sort of a DAG (Kahn's Algorithm)"""

# This module demonstrates topological sorting of a DIRECTED graph using
# Kahn's algorithm.
#
# Key concepts:
#   - A topological ordering is an ordering of nodes such that for every
#     directed edge u -> v, node u appears before node v.
#   - A topological ordering exists if and only if the graph is acyclic
#     (i.e., the graph is a DAG: Directed Acyclic Graph).
#   - If the graph contains a cycle, no topological ordering exists.
#
# Graph representation:
#   - `adj` is an adjacency list where:
#       adj[u] = list of all nodes v such that there is an edge u -> v
#   - Nodes are assumed to be labeled 0..n-1.

from collections import deque


def topo_sort_kahn(n: int, adj: list[list[int]]) -> list[int]:
    """
    Topological sort using Kahn's algorithm.

    Parameters:
        n   : number of nodes in the graph (nodes are 0..n-1)
        adj : adjacency list for a directed graph (u -> v stored in adj[u])

    Returns:
        order : list of nodes in topological order if the graph is a DAG
        []    : empty list if a cycle is detected (topo ordering impossible)

    Kahn's algorithm (high-level):
        1) Compute indegree[v] = number of incoming edges to v for each node v.
        2) Initialize a queue with all nodes that have indegree 0.
        3) While the queue is not empty:
             - pop a node u
             - append u to output order
             - for each edge u -> v:
                 - decrement indegree[v]
                 - if indegree[v] becomes 0, push v into the queue
        4) If output contains all nodes, success.
           Otherwise, nodes remaining with indegree > 0 are part of a cycle.
    """
    # indeg[v] counts how many prerequisites (incoming edges) node v has.
    indeg = [0] * n

    # Step 1: Compute indegrees by scanning all adjacency lists.
    for u in range(n):
        for v in adj[u]:
            indeg[v] += 1

    # Step 2: Start with all nodes that have indegree 0 (no incoming edges).
    # Using deque for efficient pops from the left.
    q: deque[int] = deque(u for u in range(n) if indeg[u] == 0)

    # This will store the resulting topological ordering.
    order: list[int] = []

    # Step 3: Process nodes in BFS-like order.
    while q:
        u = q.popleft()

        # u has no remaining incoming edges, so it can appear next in the topo order.
        order.append(u)

        # "Remove" each outgoing edge u -> v by decrementing indegree[v].
        for v in adj[u]:
            indeg[v] -= 1
            # When v has no more incoming edges, it becomes ready to process.
            if indeg[v] == 0:
                q.append(v)

    # Step 4: If we didn't output all nodes, a cycle exists.
    # Nodes that are part of (or depend on) cycles never reach indegree 0.
    if len(order) != n:
        return []  # cycle detected

    return order


# ===========================
# Test Harness for topo_sort_kahn
# ===========================

def make_graph(n: int) -> list[list[int]]:
    """
    Create an empty adjacency list for a directed graph with n nodes.

    Parameters:
        n : number of nodes

    Returns:
        adj : list of n empty lists; adj[u] will contain outgoing neighbors of u
    """
    return [[] for _ in range(n)]


def add_edge(adj: list[list[int]], u: int, v: int) -> None:
    """
    Add a DIRECTED edge u -> v to the adjacency list.

    Parameters:
        adj : adjacency list
        u   : source node
        v   : destination node

    Note:
        - This is directed (unlike undirected component problems).
        - No bounds checking is performed.
    """
    adj[u].append(v)   # directed edge u -> v


def run_test(name: str, n: int, adj: list[list[int]]):
    """
    Run a test case and print the topological order result.

    Parameters:
        name : label for the test
        n    : number of nodes
        adj  : adjacency list

    Output convention:
        - Prints "[] (cycle detected)" if the returned list is empty.
          (In these tests n > 0, so empty indicates a cycle.)
    """
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

# Classic DAG example:
# 5 → 2, 5 → 0
# 4 → 0, 4 → 1
# 2 → 3
# 3 → 1
#
# Many valid topological orders exist; output may vary but must respect edges.
num = 6
adjac = make_graph(num)

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

# DAG:
# 0 → 1, 0 → 2
# 1 → 3
# 2 → 3
#
# Valid topological orders include [0,1,2,3] or [0,2,1,3].
num = 4
adjac = make_graph(num)

add_edge(adjac, 0, 1)
add_edge(adjac, 0, 2)
add_edge(adjac, 1, 3)
add_edge(adjac, 2, 3)

run_test("Another DAG", num, adjac)


# ===========================
# Test 3: Graph with a cycle
# ===========================
print("=== Test 3: Graph with cycle ===")

# Cycle:
# 0 → 1 → 2 → 0
#
# Cyclic graphs do not have a topological ordering; algorithm should return [].
num = 3
adjac = make_graph(num)

add_edge(adjac, 0, 1)
add_edge(adjac, 1, 2)
add_edge(adjac, 2, 0)

run_test("Graph with cycle", num, adjac)
