package code_samples.section5.problems.problem5_4;

class TreeNode {
    int val;
    TreeNode left;
    TreeNode right;
    TreeNode(int v) { val = v; }
}

public class problem5_4 {

    boolean hasPathSum(TreeNode root, int targetSum) {
        if (root == null) return false;
        if (root.left == null && root.right == null) {
            return root.val == targetSum;
        }
        int remaining = targetSum - root.val;
        return hasPathSum(root.left, remaining) ||
               hasPathSum(root.right, remaining);
    }

    /* -------------------------------------------------------
       Helpers for testing
       ------------------------------------------------------- */

    private TreeNode node(int v) {
        return new TreeNode(v);
    }

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

    private void testCase(String label, TreeNode root, int target) {
        System.out.println("==== " + label + " ====");
        printTree(root);
        System.out.println("Target sum: " + target);
        System.out.println("hasPathSum: " + hasPathSum(root, target));
        System.out.println();
    }

    /* -------------------------------------------------------
       Main test harness
       ------------------------------------------------------- */

    public static void main(String[] args) {

        problem5_4 p = new problem5_4();

        // 1) Empty tree
        TreeNode empty = null;

        // 2) Single node
        TreeNode single = p.node(7);

        // 3) Test tree:
        //
        //         5
        //       /   \
        //      4     8
        //     /     / \
        //    11    13  4
        //   /  \        \
        //  7    2        1
        TreeNode t = p.node(5);
        t.left = p.node(4);
        t.right = p.node(8);

        t.left.left = p.node(11);
        t.left.left.left = p.node(7);
        t.left.left.right = p.node(2);

        t.right.left = p.node(13);
        t.right.right = p.node(4);
        t.right.right.right = p.node(1);

        // Run test cases
        p.testCase("Empty tree", empty, 10);
        p.testCase("Single node (7 == 7 → true)", single, 7);
        p.testCase("Single node (7 != 10 → false)", single, 10);

        p.testCase("Large tree (has path sum 22 → true)", t, 22); // 5+4+11+2
        p.testCase("Large tree (has path sum 26 → true)", t, 26); // 5+8+13
        p.testCase("Large tree (has path sum 18 → true)", t, 18); // 5+8+4+1
        p.testCase("Large tree (no path sum == 5 → false)", t, 5); // 5 is not a leaf

    }
}
