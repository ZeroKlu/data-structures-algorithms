#nullable enable

static ListNode InsertHead(ListNode? head, int x) {
    return new ListNode(x, head);
}

static void InsertAfter(ListNode? p, int x) {
    if (p == null) return;
    var node = new ListNode(x, p.Next);
    p.Next = node;
}

static void DeleteAfter(ListNode? p) {
    if (p == null || p.Next == null) return;
    var doomed = p.Next;
    p.Next = doomed.Next;
}

static ListNode? ReverseList(ListNode? head) {
    ListNode? prev = null;
    ListNode? curr = head;

    while (curr != null) {
        var nxt = curr.Next;
        curr.Next = prev;
        prev = curr;
        curr = nxt;
    }
    return prev;
}

static bool HasCycle(ListNode? head) {
    if (head == null) return false;
    var slow = head;
    var fast = head;
    while (fast != null && fast.Next != null) {
        slow = slow!.Next!;
        fast = fast.Next.Next!;
        if (slow == fast) return true;
    }
    return false;
}

static ListNode? MiddleNode(ListNode? head) {
    var slow = head;
    var fast = head;

    while (fast != null && fast.Next != null) {
        slow = slow!.Next;
        fast = fast.Next.Next;
    }
    return slow;
}

static ListNode? MergeTwoLists(ListNode? l1, ListNode? l2) {
    var dummy = new ListNode(0);
    var tail = dummy;

    var a = l1;
    var b = l2;
    while (a != null && b != null) {
        if (a.Val <= b.Val) {
            tail.Next = a;
            a = a.Next;
        } else {
            tail.Next = b;
            b = b.Next;
        }
        tail = tail.Next!;
    }
    if (a != null) tail.Next = a;
    if (b != null) tail.Next = b;

    return dummy.Next;
}

static ListNode? RemoveNthFromEnd(ListNode? head, int n) {
    var dummy = new ListNode(0) { Next = head };
    var fast = dummy;
    var slow = dummy;

    for (int i = 0; i <= n; i++) {
        if (fast == null) return head; // n too large
        fast = fast.Next!;
    }

    while (fast != null) {
        fast = fast.Next!;
        slow = slow.Next!;
    }

    var doomed = slow.Next;
    if (doomed != null) {
        slow.Next = doomed.Next;
    }
    return dummy.Next;
}

static void PrintList(ListNode? head) {
    var current = head;
    Console.WriteLine();
    while (current != null) {
        Console.Write(current.Val);
        current = current.Next;
    }
}

// ----- Build list: 1 2 3 4 5 -----
ListNode? head = null;
head = InsertHead(head, 5);
head = InsertHead(head, 4);
head = InsertHead(head, 3);
head = InsertHead(head, 2);
head = InsertHead(head, 1);

// ----- Insert 6 at the end -----
ListNode tail = head!;
while (tail.Next != null) {
    tail = tail.Next;
}
InsertAfter(tail, 6);

PrintList(head);   // 123456

// ----- Delete 6 -----
DeleteAfter(tail);

PrintList(head);   // 12345

// ----- Reverse the list -----
head = ReverseList(head);

PrintList(head);   // 54321

// ----- Check for cycle -----
Console.WriteLine($"\nHas cycle: {(HasCycle(head) ? "true" : "false")}");

// ----- Build separate cycle list: 1 2 3 4 -----
ListNode? cycle = null;
cycle = InsertHead(cycle, 4);
cycle = InsertHead(cycle, 3);
cycle = InsertHead(cycle, 2);
cycle = InsertHead(cycle, 1);

PrintList(cycle);  // 1234

// Create cycle: 4 -> 3
ListNode p = cycle!.Next!.Next!;       // node 3
ListNode q = cycle.Next!.Next!.Next!;    // node 4
q.Next = p;

Console.WriteLine($"\nHas cycle: {(HasCycle(cycle) ? "true" : "false")}");

// ----- Reverse head again -----
head = ReverseList(head);   // 54321 -> 12345

PrintList(head);            // 12345

// ----- Middle node -----
ListNode? mid = MiddleNode(head);
if (mid != null) {
    Console.WriteLine($"\nMiddle node value: {mid.Val}");
}

// ----- Build second list: 6 7 8 -----
ListNode? more = null;
more = InsertHead(more, 8);
more = InsertHead(more, 7);
more = InsertHead(more, 6);

PrintList(more);            // 678

// ----- Merge lists -----
ListNode? merged = MergeTwoLists(head, more);

PrintList(merged);          // 12345678

// ----- Remove 7 (2nd from end) -----
merged = RemoveNthFromEnd(merged, 2);

PrintList(merged);          // 1234568

class ListNode(int val, ListNode? next = null)
{
    public int Val = val;
    public ListNode? Next = next;
}
