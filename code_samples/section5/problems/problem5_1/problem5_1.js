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
// TREE TRAVERSALS (DFS)
// ==========================

// Preorder traversal
// Order: Root -> Left -> Right
function preorder(root) {
    const out = []; // Collects traversal result

    // Recursive DFS helper
    function dfs(node) {
        // Base case: empty subtree
        if (node === null) return;

        // Visit current node
        out.push(node.val);

        // Traverse left subtree
        dfs(node.left);

        // Traverse right subtree
        dfs(node.right);
    }

    // Start traversal from the root
    dfs(root);

    // Return traversal result
    return out;
}

// Inorder traversal
// Order: Left -> Root -> Right
function inorder(root) {
    const out = []; // Collects traversal result

    // Recursive DFS helper
    function dfs(node) {
        // Base case: empty subtree
        if (node === null) return;

        // Traverse left subtree
        dfs(node.left);

        // Visit current node
        out.push(node.val);

        // Traverse right subtree
        dfs(node.right);
    }

    // Start traversal from the root
    dfs(root);

    // Return traversal result
    return out;
}

// Postorder traversal
// Order: Left -> Right -> Root
function postorder(root) {
    const out = []; // Collects traversal result

    // Recursive DFS helper
    function dfs(node) {
        // Base case: empty subtree
        if (node === null) return;

        // Traverse left subtree
        dfs(node.left);

        // Traverse right subtree
        dfs(node.right);

        // Visit current node last
        out.push(node.val);
    }

    // Start traversal from the root
    dfs(root);

    // Return traversal result
    return out;
}

// ----------------------------
// Tree Printer (sideways view)
// ----------------------------

// Prints the tree sideways for easy visualization
// - Right subtree appears on top
// - Left subtree appears on bottom
function printTree(root) {
    console.log("Tree (sideways):");
    printTreeImpl(root, 0);
    console.log("");
}

// Number of spaces to indent per tree level
const INDENT = 4;

// Recursive helper to print the tree sideways
function printTreeImpl(node, indent) {
    // Base case: nothing to print
    if (!node) return;

    // Print right subtree first (appears above)
    printTreeImpl(node.right, indent + INDENT);

    // Print current node with indentation
    console.log(" ".repeat(indent) + node.val);

    // Print left subtree last (appears below)
    printTreeImpl(node.left, indent + INDENT);
}

// ----------------------------
// Test Harness
// ----------------------------

// Build the test tree:
//
//         1
//       /   \
//      2     3
//     / \   /
//    4   5 6
//
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

// Print the tree visually
printTree(root);

// Run traversals
const pre = preorder(root);
const inn = inorder(root);
const post = postorder(root);

// Output traversal results
console.log("Preorder: ", pre);
console.log("Inorder:  ", inn);
console.log("Postorder:", post);
