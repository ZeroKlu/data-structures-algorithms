"""Problem 5.4: Path Sum"""

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

def has_path_sum(root: Optional[TreeNode], target_sum: int) -> bool:
    """Return True if the binary tree has a path sum equal to the target sum."""
    if root is None:
        return False
    if root.left is None and root.right is None:
        return root.val == target_sum
    remaining = target_sum - root.val
    return has_path_sum(root.left, remaining) or \
           has_path_sum(root.right, remaining)

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
    if node is None:
        return

    _print_tree_impl(node.right, indent + INDENT)

    print(" " * indent + str(node.val))

    _print_tree_impl(node.left, indent + INDENT)


# ----------------------------
# Test utility
# ----------------------------

def test_case(label, root, target):
    """Run a test case."""
    print(f"==== {label} ====")
    print_tree(root)
    print("Target sum:", target)
    print("has_path_sum:", has_path_sum(root, target))
    print()


# ----------------------------
# Build test trees
# ----------------------------

# 1. Empty tree
empty = None

# 2. Single node
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

# Valid path sums:
#   22 → 5 + 4 + 11 + 2
#   26 → 5 + 8 + 13
#   18 → 5 + 8 + 4 + 1
# Invalid:
#    5 → root-only sum, but 5 is not a leaf

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
