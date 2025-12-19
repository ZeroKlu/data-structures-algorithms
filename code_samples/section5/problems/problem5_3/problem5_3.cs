#nullable enable  // Enable nullable reference types (TreeNode? indicates it may be null)

// ==========================
// VALIDATE BST (RANGE METHOD)
// ==========================

// Returns true if the binary tree is a valid Binary Search Tree (BST).
// BST rule (strict):
// - All values in the left subtree are < node.Val
// - All values in the right subtree are > node.Val
//
// This implementation uses a "range" approach:
// each node must fall within an allowable (low, high) bound inherited from its ancestors.
static bool IsValidBST(TreeNode? root)
{
    // Helper validates the subtree using optional bounds:
    // - low  : lower bound that node.Val must be greater than (exclusive)
    // - high : upper bound that node.Val must be less than (exclusive)
    //
    // Using int? allows "no bound" (null) at the start.
    static bool Helper(TreeNode? node, int? low, int? high)
    {
        // Base case: empty subtree is valid
        if (node == null) return true;

        // If we have a lower bound, current value must be strictly greater than it
        if (low.HasValue && node.Val <= low.Value) return false;

        // If we have an upper bound, current value must be strictly less than it
        if (high.HasValue && node.Val >= high.Value) return false;

        // Left subtree must be within (low, node.Val)
        // Right subtree must be within (node.Val, high)
        return Helper(node.Left, low, node.Val) &&
               Helper(node.Right, node.Val, high);
    }

    // Start with no bounds at the root
    return Helper(root, null, null);
}

// ----------------------------
// Tree printer (sideways)
// ----------------------------

// Prints the tree sideways for visualization:
// - Right subtree appears above the node
// - Left subtree appears below the node
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

    // Print right subtree first (appears "up")
    PrintTreeImpl(node.Right, indent + INDENT);

    // Print current node with indentation based on depth
    Console.WriteLine(new string(' ', indent) + node.Val);

    // Print left subtree last (appears "down")
    PrintTreeImpl(node.Left, indent + INDENT);
}

// ----------------------------
// Test utility
// ----------------------------

// Runs a labeled test: prints the tree and whether it is a valid BST
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

// 1) Empty tree (should be true)
TreeNode? empty = null;

// 2) Single node (should be true)
TreeNode single = new(10);

// 3) Valid BST:
//
//        4
//      /   \
//     2     6
//    / \   / \
//   1  3  5  7
TreeNode valid = new(4)
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

// 4) Invalid BST (local violation):
//
//      5
//     / \
//    1   4   <-- 4 is in the right subtree of 5 but 4 < 5 (invalid)
//       / \
//      3   6
TreeNode invalidLocal = new(5)
{
    Left = new TreeNode(1),
    Right = new TreeNode(4)
    {
        Left = new TreeNode(3),
        Right = new TreeNode(6)
    }
};

// 5) Invalid BST (deep violation):
//
//        5
//      /   \
//     2     8
//      \
//       6   <-- 6 is in the LEFT subtree of 5 but 6 > 5 (invalid)
TreeNode invalidDeep = new(5)
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
