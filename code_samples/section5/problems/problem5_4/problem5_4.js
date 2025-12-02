class TreeNode {
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

function hasPathSum(root, targetSum) {
    if (root === null) return false;
    if (root.left === null && root.right === null) {
        return root.val === targetSum;
    }
    const remaining = targetSum - root.val;
    return hasPathSum(root.left, remaining) ||
           hasPathSum(root.right, remaining);
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

// 1. Empty tree
const empty = null;

// 2. Single node
const single = new TreeNode(7);

// 3. Large test tree:
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

testCase("Empty tree", empty, 10);
testCase("Single node (7 == 7 → true)", single, 7);
testCase("Single node (7 != 10 → false)", single, 10);

testCase("Large tree (has path sum 22 → true)", t, 22); // 5+4+11+2
testCase("Large tree (has path sum 26 → true)", t, 26); // 5+8+13
testCase("Large tree (has path sum 18 → true)", t, 18); // 5+8+4+1
testCase("Large tree (no path sum == 5 → false)", t, 5); // root not leaf
