"""Problem 5.5: Lowest Common Ancestor"""

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

def lowest_common_ancestor(root_node: Optional[TreeNode],
                           p: TreeNode,
                           q: TreeNode) -> Optional[TreeNode]:
    """Return the lowest common ancestor of p and q."""
    node = root_node
    pv = p.val
    qv = q.val
    while node is not None:
        if pv < node.val and qv < node.val:
            node = node.left
        elif pv > node.val and qv > node.val:
            node = node.right
        else:
            return node
    return None

# ----------------------------
# Tree printer (sideways)
# ----------------------------

def print_tree(root_node):
    """Print the tree sideways."""
    print("Tree (sideways, right is up):")
    _print_tree_impl(root_node, 0)
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

def test_lca(label, root_node, p, q):
    """Run a test case."""
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

root = TreeNode(6)
root.left = TreeNode(2)
root.right = TreeNode(8)

root.left.left = TreeNode(0)
root.left.right = TreeNode(4)
root.left.right.left = TreeNode(3)
root.left.right.right = TreeNode(5)

root.right.left = TreeNode(7)
root.right.right = TreeNode(9)

# references for convenience
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

print_tree(root)

test_lca("LCA(2, 8) → expected 6", root, n2, n8)
test_lca("LCA(2, 4) → expected 2", root, n2, n4)
test_lca("LCA(3, 5) → expected 4", root, n3, n5)
test_lca("LCA(0, 5) → expected 2", root, n0, n5)
test_lca("LCA(7, 9) → expected 8", root, n7, n9)
