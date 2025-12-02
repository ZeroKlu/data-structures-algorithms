#nullable enable

static bool IsValidBST(TreeNode? root) {
    static bool Helper(TreeNode? node, int? low, int? high) {
        if (node == null) return true;
        if (low.HasValue && node.Val <= low.Value) return false;
        if (high.HasValue && node.Val >= high.Value) return false;
        return Helper(node.Left, low, node.Val) &&
               Helper(node.Right, node.Val, high);
    }
    return Helper(root, null, null);
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
    Console.WriteLine("IsValidBST: " + IsValidBST(root));
    Console.WriteLine();
}

// ----------------------------
// Build test trees
// ----------------------------

// 1. Empty tree (should be true)
TreeNode? empty = null;

// 2. Single node (should be true)
TreeNode single = new TreeNode(10);

// 3. Valid BST:
//
//        4
//      /   \
//     2     6
//    / \   / \
//   1  3  5  7
TreeNode valid = new TreeNode(4)
{
    Left = new TreeNode(2)
    {
        Left = new TreeNode(1),
        Right = new TreeNode(3)
    },
    Right = new TreeNode(6)
    {
        Left = new TreeNode(5),
        Right = new TreeNode(7)
    }
};

// 4. Invalid BST (local violation):
//
//      5
//     / \
//    1   4   <-- 4 < 5 but in right subtree of 5
//       / \
//      3   6
TreeNode invalidLocal = new TreeNode(5)
{
    Left = new TreeNode(1),
    Right = new TreeNode(4)
    {
        Left = new TreeNode(3),
        Right = new TreeNode(6)
    }
};

// 5. Invalid BST (deep violation):
//
//        5
//      /   \
//     2     8
//      \
//       6   <-- 6 > 5 but inside left subtree of 5
TreeNode invalidDeep = new TreeNode(5)
{
    Left = new TreeNode(2)
    {
        Right = new TreeNode(6)
    },
    Right = new TreeNode(8)
};

// ----------------------------
// Run tests
// ----------------------------

TestTree("Empty tree (should be true)", empty);
TestTree("Single node (should be true)", single);
TestTree("Valid BST (should be true)", valid);
TestTree("Invalid BST - local violation (should be false)", invalidLocal);
TestTree("Invalid BST - deep violation (should be false)", invalidDeep);

class TreeNode(int v)
{
    public int Val = v;
    public TreeNode? Left;
    public TreeNode? Right;
}
