"""Merge k sorted linked lists and return it as one sorted list."""

import heapq
from typing import Optional

#pylint: disable=too-few-public-methods

class ListNode:
    """Definition for singly-linked list."""
    def __init__(self, val: int = 0, next_node: "Optional[ListNode]" = None):
        self.val = val
        self.next = next_node

def merge_k_lists(lists: list[Optional[ListNode]]) -> Optional[ListNode]:
    """Merge k sorted linked lists and return it as one sorted list."""
    heap: list[tuple[int, int, ListNode]] = []
    seq = 0  # to avoid comparing ListNode objects
    for node in lists:
        if node is not None:
            heapq.heappush(heap, (node.val, seq, node))
            seq += 1

    dummy = ListNode(0)
    cur = dummy
    while heap:
        _, _, node = heapq.heappop(heap)
        cur.next = node
        cur = cur.next
        if node.next is not None:
            heapq.heappush(heap, (node.next.val, seq, node.next))
            seq += 1
    return dummy.next

# ===== Helpers =====

def build_list(values: list[int]) -> Optional[ListNode]:
    """Build linked list."""
    head = None
    tail = None
    for v in values:
        node = ListNode(v)
        if head is None:
            head = tail = node
        else:
            tail.next = node
            tail = node
    return head

def print_list(label: str, head: Optional[ListNode]) -> None:
    """Print linked list."""
    print(label, end="")
    cur = head
    while cur is not None:
        print(cur.val, end=" ")
        cur = cur.next
    print()

# ===== Tests =====

def test_merge_k_lists():
    """Test merge_k_lists."""
    # ---- Test 1: three sorted lists ----
    print("=== Test 1: three sorted lists ===")
    l1 = build_list([1, 4, 7])
    l2 = build_list([2, 5, 8])
    l3 = build_list([3, 6, 9])

    print_list("List 1: ", l1)
    print_list("List 2: ", l2)
    print_list("List 3: ", l3)

    merged1 = merge_k_lists([l1, l2, l3])
    print_list("Merged: ", merged1)
    print("(Expected: 1 2 3 4 5 6 7 8 9)\n")

    # ---- Test 2: mix of empty + non-empty ----
    print("=== Test 2: some lists empty ===")
    a = build_list([1, 3, 5])
    b = None
    c = build_list([2, 4, 6, 8])
    d = None

    print_list("List A: ", a)
    print_list("List B: ", b)
    print_list("List C: ", c)
    print_list("List D: ", d)

    merged2 = merge_k_lists([a, b, c, d])
    print_list("Merged: ", merged2)
    print("(Expected: 1 2 3 4 5 6 8)\n")

    # ---- Test 3: all lists empty ----
    print("=== Test 3: all empty ===")
    merged3 = merge_k_lists([None, None, None])
    print_list("Merged: ", merged3)
    print("(Expected: <empty>)\n")

    # ---- Test 4: single list ----
    print("=== Test 4: single list ===")
    single = build_list([10, 20, 30])
    print_list("Input:  ", single)

    merged4 = merge_k_lists([single])
    print_list("Merged: ", merged4)
    print("(Expected: 10 20 30)\n")

    # ---- Test 5: zero lists ----
    print("=== Test 5: zero lists ===")
    merged5 = merge_k_lists([])
    print_list("Merged: ", merged5)
    print("(Expected: <empty>)\n")


# Only run tests if executing this file directly
if __name__ == "__main__":
    test_merge_k_lists()
