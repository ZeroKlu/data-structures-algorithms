class TreeNode {
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

function height(root) {
    if (root === null) return -1;
    return 1 + Math.max(height(root.left), height(root.right));
}

function isBalanced(root) {
    function check(node) {
        if (node === null) return { balanced: true, height: -1 };
        const left = check(node.left);
        if (!left.balanced) return { balanced: false, height: 0 };
        const right = check(node.right);
        if (!right.balanced) return { balanced: false, height: 0 };
        const diff = Math.abs(left.height - right.height);
        const bal = diff <= 1;
        return { balanced: bal, height: 1 + Math.max(left.height, right.height) };
    }
    return check(root).balanced;
}

// ----------------------------
// Tree printer (sideways)
// ----------------------------

function printTree(root) {
    console.log("Tree (sideways, right is up):");
    printTreeImpl(root, 0);
    console.log("");
}

const INDENT = 4;

function printTreeImpl(node, indent) {
    if (node === null) return;

    printTreeImpl(node.right, indent + INDENT);

    console.log(" ".repeat(indent) + node.val);

    printTreeImpl(node.left, indent + INDENT);
}

// ----------------------------
// Test helper
// ----------------------------

function testTree(label, root) {
    console.log(`==== ${label} ====`);
    printTree(root);
    console.log("height:", height(root));
    console.log("isBalanced:", isBalanced(root));
    console.log("");
}

// ----------------------------
// Build test trees
// ----------------------------

// 1. Empty tree
const empty = null;

// 2. Single node
const single = new TreeNode(1);

// 3. Perfectly balanced tree:
//        1
//      /   \
//     2     3
//    / \   / \
//   4  5  6   7
const balanced = new TreeNode(1,
    new TreeNode(2,
        new TreeNode(4),
        new TreeNode(5)
    ),
    new TreeNode(3,
        new TreeNode(6),
        new TreeNode(7)
    )
);

// 4. Slightly unbalanced but still balanced:
//      1
//    /   \
//   2     3
//  /
// 4
const shallow = new TreeNode(1,
    new TreeNode(2,
        new TreeNode(4),
        null
    ),
    new TreeNode(3)
);

// 5. Deep unbalanced tree (should be false):
//      1
//     /
//    2
//   /
//  3
//   \
//    4
const deep = new TreeNode(1,
    new TreeNode(2,
        new TreeNode(3,
            null,
            new TreeNode(4)
        ),
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
