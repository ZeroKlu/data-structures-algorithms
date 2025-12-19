// ==========================
// BINARY TREE NODE DEFINITION
// ==========================

// Represents a single node in a binary tree
class TreeNode {
    // val   : value stored in this node
    // left  : reference to left child (null if none)
    // right : reference to right child (null if none)
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

// ==========================
// HAS PATH SUM (ROOT-TO-LEAF)
// ==========================

// Returns true if there exists a ROOT-to-LEAF path
// such that the sum of node values along the path
// equals targetSum.
//
// Important:
// - The path must end at a leaf node (no children).
function hasPathSum(root, targetSum) {
    // Base case: empty subtree has no path
    if (root === null) return false;

    // If this is a leaf node, check whether its value
    // exactly matches the remaining target sum
    if (root.left === null && root.right === null) {
        return root.val === targetSum;
    }

    // Subtract current node's value from the target
    const remaining = targetSum - root.val;

    // Recursively check left and right subtrees
    // If either subtree has a valid path, return true
    return hasPathSum(root.left, remaining) ||
           hasPathSum(root.right, remaining);
}

// ----------------------------
// Tree printer (sideways)
// ----------------------------

// Prints the tree sideways for visualization:
// - Right subtree appears above the node
// - Left subtree appears below the node
function printTree(root) {
    console.log("Tree (sideways, right is up):");
    printTreeImpl(root, 0);
    console.log();
}

// Number of spaces to indent per tree level
const INDENT = 4;

// Recursive helper for sideways printing
function printTreeImpl(node, indent) {
    // Base case: nothing to print
    if (node === null) return;

    // Print right subtree first (appears "up")
    printTreeImpl(node.right, indent + INDENT);

    // Print current node with indentation based on depth
    console.log(" ".repeat(indent) + node.val);

    // Print left subtree last (appears "down")
    printTreeImpl(node.left, indent + INDENT);
}

// ----------------------------
// Test utility
// ----------------------------

// Runs a labeled test case:
// prints the tree, target sum, and result
function testCase(label, root, target) {
    console.log(`==== ${label} ====`);
    printTree(root);
    console.log("Target Sum:", target);
    console.log("hasPathSum:", hasPathSum(root, target));
    console.log();
}

// ----------------------------
// Build test trees
// ----------------------------

// 1) Empty tree (always false)
const empty = null;

// 2) Single node tree
const single = new TreeNode(7);

// 3) Large test tree:
//
//         5
//       /   \
//      4     8
//     /     / \
//    11    13  4
//   /  \        \
//  7    2        1
const t = new TreeNode(
    5,
    new TreeNode(
        4,
        new TreeNode(
            11,
            new TreeNode(7),
            new TreeNode(2)
        ),
        null
    ),
    new TreeNode(
        8,
        new TreeNode(13),
        new TreeNode(
            4,
            null,
            new TreeNode(1)
        )
    )
);

// ----------------------------
// Run test cases
// ----------------------------

// Empty tree
testCase("Empty tree", empty, 10);

// Single node tests
testCase("Single node (7 == 7 → true)", single, 7);
testCase("Single node (7 != 10 → false)", single, 10);

// Root-to-leaf path sums in t:
// 5 + 4 + 11 + 2 = 22
// 5 + 8 + 13     = 26
// 5 + 8 + 4 + 1  = 18
testCase("Large tree (has path sum 22 → true)", t, 22);
testCase("Large tree (has path sum 26 → true)", t, 26);
testCase("Large tree (has path sum 18 → true)", t, 18);

// This is false because the path must end at a leaf
testCase("Large tree (no path sum == 5 → false)", t, 5);
