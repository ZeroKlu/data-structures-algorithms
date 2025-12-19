package code_samples.section5.problems.problem5_3;

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

public class problem5_3 {

    // ==========================
    // VALIDATE BST (RANGE METHOD)
    // ==========================

    // Returns true if the tree is a valid Binary Search Tree (BST).
    // BST rule (strict):
    // - All values in the left subtree are < node.val
    // - All values in the right subtree are > node.val
    //
    // This uses a "range" approach: each node must stay within (low, high)
    // where the bounds come from its ancestors.
    boolean isValidBST(TreeNode root) {
        // Start with no bounds at the root
        return helper(root, null, null);
    }

    // Recursively validates BST property using optional bounds:
    // - low  : lower bound (node.val must be strictly greater than low)
    // - high : upper bound (node.val must be strictly less than high)
    //
    // We use Integer (object) instead of int so we can represent "no bound" as null.
    boolean helper(TreeNode node, Integer low, Integer high) {
        // Base case: empty subtree is valid
        if (node == null) return true;

        // If a lower bound exists, current value must be > low
        if (low != null && node.val <= low) return false;

        // If an upper bound exists, current value must be < high
        if (high != null && node.val >= high) return false;

        // Left subtree must be within (low, node.val)
        // Right subtree must be within (node.val, high)
        return helper(node.left, low, node.val)
            && helper(node.right, node.val, high);
    }

    // -------------------------------------------------------
    // Helpers
    // -------------------------------------------------------

    // Convenience function to create a new node
    TreeNode node(int v) {
        return new TreeNode(v);
    }

    // Number of spaces to indent per tree level (for sideways printing)
    static final int INDENT = 4;

    // Prints the tree sideways:
    // - Right subtree appears above
    // - Left subtree appears below
    void printTree(TreeNode root) {
        System.out.println("Tree (sideways, right is up):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    // Recursive helper for sideways printing
    void printTreeImpl(TreeNode root, int indent) {
        // Base case: nothing to print
        if (root == null) return;

        // Print right subtree first so it appears "up"
        printTreeImpl(root.right, indent + INDENT);

        // Print current node with indentation based on depth
        System.out.println(" ".repeat(indent) + root.val);

        // Print left subtree last so it appears "down"
        printTreeImpl(root.left, indent + INDENT);
    }

    // Runs a labeled test: prints the tree and whether it's a valid BST
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

        // 1) Empty tree (should be true)
        TreeNode empty = null;

        // 2) Single node (should be true)
        TreeNode single = p.node(10);

        // 3) Valid BST:
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

        // 4) Invalid BST (local violation):
        //
        //      5
        //     / \
        //    1   4   <-- 4 is in right subtree of 5 but 4 < 5 (invalid)
        //       / \
        //      3   6
        TreeNode invalidLocal = p.node(5);
        invalidLocal.left = p.node(1);
        invalidLocal.right = p.node(4);
        invalidLocal.right.left = p.node(3);
        invalidLocal.right.right = p.node(6);

        // 5) Invalid BST (deep violation):
        //
        //        5
        //      /   \
        //     2     8
        //      \
        //       6   <-- 6 is in LEFT subtree of 5 but 6 > 5 (invalid)
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
