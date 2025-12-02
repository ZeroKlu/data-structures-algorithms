package code_samples.section5.problems.problem5_2;

class TreeNode {
    int val;
    TreeNode left;
    TreeNode right;
    TreeNode(int v) { val = v; }
}

class Info {
    boolean balanced;
    int height;
    Info(boolean b, int h) { balanced = b; height = h; }
}

class problem5_2 {

    int height(TreeNode root) {
        if (root == null) return -1;
        return 1 + Math.max(height(root.left), height(root.right));
    }

    private Info check(TreeNode node) {
        if (node == null) return new Info(true, -1);
        Info left = check(node.left);
        if (!left.balanced) return new Info(false, 0);
        Info right = check(node.right);
        if (!right.balanced) return new Info(false, 0);
        boolean bal = Math.abs(left.height - right.height) <= 1;
        int h = 1 + Math.max(left.height, right.height);
        return new Info(bal, h);
    }

    boolean isBalanced(TreeNode root) {
        return check(root).balanced;
    }

    // -----------------------------------------------------------
    // Helpers
    // -----------------------------------------------------------

    private TreeNode node(int v) {
        return new TreeNode(v);
    }

    private static final int INDENT = 4;

    private void printTree(TreeNode root) {
        System.out.println("Tree (sideways, right is up):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    private void printTreeImpl(TreeNode node, int indent) {
        if (node == null) return;

        printTreeImpl(node.right, indent + INDENT);

        System.out.println(" ".repeat(indent) + node.val);

        printTreeImpl(node.left, indent + INDENT);
    }

    private void testTree(String label, TreeNode root) {
        System.out.println("==== " + label + " ====");
        printTree(root);
        System.out.println("height: " + height(root));
        System.out.println("isBalanced: " + (isBalanced(root) ? "true" : "false"));
        System.out.println();
    }

    // -----------------------------------------------------------
    // Main test harness
    // -----------------------------------------------------------

    public static void main(String[] args) {
        problem5_2 p = new problem5_2();

        // 1. Empty tree
        TreeNode empty = null;

        // 2. Single node
        TreeNode single = p.node(1);

        // 3. Perfectly balanced tree
        //
        //        1
        //      /   \
        //     2     3
        //    / \   / \
        //   4   5 6   7
        TreeNode balanced = p.node(1);
        balanced.left = p.node(2);
        balanced.right = p.node(3);
        balanced.left.left = p.node(4);
        balanced.left.right = p.node(5);
        balanced.right.left = p.node(6);
        balanced.right.right = p.node(7);

        // 4. Slightly unbalanced but still balanced
        //
        //      1
        //    /   \
        //   2     3
        //  /
        // 4
        TreeNode shallow = p.node(1);
        shallow.left = p.node(2);
        shallow.right = p.node(3);
        shallow.left.left = p.node(4);

        // 5. Deep unbalanced tree
        //
        //      1
        //     /
        //    2
        //   /
        //  3
        //   \
        //    4
        TreeNode deep = p.node(1);
        deep.left = p.node(2);
        deep.left.left = p.node(3);
        deep.left.left.right = p.node(4);

        // Run tests
        p.testTree("Empty tree", empty);
        p.testTree("Single node", single);
        p.testTree("Perfectly balanced tree", balanced);
        p.testTree("Shallow unbalanced but balanced", shallow);
        p.testTree("Deep unbalanced (should be false)", deep);
    }
}
