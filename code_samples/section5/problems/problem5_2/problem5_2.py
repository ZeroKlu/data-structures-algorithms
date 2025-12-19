"""Problem 5.2 - Check Balanced Binary Tree"""

from typing import Optional, Tuple

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


def height(root: Optional[TreeNode]) -> int:
    """
    Return the height of a binary tree.

    We use the convention:
    - empty tree has height -1
    - single node has height 0

    Height definition:
      height(node) = 1 + max(height(left), height(right))
    """
    if root is None:
        return -1
    return 1 + max(height(root.left), height(root.right))


def _check(node: Optional[TreeNode]) -> Tuple[bool, int]:
    """
    Check if the tree rooted at 'node' is height-balanced, and return:
      (is_balanced, height)

    A binary tree is "balanced" if for every node:
      abs(height(left) - height(right)) <= 1

    Key idea (efficient approach):
    - Compute balance + height in ONE DFS pass.
    - If a subtree is already unbalanced, we "early return" and stop work.
    """
    # Base case: empty subtree is balanced and has height -1
    if node is None:
        return True, -1

    # Recurse into left subtree
    left_bal, left_h = _check(node.left)

    # If left subtree is unbalanced, no need to keep checking
    if not left_bal:
        return False, 0  # height doesn't matter anymore once unbalanced

    # Recurse into right subtree
    right_bal, right_h = _check(node.right)

    # If right subtree is unbalanced, stop early
    if not right_bal:
        return False, 0

    # Current node is balanced if left and right heights differ by at most 1
    bal = abs(left_h - right_h) <= 1

    # Height of current node = 1 + max(child heights)
    h = 1 + max(left_h, right_h)

    return bal, h


def is_balanced(root: Optional[TreeNode]) -> bool:
    """
    Public API: return True if the entire tree is balanced, else False.

    We call _check(root) which returns (balanced, height),
    and we only care about the balanced flag.
    """
    bal, _ = _check(root)
    return bal


# ----------------------------
# Tree printer (sideways)
# ----------------------------

def print_tree(root: Optional[TreeNode]):
    """
    Print the binary tree sideways.

    Convention:
    - Right subtree prints first (appears "up" on the screen)
    - Left subtree prints last (appears "down")
    """
    print("Tree (sideways, right is up):")
    _print_tree_impl(root, 0)
    print()


INDENT = 4  # spaces per depth level when printing


def _print_tree_impl(node: Optional[TreeNode], indent: int):
    """Recursive helper for sideways tree printing."""
    if node is None:
        return

    # Print right subtree first so it appears above the root
    _print_tree_impl(node.right, indent + INDENT)

    # Print current node with indentation
    print(" " * indent + str(node.val))

    # Print left subtree last so it appears below the root
    _print_tree_impl(node.left, indent + INDENT)


# ----------------------------
# Test utility
# ----------------------------

def test_tree(label: str, root: Optional[TreeNode]):
    """
    Print a labeled test case:
    - shows the tree
    - prints its height
    - prints whether it is balanced
    """
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

# 2. Single node (balanced by definition)
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

# 4. Slightly unbalanced but still balanced (diff <= 1):
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
