#nullable enable  // Enable nullable reference types (TreeNode? indicates it may be null)

// ==========================
// HEIGHT (O(n))
// ==========================

// Computes the height of a binary tree.
// Convention used here:
// - Empty tree height = -1
// - Single node height = 0
int Height(TreeNode? root)
{
    // Base case: empty subtree
    if (root == null) return -1;

    // Height is 1 + max(height(left), height(right))
    return 1 + Math.Max(Height(root.Left), Height(root.Right));
}

// ==========================
// BALANCE CHECK (O(n) SINGLE PASS)
// ==========================

// Returns two pieces of information at once:
// - balanced: whether the subtree is height-balanced
// - height:   the height of the subtree
//
// This avoids recomputing heights repeatedly (unlike a naive approach),
// because each node's height is computed once as recursion unwinds.
(bool balanced, int height) Check(TreeNode? node)
{
    // Base case: empty subtree is balanced and has height -1
    if (node == null) return (true, -1);

    // Recursively check left subtree
    var left = Check(node.Left);

    // Early exit: if left subtree is already unbalanced, stop
    if (!left.balanced) return (false, 0);

    // Recursively check right subtree
    var right = Check(node.Right);

    // Early exit: if right subtree is already unbalanced, stop
    if (!right.balanced) return (false, 0);

    // Current node is balanced if children heights differ by at most 1
    bool bal = Math.Abs(left.height - right.height) <= 1;

    // Height of current subtree is 1 + max(left height, right height)
    int h = 1 + Math.Max(left.height, right.height);

    // Return both balance info and height
    return (bal, h);
}

// Public wrapper: returns true if the tree is height-balanced
bool IsBalanced(TreeNode? root)
{
    return Check(root).balanced;
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

    // Print right subtree first (appears "up" in sideways view)
    PrintTreeImpl(node.Right, indent + INDENT);

    // Print current node with indentation based on depth
    Console.WriteLine(new string(' ', indent) + node.Val);

    // Print left subtree last (appears "down" in sideways view)
    PrintTreeImpl(node.Left, indent + INDENT);
}

// ----------------------------
// Test utility
// ----------------------------

// Prints a label, the tree, its height, and whether it is balanced
void TestTree(string label, TreeNode? root)
{
    Console.WriteLine($"==== {label} ====");
    PrintTree(root);

    // Height computed independently (O(n))
    Console.WriteLine("Height: " + Height(root));

    // Balance computed using Check() (O(n) single pass)
    Console.WriteLine("IsBalanced: " + IsBalanced(root));
    Console.WriteLine();
}

// ----------------------------
// Build test trees
// ----------------------------

// 1) Empty tree
TreeNode? empty = null;

// 2) Single node tree
TreeNode single = new TreeNode(1);

// 3) Perfectly balanced tree:
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

// 4) Slightly unbalanced but still balanced (height diff <= 1 everywhere):
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

// 5) Deep unbalanced tree (should be false):
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
