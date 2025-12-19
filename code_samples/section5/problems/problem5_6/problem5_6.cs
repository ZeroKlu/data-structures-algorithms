#nullable enable

using System.Text;

/* ----------------------------
   Tree Printer (sideways)
   ---------------------------- */

// Number of spaces to indent per tree level in the sideways view
const int INDENT = 4;

// Prints a tree sideways for visualization:
// - Right subtree appears above ("up")
// - Left subtree appears below ("down")
void PrintTree(TreeNode? root)
{
    Console.WriteLine("Tree (sideways, right is up):");
    PrintTreeImpl(root, 0);
    Console.WriteLine();
}

// Recursive helper that prints the tree with indentation based on depth
void PrintTreeImpl(TreeNode? node, int indent)
{
    // Base case: nothing to print
    if (node == null) return;

    // Print right subtree first so it appears "up"
    PrintTreeImpl(node.Right, indent + INDENT);

    // Print current node value with indentation
    Console.WriteLine($"{new string(' ', indent)}{node.Val}");

    // Print left subtree last so it appears "down"
    PrintTreeImpl(node.Left, indent + INDENT);
}

/* ----------------------------
   Round-trip test
   ---------------------------- */

// RoundTrip does a "codec round-trip" test:
// 1) Serialize the tree
// 2) Deserialize back into a new tree
// 3) Serialize the new tree again
// 4) Compare the two serialized strings
//
// If the codec is correct (and deterministic), the strings should match.
void RoundTrip(string label, Codec codec, TreeNode? root)
{
    Console.WriteLine($"==== {label} ====");

    // Print original tree
    PrintTree(root);

    // Serialize original tree to a string
    string s1 = Codec.Serialize(root);
    Console.WriteLine($"Serialized:     {s1}");

    // Deserialize into a new tree instance
    TreeNode? copy = Codec.Deserialize(s1);

    // Print reconstructed tree
    Console.WriteLine("Deserialized:");
    PrintTree(copy);

    // Serialize reconstructed tree again
    string s2 = Codec.Serialize(copy);
    Console.WriteLine($"Re-serialized:  {s2}");

    // Compare strings to validate the round-trip
    if (s1 == s2)
        Console.WriteLine("Round-trip OK (strings match)\n");
    else
        Console.WriteLine("Round-trip MISMATCH!\n");
}

/* ----------------------------
   Build test trees
   ---------------------------- */

// 1) Empty tree
TreeNode? empty = null;

// 2) Single node tree
TreeNode single = new(42);

// 3) Larger example tree:
//
//         1
//       /   \
//      2     3
//     / \   /
//    4   5 6
//
TreeNode root = new(1)
{
    Left = new TreeNode(2)
    {
        Left = new TreeNode(4),
        Right = new TreeNode(5)
    },
    Right = new TreeNode(3)
    {
        Left = new TreeNode(6)
    }
};

/* ----------------------------
   Run tests
   ---------------------------- */

// Create codec instance (provides Serialize / Deserialize)
Codec codec = new();

// Run round-trip tests on all sample trees
RoundTrip("Empty tree", codec, empty);
RoundTrip("Single-node tree", codec, single);
RoundTrip("Larger example tree", codec, root);

// ==========================
// DATA STRUCTURES
// ==========================

// Basic binary tree node (nullable children allowed)
class TreeNode(int v)
{
    public int Val = v;         // value stored in this node
    public TreeNode? Left;      // left child (null if none)
    public TreeNode? Right;     // right child (null if none)
}

// ==========================
// CODEC: SERIALIZE / DESERIALIZE
// ==========================

class Codec
{
    // Serializes a binary tree to a string using preorder traversal
    // with null markers.
    //
    // Format:
    // - Each node is written as "value,"
    // - Null pointers are written as "#,"
    //
    // Example (single node 1):
    // "1,#,#,"
    public static string Serialize(TreeNode? root)
    {
        var sb = new StringBuilder();

        // Local DFS helper that appends preorder tokens into sb
        void Dfs(TreeNode? node)
        {
            // Null subtree => write marker
            if (node == null)
            {
                sb.Append("#,");
                return;
            }

            // Write node value followed by comma
            sb.Append(node.Val);
            sb.Append(',');

            // Serialize left then right
            Dfs(node.Left);
            Dfs(node.Right);
        }

        // Start serialization
        Dfs(root);

        return sb.ToString();
    }

    // Deserializes the string produced by Serialize back into a tree.
    //
    // Steps:
    // 1) Split by commas into tokens
    // 2) Rebuild using preorder recursion:
    //    - token "#" => null
    //    - number => create node, build left, then build right
    public static TreeNode? Deserialize(string data)
    {
        // Split into tokens; RemoveEmptyEntries drops the empty token caused
        // by the trailing comma.
        var tokens = new Queue<string>(
            data.Split(',', StringSplitOptions.RemoveEmptyEntries)
        );

        // Local DFS that consumes tokens in preorder order
        TreeNode? Dfs()
        {
            // Safety: no tokens left
            if (tokens.Count == 0) return null;

            // Get next token
            string t = tokens.Dequeue();

            // Null marker => return null pointer
            if (t == "#") return null;

            // Parse node value and create node
            int val = int.Parse(t);
            var node = new TreeNode(val)
            {
                // Rebuild left subtree then right subtree
                Left = Dfs(),
                Right = Dfs()
            };

            return node;
        }

        // Start rebuild from the first token
        return Dfs();
    }
}
