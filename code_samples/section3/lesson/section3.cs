#nullable enable

static ListNode? PushFront(ListNode? head, int value) {
    return new ListNode(value, head);
}

static void PrintList(ListNode? head) {
    var current = head;
    while (current != null) {
        Console.WriteLine(current.Value);
        current = current.Next;
    }
}

static ListNode? ReverseList(ListNode? head) {
    ListNode? prev = null;
    var current = head;
    while (current != null) {
        var nextNode = current.Next;
        current.Next = prev;
        prev = current;
        current = nextNode;
    }
    return prev;
}

static void PrintListRecursive(ListNode? head) {
    if (head == null) {
        return;
    }
    Console.WriteLine(head.Value);
    PrintListRecursive(head.Next);
}

var head = PushFront(null, 1);
head = PushFront(head, 2);
head = PushFront(head, 3);
PrintList(head);
Console.WriteLine();
head = ReverseList(head);
PrintList(head);
Console.WriteLine();
PrintListRecursive(head);

class ListNode(int value, ListNode? next = null)
{
    public int Value = value;
    public ListNode? Next = next;
}
