"""
Merge k sorted linked lists and return it as one sorted list.

This file demonstrates the classic "k-way merge" technique using a heap.

Core idea:
  - At any moment, the next smallest value among all lists must be one of the
    current head nodes of those lists.
  - A min-heap lets us efficiently select the smallest head in O(log k) time.
  - Each time we pop a node from the heap, we push that node's successor
    (node.next) back into the heap (if it exists).

Complexity:
  - Let N be the total number of nodes across all lists.
  - Heap size is at most k (one active node per list at a time).
  - Each node is pushed and popped once:
        Time:  O(N log k)
        Space: O(k) for the heap (excluding node storage).

Implementation note about tie-breaking:
  - heapq compares tuple elements in order.
  - If two nodes have the same value, heapq would try to compare ListNode objects
    to break ties, which is not allowed by default.
  - To avoid that, we store (value, seq, node) where seq is a monotonically
    increasing integer, guaranteeing all heap entries are comparable.
"""

import heapq
from typing import Optional

# pylint: disable=too-few-public-methods


class ListNode:
    """
    Definition for singly-linked list.

    Attributes:
        val:  Integer value stored in the node.
        next: Pointer to the next node in the list (or None).
    """

    def __init__(self, val: int = 0, next_node: "Optional[ListNode]" = None):
        # Node value
        self.val = val
        # Next pointer (None means end of list)
        self.next = next_node


def merge_k_lists(lists: list[Optional[ListNode]]) -> Optional[ListNode]:
    """
    Merge k sorted linked lists and return it as one sorted list.

    Parameters:
        lists:
            A list of head pointers. Each head may be None, and each list is assumed
            to be sorted in ascending order.

    Returns:
        The head of the merged sorted linked list, or None if there are no nodes.

    Steps:
        1) Push the first node of each non-empty list into a min-heap.
        2) Repeatedly pop the smallest node from the heap and append it to the output.
        3) If the popped node has a next node, push that next node into the heap.
        4) Continue until the heap is empty.

    Memory behavior:
        - This function reuses the original nodes (re-links them).
        - Only a dummy head node is newly allocated to simplify construction.
    """
    # Heap entries are (node_value, sequence_number, node_reference).
    # The sequence_number prevents heapq from ever needing to compare ListNode objects.
    heap: list[tuple[int, int, ListNode]] = []

    seq = 0  # Monotonic counter used as a stable tie-breaker.

    # Seed heap with the head of each non-empty list.
    for node in lists:
        if node is not None:
            heapq.heappush(heap, (node.val, seq, node))
            seq += 1

    # Dummy head simplifies list building (avoids special-case for first append).
    dummy = ListNode(0)
    cur = dummy  # Tail pointer for the merged output list.

    # While there are nodes available across any lists...
    while heap:
        # Pop the smallest available node.
        _, _, node = heapq.heappop(heap)

        # Append it to the merged list.
        cur.next = node
        cur = cur.next

        # Push the next node from the same list (if it exists).
        if node.next is not None:
            heapq.heappush(heap, (node.next.val, seq, node.next))
            seq += 1

    # Optional safety: ensure merged list terminates properly.
    cur.next = None

    return dummy.next


# ===== Helpers =====

def build_list(values: list[int]) -> Optional[ListNode]:
    """
    Build a linked list from a Python list of ints.

    Parameters:
        values: list of integers to place into nodes in the same order.

    Returns:
        Head pointer of the linked list, or None if values is empty.
    """
    head: Optional[ListNode] = None
    tail: Optional[ListNode] = None

    for v in values:
        node = ListNode(v)
        if head is None:
            # First node initializes both head and tail.
            head = tail = node
        else:
            # Append to tail and advance tail.
            # tail is guaranteed not None here because head is not None.
            tail.next = node  # type: ignore[union-attr]
            tail = node

    return head


def print_list(label: str, head: Optional[ListNode]) -> None:
    """
    Print a linked list as space-separated values preceded by a label.

    If head is None, prints just the label and a newline.
    """
    print(label, end="")
    cur = head
    while cur is not None:
        print(cur.val, end=" ")
        cur = cur.next
    print()


# ===== Tests =====

def test_merge_k_lists() -> None:
    """
    Basic test harness for merge_k_lists.

    Tests cover:
      1) Three non-empty sorted lists
      2) Mix of empty and non-empty lists
      3) All lists empty
      4) Single list
      5) Zero lists (empty input)
    """

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
