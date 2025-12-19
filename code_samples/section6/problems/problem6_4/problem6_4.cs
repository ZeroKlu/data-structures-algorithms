#nullable enable

/*
    This script demonstrates merging K sorted linked lists into one sorted list
    using .NET's PriorityQueue<TElement, TPriority>.

    High-level algorithm ("k-way merge"):
      1) Insert the head of each non-empty list into a MIN-HEAP keyed by node.val.
      2) Repeatedly:
           - Dequeue the smallest node
           - Append it to the merged output list
           - If that node has a next node, enqueue node.next
      3) Stop when the heap is empty.

    Why a priority queue?
      - At any moment, the next smallest element must be one of the current list heads.
      - Using a min-heap gives O(log k) for selecting the next node among k lists.

    Complexity:
      - Let N be the total number of nodes across all lists.
      - Each node is enqueued and dequeued at most once.
      - Time:  O(N log k)
      - Space: O(k) for the heap (excluding the nodes themselves).

    Nullable context:
      - #nullable enable means reference nullability is enforced by the compiler.
      - Lists and nodes are nullable because lists may be empty (null heads).
*/

/*
    MergeKLists
    -----------
    Merges an array of sorted linked lists into a single sorted list.

    Parameters:
      - lists: array of list head pointers (each head may be null)

    Returns:
      - head of the merged list, or null if all lists are empty (or lists is empty)

    Implementation notes:
      - Uses PriorityQueue<ListNode, int> where priority is node.val.
      - This function REUSES the existing nodes; it does not allocate new nodes
        for the merged content (other than the dummy head).
*/
static ListNode? MergeKLists(ListNode?[] lists) {
    /*
        PriorityQueue in .NET is a MIN-HEAP by default:
          - Dequeue() returns the element with the smallest priority.

        We use node.val as the priority, so the smallest-valued node is returned first.
    */
    var pq = new PriorityQueue<ListNode, int>();

    // Seed the heap with the first node of each non-empty list.
    foreach (var node in lists) {
        if (node != null) pq.Enqueue(node, node.val);
    }

    /*
        Dummy head pattern:
          - dummy is a fixed starting node
          - cur tracks the tail of the merged list
          - result starts at dummy.next
    */
    var dummy = new ListNode(0);
    var cur = dummy;

    // While there are candidate nodes remaining across all lists...
    while (pq.Count > 0) {
        // Get the smallest current node.
        var node = pq.Dequeue();

        // Append it to the merged list.
        cur.next = node;
        cur = cur.next;

        // Push the next node from the same list (if it exists).
        if (node.next != null) {
            pq.Enqueue(node.next, node.next.val);
        }
    }

    /*
        Optional safety:
          - Ensure the last node in the merged list points to null.
          - In well-formed input lists this will already be true, but it's harmless
            and can prevent surprises if lists were reused incorrectly elsewhere.
    */
    cur.next = null;

    return dummy.next;
}

// ===== Helpers =====

/*
    BuildList
    ---------
    Builds a linked list from the provided values.

    Parameters:
      - values: sequence of ints to place in nodes in the given order

    Returns:
      - head of the new list, or null if no values were provided

    Note:
      - Allocates new nodes with 'new'. In a long-running app you'd want to
        free or otherwise manage them, but for a short demo/test it's fine.
*/
static ListNode? BuildList(params int[] values)
{
    ListNode? head = null;
    ListNode? tail = null;

    foreach (var v in values)
    {
        var node = new ListNode(v);

        if (head == null)
        {
            // First element initializes both head and tail.
            head = tail = node;
        }
        else
        {
            // tail is known non-null here; '!' tells the compiler we guarantee it.
            tail!.next = node;
            tail = node;
        }
    }

    return head;
}

/*
    PrintList
    ---------
    Prints a list as space-separated values with a leading label.

    If head is null, prints only the label and a newline.
*/
static void PrintList(string label, ListNode? head)
{
    Console.Write(label);
    var cur = head;

    while (cur != null)
    {
        Console.Write(cur.val + " ");
        cur = cur.next;
    }

    Console.WriteLine();
}

// ===== Tests =====

/*
    TestMergeKLists
    --------------
    Simple print-based test harness that checks merge behavior for:
      1) three non-empty sorted lists
      2) mix of empty and non-empty lists
      3) all lists empty
      4) single list
      5) zero lists (empty input array)

    For each test:
      - print input lists
      - merge them
      - print the merged result
*/
static void TestMergeKLists()
{
    // --- Test 1: three sorted lists ---
    Console.WriteLine("=== Test 1: three sorted lists ===");
    var l1 = BuildList(1, 4, 7);
    var l2 = BuildList(2, 5, 8);
    var l3 = BuildList(3, 6, 9);

    PrintList("List 1: ", l1);
    PrintList("List 2: ", l2);
    PrintList("List 3: ", l3);

    var merged1 = MergeKLists(new ListNode?[] { l1, l2, l3 });
    PrintList("Merged: ", merged1);
    Console.WriteLine("(Expected: 1 2 3 4 5 6 7 8 9)");
    Console.WriteLine();

    // --- Test 2: mix of empty and non-empty lists ---
    Console.WriteLine("=== Test 2: mix of empty and non-empty lists ===");
    var a = BuildList(1, 3, 5);
    ListNode? b = null;
    var c = BuildList(2, 4, 6, 8);
    ListNode? d = null;

    PrintList("List A: ", a);
    PrintList("List B: ", b);
    PrintList("List C: ", c);
    PrintList("List D: ", d);

    var merged2 = MergeKLists([a, b, c, d]);
    PrintList("Merged: ", merged2);
    Console.WriteLine("(Expected: 1 2 3 4 5 6 8)");
    Console.WriteLine();

    // --- Test 3: all lists empty ---
    Console.WriteLine("=== Test 3: all lists empty ===");
    var merged3 = MergeKLists([null, null, null]);
    PrintList("Merged: ", merged3);
    Console.WriteLine("(Expected: <empty>)");
    Console.WriteLine();

    // --- Test 4: single list ---
    Console.WriteLine("=== Test 4: single list ===");
    var single = BuildList(10, 20, 30);
    PrintList("Input:  ", single);

    var merged4 = MergeKLists([single]);
    PrintList("Merged: ", merged4);
    Console.WriteLine("(Expected: 10 20 30)");
    Console.WriteLine();

    // --- Test 5: zero lists ---
    Console.WriteLine("=== Test 5: zero lists ===");
    var merged5 = MergeKLists(Array.Empty<ListNode?>());
    PrintList("Merged: ", merged5);
    Console.WriteLine("(Expected: <empty>)");
    Console.WriteLine();
}

// Call the test runner (top-level script / Program.Main)
TestMergeKLists();

/*
    ListNode
    --------
    Minimal singly-linked list node type.

    Using "primary constructor" syntax (C# 12 style):
        class ListNode(int v) { ... }

    Fields:
      - val : node value (int)
      - next: pointer to next node (nullable)
*/
class ListNode(int v)
{
    public int val = v;
    public ListNode? next;
}
