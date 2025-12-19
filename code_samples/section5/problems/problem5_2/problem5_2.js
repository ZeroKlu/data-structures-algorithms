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
// HEIGHT (O(n))
// ==========================

// Computes the height of a binary tree.
// Convention used here:
// - empty tree height = -1
// - single node height = 0
function height(root) {
    // Base case: empty subtree
    if (root === null) return -1;

    // Height = 1 + max(height(left), height(right))
    return 1 + Math.max(height(root.left), height(root.right));
}

// ==========================
// BALANCED TREE CHECK (O(n) SINGLE PASS)
// ==========================

// Returns true if the tree is height-balanced.
// A tree is "balanced" if for every node, the left and right subtree heights
// differ by at most 1.
function isBalanced(root) {
    // check(node) returns BOTH:
    // - balanced: whether subtree is balanced
    // - height:   height of subtree
    //
    // Returning both avoids recomputing heights repeatedly (more efficient).
    function check(node) {
        // Base case: empty subtree is balanced with height -1
        if (node === null) return { balanced: true, height: -1 };

        // Recursively check left subtree
        const left = check(node.left);

        // Early exit: if left subtree is unbalanced, no need to continue
        if (!left.balanced) return { balanced: false, height: 0 };

        // Recursively check right subtree
        const right = check(node.right);

        // Early exit: if right subtree is unbalanced, no need to continue
        if (!right.balanced) return { balanced: false, height: 0 };

        // Current node is balanced if child heights differ by at most 1
        const diff = Math.abs(left.height - right.height);
        const bal = diff <= 1;

        // Height of this subtree is 1 + max(left height, right height)
        return {
            balanced: bal,
            height: 1 + Math.max(left.height, right.height)
        };
    }

    // We only need the "balanced" result at the root
    return check(root).balanced;
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
    console.log("");
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
// Test helper
// ----------------------------

// Runs a labeled test: prints the tree, its height, and whether it's balanced
function testTree(label, root) {
    console.log(`==== ${label} ====`);
    printTree(root);

    // height() computes height independently (O(n))
    console.log("height:", height(root));

    // isBalanced() uses the single-pass check() (O(n))
    console.log("isBalanced:", isBalanced(root));
    console.log("");
}

// ----------------------------
// Build test trees
// ----------------------------

// 1) Empty tree
const empty = null;

// 2) Single node tree
const single = new TreeNode(1);

// 3) Perfectly balanced tree:
//
//        1
//      /   \
//     2     3
//    / \   / \
//   4  5  6   7
const balanced = new TreeNode(
    1,
    new TreeNode(2, new TreeNode(4), new TreeNode(5)),
    new TreeNode(3, new TreeNode(6), new TreeNode(7))
);

// 4) Slightly unbalanced but still balanced (diff <= 1 everywhere):
//
//      1
//    /   \
//   2     3
//  /
// 4
const shallow = new TreeNode(
    1,
    new TreeNode(2, new TreeNode(4), null),
    new TreeNode(3)
);

// 5) Deep unbalanced tree (should be false):
//
//      1
//     /
//    2
//   /
//  3
//   \
//    4
const deep = new TreeNode(
    1,
    new TreeNode(
        2,
        new TreeNode(3, null, new TreeNode(4)),
        null
    ),
    null
);

// ----------------------------
// Run tests
// ----------------------------

testTree("Empty tree", empty);
testTree("Single node", single);
testTree("Perfectly balanced tree", balanced);
testTree("Shallow unbalanced but balanced", shallow);
testTree("Deep unbalanced (should be false)", deep);
