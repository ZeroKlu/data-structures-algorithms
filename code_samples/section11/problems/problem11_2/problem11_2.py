"""Problem 11.2 Connected Components in an Undirected Graph (DFS)"""

# This module implements connected-component detection for an UNDIRECTED graph
# using Depth-First Search (DFS).
#
# Core idea:
#   - Maintain an array `comp` where comp[u] stores the component ID of node u.
#   - Initialize all entries of `comp` to -1 (meaning "unvisited").
#   - Iterate through all nodes; whenever an unvisited node is found,
#     start a DFS from it and label every reachable node with the same
#     component ID.
#
# Nodes are assumed to be labeled 0..n-1, and the graph is represented
# as an adjacency list.


def _dfs_comp(u: int, cid: int, adj: list[list[int]], comp: list[int]) -> None:
    """
    DFS helper that labels all nodes reachable from `u` with component id `cid`.

    Parameters:
        u    : current node being visited
        cid  : component id assigned to this connected component
        adj  : adjacency list of an UNDIRECTED graph
        comp : list where comp[x] == -1 means "unvisited",
               otherwise comp[x] stores the component id of node x

    Behavior:
        - Marks node `u` as belonging to component `cid`
        - Recursively explores all neighbors that are still unvisited

    Note:
        - This implementation is recursive; for very deep graphs,
          recursion depth could become an issue.
    """
    # Assign the component id to the current node
    comp[u] = cid

    # Explore all neighbors of u
    for v in adj[u]:
        # If neighbor v has not yet been assigned a component,
        # it belongs to the same connected component
        if comp[v] == -1:
            _dfs_comp(v, cid, adj, comp)


def connected_components(n: int, adj: list[list[int]]) -> tuple[int, list[int]]:
    """
    Identify connected components in an undirected graph using DFS.

    Parameters:
        n   : number of nodes in the graph
        adj : adjacency list representation of the graph

    Returns:
        (count, comp) where:
            count : total number of connected components
            comp  : list of length n, where comp[u] is the component id of node u

    Approach:
        - Initialize all nodes as unvisited (comp[u] = -1).
        - Scan nodes from 0 to n-1:
            - If a node is unvisited, start a DFS from it.
            - Label all reachable nodes with the current component id.
            - Increment the component id counter.
    """
    # Initialize component labels: -1 means "unvisited"
    comp = [-1] * n

    # Component id counter
    cid = 0

    # Iterate over all nodes
    for u in range(n):
        # If node u has not been visited, it starts a new component
        if comp[u] == -1:
            _dfs_comp(u, cid, adj, comp)
            cid += 1

    # cid now equals the total number of components found
    return cid, comp


# ===========================
# Test Harness
# ===========================

def make_graph(n: int) -> list[list[int]]:
    """
    Create an empty adjacency list for a graph with n nodes.

    Parameters:
        n : number of nodes

    Returns:
        adj : list of n empty lists, one per node
    """
    return [[] for _ in range(n)]


def add_edge(adj: list[list[int]], u: int, v: int) -> None:
    """
    Add an UNDIRECTED edge between nodes u and v.

    Parameters:
        adj : adjacency list
        u,v : endpoints of the edge

    Behavior:
        - Adds v to u's neighbor list
        - Adds u to v's neighbor list
    """
    adj[u].append(v)
    adj[v].append(u)


def run_test(name: str, n: int, adj: list[list[int]]):
    """
    Run a connected-components test and print the results.

    Parameters:
        name : descriptive test name
        n    : number of nodes
        adj  : adjacency list
    """
    print(name)

    # Compute connected components
    count, comp = connected_components(n, adj)

    # Print results
    print("Component count =", count)
    print("Component IDs   =", comp)
    print()


# ===========================
# Test 1: Fully connected graph
# ===========================
print("=== Test 1: Fully connected graph ===")

# Build a complete graph (clique) on 5 nodes.
# Every node is connected to every other node,
# so there should be exactly 1 connected component.
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

# Graph structure:
#   - component 0: 0-1-2
#   - component 1: 3-4
#   - component 2: {5} isolated
num = 6
adjac = make_graph(num)

add_edge(adjac, 0, 1)
add_edge(adjac, 1, 2)
add_edge(adjac, 3, 4)
# node 5 remains isolated

run_test("Two components plus isolated", num, adjac)


# ===========================
# Test 3: All nodes isolated
# ===========================
print("=== Test 3: All nodes isolated ===")

# No edges at all: each node is its own component.
# With n = 4, expected component count is 4.
num = 4
adjac = make_graph(num)  # no edges

run_test("Four isolated nodes", num, adjac)


# ===========================
# Test 4: Mixed graph
# ===========================
print("=== Test 4: Mixed graph ===")

# Graph structure:
#   - component 0: 0-1-2
#   - component 1: 3-4-5
#   - component 2: {6} isolated
num = 7
adjac = make_graph(num)

add_edge(adjac, 0, 1)
add_edge(adjac, 1, 2)
add_edge(adjac, 3, 4)
add_edge(adjac, 4, 5)
# node 6 remains isolated

run_test("Mixed graph", num, adjac)
