#nullable enable

static TreeNode? LowestCommonAncestor(TreeNode? root, TreeNode p, TreeNode q) {
    TreeNode? node = root;
    int pv = p.Val;
    int qv = q.Val;
    while (node != null) {
        if (pv < node.Val && qv < node.Val) {
            node = node.Left;
        } else if (pv > node.Val && qv > node.Val) {
            node = node.Right;
        } else {
            return node;
        }
    }
    return null;
}

// ----------------------------
// Tree printer (sideways)
// ----------------------------

void PrintTree(TreeNode? root)
{
    Console.WriteLine("Tree (sideways, right is up):");
    PrintTreeImpl(root, 0);
    Console.WriteLine();
}

const int INDENT = 4;

void PrintTreeImpl(TreeNode? node, int indent)
{
    if (node == null) return;

    PrintTreeImpl(node.Right, indent + INDENT);

    Console.WriteLine(new string(' ', indent) + node.Val);

    PrintTreeImpl(node.Left, indent + INDENT);
}

// ----------------------------
// Test utility
// ----------------------------

void TestLCA(string label, TreeNode? root, TreeNode p, TreeNode q)
{
    Console.WriteLine($"==== {label} ====");
    Console.WriteLine($"Find LCA({p.Val}, {q.Val})");
    var ans = LowestCommonAncestor(root, p, q);
    Console.WriteLine("LCA = " + (ans?.Val.ToString() ?? "null"));
    Console.WriteLine();
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

TreeNode root = new TreeNode(6)
{
    Left = new TreeNode(2)
    {
        Left = new TreeNode(0),
        Right = new TreeNode(4)
        {
            Left = new TreeNode(3),
            Right = new TreeNode(5)
        }
    },
    Right = new TreeNode(8)
    {
        Left = new TreeNode(7),
        Right = new TreeNode(9)
    }
};

PrintTree(root);

// references
TreeNode n2 = root.Left!;
TreeNode n8 = root.Right!;
TreeNode n4 = root.Left!.Right!;
TreeNode n3 = n4.Left!;
TreeNode n5 = n4.Right!;
TreeNode n0 = root.Left!.Left!;
TreeNode n7 = root.Right!.Left!;
TreeNode n9 = root.Right!.Right!;

// ----------------------------
// Run test cases
// ----------------------------

TestLCA("LCA(2, 8) → expected 6", root, n2, n8);
TestLCA("LCA(2, 4) → expected 2", root, n2, n4);
TestLCA("LCA(3, 5) → expected 4", root, n3, n5);
TestLCA("LCA(0, 5) → expected 2", root, n0, n5);
TestLCA("LCA(7, 9) → expected 8", root, n7, n9);

class TreeNode(int v)
{
    public int Val = v;
    public TreeNode? Left;
    public TreeNode? Right;
}
