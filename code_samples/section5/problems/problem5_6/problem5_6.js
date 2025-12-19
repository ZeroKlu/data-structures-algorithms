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
// CODEC: SERIALIZE / DESERIALIZE
// ==========================
//
// This codec uses PREORDER traversal (node, left, right)
// and "#" as a null marker to preserve tree structure.

class Codec {

    // Serializes a binary tree into a string.
    //
    // Format:
    // - Each value is written as a string
    // - Null pointers are written as "#"
    // - Tokens are joined with commas
    //
    // Example (single node 1):
    //   "1,#,#"
    serialize(root) {
        const out = [];

        // DFS helper for preorder traversal
        function dfs(node) {
            // Null node → write marker
            if (node === null) {
                out.push("#");
                return;
            }

            // Write node value
            out.push(String(node.val));

            // Serialize left subtree then right subtree
            dfs(node.left);
            dfs(node.right);
        }

        // Start traversal
        dfs(root);

        // Join tokens into a single string
        return out.join(",");
    }

    // Deserializes a string produced by serialize()
    // back into the original binary tree.
    //
    // Steps:
    // 1) Split the string by commas into tokens
    // 2) Rebuild the tree using preorder recursion:
    //    - "#" => null
    //    - number => create node, then build left, then right
    deserialize(data) {
        const tokens = data.split(",");
        let idx = 0;

        // DFS helper that consumes tokens in order
        function dfs() {
            // Safety check: no tokens left
            if (idx >= tokens.length) return null;

            const t = tokens[idx++];

            // Null marker (or empty token)
            if (t === "#" || t === "") return null;

            // Parse integer value and create node
            const node = new TreeNode(parseInt(t, 10));

            // Rebuild left subtree then right subtree
            node.left = dfs();
            node.right = dfs();

            return node;
        }

        // Start rebuild
        return dfs();
    }
}

// ----------------------------
// Tree printer (sideways)
// ----------------------------

// Number of spaces to indent per tree level
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
// Round-trip test utility
// ----------------------------

// Performs a round-trip test:
// 1) serialize original tree
// 2) deserialize into a new tree
// 3) serialize again
// 4) compare serialized strings
function roundTrip(label, codec, root) {
    console.log(`==== ${label} ====`);
    printTree(root);

    // Serialize original tree
    const serialized = codec.serialize(root);
    console.log("Serialized:     ", serialized);

    // Deserialize into a new tree
    const copy = codec.deserialize(serialized);
    console.log("Deserialized:");
    printTree(copy);

    // Serialize the reconstructed tree
    const reSerialized = codec.serialize(copy);
    console.log("Re-serialized: ", reSerialized);

    // Compare results
    if (serialized === reSerialized) {
        console.log("Round-trip OK (strings match)\n");
    } else {
        console.log("Round-trip MISMATCH!\n");
    }
}

// ----------------------------
// Build test trees
// ----------------------------

// 1) Empty tree
const empty = null;

// 2) Single-node tree
const single = new TreeNode(42);

// 3) Larger example tree:
//
//         1
//       /   \
//      2     3
//     / \   /
//    4   5 6
//
const root = new TreeNode(
    1,
    new TreeNode(2, new TreeNode(4), new TreeNode(5)),
    new TreeNode(3, new TreeNode(6), null)
);

// ----------------------------
// Run tests
// ----------------------------

const codec = new Codec();

roundTrip("Empty tree", codec, empty);
roundTrip("Single-node tree", codec, single);
roundTrip("Larger example tree", codec, root);
