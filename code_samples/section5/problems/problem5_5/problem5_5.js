class TreeNode {
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

function lowestCommonAncestor(root, p, q) {
    let node = root;
    const pv = p.val;
    const qv = q.val;
    while (node !== null) {
        if (pv < node.val && qv < node.val) {
            node = node.left;
        } else if (pv > node.val && qv > node.val) {
            node = node.right;
        } else {
            return node;
        }
    }
    return null;
}

// ----------------------------
// Tree printer (sideways)
// ----------------------------

const INDENT = 4;

function printTree(root) {
    console.log("Tree (sideways, right is up):");
    printTreeImpl(root, 0);
    console.log();
}

function printTreeImpl(node, indent) {
    if (node === null) return;

    printTreeImpl(node.right, indent + INDENT);

    console.log(" ".repeat(indent) + node.val);

    printTreeImpl(node.left, indent + INDENT);
}

// ----------------------------
// Test utility
// ----------------------------

function testLCA(label, root, p, q) {
    console.log(`==== ${label} ====`);
    console.log(`Find LCA(${p.val}, ${q.val})`);
    const ans = lowestCommonAncestor(root, p, q);
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

const root = new TreeNode(6);
root.left = new TreeNode(2);
root.right = new TreeNode(8);

root.left.left = new TreeNode(0);
root.left.right = new TreeNode(4);
root.left.right.left = new TreeNode(3);
root.left.right.right = new TreeNode(5);

root.right.left = new TreeNode(7);
root.right.right = new TreeNode(9);

// References for convenience
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

printTree(root);

testLCA("LCA(2, 8) → expected 6", root, n2, n8);
testLCA("LCA(2, 4) → expected 2", root, n2, n4);
testLCA("LCA(3, 5) → expected 4", root, n3, n5);
testLCA("LCA(0, 5) → expected 2", root, n0, n5);
testLCA("LCA(7, 9) → expected 8", root, n7, n9);
