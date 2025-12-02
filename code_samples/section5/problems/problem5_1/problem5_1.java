package code_samples.section5.problems.problem5_1;

import java.util.ArrayList;
import java.util.List;

public class problem5_1 {
    
    static class TreeNode {
        int val;
        TreeNode left, right;
        TreeNode(int v) { val = v; }
    }

    // ------------------------
    // Traversal functions
    // ------------------------

    List<Integer> preorder(TreeNode root) {
        List<Integer> out = new ArrayList<>();
        preorderDfs(root, out);
        return out;
    }

    void preorderDfs(TreeNode node, List<Integer> out) {
        if (node == null) return;
        out.add(node.val);
        preorderDfs(node.left, out);
        preorderDfs(node.right, out);
    }

    List<Integer> inorder(TreeNode root) {
        List<Integer> out = new ArrayList<>();
        inorderDfs(root, out);
        return out;
    }

    void inorderDfs(TreeNode node, List<Integer> out) {
        if (node == null) return;
        inorderDfs(node.left, out);
        out.add(node.val);
        inorderDfs(node.right, out);
    }

    List<Integer> postorder(TreeNode root) {
        List<Integer> out = new ArrayList<>();
        postorderDfs(root, out);
        return out;
    }

    void postorderDfs(TreeNode node, List<Integer> out) {
        if (node == null) return;
        postorderDfs(node.left, out);
        postorderDfs(node.right, out);
        out.add(node.val);
    }

    // ------------------------
    // Tree Printing
    // ------------------------

    void printTree(TreeNode root) {
        System.out.println("Tree (sideways):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    private static final int INDENT = 4;

    void printTreeImpl(TreeNode node, int indent) {
        if (node == null) return;

        printTreeImpl(node.right, indent + INDENT);

        System.out.print(" ".repeat(indent));
        System.out.println(node.val);

        printTreeImpl(node.left, indent + INDENT);
    }

    // ------------------------
    // Main test harness
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

        TreeNode root = new TreeNode(1);
        root.left = new TreeNode(2);
        root.right = new TreeNode(3);
        root.left.left = new TreeNode(4);
        root.left.right = new TreeNode(5);
        root.right.left = new TreeNode(6);

        // Visual print
        m.printTree(root);

        // Traversals
        List<Integer> pre = m.preorder(root);
        List<Integer> in = m.inorder(root);
        List<Integer> post = m.postorder(root);

        System.out.println("Preorder:  " + pre);
        System.out.println("Inorder:   " + in);
        System.out.println("Postorder: " + post);
    }
}
