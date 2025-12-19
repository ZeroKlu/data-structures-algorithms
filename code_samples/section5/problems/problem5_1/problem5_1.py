"""Problem 5.1 - Traversals (Preorder, Inorder, Postorder)"""

from typing import List, Optional

#pylint: disable=too-few-public-methods

class TreeNode:
    """Model for a binary tree node."""
    def __init__(self, val: int = 0,
                 left: "Optional[TreeNode]" = None,
                 right: "Optional[TreeNode]" = None):
        self.val = val
        self.left = left
        self.right = right

def preorder(root_node: Optional[TreeNode]) -> List[int]:
    """Preorder traversal."""
    if root_node is None:
        return []
    return [root_node.val] + preorder(root_node.left) + preorder(root_node.right)

def inorder(root_node: Optional[TreeNode]) -> List[int]:
    """Inorder traversal."""
    if root_node is None:
        return []
    return inorder(root_node.left) + [root_node.val] + inorder(root_node.right)

def postorder(root_node: Optional[TreeNode]) -> List[int]:
    """Postorder traversal."""
    if root_node is None:
        return []
    return postorder(root_node.left) + postorder(root_node.right) + [root_node.val]

# ----------------------------
# Tree Printer (sideways)
# ----------------------------

def print_tree(root_node: Optional[TreeNode]):
    """Print the tree sideways."""
    print("Tree (sideways):")
    _print_tree_impl(root_node, 0)
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
# Test Harness
# ----------------------------

# Build the test tree:
#         1
#       /   \
#      2     3
#     / \   /
#    4   5 6

root = TreeNode(
    1,
    left=TreeNode(
        2,
        left=TreeNode(4),
        right=TreeNode(5)
    ),
    right=TreeNode(
        3,
        left=TreeNode(6)
    )
)

# Print the tree
print_tree(root)

# Run traversals
pre = preorder(root)
inn = inorder(root)
post = postorder(root)

# Output results
print("Preorder: ", pre)
print("Inorder:  ", inn)
print("Postorder:", post)
