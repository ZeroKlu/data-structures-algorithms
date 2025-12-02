#nullable enable

/* ----------------------------
   Tree Printer (sideways)
   ---------------------------- */

const int INDENT = 4;

void PrintTree(TreeNode? root)
{
    Console.WriteLine("Tree (sideways, right is up):");
    PrintTreeImpl(root, 0);
    Console.WriteLine();
}

void PrintTreeImpl(TreeNode? node, int indent)
{
    if (node == null) return;

    PrintTreeImpl(node.Right, indent + INDENT);

    Console.WriteLine($"{new string(' ', indent)}{node.Val}");

    PrintTreeImpl(node.Left, indent + INDENT);
}

/* ----------------------------
   Round-trip test
   ---------------------------- */

void RoundTrip(string label, Codec codec, TreeNode? root)
{
    Console.WriteLine($"==== {label} ====");
    PrintTree(root);

    string s1 = codec.Serialize(root);
    Console.WriteLine($"Serialized:     {s1}");

    TreeNode? copy = codec.Deserialize(s1);
    Console.WriteLine("Deserialized:");
    PrintTree(copy);

    string s2 = codec.Serialize(copy);
    Console.WriteLine($"Re-serialized:  {s2}");

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
TreeNode single = new TreeNode(42);

// 3) Larger example tree:
//
//         1
//       /   \
//      2     3
//     / \   /
//    4   5 6
//
TreeNode root = new TreeNode(1)
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

Codec codec = new Codec();

RoundTrip("Empty tree", codec, empty);
RoundTrip("Single-node tree", codec, single);
RoundTrip("Larger example tree", codec, root);

class TreeNode(int v)
{
    public int Val = v;
    public TreeNode? Left;
    public TreeNode? Right;
}

class Codec {
    public string Serialize(TreeNode? root) {
        var sb = new StringBuilder();
        void Dfs(TreeNode? node) {
            if (node == null) {
                sb.Append("#,");
                return;
            }
            sb.Append(node.Val);
            sb.Append(',');
            Dfs(node.Left);
            Dfs(node.Right);
        }
        Dfs(root);
        return sb.ToString();
    }

    public TreeNode? Deserialize(string data) {
        var tokens = new Queue<string>(data.Split(',', StringSplitOptions.RemoveEmptyEntries));
        TreeNode? Dfs() {
            if (tokens.Count == 0) return null;
            string t = tokens.Dequeue();
            if (t == "#") return null;
            int val = int.Parse(t);
            var node = new TreeNode(val);
            node.Left = Dfs();
            node.Right = Dfs();
            return node;
        }
        return Dfs();
    }
}
