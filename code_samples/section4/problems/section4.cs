#nullable enable

// Check if a string of parentheses is valid
static bool IsValid(string s) {
    var stack = new Stack<char>();
    var match = new Dictionary<char, char> {
        { ')', '(' },
        { ']', '[' },
        { '}', '{' }
    };

    foreach (char c in s) {
        if (c == '(' || c == '[' || c == '{') {
            stack.Push(c);
        } else {
            if (stack.Count == 0) return false;
            char open = stack.Pop();
            if (!match.TryGetValue(c, out char want) || open != want) {
                return false;
            }
        }
    }
    return stack.Count == 0;
}

// Find the next greater element for each element
static int[] NextGreaterElements(int[] nums) {
    int n = nums.Length;
    int[] ans = new int[n];
    for (int i = 0; i < n; i++) ans[i] = -1;

    var stack = new Stack<int>(); // values

    for (int i = n - 1; i >= 0; i--) {
        int x = nums[i];
        while (stack.Count > 0 && stack.Peek() <= x) {
            stack.Pop();
        }
        ans[i] = stack.Count == 0 ? -1 : stack.Peek();
        stack.Push(x);
    }
    return ans;
}

// Calculate number of days temperature will continue to rise
static int[] DailyTemperatures(int[] temps) {
    int n = temps.Length;
    int[] ans = new int[n];
    var stack = new Stack<int>(); // indices

    for (int i = 0; i < n; i++) {
        while (stack.Count > 0 && temps[i] > temps[stack.Peek()]) {
            int idx = stack.Pop();
            ans[idx] = i - idx;
        }
        stack.Push(i);
    }
    return ans;
}

// Level order traversal - returns 2D array
static IList<IList<int>> LevelOrder(TreeNode? root) {
    var result = new List<IList<int>>();
    if (root == null) return result;

    var q = new Queue<TreeNode>();
    q.Enqueue(root);

    while (q.Count > 0) {
        int levelSize = q.Count;
        var level = new List<int>();
        for (int i = 0; i < levelSize; i++) {
            var node = q.Dequeue();
            level.Add(node.Val);
            if (node.Left != null) q.Enqueue(node.Left);
            if (node.Right != null) q.Enqueue(node.Right);
        }
        result.Add(level);
    }
    return result;
}

// Calculate max sliding window
static int[] MaxSlidingWindow(int[] nums, int k) {
    int n = nums.Length;
    if (n == 0 || k == 0) return [];

    var result = new int[n - k + 1];
    var dq = new LinkedList<int>(); // indices
    int idx = 0;

    for (int i = 0; i < n; i++) {
        while (dq.Count > 0 && dq.First!.Value <= i - k) {
            dq.RemoveFirst();
        }
        while (dq.Count > 0 && nums[dq.Last!.Value] <= nums[i]) {
            dq.RemoveLast();
        }
        dq.AddLast(i);
        if (i >= k - 1) {
            result[idx++] = nums[dq.First!.Value];
        }
    }
    return result;
}

// ==== TEST IsValid ====
Console.WriteLine("==== TEST IsValid ====");

var parenTests = new[] {
    "()",
    "([])",
    "{[()]}",
    "([)]",
    "((())",
    "abc(def[ghi]{jkl})",
    "",
    "{[}]"
};

foreach (var s in parenTests)
{
    Console.WriteLine(
        $"Test \"{s}\": {(IsValid(s) ? "valid" : "invalid")}"
    );
}

// ==== TEST MyQueue ====
Console.WriteLine();
Console.WriteLine("==== TEST MyQueue ====");
var q = new MyQueue();

Console.WriteLine("Enqueue: 1, 2, 3");
q.Enqueue(1);
q.Enqueue(2);
q.Enqueue(3);

Console.WriteLine($"Front: {q.Front()}");
Console.WriteLine($"Dequeue: {q.Dequeue()}");
Console.WriteLine($"Dequeue: {q.Dequeue()}");
Console.WriteLine($"Empty? {(q.Empty() ? "true" : "false")}");
Console.WriteLine($"Dequeue: {q.Dequeue()}");
Console.WriteLine($"Empty? {(q.Empty() ? "true" : "false")}");

// ==== TEST NextGreaterElements ====
Console.WriteLine();
Console.WriteLine("==== TEST NextGreaterElements ====");
var nums = new[] { 2, 1, 2, 4, 3 };
var nge = NextGreaterElements(nums);
Console.WriteLine("nums: " + string.Join(" ", nums));
Console.WriteLine("NGE : " + string.Join(" ", nge));

// ==== TEST DailyTemperatures ====
Console.WriteLine();
Console.WriteLine("==== TEST DailyTemperatures ====");
var temps = new[] { 73, 74, 75, 71, 69, 72, 76, 73 };
var waits = DailyTemperatures(temps);
Console.WriteLine("temps: " + string.Join(" ", temps));
Console.WriteLine("wait : " + string.Join(" ", waits));

// ==== TEST LevelOrder ====
Console.WriteLine();
Console.WriteLine("==== TEST LevelOrder ====");

// Build this tree:
//
//        1
//       / \
//      2   3
//     /   / \
//    4   5   6
//
var node4 = new TreeNode(4);
var node5 = new TreeNode(5);
var node6 = new TreeNode(6);
var node2 = new TreeNode(2) { Left = node4 };
var node3 = new TreeNode(3) { Left = node5, Right = node6 };
var root  = new TreeNode(1) { Left = node2, Right = node3 };

var levels = LevelOrder(root);
for (int i = 0; i < levels.Count; i++)
{
    Console.WriteLine($"level {i}: {string.Join(" ", levels[i])}");
}

// ==== TEST MaxSlidingWindow ====
Console.WriteLine();
Console.WriteLine("==== TEST MaxSlidingWindow ====");
var arr = new[] { 1, 3, -1, -3, 5, 3, 6, 7 };
int k = 3;
var msw = MaxSlidingWindow(arr, k);
Console.WriteLine("nums: " + string.Join(" ", arr));
Console.WriteLine($"max window size {k}: " + string.Join(" ", msw));

Console.WriteLine();
Console.WriteLine("==== ALL TESTS COMPLETE ====");

// Defines a queue of integers
class MyQueue {
    // Optimize enqueue and dequeue by using two stacks
    // in = input, out = output
    private Stack<int> _in = new();
    private Stack<int> _out = new();

    // Move all elements from in to out
    private void MoveInToOut() {
        if (_out.Count == 0) {
            while (_in.Count > 0) {
                _out.Push(_in.Pop());
            }
        }
    }

    // Add value to the queue
    public void Enqueue(int x) {
        _in.Push(x);
    }

    // Remove value from the queue
    public int Dequeue() {
        MoveInToOut();
        return _out.Pop();
    }

    // Get the value at the front of the queue
    public int Front() {
        MoveInToOut();
        return _out.Peek();
    }

    // Check if the queue is empty
    public bool Empty() {
        return _in.Count == 0 && _out.Count == 0;
    }
}

// Defines a binary tree node
class TreeNode(int val)
{
    // Value of the node
    public int Val = val;

    // Reference to left child node
    public TreeNode? Left;

    // Reference to right child node
    public TreeNode? Right;
}
