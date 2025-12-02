class TreeNode {
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

function isValidBST(root) {
    function helper(node, low, high) {
        if (node === null) return true;
        if (low !== null && node.val <= low) return false;
        if (high !== null && node.val >= high) return false;
        return helper(node.left, low, node.val) &&
               helper(node.right, node.val, high);
    }
    return helper(root, null, null);
}

// ----------------------------
// Tree printer (sideways)
// ----------------------------

function printTree(root) {
    console.log("Tree (sideways, right is up):");
    printTreeImpl(root, 0);
    console.log();
}

const INDENT = 4;

function printTreeImpl(node, indent) {
    if (node === null) return;

    printTreeImpl(node.right, indent + INDENT);

    console.log(" ".repeat(indent) + node.val);

    printTreeImpl(node.left, indent + INDENT);
}

// ----------------------------
// Test utility
// ----------------------------

function testTree(label, root) {
    console.log(`==== ${label} ====`);
    printTree(root);
    console.log("isValidBST:", isValidBST(root));
    console.log();
}

// ----------------------------
// Build test trees
// ----------------------------

// 1. Empty
const empty = null;

// 2. Single node
const single = new TreeNode(10);

// 3. Valid BST:
//
//        4
//      /   \
//     2     6
//    / \   / \
//   1  3  5  7
const valid = new TreeNode(4,
    new TreeNode(2,
        new TreeNode(1),
        new TreeNode(3)
    ),
    new TreeNode(6,
        new TreeNode(5),
        new TreeNode(7)
    )
);

// 4. Invalid BST (local violation):
//
//      5
//     / \
//    1   4   <-- 4 < 5 but placed in right subtree of 5
//       / \
//      3   6
const invalidLocal = new TreeNode(5,
    new TreeNode(1),
    new TreeNode(4,
        new TreeNode(3),
        new TreeNode(6)
    )
);

// 5. Invalid BST (deep violation):
//
//        5
//      /   \
//     2     8
//      \
//       6   <-- 6 > 5 but nested inside left subtree
const invalidDeep = new TreeNode(5,
    new TreeNode(2,
        null,
        new TreeNode(6)
    ),
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
