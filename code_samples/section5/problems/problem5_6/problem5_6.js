class TreeNode {
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

class Codec {
    serialize(root) {
        const out = [];
        function dfs(node) {
            if (node === null) {
                out.push("#");
                return;
            }
            out.push(String(node.val));
            dfs(node.left);
            dfs(node.right);
        }
        dfs(root);
        return out.join(",");
    }

    deserialize(data) {
        const tokens = data.split(",");
        let idx = 0;
        function dfs() {
            if (idx >= tokens.length) return null;
            const t = tokens[idx++];
            if (t === "#" || t === "") return null;
            const node = new TreeNode(parseInt(t, 10));
            node.left = dfs();
            node.right = dfs();
            return node;
        }
        return dfs();
    }
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
// Round-trip test utility
// ----------------------------

function roundTrip(label, codec, root) {
    console.log(`==== ${label} ====`);
    printTree(root);

    const serialized = codec.serialize(root);
    console.log("Serialized:     ", serialized);

    const copy = codec.deserialize(serialized);
    console.log("Deserialized:");
    printTree(copy);

    const reSerialized = codec.serialize(copy);
    console.log("Re-serialized: ", reSerialized);

    if (serialized === reSerialized) {
        console.log("Round-trip OK (strings match)\n");
    } else {
        console.log("Round-trip MISMATCH!\n");
    }
}

// ----------------------------
// Build test trees
// ----------------------------

// 1. Empty tree
const empty = null;

// 2. Single-node tree
const single = new TreeNode(42);

// 3. Larger example tree:
//
//         1
//       /   \
//      2     3
//     / \   /
//    4   5 6
//
const root = new TreeNode(1,
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
