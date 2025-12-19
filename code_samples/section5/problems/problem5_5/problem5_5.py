"""Problem 5.5: Lowest Common Ancestor"""

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


def lowest_common_ancestor(
    root_node: Optional[TreeNode],
    p: TreeNode,
    q: TreeNode
) -> Optional[TreeNode]:
    """
    Return the Lowest Common Ancestor (LCA) of p and q in a BST.

    IMPORTANT: This solution assumes the tree is a *Binary Search Tree* (BST).

    BST property:
    - All values in left subtree  < node.val
    - All values in right subtree > node.val

    Key idea:
    Starting at the root, compare p and q to the current node:
    - If BOTH p and q are smaller than node.val, LCA must be in the left subtree.
    - If BOTH p and q are larger than node.val, LCA must be in the right subtree.
    - Otherwise (they split, or one equals the node), the current node is the LCA.
      This is the “first split point” on the path from root downward.

    Runs in O(h) time, where h is tree height, and O(1) extra space.
    """
    node = root_node

    # Cache values to avoid repeatedly reading p.val / q.val in the loop
    pv = p.val
    qv = q.val

    # Walk down the BST until we find the split point
    while node is not None:
        # If both targets are smaller, LCA must be to the left
        if pv < node.val and qv < node.val:
            node = node.left

        # If both targets are larger, LCA must be to the right
        elif pv > node.val and qv > node.val:
            node = node.right

        # Otherwise, node is the split point (or equals one target) => LCA found
        else:
            return node

    # If we somehow fall off the tree, return None (e.g., root_node was None)
    return None


# ----------------------------
# Tree printer (sideways)
# ----------------------------

def print_tree(root_node: Optional[TreeNode]):
    """
    Print the tree sideways for visualization.

    Convention:
    - Right subtree prints first (appears "up")
    - Left subtree prints last (appears "down")
    """
    print("Tree (sideways, right is up):")
    _print_tree_impl(root_node, 0)
    print()


INDENT = 4  # spaces per depth level when printing


def _print_tree_impl(node: Optional[TreeNode], indent: int):
    """Recursive helper for sideways tree printing."""
    if node is None:
        return

    _print_tree_impl(node.right, indent + INDENT)

    print(" " * indent + str(node.val))

    _print_tree_impl(node.left, indent + INDENT)


# ----------------------------
# Test utility
# ----------------------------

def test_lca(label: str, root_node: Optional[TreeNode], p: TreeNode, q: TreeNode):
    """
    Print a labeled LCA test:
    - shows which two nodes we're querying
    - prints the LCA value (or None)
    """
    print(f"==== {label} ====")
    ans = lowest_common_ancestor(root_node, p, q)
    print(f"LCA({p.val}, {q.val}) ->", ans.val if ans else None)
    print()


# ----------------------------
# Build BST for testing
# ----------------------------
#
#            6
#          /   \
#         2     8
#        / \   / \
#       0   4 7   9
#          / \
#         3   5
#
# This is a BST, so we can use the fast iterative split-point approach.
root = TreeNode(6)
root.left = TreeNode(2)
root.right = TreeNode(8)

root.left.left = TreeNode(0)
root.left.right = TreeNode(4)
root.left.right.left = TreeNode(3)
root.left.right.right = TreeNode(5)

root.right.left = TreeNode(7)
root.right.right = TreeNode(9)

# References for convenience (these are the actual node objects in the tree)
n2 = root.left
n8 = root.right
n4 = root.left.right
n3 = n4.left
n5 = n4.right
n0 = root.left.left
n7 = root.right.left
n9 = root.right.right


# ----------------------------
# Run test cases
# ----------------------------

# Print tree visually so test output is easier to follow
print_tree(root)

test_lca("LCA(2, 8) → expected 6", root, n2, n8)
test_lca("LCA(2, 4) → expected 2", root, n2, n4)
test_lca("LCA(3, 5) → expected 4", root, n3, n5)
test_lca("LCA(0, 5) → expected 2", root, n0, n5)
test_lca("LCA(7, 9) → expected 8", root, n7, n9)
