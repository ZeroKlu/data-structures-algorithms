#nullable enable  // Enable nullable reference types (TreeNode? can be null)

// ==========================
// LOWEST COMMON ANCESTOR (BST)
// ==========================

// Returns the Lowest Common Ancestor (LCA) of nodes p and q in a Binary Search Tree (BST).
//
// BST property used:
// - All values in the left subtree  < node.Val
// - All values in the right subtree > node.Val
//
// Key idea:
// - If BOTH p and q are < current node, the LCA must be in the left subtree.
// - If BOTH p and q are > current node, the LCA must be in the right subtree.
// - Otherwise, current node is the "split point" (or equals p/q) and is the LCA.
//
// This implementation is iterative (no recursion).
static TreeNode? LowestCommonAncestor(TreeNode? root, TreeNode p, TreeNode q)
{
    // Start walking down the tree from the root
    TreeNode? node = root;

    // Cache values for comparisons (avoid repeated property access)
    int pv = p.Val;
    int qv = q.Val;

    // Keep moving down until we find the split point
    while (node != null)
    {
        // If both p and q are smaller, LCA is on the left side
        if (pv < node.Val && qv < node.Val)
        {
            node = node.Left;
        }
        // If both p and q are larger, LCA is on the right side
        else if (pv > node.Val && qv > node.Val)
        {
            node = node.Right;
        }
        // Otherwise we've split (or node equals p/q) => this node is the LCA
        else
        {
            return node;
        }
    }

    // If root is null (or nodes aren't in the tree), no LCA found
    return null;
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

// Number of spaces to indent per level
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

// Runs a labeled LCA test: prints inputs and the returned LCA value
void TestLCA(string label, TreeNode? root, TreeNode p, TreeNode q)
{
    Console.WriteLine($"==== {label} ====");
    Console.WriteLine($"Find LCA({p.Val}, {q.Val})");

    // Compute the LCA using BST logic
    var ans = LowestCommonAncestor(root, p, q);

    // Print result (ans may be null)
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
TreeNode root = new(6)
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

// Print tree for clarity
PrintTree(root);

// References (pointers) to specific nodes inside the tree.
// The '!' tells the compiler we know these aren't null here.
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
