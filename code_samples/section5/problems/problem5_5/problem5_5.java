package code_samples.section5.problems.problem5_5;

// ==========================
// BINARY SEARCH TREE NODE
// ==========================

// Represents a single node in a Binary Search Tree (BST)
class TreeNode {
    int val;        // value stored in the node
    TreeNode left;  // reference to left child
    TreeNode right; // reference to right child

    // Constructor initializes the node value
    TreeNode(int v) {
        val = v;
    }
}

public class problem5_5 {

    // ==========================
    // LOWEST COMMON ANCESTOR (BST)
    // ==========================

    // Returns the Lowest Common Ancestor (LCA) of nodes p and q in a BST.
    //
    // BST properties used:
    // - All values in the left subtree  < node.val
    // - All values in the right subtree > node.val
    //
    // Algorithm:
    // - Start at the root.
    // - If BOTH p and q are smaller than the current node,
    //   move to the left subtree.
    // - If BOTH p and q are larger than the current node,
    //   move to the right subtree.
    // - Otherwise, the current node is the split point and
    //   therefore the LCA.
    //
    // Time Complexity: O(h), where h is the height of the tree
    // Space Complexity: O(1), iterative approach
    TreeNode lowestCommonAncestor(TreeNode root, TreeNode p, TreeNode q) {
        TreeNode node = root;

        // Cache values for easier comparisons
        int pv = p.val;
        int qv = q.val;

        // Walk down the tree until the LCA is found
        while (node != null) {

            // Both targets are in the left subtree
            if (pv < node.val && qv < node.val) {
                node = node.left;

            // Both targets are in the right subtree
            } else if (pv > node.val && qv > node.val) {
                node = node.right;

            // Split point found (or node equals p or q)
            } else {
                return node;
            }
        }

        // If the tree is empty or nodes are not present
        return null;
    }

    /* -------------------------------------------------------
       Helpers
       ------------------------------------------------------- */

    // Convenience method to create a new tree node
    private TreeNode node(int v) {
        return new TreeNode(v);
    }

    // Number of spaces to indent per tree level
    private static final int INDENT = 4;

    // Prints the tree sideways for visualization
    // Right subtree appears "up", left subtree appears "down"
    private void printTree(TreeNode root) {
        System.out.println("Tree (sideways, right is up):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    // Recursive helper for sideways tree printing
    private void printTreeImpl(TreeNode root, int indent) {
        if (root == null) return;

        // Print right subtree first
        printTreeImpl(root.right, indent + INDENT);

        // Print current node with indentation
        System.out.println(" ".repeat(indent) + root.val);

        // Print left subtree
        printTreeImpl(root.left, indent + INDENT);
    }

    // Runs and prints a single LCA test case
    private void testLCA(String label, TreeNode root, TreeNode p, TreeNode q) {
        System.out.println("==== " + label + " ====");
        System.out.println("Find LCA(" + p.val + ", " + q.val + ")");

        TreeNode ans = lowestCommonAncestor(root, p, q);

        if (ans == null)
            System.out.println("LCA = null\n");
        else
            System.out.println("LCA = " + ans.val + "\n");
    }

    /* -------------------------------------------------------
       Main Test Harness
       ------------------------------------------------------- */

    public static void main(String[] args) {
        problem5_5 p = new problem5_5();

        /*
               6
             /   \
            2     8
           / \   / \
          0   4 7   9
             / \
            3   5
        */

        // Build the BST
        TreeNode root = p.node(6);
        root.left = p.node(2);
        root.right = p.node(8);

        root.left.left = p.node(0);
        root.left.right = p.node(4);
        root.left.right.left = p.node(3);
        root.left.right.right = p.node(5);

        root.right.left = p.node(7);
        root.right.right = p.node(9);

        // Visualize the tree
        p.printTree(root);

        // References to specific nodes in the tree
        TreeNode n2 = root.left;
        TreeNode n8 = root.right;
        TreeNode n4 = root.left.right;
        TreeNode n3 = root.left.right.left;
        TreeNode n5 = root.left.right.right;
        TreeNode n0 = root.left.left;
        TreeNode n7 = root.right.left;
        TreeNode n9 = root.right.right;

        // Run LCA test cases
        p.testLCA("LCA(2, 8) → expected 6", root, n2, n8);
        p.testLCA("LCA(2, 4) → expected 2", root, n2, n4);
        p.testLCA("LCA(3, 5) → expected 4", root, n3, n5);
        p.testLCA("LCA(0, 5) → expected 2", root, n0, n5);
        p.testLCA("LCA(7, 9) → expected 8", root, n7, n9);
    }
}
