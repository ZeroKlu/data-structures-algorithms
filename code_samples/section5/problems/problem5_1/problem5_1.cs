#nullable enable

IList<int> Inorder(TreeNode? root) {
    var result = new List<int>();
    void Dfs(TreeNode? node) {
        if (node == null) return;
        Dfs(node.Left);
        result.Add(node.Val);
        Dfs(node.Right);
    }
    Dfs(root);
    return result;
}

IList<int> Postorder(TreeNode? root) {
    var result = new List<int>();
    void Dfs(TreeNode? node) {
        if (node == null) return;
        Dfs(node.Left);
        Dfs(node.Right);
        result.Add(node.Val);
    }
    Dfs(root);
    return result;
}

// ----------------------------
// Tree Printer (Sideways)
// ----------------------------

void PrintTree(TreeNode? root)
{
    Console.WriteLine("Tree (sideways):");
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
// Test Harness
// ----------------------------

// Build test tree:
//         1
//       /   \
//      2     3
//     / \   /
//    4   5 6

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

// Run traversals
var pre = Preorder(root);
var inn = Inorder(root);
var post = Postorder(root);

// Print results
Console.WriteLine("Preorder:  " + string.Join(", ", pre));
Console.WriteLine("Inorder:   " + string.Join(", ", inn));
Console.WriteLine("Postorder: " + string.Join(", ", post));

IList<int> Preorder(TreeNode? root) {
    var result = new List<int>();
    void Dfs(TreeNode? node) {
        if (node == null) return;
        result.Add(node.Val);
        Dfs(node.Left);
        Dfs(node.Right);
    }
    Dfs(root);
    return result;
}

class TreeNode {
    public int Val;
    public TreeNode? Left;
    public TreeNode? Right;
    public TreeNode(int v) { Val = v; }
}
