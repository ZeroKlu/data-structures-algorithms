"""Problem 5.3: Validate Binary Search Tree"""

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


def is_valid_bst(root: Optional[TreeNode]) -> bool:
    """
    Return True if the binary tree is a valid Binary Search Tree (BST).

    BST rule (strict):
    - All values in the LEFT subtree must be < node.val
    - All values in the RIGHT subtree must be > node.val
    - This must hold for EVERY node, not just the direct children

    Strategy:
    Use recursion with allowable bounds:
    - low  = the smallest value this node is allowed to be greater than
    - high = the largest value this node is allowed to be less than

    If any node violates its bounds, the tree is not a valid BST.
    """

    def helper(node: Optional[TreeNode], low: Optional[int], high: Optional[int]) -> bool:
        """
        Validate the subtree rooted at 'node' where:
        - node.val must be strictly greater than low (if low is not None)
        - node.val must be strictly less than high (if high is not None)

        Returns True if this subtree is a valid BST under these constraints.
        """
        # Empty subtree is valid by definition
        if node is None:
            return True

        # If a lower bound exists, node must be strictly greater than it
        if low is not None and node.val <= low:
            return False

        # If an upper bound exists, node must be strictly less than it
        if high is not None and node.val >= high:
            return False

        # Recurse:
        # - Left subtree must be in (low, node.val)
        # - Right subtree must be in (node.val, high)
        return helper(node.left, low, node.val) and helper(node.right, node.val, high)

    # Start with no bounds at the root
    return helper(root, None, None)


# ----------------------------
# Tree printer (sideways)
# ----------------------------

def print_tree(root: Optional[TreeNode]):
    """
    Print the tree sideways (right subtree at the top).

    This is just a visualization tool to help see structure.
    """
    print("Tree (sideways, right is up):")
    _print_tree_impl(root, 0)
    print()


INDENT = 4  # number of spaces per depth level when printing


def _print_tree_impl(node: Optional[TreeNode], indent: int):
    """Recursive helper for sideways tree printing."""
    if node is None:
        return

    # Print right subtree first so it appears "above"
    _print_tree_impl(node.right, indent + INDENT)

    # Print this node with indentation based on depth
    print(" " * indent + str(node.val))

    # Print left subtree last so it appears "below"
    _print_tree_impl(node.left, indent + INDENT)


# ----------------------------
# Test utility
# ----------------------------

def test_tree(label: str, root: Optional[TreeNode]):
    """
    Print a labeled test case:
    - shows the tree
    - prints whether it is a valid BST
    """
    print(f"==== {label} ====")
    print_tree(root)
    print("is_valid_bst:", is_valid_bst(root))
    print()


# ----------------------------
# Build test trees
# ----------------------------

# 1. Empty tree (valid BST)
empty = None

# 2. Single node (valid BST)
single = TreeNode(10)

# 3. Valid BST:
#
#        4
#      /   \
#     2     6
#    / \   / \
#   1  3  5  7
valid = TreeNode(
    4,
    left=TreeNode(
        2,
        left=TreeNode(1),
        right=TreeNode(3),
    ),
    right=TreeNode(
        6,
        left=TreeNode(5),
        right=TreeNode(7),
    )
)

# 4. Invalid BST (local violation):
#
#      5
#     / \
#    1   4   <-- 4 < 5 but placed in right subtree of 5
#       / \
#      3   6
#
# The helper/bounds method catches this because "4" is in the right subtree
# of 5, so it must be > 5, but it's not.
invalid_local = TreeNode(
    5,
    left=TreeNode(1),
    right=TreeNode(
        4,
        left=TreeNode(3),
        right=TreeNode(6)
    )
)

# 5. Invalid BST (deep violation):
#
#        5
#      /   \
#     2     8
#      \
#       6   <-- 6 > 5 but inside LEFT subtree of 5
#
# This is the classic example showing why checking only parent/child is not enough.
# Even though 6 > 2 (so it "looks fine" locally), it breaks the global bound:
# anything in the left subtree of 5 must be < 5.
invalid_deep = TreeNode(
    5,
    left=TreeNode(
        2,
        right=TreeNode(6)  # violates BST invariant (must be < 5 here)
    ),
    right=TreeNode(8)
)


# ----------------------------
# Run tests
# ----------------------------

test_tree("Empty tree (should be true)", empty)
test_tree("Single node (should be true)", single)
test_tree("Valid BST (should be true)", valid)
test_tree("Invalid BST - local violation (should be false)", invalid_local)
test_tree("Invalid BST - deep violation (should be false)", invalid_deep)
