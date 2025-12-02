"""Problem 5.6 - Serialize and Deserialize Binary Tree"""

from typing import Optional, Iterator

#pylint: disable=too-few-public-methods

class TreeNode:
    """Model for a binary tree node."""
    def __init__(self, val: int = 0,
                 left: "Optional[TreeNode]" = None,
                 right: "Optional[TreeNode]" = None):
        self.val = val
        self.left = left
        self.right = right

class Codec:
    """Encodes and decodes a binary tree."""
    def serialize(self, root_node: Optional[TreeNode]) -> str:
        """Encodes a tree to a single string."""
        vals: list[str] = []

        def dfs(node: Optional[TreeNode]) -> None:
            if node is None:
                vals.append("#")
                return
            vals.append(str(node.val))
            dfs(node.left)
            dfs(node.right)

        dfs(root_node)
        return ",".join(vals)

    def deserialize(self, data: str) -> Optional[TreeNode]:
        """Decodes a single string to a tree."""
        tokens = iter(data.split(","))

        def dfs(it: Iterator[str]) -> Optional[TreeNode]:
            try:
                t = next(it)
            except StopIteration:
                return None
            if t == "#":
                return None
            node = TreeNode(int(t))
            node.left = dfs(it)
            node.right = dfs(it)
            return node

        return dfs(tokens)

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
# Round-trip test utility
# ----------------------------

def roundtrip(label: str, codec_obj: Codec, root_node: TreeNode) -> None:
    """Run a round-trip test."""
    print(f"==== {label} ====")
    print_tree(root_node)

    s1 = codec_obj.serialize(root_node)
    print("Serialized:     ", s1)

    copy = codec_obj.deserialize(s1)
    print("Deserialized:")
    print_tree(copy)

    s2 = codec_obj.serialize(copy)
    print("Re-serialized:  ", s2)

    if s1 == s2:
        print("Round-trip OK (strings match)\n")
    else:
        print("Round-trip MISMATCH!\n")


# ----------------------------
# Build test trees
# ----------------------------

# 1. Empty tree
empty = None

# 2. Single-node tree
single = TreeNode(42)

# 3. Larger example tree:
#
#         1
#       /   \
#      2     3
#     / \   /
#    4   5 6
#
root = TreeNode(
    1,
    left=TreeNode(2, TreeNode(4), TreeNode(5)),
    right=TreeNode(3, TreeNode(6), None)
)

# ----------------------------
# Run tests
# ----------------------------

codec = Codec()

roundtrip("Empty tree", codec, empty)
roundtrip("Single-node tree", codec, single)
roundtrip("Larger example tree", codec, root)
