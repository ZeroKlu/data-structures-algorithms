"""Lesson 9 - Advanced Dynamic Programming"""

from __future__ import annotations
from dataclasses import dataclass
from typing import Optional

@dataclass
class TreeNode:
    """Model for a binary tree node."""
    val: int
    left: Optional["TreeNode"] = None
    right: Optional["TreeNode"] = None

def max_root_to_leaf(root: Optional[TreeNode]) -> int:
    """Return the maximum sum of a root-to-leaf path in a binary tree."""
    if root is None:
        return float("-inf")
    if root.left is None and root.right is None:
        return root.val
    left_best = max_root_to_leaf(root.left)
    right_best = max_root_to_leaf(root.right)
    return root.val + max(left_best, right_best)

def tsp_bitmask(n: int, dist: list[list[int]]) -> int:
    """Return the minimum cost of a traveling salesman problem."""
    max_mask = 1 << n
    INF = 10**15
    dp = [[INF] * n for _ in range(max_mask)]
    dp[1 << 0][0] = 0

    for mask in range(max_mask):
        for j in range(n):
            if not mask & (1 << j):
                continue
            curr = dp[mask][j]
            if curr == INF:
                continue
            for k in range(n):
                if mask & (1 << k):
                    continue
                next_mask = mask | (1 << k)
                cand = curr + dist[j][k]
                if cand < dp[next_mask][k]:
                    dp[next_mask][k] = cand

    full_mask = max_mask - 1
    return min(dp[full_mask])

def matrix_chain_order(p: list[int]) -> int:
    """Return the minimum cost of matrix multiplication."""
    n = len(p) - 1
    INF = 10**15
    dp = [[0] * (n + 1) for _ in range(n + 1)]

    for length in range(2, n + 1):
        for i in range(1, n - length + 2):
            j = i + length - 1
            dp[i][j] = INF
            for k in range(i, j):
                cost = dp[i][k] + dp[k + 1][j] + p[i - 1] * p[k] * p[j]
                if cost < dp[i][j]:
                    dp[i][j] = cost
    return dp[1][n]

# ===========================
# Tests for Lesson 9
# ===========================

if __name__ == "__main__":
    print("=== Test 1: max_root_to_leaf ===\n")

    # Build tree:
    #       10
    #      /  \
    #     5    20
    #    / \     \
    #   3   7     25
    #
    # Best root-to-leaf sum = 10 + 20 + 25 = 55
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

    best_path = max_root_to_leaf(root_node)
    print(f"max_root_to_leaf(tree) = {best_path} (expected 55)")

    single = TreeNode(42)
    single_best = max_root_to_leaf(single)
    print(f"max_root_to_leaf(single=42) = {single_best} (expected 42)")

    null_best = max_root_to_leaf(None)
    print(f"max_root_to_leaf(None) = {null_best} (expected {float('-inf')})")

    print()
    print("=== Test 2: tsp_bitmask ===\n")

    # 4-node example:
    distances = [
        [0, 10, 15, 20],
        [10, 0, 35, 25],
        [15, 35, 0, 30],
        [20, 25, 30, 0],
    ]

    # Best path from 0 visiting all nodes once has cost 65
    tsp_cost = tsp_bitmask(4, distances)
    print(f"tsp_bitmask(4 nodes) = {tsp_cost} (expected 65)")

    print()
    print("=== Test 3: matrix_chain_order ===\n")

    # CLRS example
    p1 = [30, 35, 15, 5, 10, 20, 25]  # 6 matrices
    cost1 = matrix_chain_order(p1)
    print(f"matrix_chain_order(CLRS) = {cost1} (expected 15125)")

    # Simple 2-matrix example: 10x20, 20x30 -> 10*20*30 = 6000
    p2 = [10, 20, 30]
    cost2 = matrix_chain_order(p2)
    print(f"matrix_chain_order([10,20,30]) = {cost2} (expected 6000)")

    # Single matrix: cost 0
    p3 = [5, 10]
    cost3 = matrix_chain_order(p3)
    print(f"matrix_chain_order(single) = {cost3} (expected 0)")
