"""Problem 5.6 - Serialize and Deserialize Binary Tree"""

from typing import Optional, Iterator

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


class Codec:
    """
    Encodes and decodes a binary tree.

    Approach:
    - Serialize using PREORDER traversal (node, left, right)
    - Use a sentinel (here "#") to represent null pointers
    - Join tokens with commas

    Why we need "#":
    - Preorder values alone are not enough to reconstruct shape.
    - Including null markers makes the representation unambiguous.
    """

    def serialize(self, root_node: Optional[TreeNode]) -> str:
        """
        Encode a tree into a single string.

        Output format:
        - preorder traversal
        - "#" for None
        Example: "1,2,#,#,3,#,#"
        """
        vals: list[str] = []  # collected output tokens as strings

        def dfs(node: Optional[TreeNode]) -> None:
            """Preorder DFS that appends values (or '#') into vals."""
            if node is None:
                # Use a marker so we can rebuild structure during deserialization
                vals.append("#")
                return

            # Visit current node first (preorder)
            vals.append(str(node.val))

            # Then serialize left and right subtrees
            dfs(node.left)
            dfs(node.right)

        dfs(root_node)
        return ",".join(vals)

    def deserialize(self, data: str) -> Optional[TreeNode]:
        """
        Decode a single string back into a tree.

        Strategy:
        - Read tokens in the same order they were produced (preorder)
        - When we see "#", return None
        - Otherwise create a node, then recursively build its left and right children
        """
        # Turn the comma-separated string into an iterator of tokens
        tokens = iter(data.split(","))

        def dfs(it: Iterator[str]) -> Optional[TreeNode]:
            """Consume tokens from 'it' and rebuild the tree recursively."""
            try:
                t = next(it)  # get the next token
            except StopIteration:
                # If data is malformed or empty, stop gracefully
                return None

            # "#" represents a null child
            if t == "#":
                return None

            # Otherwise, token is a node value
            node = TreeNode(int(t))

            # Rebuild left subtree first (matches preorder)
            node.left = dfs(it)

            # Then rebuild right subtree
            node.right = dfs(it)

            return node

        return dfs(tokens)


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
# Round-trip test utility
# ----------------------------

def roundtrip(label: str, codec_obj: Codec, root_node: Optional[TreeNode]) -> None:
    """
    Run a round-trip test:
    - serialize the tree
    - deserialize it back
    - serialize again
    - compare strings to ensure the structure/values survived
    """
    print(f"==== {label} ====")
    print_tree(root_node)

    # Serialize original tree
    s1 = codec_obj.serialize(root_node)
    print("Serialized:     ", s1)

    # Deserialize into a copy
    copy = codec_obj.deserialize(s1)
    print("Deserialized:")
    print_tree(copy)

    # Serialize the copy again
    s2 = codec_obj.serialize(copy)
    print("Re-serialized:  ", s2)

    # If the serialization format is deterministic, these should match
    if s1 == s2:
        print("Round-trip OK (strings match)\n")
    else:
        print("Round-trip MISMATCH!\n")


# ----------------------------
# Build test trees
# ----------------------------

# 1) Empty tree
empty = None

# 2) Single-node tree
single = TreeNode(42)

# 3) Larger example tree:
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
