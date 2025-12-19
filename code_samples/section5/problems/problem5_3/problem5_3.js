// ==========================
// BINARY TREE NODE DEFINITION
// ==========================

// Represents a single node in a binary tree
class TreeNode {
    // val   : value stored in the node
    // left  : reference to left child (null if none)
    // right : reference to right child (null if none)
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

// ==========================
// VALIDATE BST (RANGE METHOD)
// ==========================

// Returns true if the binary tree is a valid Binary Search Tree (BST).
// BST rule (strict):
// - All values in the left subtree are < node.val
// - All values in the right subtree are > node.val
//
// This implementation uses a "range" approach, where each node
// must fall within an allowable (low, high) range inherited
// from its ancestors.
function isValidBST(root) {

    // Recursive helper that checks validity within bounds
    // - low  : lower bound (node.val must be > low), or null if none
    // - high : upper bound (node.val must be < high), or null if none
    function helper(node, low, high) {
        // Base case: empty subtree is valid
        if (node === null) return true;

        // Check against lower bound if it exists
        if (low !== null && node.val <= low) return false;

        // Check against upper bound if it exists
        if (high !== null && node.val >= high) return false;

        // Left subtree must be within (low, node.val)
        // Right subtree must be within (node.val, high)
        return helper(node.left, low, node.val) &&
               helper(node.right, node.val, high);
    }

    // Start recursion with no bounds at the root
    return helper(root, null, null);
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

// Runs a labeled test: prints the tree and whether it is a valid BST
function testTree(label, root) {
    console.log(`==== ${label} ====`);
    printTree(root);
    console.log("isValidBST:", isValidBST(root));
    console.log();
}

// ----------------------------
// Build test trees
// ----------------------------

// 1) Empty tree (should be true)
const empty = null;

// 2) Single node tree (should be true)
const single = new TreeNode(10);

// 3) Valid BST:
//
//        4
//      /   \
//     2     6
//    / \   / \
//   1  3  5  7
const valid = new TreeNode(
    4,
    new TreeNode(2, new TreeNode(1), new TreeNode(3)),
    new TreeNode(6, new TreeNode(5), new TreeNode(7))
);

// 4) Invalid BST (local violation):
//
//      5
//     / \
//    1   4   <-- 4 is in right subtree of 5 but 4 < 5
//       / \
//      3   6
const invalidLocal = new TreeNode(
    5,
    new TreeNode(1),
    new TreeNode(4, new TreeNode(3), new TreeNode(6))
);

// 5) Invalid BST (deep violation):
//
//        5
//      /   \
//     2     8
//      \
//       6   <-- 6 > 5 but nested inside left subtree
const invalidDeep = new TreeNode(
    5,
    new TreeNode(2, null, new TreeNode(6)),
    new TreeNode(8)
);

// ----------------------------
// Run tests
// ----------------------------

testTree("Empty tree (should be true)", empty);
testTree("Single node (should be true)", single);
testTree("Valid BST (should be true)", valid);
testTree("Invalid BST - local violation (should be false)", invalidLocal);
testTree("Invalid BST - deep violation (should be false)", invalidDeep);
