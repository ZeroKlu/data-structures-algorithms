#nullable enable

int Height(TreeNode? root) {
    if (root == null) return -1;
    return 1 + Math.Max(Height(root.Left), Height(root.Right));
}

(bool balanced, int height) Check(TreeNode? node) {
    if (node == null) return (true, -1);
    var left = Check(node.Left);
    if (!left.balanced) return (false, 0);
    var right = Check(node.Right);
    if (!right.balanced) return (false, 0);
    bool bal = Math.Abs(left.height - right.height) <= 1;
    int h = 1 + Math.Max(left.height, right.height);
    return (bal, h);
}

bool IsBalanced(TreeNode? root) {
    return Check(root).balanced;
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

void TestTree(string label, TreeNode? root)
{
    Console.WriteLine($"==== {label} ====");
    PrintTree(root);
    Console.WriteLine("Height: " + Height(root));
    Console.WriteLine("IsBalanced: " + IsBalanced(root));
    Console.WriteLine();
}

// ----------------------------
// Build test trees
// ----------------------------

// 1. Empty tree
TreeNode? empty = null;

// 2. Single node
TreeNode single = new TreeNode(1);

// 3. Perfectly balanced tree:
//
//        1
//      /   \
//     2     3
//    / \   / \
//   4  5  6   7
TreeNode balanced = new TreeNode(1)
{
    Left = new TreeNode(2)
    {
        Left = new TreeNode(4),
        Right = new TreeNode(5)
    },
    Right = new TreeNode(3)
    {
        Left = new TreeNode(6),
        Right = new TreeNode(7)
    }
};

// 4. Slightly unbalanced but still balanced:
//
//      1
//    /   \
//   2     3
//  /
// 4
TreeNode shallow = new TreeNode(1)
{
    Left = new TreeNode(2)
    {
        Left = new TreeNode(4)
    },
    Right = new TreeNode(3)
};

// 5. Deep unbalanced tree (should be false):
//
//      1
//     /
//    2
//   /
//  3
//   \
//    4
TreeNode deep = new TreeNode(1)
{
    Left = new TreeNode(2)
    {
        Left = new TreeNode(3)
        {
            Right = new TreeNode(4)
        }
    }
};

// ----------------------------
// Run tests
// ----------------------------

TestTree("Empty tree", empty);
TestTree("Single node", single);
TestTree("Perfectly balanced tree", balanced);
TestTree("Shallow unbalanced but balanced", shallow);
TestTree("Deep unbalanced (should be false)", deep);

class TreeNode(int v)
{
    public int Val = v;
    public TreeNode? Left;
    public TreeNode? Right;
}
