"""Section 5 - Trees and Graphs"""

from collections import deque

#pylint: disable=too-few-public-methods

class TreeNode:
    """Tree node."""
    def __init__(self, value: int, left: "TreeNode | None" = None,
                 right: "TreeNode | None" = None) -> None:
        self.value = value
        self.left = left
        self.right = right

def preorder(root: "TreeNode | None") -> None:
    """Preorder traversal."""
    if root is None:
        return
    print(root.value)
    preorder(root.left)
    preorder(root.right)

def level_order(root: "TreeNode | None") -> None:
    """Level order traversal."""
    if root is None:
        return
    queue: deque[TreeNode] = deque([root])
    while queue:
        node = queue.popleft()
        print(node.value)
        if node.left is not None:
            queue.append(node.left)
        if node.right is not None:
            queue.append(node.right)

def dfs_util(node: int, graph: list[list[int]], visited: list[bool]) -> None:
    """Depth-first search utility function."""
    visited[node] = True
    print(node)
    for neighbor in graph[node]:
        if not visited[neighbor]:
            dfs_util(neighbor, graph, visited)

def dfs(start: int, graph: list[list[int]]) -> None:
    """Depth-first search."""
    n = len(graph)
    visited = [False] * n
    dfs_util(start, graph, visited)

def shortest_distances(start: int, graph: list[list[int]]) -> list[int]:
    """Shortest distances."""
    n = len(graph)
    dist = [-1] * n
    queue: deque[int] = deque()

    dist[start] = 0
    queue.append(start)

    while queue:
        node = queue.popleft()
        for neighbor in graph[node]:
            if dist[neighbor] == -1:
                dist[neighbor] = dist[node] + 1
                queue.append(neighbor)
    return dist

def main() -> None:
    """Main function."""
    print("==== TEST TREE (preorder + level_order) ====")

    # Build this tree:
    #
    #        1
    #       / \
    #      2   3
    #     / \   \
    #    4   5   6
    #
    node4 = TreeNode(4)
    node5 = TreeNode(5)
    node6 = TreeNode(6)
    node2 = TreeNode(2, left=node4, right=node5)
    node3 = TreeNode(3, right=node6)
    root = TreeNode(1, left=node2, right=node3)

    print("Preorder (expect 1 2 4 5 3 6):")
    preorder(root)

    print("\nLevel order (expect 1 2 3 4 5 6):")
    level_order(root)

    print("\n==== TEST DFS (graph) ====")

    # Graph (undirected):
    #
    # 0 -- 1
    # |  /
    # 2
    # |
    # 3
    #
    graph = [
        [1, 2],      # neighbors of 0
        [0, 2],      # neighbors of 1
        [0, 1, 3],   # neighbors of 2
        [2]          # neighbors of 3
    ]

    print("DFS starting at node 0 (expect 0 1 2 3):")
    dfs(0, graph)

    print("\n==== TEST shortest_distances (BFS) ====")
    dist = shortest_distances(0, graph)

    print("Shortest distances from node 0:")
    for i, d in enumerate(dist):
        print(f"dist to {i} = {d}")

    # Expected:
    # dist[0] = 0
    # dist[1] = 1
    # dist[2] = 1
    # dist[3] = 2

    print("\n==== ALL TESTS COMPLETE ====")

if __name__ == "__main__":
    main()
