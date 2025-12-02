package code_samples.section5.problems.problem5_6;

import java.util.*;

class TreeNode {
    int val;
    TreeNode left;
    TreeNode right;
    TreeNode(int v) { val = v; }
}

class Codec {
    // Serialize using preorder with "#" for nulls.
    public String serialize(TreeNode root) {
        StringBuilder sb = new StringBuilder();
        serializeDfs(root, sb);
        return sb.toString();
    }

    private void serializeDfs(TreeNode node, StringBuilder sb) {
        if (node == null) {
            sb.append("#,");
            return;
        }
        sb.append(node.val).append(',');
        serializeDfs(node.left, sb);
        serializeDfs(node.right, sb);
    }

    public TreeNode deserialize(String data) {
        String[] parts = data.split(",");
        Deque<String> q = new ArrayDeque<>(Arrays.asList(parts));
        return deserializeDfs(q);
    }

    private TreeNode deserializeDfs(Deque<String> q) {
        if (q.isEmpty()) return null;
        String t = q.removeFirst();
        if (t.equals("#")) return null;
        int val = Integer.parseInt(t);
        TreeNode node = new TreeNode(val);
        node.left = deserializeDfs(q);
        node.right = deserializeDfs(q);
        return node;
    }
}

public class problem5_6 {

    /* -------------------------------
       Helper: sideways tree printer
       ------------------------------- */

    private static final int INDENT = 4;

    private static void printTree(TreeNode root) {
        System.out.println("Tree (sideways, right is up):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    private static void printTreeImpl(TreeNode node, int indent) {
        if (node == null) return;

        printTreeImpl(node.right, indent + INDENT);

        System.out.println(" ".repeat(indent) + node.val);

        printTreeImpl(node.left, indent + INDENT);
    }

    /* -------------------------------
       Round-trip test
       ------------------------------- */

    private static void roundTrip(String label, Codec codec, TreeNode root) {
        System.out.println("==== " + label + " ====");
        printTree(root);

        String s1 = codec.serialize(root);
        System.out.println("Serialized:     " + s1);

        TreeNode copy = codec.deserialize(s1);
        System.out.println("Deserialized:");
        printTree(copy);

        String s2 = codec.serialize(copy);
        System.out.println("Re-serialized:  " + s2);

        if (s1.equals(s2))
            System.out.println("Round-trip OK (strings match)\n");
        else
            System.out.println("Round-trip MISMATCH!\n");
    }

    /* -------------------------------
       Main test harness
       ------------------------------- */

    public static void main(String[] args) {
        Codec codec = new Codec();

        // 1. Empty tree
        TreeNode empty = null;

        // 2. Single-node tree
        TreeNode single = new TreeNode(42);

        // 3. Larger example tree:
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
