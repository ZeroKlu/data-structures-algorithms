#nullable enable  // Enable nullable reference type analysis (TreeNode? means it may be null)

// ==========================
// INORDER TRAVERSAL (DFS)
// ==========================

// Returns the inorder traversal of a binary tree as a list of ints
// Order: Left -> Root -> Right
IList<int> Inorder(TreeNode? root)
{
    // This list will collect the traversal output
    var result = new List<int>();

    // Local recursive DFS function that performs the traversal
    void Dfs(TreeNode? node)
    {
        // Base case: empty subtree
        if (node == null) return;

        // Visit left subtree first
        Dfs(node.Left);

        // Visit current node
        result.Add(node.Val);

        // Visit right subtree
        Dfs(node.Right);
    }

    // Start DFS from the root
    Dfs(root);

    // Return the collected results
    return result;
}

// ==========================
// POSTORDER TRAVERSAL (DFS)
// ==========================

// Returns the postorder traversal of a binary tree as a list of ints
// Order: Left -> Right -> Root
IList<int> Postorder(TreeNode? root)
{
    // This list will collect the traversal output
    var result = new List<int>();

    // Local recursive DFS function that performs the traversal
    void Dfs(TreeNode? node)
    {
        // Base case: empty subtree
        if (node == null) return;

        // Visit left subtree
        Dfs(node.Left);

        // Visit right subtree
        Dfs(node.Right);

        // Visit current node last
        result.Add(node.Val);
    }

    // Start DFS from the root
    Dfs(root);

    // Return the collected results
    return result;
}

// ----------------------------
// Tree Printer (Sideways)
// ----------------------------

// Prints the tree "sideways" so structure is easy to see:
// - Right subtree appears on top
// - Left subtree appears on bottom
void PrintTree(TreeNode? root)
{
    Console.WriteLine("Tree (sideways):");
    PrintTreeImpl(root, 0);
    Console.WriteLine(); // blank line after the tree
}

// Number of spaces to indent per tree level
const int INDENT = 4;

// Recursive helper that prints the tree sideways
void PrintTreeImpl(TreeNode? node, int indent)
{
    // Base case: nothing to print
    if (node == null) return;

    // Print right subtree first so it appears "above" the current node
    PrintTreeImpl(node.Right, indent + INDENT);

    // Print the current node with indentation based on depth
    Console.WriteLine(new string(' ', indent) + node.Val);

    // Print left subtree last so it appears "below" the current node
    PrintTreeImpl(node.Left, indent + INDENT);
}

// ----------------------------
// Test Harness
// ----------------------------

// Build test tree:
//
//         1
//       /   \
//      2     3
//     / \   /
//    4   5 6
//
var root = new TreeNode(1)
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

// Print the tree visually
PrintTree(root);

// Run traversals (each returns a list of visited node values)
var pre = Preorder(root);
var inn = Inorder(root);
var post = Postorder(root);

// Print results as comma-separated lists
Console.WriteLine("Preorder:  " + string.Join(", ", pre));
Console.WriteLine("Inorder:   " + string.Join(", ", inn));
Console.WriteLine("Postorder: " + string.Join(", ", post));

// ==========================
// PREORDER TRAVERSAL (DFS)
// ==========================

// Returns the preorder traversal of a binary tree as a list of ints
// Order: Root -> Left -> Right
IList<int> Preorder(TreeNode? root)
{
    // This list will collect the traversal output
    var result = new List<int>();

    // Local recursive DFS function that performs the traversal
    void Dfs(TreeNode? node)
    {
        // Base case: empty subtree
        if (node == null) return;

        // Visit current node first
        result.Add(node.Val);

        // Visit left subtree
        Dfs(node.Left);

        // Visit right subtree
        Dfs(node.Right);
    }

    // Start DFS from the root
    Dfs(root);

    // Return the collected results
    return result;
}

// ==========================
// TREE NODE DEFINITION
// ==========================

// Simple binary tree node class used by the traversal methods
class TreeNode
{
    // Value stored in this node
    public int Val;

    // References to children (nullable because a node may not have children)
    public TreeNode? Left;
    public TreeNode? Right;

    // Constructor initializes the node with a value
    public TreeNode(int v) { Val = v; }
}
