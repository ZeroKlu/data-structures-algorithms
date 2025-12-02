class TreeNode {
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

function preorder(root) {
    const out = [];
    function dfs(node) {
        if (node === null) return;
        out.push(node.val);
        dfs(node.left);
        dfs(node.right);
    }
    dfs(root);
    return out;
}

function inorder(root) {
    const out = [];
    function dfs(node) {
        if (node === null) return;
        dfs(node.left);
        out.push(node.val);
        dfs(node.right);
    }
    dfs(root);
    return out;
}

function postorder(root) {
    const out = [];
    function dfs(node) {
        if (node === null) return;
        dfs(node.left);
        dfs(node.right);
        out.push(node.val);
    }
    dfs(root);
    return out;
}

// ----------------------------
// Tree Printer (sideways view)
// ----------------------------

function printTree(root) {
    console.log("Tree (sideways):");
    printTreeImpl(root, 0);
    console.log("");
}

const INDENT = 4;

function printTreeImpl(node, indent) {
    if (!node) return;

    printTreeImpl(node.right, indent + INDENT);

    console.log(" ".repeat(indent) + node.val);

    printTreeImpl(node.left, indent + INDENT);
}

// ----------------------------
// Test Harness
// ----------------------------

// Build the same test tree:
//         1
//       /   \
//      2     3
//     / \   /
//    4   5 6

const root = new TreeNode(
    1,
    new TreeNode(
        2,
        new TreeNode(4),
        new TreeNode(5)
    ),
    new TreeNode(
        3,
        new TreeNode(6),
        null
    )
);

// Print tree visually
printTree(root);

// Run traversals
const pre = preorder(root);
const inn = inorder(root);
const post = postorder(root);

// Output results
console.log("Preorder: ", pre);
console.log("Inorder:  ", inn);
console.log("Postorder:", post);
