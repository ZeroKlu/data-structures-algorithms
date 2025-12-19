"""Problem 5.2 - Check Balanced Binary Tree"""
from typing import Optional, Tuple

#pylint: disable=too-few-public-methods

class TreeNode:
    """Model for a binary tree node."""
    def __init__(self, val: int = 0,
                 left: "Optional[TreeNode]" = None,
                 right: "Optional[TreeNode]" = None):
        self.val = val
        self.left = left
        self.right = right

def height(root: Optional[TreeNode]) -> int:
    """Return the height of a binary tree."""
    if root is None:
        return -1
    return 1 + max(height(root.left), height(root.right))

def _check(node: Optional[TreeNode]) -> Tuple[bool, int]:
    """Check if a binary tree is balanced."""
    if node is None:
        return True, -1
    left_bal, left_h = _check(node.left)
    if not left_bal:
        return False, 0
    right_bal, right_h = _check(node.right)
    if not right_bal:
        return False, 0
    bal = abs(left_h - right_h) <= 1
    h = 1 + max(left_h, right_h)
    return bal, h

def is_balanced(root: Optional[TreeNode]) -> bool:
    """Check if a binary tree is balanced."""
    bal, _ = _check(root)
    return bal

# ----------------------------
# Tree printer (sideways)
# ----------------------------

def print_tree(root):
    print("Tree (sideways, right is up):")
    _print_tree_impl(root, 0)
    print()

INDENT = 4

def _print_tree_impl(node, indent):
    if node is None:
        return

    _print_tree_impl(node.right, indent + INDENT)

    print(" " * indent + str(node.val))

    _print_tree_impl(node.left, indent + INDENT)


# ----------------------------
# Test utility
# ----------------------------

def test_tree(label, root):
    print(f"==== {label} ====")
    print_tree(root)
    print("height:", height(root))
    print("is_balanced:", is_balanced(root))
    print()


# ----------------------------
# Build test trees
# ----------------------------

# 1. Empty tree
empty = None

# 2. Single node
single = TreeNode(1)

# 3. Perfectly balanced:
#
#        1
#      /   \
#     2     3
#    / \   / \
#   4  5  6   7
balanced = TreeNode(
    1,
    left=TreeNode(
        2,
        left=TreeNode(4),
        right=TreeNode(5),
    ),
    right=TreeNode(
        3,
        left=TreeNode(6),
        right=TreeNode(7),
    )
)

# 4. Slightly unbalanced but still balanced:
#
#      1
#    /   \
#   2     3
#  /
# 4
shallow = TreeNode(
    1,
    left=TreeNode(
        2,
        left=TreeNode(4)
    ),
    right=TreeNode(3)
)

# 5. Deep unbalanced tree (should be false):
#
#      1
#     /
#    2
#   /
#  3
#   \
#    4
deep = TreeNode(
    1,
    left=TreeNode(
        2,
        left=TreeNode(
            3,
            right=TreeNode(4)
        )
    )
)


# ----------------------------
# Run all tests
# ----------------------------

test_tree("Empty tree", empty)
test_tree("Single node", single)
test_tree("Perfectly balanced tree", balanced)
test_tree("Shallow unbalanced but balanced", shallow)
test_tree("Deep unbalanced (should be false)", deep)
