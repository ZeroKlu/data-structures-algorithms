package code_samples.section5.problems.problem5_5;

class TreeNode {
    int val;
    TreeNode left;
    TreeNode right;
    TreeNode(int v) { val = v; }
}

public class problem5_5 {
    TreeNode lowestCommonAncestor(TreeNode root, TreeNode p, TreeNode q) {
        TreeNode node = root;
        int pv = p.val;
        int qv = q.val;
        while (node != null) {
            if (pv < node.val && qv < node.val) {
                node = node.left;
            } else if (pv > node.val && qv > node.val) {
                node = node.right;
            } else {
                return node;
            }
        }
        return null;
    }

    /* -------------------------------------------------------
       Helpers
       ------------------------------------------------------- */

    private TreeNode node(int v) { return new TreeNode(v); }

    private static final int INDENT = 4;

    private void printTree(TreeNode root) {
        System.out.println("Tree (sideways, right is up):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    private void printTreeImpl(TreeNode root, int indent) {
        if (root == null) return;

        printTreeImpl(root.right, indent + INDENT);

        System.out.println(" ".repeat(indent) + root.val);

        printTreeImpl(root.left, indent + INDENT);
    }

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

        TreeNode root = p.node(6);
        root.left = p.node(2);
        root.right = p.node(8);

        root.left.left = p.node(0);
        root.left.right = p.node(4);
        root.left.right.left = p.node(3);
        root.left.right.right = p.node(5);

        root.right.left = p.node(7);
        root.right.right = p.node(9);

        p.printTree(root);

        // References for convenience
        TreeNode n2 = root.left;
        TreeNode n8 = root.right;
        TreeNode n4 = root.left.right;
        TreeNode n3 = root.left.right.left;
        TreeNode n5 = root.left.right.right;
        TreeNode n0 = root.left.left;
        TreeNode n7 = root.right.left;
        TreeNode n9 = root.right.right;

        p.testLCA("LCA(2, 8) → expected 6", root, n2, n8);
        p.testLCA("LCA(2, 4) → expected 2", root, n2, n4);
        p.testLCA("LCA(3, 5) → expected 4", root, n3, n5);
        p.testLCA("LCA(0, 5) → expected 2", root, n0, n5);
        p.testLCA("LCA(7, 9) → expected 8", root, n7, n9);
    }
}
