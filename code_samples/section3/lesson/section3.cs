#nullable enable

// Pushes a new node to the front of the list
static ListNode? PushFront(ListNode? head, int value) {
    return new ListNode(value, head);
}

// Prints a linked list
static void PrintList(ListNode? head) {
    var current = head;
    while (current != null) {
        Console.WriteLine(current.Value);
        current = current.Next;
    }
}

// Reverses a linked list
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

// Prints a linked list recursively
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

// Definition for singly-linked list node.
class ListNode(int value, ListNode? next = null)
{
    // Node value
    public int Value = value;

    // Pointer to next node
    public ListNode? Next = next;
}
