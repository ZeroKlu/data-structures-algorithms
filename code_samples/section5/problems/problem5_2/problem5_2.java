package code_samples.section5.problems.problem5_2;

// ==========================
// TREE NODE DEFINITION
// ==========================

// Represents a single node in a binary tree
class TreeNode {
    int val;         // Value stored in this node
    TreeNode left;   // Reference to left child (null if none)
    TreeNode right;  // Reference to right child (null if none)

    // Constructor initializes the node value
    TreeNode(int v) { val = v; }
}

// ==========================
// INFO HELPER CLASS
// ==========================

// Used by check() to return two pieces of information at once:
// - balanced: whether the subtree is height-balanced
// - height:   height of the subtree
class Info {
    boolean balanced;
    int height;

    // Constructor initializes both fields
    Info(boolean b, int h) { balanced = b; height = h; }
}

// ==========================
// PROBLEM 5_2: BALANCED TREE CHECK
// ==========================

class problem5_2 {

    // ----------------------------
    // Height (O(n))
    // ----------------------------

    // Computes the height of a binary tree.
    // Convention used here:
    // - empty tree height = -1
    // - single node height = 0
    int height(TreeNode root) {
        // Base case: empty subtree
        if (root == null) return -1;

        // Height = 1 + max(height(left), height(right))
        return 1 + Math.max(height(root.left), height(root.right));
    }

    // ----------------------------
    // Balance check (O(n) single pass)
    // ----------------------------

    // Returns Info for the subtree rooted at 'node'.
    // By returning both height and balanced status together,
    // we avoid recomputing heights repeatedly.
    private Info check(TreeNode node) {
        // Base case: empty subtree is balanced with height -1
        if (node == null) return new Info(true, -1);

        // Recursively check left subtree
        Info left = check(node.left);

        // Early exit: if left subtree is unbalanced, the whole subtree is unbalanced
        if (!left.balanced) return new Info(false, 0);

        // Recursively check right subtree
        Info right = check(node.right);

        // Early exit: if right subtree is unbalanced, the whole subtree is unbalanced
        if (!right.balanced) return new Info(false, 0);

        // Current node is balanced if child heights differ by at most 1
        boolean bal = Math.abs(left.height - right.height) <= 1;

        // Height of this subtree is 1 + max(left height, right height)
        int h = 1 + Math.max(left.height, right.height);

        // Return both balanced status and height
        return new Info(bal, h);
    }

    // Public wrapper: returns true if the whole tree is height-balanced
    boolean isBalanced(TreeNode root) {
        return check(root).balanced;
    }

    // -----------------------------------------------------------
    // Helpers
    // -----------------------------------------------------------

    // Convenience function to create a new node
    private TreeNode node(int v) {
        return new TreeNode(v);
    }

    // Number of spaces to indent per tree level (for printing)
    private static final int INDENT = 4;

    // Prints the tree sideways for visualization:
    // - Right subtree appears above the node
    // - Left subtree appears below the node
    private void printTree(TreeNode root) {
        System.out.println("Tree (sideways, right is up):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    // Recursive helper to print the tree sideways
    private void printTreeImpl(TreeNode node, int indent) {
        // Base case: nothing to print
        if (node == null) return;

        // Print right subtree first (appears "up")
        printTreeImpl(node.right, indent + INDENT);

        // Print current node with indentation based on depth
        System.out.println(" ".repeat(indent) + node.val);

        // Print left subtree last (appears "down")
        printTreeImpl(node.left, indent + INDENT);
    }

    // Runs a labeled test: prints the tree, its height, and whether it's balanced
    private void testTree(String label, TreeNode root) {
        System.out.println("==== " + label + " ====");
        printTree(root);

        // Height computed independently (O(n))
        System.out.println("height: " + height(root));

        // Balanced check computed via check() (O(n) single pass)
        System.out.println("isBalanced: " + (isBalanced(root) ? "true" : "false"));
        System.out.println();
    }

    // -----------------------------------------------------------
    // Main test harness
    // -----------------------------------------------------------

    public static void main(String[] args) {
        problem5_2 p = new problem5_2();

        // 1) Empty tree
        TreeNode empty = null;

        // 2) Single node tree
        TreeNode single = p.node(1);

        // 3) Perfectly balanced tree
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

        // 4) Slightly unbalanced but still balanced (diff <= 1 everywhere)
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

        // 5) Deep unbalanced tree (should be false)
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
