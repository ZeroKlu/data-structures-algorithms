"""
Problem 6.1 - Implement a Min-Heap (Array-Based)

This module contains:
  1) A simple array-backed MinHeap implementation for integers
  2) A small print-based test harness at the bottom

A min-heap maintains the invariant:
    For every node i (except the root),
        heap[parent(i)] <= heap[i]

Because of this:
  - The minimum element is always at index 0
  - insert is O(log n) via "bubble up"
  - pop (remove-min) is O(log n) via "heapify down"
  - peek is O(1)

Array index mapping (0-based):
  parent(i) = (i - 1) // 2
  left(i)   = 2*i + 1
  right(i)  = 2*i + 2

Empty-heap behavior:
  - peek() returns None when the heap is empty
  - pop() returns None when the heap is empty
"""

from typing import Optional


class MinHeap:
    """
    Min heap implementation (integers) backed by a Python list.

    Internal representation:
      - self.data is a list that stores elements in "heap order"
      - self.data[0] is always the minimum element (when non-empty)

    Note:
      - This implementation relies on integer comparisons.
      - For custom ordering or objects, you'd typically accept a key/comparator.
    """

    def __init__(self) -> None:
        # Backing storage for the heap. Starts empty and grows as needed.
        self.data: list[int] = []

    def _bubble_up(self, i: int) -> None:
        """
        Restore heap invariant after inserting an element.

        When we insert, we append the new element at the end of the list.
        That new element may be smaller than its parent, violating the heap
        invariant. "Bubble up" repeatedly swaps the element with its parent
        until the invariant is restored.

        Args:
            i: The index of the element to bubble upward (typically the last index).

        Complexity:
            O(log n) in the worst case (height of the heap).
        """
        while i > 0:
            # Compute parent index in a 0-based binary heap array
            p = (i - 1) // 2

            # If parent <= current, heap property is satisfied; stop.
            if self.data[i] >= self.data[p]:
                break

            # Otherwise swap current with parent and continue upward.
            self.data[i], self.data[p] = self.data[p], self.data[i]
            i = p

    def _heapify(self, i: int) -> None:
        """
        Restore heap invariant after removing the root (min element).

        Typical pop strategy:
          1) Save the root (minimum).
          2) Move the last element into index 0.
          3) Remove the last element.
          4) The moved element may be larger than its children, so we "heapify down"
             (a.k.a. sift down) by swapping it with the smaller child until the
             heap invariant is restored.

        Args:
            i: Index to heapify down from (usually 0).

        Complexity:
            O(log n) in the worst case.
        """
        n = len(self.data)

        while True:
            smallest = i
            left = 2 * i + 1
            right = 2 * i + 2

            # Pick the smallest among current node and its children (if they exist).
            if left < n and self.data[left] < self.data[smallest]:
                smallest = left
            if right < n and self.data[right] < self.data[smallest]:
                smallest = right

            # If the current node is already the smallest, heap property holds here.
            if smallest == i:
                break

            # Otherwise swap down and continue from where we swapped to.
            self.data[i], self.data[smallest] = self.data[smallest], self.data[i]
            i = smallest

    def insert(self, val: int) -> None:
        """
        Insert an element into the heap.

        Steps:
          1) Append the value to the end of the array
          2) Bubble it up to restore the heap property

        Args:
            val: Value to insert.

        Complexity:
            O(log n).
        """
        self.data.append(val)
        self._bubble_up(len(self.data) - 1)

    def peek(self) -> Optional[int]:
        """
        Return the minimum element without removing it.

        Returns:
            The minimum element (root) if present; otherwise None.

        Complexity:
            O(1).
        """
        return self.data[0] if self.data else None

    def pop(self) -> Optional[int]:
        """
        Remove and return the minimum element.

        Cases:
          - Empty heap: return None
          - One element: pop and return it
          - Otherwise:
              * save root
              * move last element to root
              * heapify down
              * return saved root

        Returns:
            The minimum element, or None if the heap is empty.

        Complexity:
            O(log n) when non-empty.
        """
        if not self.data:
            return None

        # Fast path: only one element
        if len(self.data) == 1:
            return self.data.pop()

        # Save min element
        root = self.data[0]

        # Move last element to root and shrink list
        self.data[0] = self.data.pop()

        # Restore heap property
        self._heapify(0)

        return root


# ===== Tests =====
#
# Simple print-driven checks:
#   - Insert values and show current minimum after each insert
#   - Pop until empty to verify ascending order
#   - Confirm peek/pop return None on empty
#   - Reinsert after emptying and pop again

def print_array(label: str, arr: list[int]) -> None:
    """
    Utility function to print the contents of a list.

    Note: Included for convenience/debugging. Not required by the heap itself.
    """
    print(label + " ".join(str(x) for x in arr))


print("=== Testing MinHeap ===")

h = MinHeap()
values = [5, 3, 8, 1, 7, 2]

print("Inserting values:", end=" ")
for v in values:
    print(v, end=" ")
    h.insert(v)

    # After each insert, peek should return the smallest value inserted so far
    print(f"\n  After insert {v} -> peek (min): {h.peek()}")
print()

# Repeated pop should yield values in ascending order if heap is correct.
print("Extracting in sorted order:", end=" ")
while True:
    x = h.pop()
    if x is None:
        break
    print(x, end=" ")
print("\n")

# Confirm empty-heap behavior
print("Testing peek/pop on empty heap:")
print("peek():", h.peek())
print("pop():", h.pop())
print()

# Confirm heap can be reused
print("Reinserting values: 10, 4, 6")
h.insert(10)
h.insert(4)
h.insert(6)

print("peek():", h.peek())

print("Final pop sequence:", end=" ")
while True:
    x = h.pop()
    if x is None:
        break
    print(x, end=" ")
print()
