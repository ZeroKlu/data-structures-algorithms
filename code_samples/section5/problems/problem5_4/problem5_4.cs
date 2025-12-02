#nullable enable

static bool HasPathSum(TreeNode? root, int targetSum) {
    if (root == null) return false;
    if (root.Left == null && root.Right == null) {
        return root.Val == targetSum;
    }
    int remaining = targetSum - root.Val;
    return HasPathSum(root.Left, remaining) ||
           HasPathSum(root.Right, remaining);
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

void TestCase(string label, TreeNode? root, int target)
{
    Console.WriteLine($"==== {label} ====");
    PrintTree(root);
    Console.WriteLine($"Target sum: {target}");
    Console.WriteLine("HasPathSum: " + HasPathSum(root, target));
    Console.WriteLine();
}

// ----------------------------
// Build test trees
// ----------------------------

// 1. Empty tree
TreeNode? empty = null;

// 2. Single node
TreeNode single = new TreeNode(7);

// 3. Large test tree:
//
//         5
//       /   \
//      4     8
//     /     / \
//    11    13  4
//   /  \        \
//  7    2        1
TreeNode t = new TreeNode(5)
{
    Left = new TreeNode(4)
    {
        Left = new TreeNode(11)
        {
            Left = new TreeNode(7),
            Right = new TreeNode(2)
        }
    },
    Right = new TreeNode(8)
    {
        Left = new TreeNode(13),
        Right = new TreeNode(4)
        {
            Right = new TreeNode(1)
        }
    }
};

// ----------------------------
// Run test cases
// ----------------------------

TestCase("Empty tree", empty, 10);
TestCase("Single node (7 == 7 → true)", single, 7);
TestCase("Single node (7 != 10 → false)", single, 10);

// Paths in t:
// 5+4+11+7 = 27
// 5+4+11+2 = 22
// 5+8+13   = 26
// 5+8+4+1  = 18
TestCase("Large tree (has path sum 22 → true)", t, 22);
TestCase("Large tree (has path sum 26 → true)", t, 26);
TestCase("Large tree (has path sum 18 → true)", t, 18);
TestCase("Large tree (no path sum == 5 → false)", t, 5); // 5 not a leaf

class TreeNode(int v)
{
    public int Val = v;
    public TreeNode? Left;
    public TreeNode? Right;
}
