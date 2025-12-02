"""Problem 5.3: Validate Binary Search Tree"""

from typing import Optional

#pylint: disable=too-few-public-methods

class TreeNode:
    """Model for a binary tree node."""
    def __init__(self, val: int = 0,
                 left: "Optional[TreeNode]" = None,
                 right: "Optional[TreeNode]" = None):
        self.val = val
        self.left = left
        self.right = right

def is_valid_bst(root: Optional[TreeNode]) -> bool:
    """Return True if the binary tree is a valid binary search tree."""
    def helper(node: Optional[TreeNode],
               low: Optional[int],
               high: Optional[int]) -> bool:
        if node is None:
            return True
        if low is not None and node.val <= low:
            return False
        if high is not None and node.val >= high:
            return False
        return helper(node.left, low, node.val) and \
               helper(node.right, node.val, high)
    return helper(root, None, None)

# ----------------------------
# Tree printer (sideways)
# ----------------------------

def print_tree(root):
    """Print the tree sideways."""
    print("Tree (sideways, right is up):")
    _print_tree_impl(root, 0)
    print()

INDENT = 4

def _print_tree_impl(node, indent):
    """Print the tree sideways."""
    if node is None:
        return

    _print_tree_impl(node.right, indent + INDENT)

    print(" " * indent + str(node.val))

    _print_tree_impl(node.left, indent + INDENT)


# ----------------------------
# Test utility
# ----------------------------

def test_tree(label, root):
    """Test a tree."""
    print(f"==== {label} ====")
    print_tree(root)
    print("is_valid_bst:", is_valid_bst(root))
    print()


# ----------------------------
# Build test trees
# ----------------------------

# 1. Empty tree
empty = None

# 2. Single node
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
#    1   4   <-- 4 < 5 but in right subtree of 5
#       / \
#      3   6
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
#       6   <-- 6 > 5 but placed inside left subtree of 5
invalid_deep = TreeNode(
    5,
    left=TreeNode(
        2,
        right=TreeNode(6)  # violates BST invariant
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
