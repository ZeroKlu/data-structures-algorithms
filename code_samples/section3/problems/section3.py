"""Section 3 - Linked Lists"""

#pylint: disable=too-few-public-methods
#pylint: disable=redefined-builtin

from typing import Optional

class ListNode:
    """Linked list node."""
    def __init__(self, val: int = 0, next: "Optional[ListNode]" = None) -> None:
        self.val = val
        self.next = next

def insert_head(head: Optional[ListNode], x: int) -> ListNode:
    """Insert a value at the head of the linked list."""
    return ListNode(x, head)

def insert_after(p: Optional[ListNode], x: int) -> None:
    """Insert a value after a given node."""
    if p is None:
        return
    node = ListNode(x, p.next)
    p.next = node

def delete_after(p: Optional[ListNode]) -> None:
    """Delete a node after a given node."""
    if p is None or p.next is None:
        return
    doomed = p.next
    p.next = doomed.next

def reverse_list(head: Optional[ListNode]) -> Optional[ListNode]:
    """Reverse a linked list."""
    prev: Optional[ListNode] = None
    curr = head
    while curr is not None:
        nxt = curr.next
        curr.next = prev
        prev = curr
        curr = nxt
    return prev

def has_cycle(head: Optional[ListNode]) -> bool:
    """Check if a linked list has a cycle."""
    if head is None:
        return False
    slow = head
    fast = head
    while fast is not None and fast.next is not None:
        slow = slow.next
        fast = fast.next.next
        if slow is fast:
            return True
    return False

def middle_node(head: Optional[ListNode]) -> Optional[ListNode]:
    """Find the middle node of a linked list."""
    slow = head
    fast = head
    while fast is not None and fast.next is not None:
        slow = slow.next
        fast = fast.next.next
    return slow

def merge_two_lists(l1: Optional[ListNode], l2: Optional[ListNode]) -> Optional[ListNode]:
    """Merge two sorted linked lists."""
    dummy = ListNode(0)
    tail = dummy

    a = l1
    b = l2
    while a is not None and b is not None:
        if a.val <= b.val:
            tail.next = a
            a = a.next
        else:
            tail.next = b
            b = b.next
        tail = tail.next
    if a is not None:
        tail.next = a
    if b is not None:
        tail.next = b

    return dummy.next

def remove_nth_from_end(head: Optional[ListNode], n: int) -> Optional[ListNode]:
    """Remove the nth node from the end of a linked list."""
    dummy = ListNode(0, head)
    fast: Optional[ListNode] = dummy
    slow: Optional[ListNode] = dummy

    for _ in range(n + 1):
        if fast is None:
            return head
        fast = fast.next

    while fast is not None:
        fast = fast.next
        slow = slow.next  # type: ignore[assignment]

    doomed = slow.next  # type: ignore[assignment]
    if doomed is not None:
        slow.next = doomed.next  # type: ignore[assignment]
    return dummy.next

def print_list(head: "ListNode | None") -> None:
    """Print linked list."""
    current = head
    print()
    while current is not None:
        print(current.val, end="")
        current = current.next

def main() -> None:
    """Main function."""
    # ----- Build list: 1 2 3 4 5 -----
    head: Optional[ListNode] = None
    head = insert_head(head, 5)
    head = insert_head(head, 4)
    head = insert_head(head, 3)
    head = insert_head(head, 2)
    head = insert_head(head, 1)

    # ----- Insert 6 at the end -----
    tail = head
    while tail is not None and tail.next is not None:
        tail = tail.next
    insert_after(tail, 6)

    print_list(head)   # 1 2 3 4 5 6

    # ----- Delete 6 -----
    delete_after(tail)

    print_list(head)   # 1 2 3 4 5

    # ----- Reverse the list -----
    head = reverse_list(head)

    print_list(head)   # 5 4 3 2 1

    # ----- Check for cycle -----
    print(f"\nHas cycle: {'true' if has_cycle(head) else 'false'}")

    # ----- Build separate cycle list: 1 2 3 4 -----
    cycle: Optional[ListNode] = None
    cycle = insert_head(cycle, 4)
    cycle = insert_head(cycle, 3)
    cycle = insert_head(cycle, 2)
    cycle = insert_head(cycle, 1)

    print_list(cycle)  # 1 2 3 4

    # Create cycle: 4 -> 3
    p = cycle.next.next      # type: ignore[assignment]  # node 3
    q = p.next               # node 4
    q.next = p               # 4 -> 3

    print(f"\nHas cycle: {'true' if has_cycle(cycle) else 'false'}")

    # ----- Reverse head again -----
    head = reverse_list(head)   # 5 4 3 2 1 -> 1 2 3 4 5

    print_list(head)            # 1 2 3 4 5

    # ----- Middle node -----
    mid = middle_node(head)
    if mid is not None:
        print(f"\nMiddle node value: {mid.val}")

    # ----- Build second list: 6 7 8 -----
    more: Optional[ListNode] = None
    more = insert_head(more, 8)
    more = insert_head(more, 7)
    more = insert_head(more, 6)

    print_list(more)   # 6 7 8

    # ----- Merge lists -----
    merged = merge_two_lists(head, more)

    print_list(merged) # 1 2 3 4 5 6 7 8

    # ----- Remove 7 (2nd from end) -----
    merged = remove_nth_from_end(merged, 2)

    print_list(merged) # 1 2 3 4 5 6 8

if __name__ == "__main__":
    main()
