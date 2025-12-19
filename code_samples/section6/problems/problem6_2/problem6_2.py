"""
Problem 6.2: Build a min-heap from an array (bottom-up heap construction).

This script includes:
  1) heapify(arr, n, i): recursive "sift down" to fix the subtree rooted at i
  2) build_heap(arr): in-place O(n) heap construction from an arbitrary list
  3) pop_min(arr, size_ref): test helper that pops min from heap prefix [0..size-1]
  4) A simple print-based test harness

Min-heap invariant (0-based indexing):
    For every i > 0:
        arr[parent(i)] <= arr[i]

Index formulas:
    parent(i) = (i - 1) // 2
    left(i)   = 2*i + 1
    right(i)  = 2*i + 2

Complexity:
  - build_heap: O(n) overall
  - heapify (sift-down): O(log n)
  - pop_min (test helper): O(log n)

Notes:
  - heapify is recursive; for very large heaps, an iterative version avoids deep recursion.
  - Your pop_min docstring says it returns int, but it can return None when empty.
    Using Optional[int] makes that explicit.
"""

from typing import Optional


def heapify(arr: list[int], n: int, i: int) -> None:
    """
    Heapify (sift down) the subtree rooted at index i to satisfy min-heap property.

    Parameters:
        arr: The list containing heap elements.
        n:   The current heap size (only arr[0..n-1] is considered part of heap).
        i:   The index of the subtree root to fix.

    Algorithm:
        1) Identify the smallest among i, left child, right child (if children exist).
        2) If a child is smaller than arr[i], swap with the smallest child.
        3) Recursively heapify the child index we swapped into.

    Complexity:
        O(log n) worst case (height of heap).
    """
    smallest = i
    left = 2 * i + 1
    right = 2 * i + 2

    # Compare against left child (if within heap prefix)
    if left < n and arr[left] < arr[smallest]:
        smallest = left

    # Compare against right child (if within heap prefix)
    if right < n and arr[right] < arr[smallest]:
        smallest = right

    # If the smallest is not the current node, swap and continue fixing downward
    if smallest != i:
        arr[i], arr[smallest] = arr[smallest], arr[i]
        heapify(arr, n, smallest)


def build_heap(arr: list[int]) -> None:
    """
    Build a min-heap in place from an arbitrary list.

    Bottom-up construction:
        - Leaves (indices >= n//2) are already heaps of size 1.
        - Start from the last internal node (n//2 - 1) and heapify each node
          moving backward to index 0.

    Parameters:
        arr: List to transform into a min-heap in place.

    Complexity:
        O(n) overall.
    """
    n = len(arr)
    start = n // 2 - 1  # last internal node

    for i in range(start, -1, -1):
        heapify(arr, n, i)


# ===== Tests =====

def print_array(label: str, arr: list[int]) -> None:
    """
    Print a label followed by the elements of arr separated by spaces.

    Note:
        When printing a heap array, the order shown is "heap order", not sorted order.
    """
    print(label + " ".join(str(x) for x in arr))


def pop_min(arr: list[int], size_ref: list[int]) -> Optional[int]:
    """
    Pop the min element from an array-based min-heap using prefix size.

    Why size_ref is a list:
        Python integers are immutable and passed by assignment. Using a single-element
        list emulates pass-by-reference so the function can decrement the heap size.

    Parameters:
        arr:      Heap array (elements are valid in prefix arr[0..size-1]).
        size_ref: Single-element list holding current heap size, i.e. [size].

    Returns:
        The minimum element (root) if heap is non-empty; otherwise None.

    Steps:
        1) If size == 0: return None.
        2) Save root = arr[0].
        3) Decrement size.
        4) If size > 0:
             - Move last element in prefix to root
             - Heapify down from root using the NEW size as n
        5) Return saved root.

    Complexity:
        O(log n) for non-empty heap.
    """
    size = size_ref[0]
    if size == 0:
        return None

    # Minimum is always at index 0 in a min-heap
    root = arr[0]

    # Shrink heap size
    size -= 1
    size_ref[0] = size

    if size > 0:
        # Move the last element of the active heap prefix to the root
        arr[0] = arr[size]

        # Restore heap property within prefix [0..size-1]
        heapify(arr, size, 0)

    return root


# ===== Run Tests =====

array = [9, 4, 7, 1, 0, 3, 2]

print_array("Original array: ", array)

# Transform list into min-heap in place
build_heap(array)

print_array("After build_heap (min-heap array): ", array)

# Validate correctness by extracting all elements (should be ascending)
print("Extracting elements in sorted order:")
heap_copy = array[:]  # clone heap because popping mutates the list contents/structure
size_r = [len(heap_copy)]  # mutable "reference" to logical heap size

output: list[int] = []
while size_r[0] > 0:
    v = pop_min(heap_copy, size_r)
    # v will not be None here because loop condition ensures size_r[0] > 0,
    # but we keep the Optional return type for correctness and safety.
    output.append(v)  # type: ignore[arg-type]

print_array("", output)
