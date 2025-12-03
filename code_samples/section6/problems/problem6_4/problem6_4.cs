#nullable enable

static ListNode? MergeKLists(ListNode?[] lists) {
    var pq = new PriorityQueue<ListNode, int>();
    foreach (var node in lists) {
        if (node != null) pq.Enqueue(node, node.val);
    }

    var dummy = new ListNode(0);
    var cur = dummy;

    while (pq.Count > 0) {
        var node = pq.Dequeue();
        cur.next = node;
        cur = cur.next;
        if (node.next != null) {
            pq.Enqueue(node.next, node.next.val);
        }
    }
    return dummy.next;
}

// ===== Helpers =====

static ListNode? BuildList(params int[] values)
{
    ListNode? head = null;
    ListNode? tail = null;

    foreach (var v in values)
    {
        var node = new ListNode(v);
        if (head == null)
        {
            head = tail = node;
        }
        else
        {
            tail!.next = node;
            tail = node;
        }
    }

    return head;
}

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

    var merged2 = MergeKLists(new ListNode?[] { a, b, c, d });
    PrintList("Merged: ", merged2);
    Console.WriteLine("(Expected: 1 2 3 4 5 6 8)");
    Console.WriteLine();

    // --- Test 3: all lists empty ---
    Console.WriteLine("=== Test 3: all lists empty ===");
    var merged3 = MergeKLists(new ListNode?[] { null, null, null });
    PrintList("Merged: ", merged3);
    Console.WriteLine("(Expected: <empty>)");
    Console.WriteLine();

    // --- Test 4: single list ---
    Console.WriteLine("=== Test 4: single list ===");
    var single = BuildList(10, 20, 30);
    PrintList("Input:  ", single);

    var merged4 = MergeKLists(new ListNode?[] { single });
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

class ListNode(int v)
{
    public int val = v;
    public ListNode? next;
}
