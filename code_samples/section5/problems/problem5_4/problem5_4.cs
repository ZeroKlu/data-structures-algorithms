#nullable enable  // Enable nullable reference types (TreeNode? can be null)

// ==========================
// HAS PATH SUM (ROOT-TO-LEAF)
// ==========================

// Returns true if there exists a ROOT-to-LEAF path where the sum of node values
// along that path equals targetSum.
//
// Important: the path must end at a leaf node (a node with no children).
static bool HasPathSum(TreeNode? root, int targetSum)
{
    // Base case: empty subtree has no path
    if (root == null) return false;

    // If this is a leaf node, check if its value matches the remaining target sum
    if (root.Left == null && root.Right == null)
    {
        return root.Val == targetSum;
    }

    // Otherwise, subtract this node's value and continue searching
    int remaining = targetSum - root.Val;

    // If either left or right subtree contains a valid path, return true
    return HasPathSum(root.Left, remaining) ||
           HasPathSum(root.Right, remaining);
}

// ----------------------------
// Tree printer (sideways)
// ----------------------------

// Prints the tree sideways for visualization:
// - Right subtree appears above ("up")
// - Left subtree appears below ("down")
void PrintTree(TreeNode? root)
{
    Console.WriteLine("Tree (sideways, right is up):");
    PrintTreeImpl(root, 0);
    Console.WriteLine();
}

// Number of spaces to indent per tree level
const int INDENT = 4;

// Recursive helper for sideways printing
void PrintTreeImpl(TreeNode? node, int indent)
{
    // Base case: nothing to print
    if (node == null) return;

    // Print right subtree first so it appears "up"
    PrintTreeImpl(node.Right, indent + INDENT);

    // Print current node with indentation based on depth
    Console.WriteLine(new string(' ', indent) + node.Val);

    // Print left subtree last so it appears "down"
    PrintTreeImpl(node.Left, indent + INDENT);
}

// ----------------------------
// Test utility
// ----------------------------

// Runs a labeled test: prints the tree, target sum, and result
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

// 1) Empty tree (always false for any target)
TreeNode? empty = null;

// 2) Single node tree
TreeNode single = new TreeNode(7);

// 3) Large test tree:
//
//         5
//       /   \
//      4     8
//     /     / \
//    11    13  4
//   /  \        \
//  7    2        1
TreeNode t = new(5)
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

// Root-to-leaf path sums in 't':
// 5 + 4 + 11 + 7 = 27
// 5 + 4 + 11 + 2 = 22
// 5 + 8 + 13     = 26   (13 is a leaf in this tree)
// 5 + 8 + 4 + 1  = 18
TestCase("Large tree (has path sum 22 → true)", t, 22);
TestCase("Large tree (has path sum 26 → true)", t, 26);
TestCase("Large tree (has path sum 18 → true)", t, 18);

// This is false because 5 is not a leaf; path sums must end at a leaf node.
TestCase("Large tree (no path sum == 5 → false)", t, 5);

// ==========================
// TREE NODE DEFINITION
// ==========================

class TreeNode(int v)
{
    // Value stored in this node
    public int Val = v;

    // Child references (nullable because children may not exist)
    public TreeNode? Left;
    public TreeNode? Right;
}
