package code_samples.section5.problems.problem5_4;

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

public class problem5_4 {

    // ==========================
    // HAS PATH SUM (ROOT-TO-LEAF)
    // ==========================

    // Returns true if there exists a ROOT-to-LEAF path such that
    // the sum of the node values along the path equals targetSum.
    //
    // Important:
    // - The path must end at a leaf node (a node with no children).
    boolean hasPathSum(TreeNode root, int targetSum) {
        // Base case: empty subtree has no valid path
        if (root == null) return false;

        // If this is a leaf node, check whether its value matches the remaining sum
        if (root.left == null && root.right == null) {
            return root.val == targetSum;
        }

        // Subtract current node's value from the target sum
        int remaining = targetSum - root.val;

        // Recursively check left and right subtrees
        // If either subtree has a valid path, return true
        return hasPathSum(root.left, remaining) ||
               hasPathSum(root.right, remaining);
    }

    /* -------------------------------------------------------
       Helpers for testing
       ------------------------------------------------------- */

    // Convenience function to create a new node
    private TreeNode node(int v) {
        return new TreeNode(v);
    }

    // Number of spaces to indent per tree level (for sideways printing)
    private static final int INDENT = 4;

    // Prints the tree sideways for visualization:
    // - Right subtree appears above the node
    // - Left subtree appears below the node
    private void printTree(TreeNode root) {
        System.out.println("Tree (sideways, right is up):");
        printTreeImpl(root, 0);
        System.out.println();
    }

    // Recursive helper for sideways tree printing
    private void printTreeImpl(TreeNode root, int indent) {
        // Base case: nothing to print
        if (root == null) return;

        // Print right subtree first (appears "up")
        printTreeImpl(root.right, indent + INDENT);

        // Print current node with indentation based on depth
        System.out.println(" ".repeat(indent) + root.val);

        // Print left subtree last (appears "down")
        printTreeImpl(root.left, indent + INDENT);
    }

    // Runs a labeled test case:
    // prints the tree, target sum, and result of hasPathSum
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

        // 1) Empty tree (should always be false)
        TreeNode empty = null;

        // 2) Single node tree
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

        // Empty tree
        p.testCase("Empty tree", empty, 10);

        // Single node tests
        p.testCase("Single node (7 == 7 → true)", single, 7);
        p.testCase("Single node (7 != 10 → false)", single, 10);

        // Root-to-leaf path sums in t:
        // 5 + 4 + 11 + 2 = 22
        // 5 + 8 + 13     = 26
        // 5 + 8 + 4 + 1  = 18
        p.testCase("Large tree (has path sum 22 → true)", t, 22);
        p.testCase("Large tree (has path sum 26 → true)", t, 26);
        p.testCase("Large tree (has path sum 18 → true)", t, 18);

        // This should be false because 5 is not a leaf node
        p.testCase("Large tree (no path sum == 5 → false)", t, 5);
    }
}
