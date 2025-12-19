"""Problem 5.4: Path Sum"""

from typing import Optional

# pylint: disable=too-few-public-methods


class TreeNode:
    """
    Model for a binary tree node.

    Each node stores:
    - val   : the integer value at this node
    - left  : reference to the left child (or None)
    - right : reference to the right child (or None)
    """
    def __init__(
        self,
        val: int = 0,
        left: "Optional[TreeNode]" = None,
        right: "Optional[TreeNode]" = None
    ):
        self.val = val
        self.left = left
        self.right = right


def has_path_sum(root: Optional[TreeNode], target_sum: int) -> bool:
    """
    Return True if there exists a ROOT-to-LEAF path whose values sum to target_sum.

    Important detail:
    - The path must end at a LEAF node (a node with no children).
    - It's not enough to match the sum at some internal node.

    Strategy (DFS recursion):
    - At each node, subtract node.val from target_sum to form a "remaining" sum.
    - Recurse down to children looking for a leaf that matches exactly.
    """
    # If we reached an empty subtree, it can't produce a valid path
    if root is None:
        return False

    # If this is a leaf node, check whether we've hit the target exactly
    if root.left is None and root.right is None:
        return root.val == target_sum

    # Otherwise, subtract this node's value and search children
    remaining = target_sum - root.val

    # If either subtree has a valid path with the remaining sum, return True
    return (
        has_path_sum(root.left, remaining)
        or has_path_sum(root.right, remaining)
    )


# ----------------------------
# Tree printer (sideways)
# ----------------------------

def print_tree(root: Optional[TreeNode]):
    """
    Print the tree sideways for visualization.

    Convention:
    - Right subtree prints first (appears "up")
    - Left subtree prints last (appears "down")
    """
    print("Tree (sideways, right is up):")
    _print_tree_impl(root, 0)
    print()


INDENT = 4  # number of spaces per depth level when printing


def _print_tree_impl(node: Optional[TreeNode], indent: int):
    """Recursive helper for sideways tree printing."""
    if node is None:
        return

    # Print right subtree first so it appears above the current node
    _print_tree_impl(node.right, indent + INDENT)

    # Print the current node with indentation based on depth
    print(" " * indent + str(node.val))

    # Print left subtree last so it appears below the current node
    _print_tree_impl(node.left, indent + INDENT)


# ----------------------------
# Test utility
# ----------------------------

def test_case(label: str, root: Optional[TreeNode], target: int):
    """
    Print a labeled test case:
    - shows the tree
    - prints the target sum
    - prints whether has_path_sum returns True/False
    """
    print(f"==== {label} ====")
    print_tree(root)
    print("Target sum:", target)
    print("has_path_sum:", has_path_sum(root, target))
    print()


# ----------------------------
# Build test trees
# ----------------------------

# 1. Empty tree (no paths exist)
empty = None

# 2. Single node tree (root is also a leaf)
single = TreeNode(7)

# 3. Large example tree:
#
#         5
#       /   \
#      4     8
#     /     / \
#    11    13  4
#   /  \        \
#  7    2        1
#
# This is a classic Path Sum example (LeetCode-style).
t = TreeNode(
    5,
    left=TreeNode(
        4,
        left=TreeNode(
            11,
            left=TreeNode(7),
            right=TreeNode(2)
        )
    ),
    right=TreeNode(
        8,
        left=TreeNode(13),
        right=TreeNode(
            4,
            right=TreeNode(1)
        )
    )
)

# Valid root-to-leaf path sums in this tree:
#   22 → 5 + 4 + 11 + 2
#   26 → 5 + 8 + 13
#   18 → 5 + 8 + 4 + 1
#
# Example of an invalid "sum" if you forget the leaf requirement:
#    5 → matches root alone, but root is NOT a leaf, so it must be False.

# ----------------------------
# Run tests
# ----------------------------

test_case("Empty tree (→ False)", empty, 10)
test_case("Single node (7 == 7 → True)", single, 7)
test_case("Single node (7 != 10 → False)", single, 10)

test_case("Large tree (path sum 22 → True)", t, 22)
test_case("Large tree (path sum 26 → True)", t, 26)
test_case("Large tree (path sum 18 → True)", t, 18)
test_case("Large tree (path sum 5 → False)", t, 5)
