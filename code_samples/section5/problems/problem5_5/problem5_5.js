// ==========================
// BINARY SEARCH TREE NODE
// ==========================

// Represents a single node in a Binary Search Tree (BST)
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
// LOWEST COMMON ANCESTOR (BST)
// ==========================

// Returns the Lowest Common Ancestor (LCA) of nodes p and q in a BST.
//
// BST properties used:
// - All values in the left subtree  < node.val
// - All values in the right subtree > node.val
//
// Key idea:
// - If BOTH p and q are smaller than the current node,
//   the LCA must be in the left subtree.
// - If BOTH p and q are larger than the current node,
//   the LCA must be in the right subtree.
// - Otherwise, the current node is the split point
//   and therefore the LCA.
//
// This is an iterative solution (no recursion).
function lowestCommonAncestor(root, p, q) {
    // Start searching from the root
    let node = root;

    // Cache values for repeated comparisons
    const pv = p.val;
    const qv = q.val;

    // Walk down the tree until the split point is found
    while (node !== null) {

        // Both target values are smaller → go left
        if (pv < node.val && qv < node.val) {
            node = node.left;

        // Both target values are larger → go right
        } else if (pv > node.val && qv > node.val) {
            node = node.right;

        // Split point found (or node equals p or q)
        } else {
            return node;
        }
    }

    // If the tree is empty or nodes are missing
    return null;
}

// ----------------------------
// Tree printer (sideways)
// ----------------------------

// Number of spaces to indent per level
const INDENT = 4;

// Prints the tree sideways for visualization:
// - Right subtree appears above the node
// - Left subtree appears below the node
function printTree(root) {
    console.log("Tree (sideways, right is up):");
    printTreeImpl(root, 0);
    console.log();
}

// Recursive helper for sideways printing
function printTreeImpl(node, indent) {
    // Base case: nothing to print
    if (node === null) return;

    // Print right subtree first (appears "up")
    printTreeImpl(node.right, indent + INDENT);

    // Print current node with indentation
    console.log(" ".repeat(indent) + node.val);

    // Print left subtree last (appears "down")
    printTreeImpl(node.left, indent + INDENT);
}

// ----------------------------
// Test utility
// ----------------------------

// Runs a labeled LCA test and prints the result
function testLCA(label, root, p, q) {
    console.log(`==== ${label} ====`);
    console.log(`Find LCA(${p.val}, ${q.val})`);

    // Compute the LCA
    const ans = lowestCommonAncestor(root, p, q);

    // Print result
    console.log("LCA =", ans ? ans.val : "null");
    console.log();
}

// ----------------------------
// Build BST for testing
// ----------------------------
//
//           6
//         /   \
//        2     8
//       / \   / \
//      0   4 7   9
//         / \
//        3   5
//

// Construct the BST
const root = new TreeNode(6);
root.left = new TreeNode(2);
root.right = new TreeNode(8);

root.left.left = new TreeNode(0);
root.left.right = new TreeNode(4);
root.left.right.left = new TreeNode(3);
root.left.right.right = new TreeNode(5);

root.right.left = new TreeNode(7);
root.right.right = new TreeNode(9);

// References to specific nodes (used as p and q)
const n2 = root.left;
const n8 = root.right;
const n4 = root.left.right;
const n3 = root.left.right.left;
const n5 = root.left.right.right;
const n0 = root.left.left;
const n7 = root.right.left;
const n9 = root.right.right;

// ----------------------------
// Execute tests
// ----------------------------

// Visualize the tree
printTree(root);

// Run LCA test cases
testLCA("LCA(2, 8) → expected 6", root, n2, n8);
testLCA("LCA(2, 4) → expected 2", root, n2, n4);
testLCA("LCA(3, 5) → expected 4", root, n3, n5);
testLCA("LCA(0, 5) → expected 2", root, n0, n5);
testLCA("LCA(7, 9) → expected 8", root, n7, n9);
