"""Section 3: Linked Lists"""

#pylint: disable=too-few-public-methods
#pylint: disable=redefined-builtin

class ListNode:
    """Linked list node."""
    def __init__(self, value: int, next: "ListNode | None" = None):
        self.value = value
        self.next = next

def push_front(head: "ListNode | None", value: int) -> "ListNode":
    """Push value onto head of linked list."""
    return ListNode(value, head)

def print_list(head: "ListNode | None") -> None:
    """Print linked list."""
    current = head
    print()
    while current is not None:
        print(current.value, end=None)
        current = current.next

def reverse_list(head: "ListNode | None") -> "ListNode | None":
    """Reverse linked list."""
    prev = None
    current = head
    while current is not None:
        next_node = current.next
        current.next = prev
        prev = current
        current = next_node
    return prev

def print_list_recursive(head: "ListNode | None") -> None:
    """Print linked list recursively."""
    if head is None:
        return
    print(head.value, end=None)
    v = head.next
    print_list_recursive(v)

def main() -> None:
    """Main function."""
    head = push_front(None, 3)
    head = push_front(head, 2)
    head = push_front(head, 1)
    print_list(head)
    head = reverse_list(head)
    print_list(head)
    print()
    print_list_recursive(head)

if __name__ == "__main__":
    main()
