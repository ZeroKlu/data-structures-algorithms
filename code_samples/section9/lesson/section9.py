"""Lesson 9 - Advanced Dynamic Programming"""

from __future__ import annotations
from dataclasses import dataclass
from typing import Optional

@dataclass
class TreeNode:
    """Model for a binary tree node.

    This is a small, immutable-by-convention data container representing a node
    in a binary tree.

    Attributes:
        val: Integer value stored at this node.
        left: Reference to the left child node, or None if absent.
        right: Reference to the right child node, or None if absent.
    """
    val: int
    left: Optional["TreeNode"] = None
    right: Optional["TreeNode"] = None

def max_root_to_leaf(root: Optional[TreeNode]) -> int:
    """Return the maximum sum of a root-to-leaf path in a binary tree.

    A "root-to-leaf" path:
      - must start at the provided `root` argument
      - must end at some leaf node (a node with no children)

    This is *not* the "maximum path sum anywhere in the tree" problem; the path
    is constrained to begin at the root.

    Approach:
      - Recursively compute the best root-to-leaf path sum for left and right
        subtrees, then take the maximum and add the current node's value.

    Base cases:
      - If root is None: return -infinity to represent "no valid path".
        Using -infinity ensures missing children never win a max() comparison.
      - If root is a leaf: return root.val.

    Args:
        root: The root TreeNode of the (sub)tree, or None.

    Returns:
        The maximum sum along any root-to-leaf path.
        If root is None, returns float("-inf") as a sentinel for "no path".
    """
    # Empty subtree => no path exists. Use -inf so it won't be chosen by max().
    if root is None:
        return float("-inf")

    # Leaf node => the best path ending here is just this node's value.
    if root.left is None and root.right is None:
        return root.val

    # Recursively compute the best sums from each child down to some leaf.
    left_best = max_root_to_leaf(root.left)
    right_best = max_root_to_leaf(root.right)

    # Choose the better child path and include the current node's value.
    return root.val + max(left_best, right_best)

def tsp_bitmask(n: int, dist: list[list[int]]) -> int:
    """Return the minimum cost of a traveling salesman problem.

    This function solves a common "TSP-like" dynamic programming variant:
      - You start at node 0
      - You must visit every node exactly once
      - You may end at any node (this implementation does *not* add a cost to
        return back to node 0)

    DP definition:
        dp[mask][j] = minimum cost to start at 0, visit exactly the nodes whose
                      bits are set in `mask`, and end at node j.

    Initialization:
        dp[1<<0][0] = 0
        (Only node 0 visited, currently at 0, cost 0.)

    Transition:
        From state (mask, j), attempt to go to an unvisited node k:
            next_mask = mask | (1<<k)
            dp[next_mask][k] = min(dp[next_mask][k], dp[mask][j] + dist[j][k])

    Answer:
        min_j dp[full_mask][j], where full_mask has all n bits set.

    Complexity:
        Time:  O(2^n * n^2)
        Space: O(2^n * n)

    Notes:
        - INF is set to a very large value (10**15) to represent unreachable states.
        - The distances in `dist` are assumed to be non-negative and finite.

    Args:
        n: Number of nodes labeled 0..n-1.
        dist: Square matrix dist[i][j] giving travel cost from i to j.

    Returns:
        Minimum cost to start at 0, visit all nodes once, and end anywhere.
    """
    # Total number of subsets of nodes (bitmasks) is 2^n.
    max_mask = 1 << n

    # Sentinel for "unreachable" DP states. Must exceed any realistic tour cost.
    INF = 10**15

    # dp[mask][j] initialized to INF for all states.
    dp = [[INF] * n for _ in range(max_mask)]

    # Starting state: only node 0 visited, currently at node 0, cost 0.
    dp[1 << 0][0] = 0

    # Iterate over all subsets of visited nodes.
    for mask in range(max_mask):
        # Try each possible current endpoint j.
        for j in range(n):
            # If j is not included in mask, then "end at j" is not a valid state.
            if not mask & (1 << j):
                continue

            curr = dp[mask][j]
            # Skip unreachable states.
            if curr == INF:
                continue

            # Try to visit an unvisited node k next.
            for k in range(n):
                # If k is already visited, we cannot visit it again.
                if mask & (1 << k):
                    continue

                next_mask = mask | (1 << k)
                cand = curr + dist[j][k]

                # Relaxation: keep the minimal cost for reaching (next_mask, k).
                if cand < dp[next_mask][k]:
                    dp[next_mask][k] = cand

    # Full mask = all nodes visited.
    full_mask = max_mask - 1

    # End anywhere: take the minimum cost among all endpoints.
    return min(dp[full_mask])

def matrix_chain_order(p: list[int]) -> int:
    """Return the minimum cost of matrix multiplication.

    Matrix Chain Multiplication (MCM) problem:
        Given a chain of matrices A1..An, choose parenthesization to minimize
        scalar multiplications.

    The dimension array `p` encodes matrices:
        A_i has dimensions p[i-1] x p[i] for i = 1..n
    where n = len(p) - 1.

    DP definition (classic CLRS formulation using 1-based matrix indices):
        dp[i][j] = minimum scalar multiplication cost to compute A_i..A_j

    Base:
        dp[i][i] = 0 (a single matrix requires no multiplications)

    Transition:
        dp[i][j] = min over k in [i..j-1] of:
            dp[i][k] + dp[k+1][j] + p[i-1] * p[k] * p[j]

    Complexity:
        Time:  O(n^3)
        Space: O(n^2)

    Notes:
        - INF is a large sentinel to represent "not yet minimized" entries.
        - dp is allocated as (n+1)x(n+1) so indices 1..n are convenient.

    Args:
        p: List of dimensions of length n+1.

    Returns:
        Minimum scalar multiplication cost to compute the full product A1..An.
    """
    # Number of matrices in the chain.
    n = len(p) - 1

    # Sentinel infinity for minimization.
    INF = 10**15

    # dp table: dp[i][j] will be filled for 1 <= i <= j <= n.
    # Defaults to 0; we'll assign INF for i < j when computing.
    dp = [[0] * (n + 1) for _ in range(n + 1)]

    # length is the number of matrices in the subchain.
    # Start at 2 because length=1 subproblems (single matrix) have cost 0.
    for length in range(2, n + 1):
        # i is the starting matrix index for this subproblem.
        for i in range(1, n - length + 2):
            # j is the ending matrix index.
            j = i + length - 1

            # Initialize as INF before taking the min over split points.
            dp[i][j] = INF

            # Try splitting between k and k+1.
            for k in range(i, j):
                # Cost = cost(left subchain) + cost(right subchain) + cost to multiply results.
                # Dimensions:
                #   (A_i..A_k) is p[i-1] x p[k]
                #   (A_{k+1}..A_j) is p[k] x p[j]
                # Multiply cost: p[i-1] * p[k] * p[j]
                cost = dp[i][k] + dp[k + 1][j] + p[i - 1] * p[k] * p[j]

                # Keep the minimal cost among all possible split points.
                if cost < dp[i][j]:
                    dp[i][j] = cost

    # The full chain corresponds to dp[1][n].
    return dp[1][n]

# ===========================
# Tests for Lesson 9
# ===========================

if __name__ == "__main__":
    # Basic smoke tests demonstrating expected outputs for each function.
    print("=== Test 1: max_root_to_leaf ===\n")

    # Build tree:
    #       10
    #      /  \
    #     5    20
    #    / \     \
    #   3   7     25
    #
    # Best root-to-leaf sum = 10 + 20 + 25 = 55
    #
    # Note: Because this is root-to-leaf, paths must start at the root (10).
    root_node = TreeNode(
        10,
        left=TreeNode(
            5,
            left=TreeNode(3),
            right=TreeNode(7),
        ),
        right=TreeNode(
            20,
            right=TreeNode(25),
        ),
    )

    # Expect 55 from the best path 10 -> 20 -> 25.
    best_path = max_root_to_leaf(root_node)
    print(f"max_root_to_leaf(tree) = {best_path} (expected 55)")

    # Single-node tree: that node is also a leaf, so the best sum is its value.
    single = TreeNode(42)
    single_best = max_root_to_leaf(single)
    print(f"max_root_to_leaf(single=42) = {single_best} (expected 42)")

    # Null root: function returns -inf sentinel by design.
    null_best = max_root_to_leaf(None)
    print(f"max_root_to_leaf(None) = {null_best} (expected {float('-inf')})")

    print()
    print("=== Test 2: tsp_bitmask ===\n")

    # 4-node example distances.
    distances = [
        [0, 10, 15, 20],
        [10, 0, 35, 25],
        [15, 35, 0, 30],
        [20, 25, 30, 0],
    ]

    # Expected best cost is 65 for the "start at 0, visit all once, end anywhere" variant.
    tsp_cost = tsp_bitmask(4, distances)
    print(f"tsp_bitmask(4 nodes) = {tsp_cost} (expected 65)")

    print()
    print("=== Test 3: matrix_chain_order ===\n")

    # CLRS example:
    # p = [30,35,15,5,10,20,25] (6 matrices) => expected optimal cost 15125
    p1 = [30, 35, 15, 5, 10, 20, 25]  # 6 matrices
    cost1 = matrix_chain_order(p1)
    print(f"matrix_chain_order(CLRS) = {cost1} (expected 15125)")

    # Simple 2-matrix example: 10x20, 20x30 -> cost = 10*20*30 = 6000
    p2 = [10, 20, 30]
    cost2 = matrix_chain_order(p2)
    print(f"matrix_chain_order([10,20,30]) = {cost2} (expected 6000)")

    # Single matrix: no multiplication needed => cost 0
    p3 = [5, 10]
    cost3 = matrix_chain_order(p3)
    print(f"matrix_chain_order(single) = {cost3} (expected 0)")
