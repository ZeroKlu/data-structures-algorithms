"""Problem 5.1 - Traversals (Preorder, Inorder, Postorder)"""

from typing import List, Optional

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


def preorder(root_node: Optional[TreeNode]) -> List[int]:
    """
    Preorder traversal (Root → Left → Right).

    Algorithm:
    1. Visit the current node
    2. Recursively traverse the left subtree
    3. Recursively traverse the right subtree
    """
    if root_node is None:
        return []

    return (
        [root_node.val]
        + preorder(root_node.left)
        + preorder(root_node.right)
    )


def inorder(root_node: Optional[TreeNode]) -> List[int]:
    """
    Inorder traversal (Left → Root → Right).

    Algorithm:
    1. Recursively traverse the left subtree
    2. Visit the current node
    3. Recursively traverse the right subtree

    In a Binary Search Tree (BST), this produces sorted order.
    """
    if root_node is None:
        return []

    return (
        inorder(root_node.left)
        + [root_node.val]
        + inorder(root_node.right)
    )


def postorder(root_node: Optional[TreeNode]) -> List[int]:
    """
    Postorder traversal (Left → Right → Root).

    Algorithm:
    1. Recursively traverse the left subtree
    2. Recursively traverse the right subtree
    3. Visit the current node

    Commonly used for deleting/freeing trees or evaluating expressions.
    """
    if root_node is None:
        return []

    return (
        postorder(root_node.left)
        + postorder(root_node.right)
        + [root_node.val]
    )


# ----------------------------
# Tree Printer (sideways)
# ----------------------------

def print_tree(root_node: Optional[TreeNode]):
    """
    Print the binary tree sideways for visualization.

    Right subtree is printed first so it appears at the top,
    left subtree is printed last so it appears at the bottom.
    """
    print("Tree (sideways):")
    _print_tree_impl(root_node, 0)
    print()


INDENT = 4  # number of spaces per tree level


def _print_tree_impl(node: Optional[TreeNode], indent: int):
    """
    Recursive helper for sideways tree printing.

    Parameters:
    - node   : current tree node
    - indent : horizontal spacing based on tree depth
    """
    if node is None:
        return

    # Print right subtree first (appears above)
    _print_tree_impl(node.right, indent + INDENT)

    # Print current node value with indentation
    print(" " * indent + str(node.val))

    # Print left subtree last (appears below)
    _print_tree_impl(node.left, indent + INDENT)


# ----------------------------
# Test Harness
# ----------------------------

# Build the test tree:
#
#         1
#       /   \
#      2     3
#     / \   /
#    4   5 6
#
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

# Print the tree visually
print_tree(root)

# Run traversals
pre = preorder(root)
inn = inorder(root)
post = postorder(root)

# Output results
print("Preorder: ", pre)
print("Inorder:  ", inn)
print("Postorder:", post)
