package code_samples.section5.problems.problem5_1;

import java.util.ArrayList;
import java.util.List;

public class problem5_1 {

    // ==========================
    // TREE NODE DEFINITION
    // ==========================

    // Represents a single node in a binary tree
    static class TreeNode {
        int val;            // Value stored in this node
        TreeNode left;      // Reference to left child (null if none)
        TreeNode right;     // Reference to right child (null if none)

        // Constructor initializes the node value
        TreeNode(int v) { val = v; }
    }

    // ------------------------
    // TRAVERSAL FUNCTIONS
    // ------------------------

    // Returns the preorder traversal of the tree
    // Order: Root -> Left -> Right
    List<Integer> preorder(TreeNode root) {
        // List to collect traversal output
        List<Integer> out = new ArrayList<>();

        // Perform DFS traversal
        preorderDfs(root, out);

        return out;
    }

    // Recursive helper for preorder traversal
    void preorderDfs(TreeNode node, List<Integer> out) {
        // Base case: empty subtree
        if (node == null) return;

        // Visit current node
        out.add(node.val);

        // Traverse left subtree
        preorderDfs(node.left, out);

        // Traverse right subtree
        preorderDfs(node.right, out);
    }

    // Returns the inorder traversal of the tree
    // Order: Left -> Root -> Right
    List<Integer> inorder(TreeNode root) {
        // List to collect traversal output
        List<Integer> out = new ArrayList<>();

        // Perform DFS traversal
        inorderDfs(root, out);

        return out;
    }

    // Recursive helper for inorder traversal
    void inorderDfs(TreeNode node, List<Integer> out) {
        // Base case: empty subtree
        if (node == null) return;

        // Traverse left subtree first
        inorderDfs(node.left, out);

        // Visit current node
        out.add(node.val);

        // Traverse right subtree
        inorderDfs(node.right, out);
    }

    // Returns the postorder traversal of the tree
    // Order: Left -> Right -> Root
    List<Integer> postorder(TreeNode root) {
        // List to collect traversal output
        List<Integer> out = new ArrayList<>();

        // Perform DFS traversal
        postorderDfs(root, out);

        return out;
    }

    // Recursive helper for postorder traversal
    void postorderDfs(TreeNode node, List<Integer> out) {
        // Base case: empty subtree
        if (node == null) return;

        // Traverse left subtree
        postorderDfs(node.left, out);

        // Traverse right subtree
        postorderDfs(node.right, out);

        // Visit current node last
        out.add(node.val);
    }

    // ------------------------
    // TREE PRINTING
    // ------------------------

    // Prints the tree sideways for easy visualization
    // Right subtree appears on top, left subtree on bottom
    void printTree(TreeNode root) {
        System.out.println("Tree (sideways):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    // Number of spaces to indent per tree level
    private static final int INDENT = 4;

    // Recursive helper that prints the tree sideways
    void printTreeImpl(TreeNode node, int indent) {
        // Base case: nothing to print
        if (node == null) return;

        // Print right subtree first (appears above)
        printTreeImpl(node.right, indent + INDENT);

        // Print current node with indentation
        System.out.print(" ".repeat(indent));
        System.out.println(node.val);

        // Print left subtree last (appears below)
        printTreeImpl(node.left, indent + INDENT);
    }

    // ------------------------
    // MAIN TEST HARNESS
    // ------------------------

    public static void main(String[] args) {
        problem5_1 m = new problem5_1();

        /*
               1
             /   \
            2     3
           / \   /
          4   5 6
        */

        // Build the test tree
        TreeNode root = new TreeNode(1);
        root.left = new TreeNode(2);
        root.right = new TreeNode(3);
        root.left.left = new TreeNode(4);
        root.left.right = new TreeNode(5);
        root.right.left = new TreeNode(6);

        // Visual print of the tree
        m.printTree(root);

        // Run traversals
        List<Integer> pre = m.preorder(root);
        List<Integer> in = m.inorder(root);
        List<Integer> post = m.postorder(root);

        // Print traversal results
        System.out.println("Preorder:  " + pre);
        System.out.println("Inorder:   " + in);
        System.out.println("Postorder: " + post);
    }
}
