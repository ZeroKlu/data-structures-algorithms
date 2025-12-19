package code_samples.section5.problems.problem5_6;

import java.util.*;

// ==========================
// BINARY TREE NODE DEFINITION
// ==========================

// Represents a single node in a binary tree
class TreeNode {
    int val;        // value stored in this node
    TreeNode left;  // reference to left child (null if none)
    TreeNode right; // reference to right child (null if none)

    // Constructor initializes the node value
    TreeNode(int v) { val = v; }
}

// ==========================
// CODEC: SERIALIZE / DESERIALIZE
// ==========================

class Codec {

    // Serializes a binary tree into a string using preorder traversal:
    //   node, left, right
    //
    // Null pointers are represented by "#".
    // Tokens are separated by commas.
    //
    // Example (single node 1):
    //   "1,#,#,"
    public String serialize(TreeNode root) {
        StringBuilder sb = new StringBuilder();
        serializeDfs(root, sb);
        return sb.toString();
    }

    // Recursive DFS helper for serialization (preorder)
    private void serializeDfs(TreeNode node, StringBuilder sb) {
        // If the node is null, write a null marker and stop
        if (node == null) {
            sb.append("#,");
            return;
        }

        // Write the node value followed by a comma
        sb.append(node.val).append(',');

        // Serialize left subtree then right subtree
        serializeDfs(node.left, sb);
        serializeDfs(node.right, sb);
    }

    // Deserializes the string produced by serialize() back into a tree.
    //
    // Steps:
    // 1) Split by commas into tokens
    // 2) Rebuild recursively in preorder order:
    //    - "#" => null
    //    - number => create node, build left, then build right
    public TreeNode deserialize(String data) {
        // Split into parts. Note:
        // - serialize ends with a trailing comma
        // - split(",") will include an empty string at the end
        String[] parts = data.split(",");

        // Put tokens into a queue so we can consume them in order
        Deque<String> q = new ArrayDeque<>(Arrays.asList(parts));

        // Rebuild tree from preorder tokens
        return deserializeDfs(q);
    }

    // Recursive DFS helper for deserialization
    private TreeNode deserializeDfs(Deque<String> q) {
        // Safety: no tokens left
        if (q.isEmpty()) return null;

        // Get the next token
        String t = q.removeFirst();

        // If token is "#" (or empty due to trailing comma), treat as null.
        // (The empty-token case can happen because serialize ends with ",")
        if (t.equals("#") || t.isEmpty()) return null;

        // Otherwise parse an integer value
        int val = Integer.parseInt(t);

        // Create the node
        TreeNode node = new TreeNode(val);

        // Rebuild left subtree then right subtree (mirrors serialization)
        node.left = deserializeDfs(q);
        node.right = deserializeDfs(q);

        return node;
    }
}

public class problem5_6 {

    /* -------------------------------
       Helper: sideways tree printer
       ------------------------------- */

    // Number of spaces to indent per tree level
    private static final int INDENT = 4;

    // Prints the tree sideways:
    // - Right subtree appears above the node
    // - Left subtree appears below the node
    private static void printTree(TreeNode root) {
        System.out.println("Tree (sideways, right is up):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    // Recursive helper for sideways printing
    private static void printTreeImpl(TreeNode node, int indent) {
        // Base case: nothing to print
        if (node == null) return;

        // Print right subtree first (appears "up")
        printTreeImpl(node.right, indent + INDENT);

        // Print current node value with indentation based on depth
        System.out.println(" ".repeat(indent) + node.val);

        // Print left subtree last (appears "down")
        printTreeImpl(node.left, indent + INDENT);
    }

    /* -------------------------------
       Round-trip test
       ------------------------------- */

    // RoundTrip does:
    // 1) serialize original tree -> s1
    // 2) deserialize s1 into a copy tree
    // 3) serialize copy tree -> s2
    // 4) compare s1 and s2 (should match if codec is correct)
    private static void roundTrip(String label, Codec codec, TreeNode root) {
        System.out.println("==== " + label + " ====");

        // Print original tree
        printTree(root);

        // Serialize original tree
        String s1 = codec.serialize(root);
        System.out.println("Serialized:     " + s1);

        // Deserialize into a new tree instance
        TreeNode copy = codec.deserialize(s1);

        // Print reconstructed tree
        System.out.println("Deserialized:");
        printTree(copy);

        // Serialize reconstructed tree again
        String s2 = codec.serialize(copy);
        System.out.println("Re-serialized:  " + s2);

        // Compare strings to validate that structure was preserved
        if (s1.equals(s2))
            System.out.println("Round-trip OK (strings match)\n");
        else
            System.out.println("Round-trip MISMATCH!\n");
    }

    /* -------------------------------
       Main test harness
       ------------------------------- */

    public static void main(String[] args) {
        // Codec instance provides serialize/deserialize
        Codec codec = new Codec();

        // 1) Empty tree
        TreeNode empty = null;

        // 2) Single-node tree
        TreeNode single = new TreeNode(42);

        // 3) Larger example tree:
        //
        //         1
        //       /   \
        //      2     3
        //     / \   /
        //    4   5 6
        TreeNode root = new TreeNode(1);
        root.left = new TreeNode(2);
        root.right = new TreeNode(3);
        root.left.left = new TreeNode(4);
        root.left.right = new TreeNode(5);
        root.right.left = new TreeNode(6);

        // Run tests
        roundTrip("Empty tree", codec, empty);
        roundTrip("Single-node tree", codec, single);
        roundTrip("Larger example tree", codec, root);
    }
}
