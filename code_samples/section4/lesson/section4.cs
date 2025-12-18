// Check if two parentheses are matching
static bool IsMatching(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

// Check if a string of parentheses is valid
static bool IsValidParentheses(string s) {
    var stack = new System.Collections.Generic.Stack<char>();
    foreach (var c in s) {
        if (c == '(' || c == '[' || c == '{') {
            stack.Push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (stack.Count == 0) return false;
            var open = stack.Pop();
            if (!IsMatching(open, c)) return false;
        }
    }
    return stack.Count == 0;
}

/**
 * This function implements the Breadth-first search algorithm.
 * It starts from a given node and explores all the connected nodes in a graph.
 * The graph is represented by an adjacency list.
 *
 * @param start: The starting node for the BFS traversal.
 * @param graph: The adjacency list representing the graph.
 */
static void Bfs(int start, System.Collections.Generic.List<System.Collections.Generic.List<int>> graph) {
    int n = graph.Count;
    var visited = new bool[n];
    var queue = new System.Collections.Generic.Queue<int>();

    // Mark the start node as visited and enqueue it
    visited[start] = true;
    queue.Enqueue(start);

    // Print the BFS traversal starting from the start node
    Console.WriteLine("BFS starting from " + start + ":");

    while (queue.Count > 0) {
        // Dequeue a node from the queue and print it
        int node = queue.Dequeue();
        Console.WriteLine("  visiting " + node);

        // Look at all possible neighbors of 'node'
        foreach (var neighbor in graph[node]) {
            // If there's an edge from node to neighbor and neighbor is not visited
            if (!visited[neighbor]) {
                // Mark the neighbor as visited and enqueue it
                visited[neighbor] = true;
                queue.Enqueue(neighbor);
            }
        }
    }
}

// ==== TESTING IntStack ====
System.Console.WriteLine("==== TESTING IntStack ====");
var stack = new IntStack();

System.Console.WriteLine("Pushing 10, 20, 30...");
stack.Push(10);
stack.Push(20);
stack.Push(30);

System.Console.WriteLine("Popping values:");
while (!stack.IsEmpty()) {
    int v = stack.Pop();
    System.Console.WriteLine($"  popped: {v}");
}

// ==== TESTING IntQueue ====
System.Console.WriteLine();
System.Console.WriteLine("==== TESTING IntQueue ====");
var q = new IntQueue();

System.Console.WriteLine("Enqueuing 1, 2, 3...");
q.Enqueue(1);
q.Enqueue(2);
q.Enqueue(3);

System.Console.WriteLine("Dequeuing values:");
while (!q.IsEmpty()) {
    int v = q.Dequeue();
    System.Console.WriteLine($"  dequeued: {v}");
}

// ==== TESTING IsValidParentheses ====
System.Console.WriteLine();
System.Console.WriteLine("==== TESTING IsValidParentheses ====");

var tests = new[] {
    "()",
    "([])",
    "{[()]}",
    "([)]",
    "((())",
    "abc(def[ghi]{jkl})",
    "",
    "{[}]"
};

foreach (var s in tests) {
    System.Console.WriteLine(
        $"Test \"{s}\": {(IsValidParentheses(s) ? "valid" : "invalid")}"
    );
}

// ==== TESTING Bfs ====
System.Console.WriteLine();
System.Console.WriteLine("==== TESTING Bfs ====");

// Graph:
// 0 -- 1
// |  /
// 2
var graph = new System.Collections.Generic.List<System.Collections.Generic.List<int>>();
graph.Add(new System.Collections.Generic.List<int> { 1, 2 }); // neighbors of 0
graph.Add(new System.Collections.Generic.List<int> { 0, 2 }); // neighbors of 1
graph.Add(new System.Collections.Generic.List<int> { 0, 1 }); // neighbors of 2

System.Console.WriteLine("BFS starting from node 0:");
Bfs(0, graph);

System.Console.WriteLine();
System.Console.WriteLine("==== ALL TESTS COMPLETE ====");

#pragma warning disable CA1050 // Declare types in namespaces
// Defines a stack of integers
public class IntStack {
    // The stack is implemented as a list
    private readonly System.Collections.Generic.List<int> _data = [];

    // Add value to the stack
    public void Push(int value) {
        _data.Add(value);
    }

    // Remove value from the stack
    public int Pop() {
        if (_data.Count == 0) {
            throw new System.InvalidOperationException("stack underflow");
        }
        int lastIndex = _data.Count - 1;
        int value = _data[lastIndex];
        _data.RemoveAt(lastIndex);
        return value;
    }

    // Check if the stack is empty
    public bool IsEmpty() {
        return _data.Count == 0;
    }
}

// Defines a queue of integers
public class IntQueue {
    // The queue is implemented as a queue
    private readonly System.Collections.Generic.Queue<int> _queue = [];

    // Add value to the queue
    public void Enqueue(int value) {
        _queue.Enqueue(value);
    }

    // Remove value from the queue
    public int Dequeue() {
        if (_queue.Count == 0) {
            throw new System.InvalidOperationException("queue underflow");
        }
        return _queue.Dequeue();
    }

    // Check if the queue is empty
    public bool IsEmpty() {
        return _queue.Count == 0;
    }
}
#pragma warning restore CA1050 // Declare types in namespaces
