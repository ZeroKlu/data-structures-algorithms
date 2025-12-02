package code_samples.section5.problems.problem5_3;

class TreeNode {
    int val;
    TreeNode left;
    TreeNode right;
    TreeNode(int v) { val = v; }
}

public class problem5_3 {
    boolean isValidBST(TreeNode root) {
        return helper(root, null, null);
    }

    boolean helper(TreeNode node, Integer low, Integer high) {
        if (node == null) return true;
        if (low != null && node.val <= low) return false;
        if (high != null && node.val >= high) return false;
        return helper(node.left, low, node.val)
            && helper(node.right, node.val, high);
    }

    // -------------------------------------------------------
    // Helpers
    // -------------------------------------------------------

    TreeNode node(int v) {
        return new TreeNode(v);
    }

    static final int INDENT = 4;

    void printTree(TreeNode root) {
        System.out.println("Tree (sideways, right is up):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    void printTreeImpl(TreeNode root, int indent) {
        if (root == null) return;
        printTreeImpl(root.right, indent + INDENT);
        System.out.println(" ".repeat(indent) + root.val);
        printTreeImpl(root.left, indent + INDENT);
    }

    void testTree(String label, TreeNode root) {
        System.out.println("==== " + label + " ====");
        printTree(root);
        System.out.println("isValidBST: " + isValidBST(root));
        System.out.println();
    }

    // -------------------------------------------------------
    // Main Test Harness
    // -------------------------------------------------------

    public static void main(String[] args) {
        problem5_3 p = new problem5_3();

        // 1. Empty tree
        TreeNode empty = null;

        // 2. Single node
        TreeNode single = p.node(10);

        // 3. Valid BST:
        //
        //         4
        //       /   \
        //      2     6
        //     / \   / \
        //    1   3 5   7
        TreeNode valid = p.node(4);
        valid.left = p.node(2);
        valid.right = p.node(6);
        valid.left.left = p.node(1);
        valid.left.right = p.node(3);
        valid.right.left = p.node(5);
        valid.right.right = p.node(7);

        // 4. Invalid BST (local violation)
        //
        //      5
        //     / \
        //    1   4   <-- 4 < 5 but placed in right subtree of 5
        //       / \
        //      3   6
        TreeNode invalidLocal = p.node(5);
        invalidLocal.left = p.node(1);
        invalidLocal.right = p.node(4);
        invalidLocal.right.left = p.node(3);
        invalidLocal.right.right = p.node(6);

        // 5. Invalid BST (deep violation)
        //
        //        5
        //      /   \
        //     2     8
        //      \
        //       6   <-- 6 > 5 but inside left subtree of 5
        TreeNode invalidDeep = p.node(5);
        invalidDeep.left = p.node(2);
        invalidDeep.right = p.node(8);
        invalidDeep.left.right = p.node(6);

        // Run tests
        p.testTree("Empty tree (should be true)", empty);
        p.testTree("Single node (should be true)", single);
        p.testTree("Valid BST (should be true)", valid);
        p.testTree("Invalid BST - local violation (should be false)", invalidLocal);
        p.testTree("Invalid BST - deep violation (should be false)", invalidDeep);
    }
}
