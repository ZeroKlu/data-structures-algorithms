"""Section 6: Heaps and Priority Queues"""

# pylint: disable=too-few-public-methods


class MinHeap:
    """
    Min-heap implementation (supports insert only).

    Heap property (min-heap):
      For every index i, parent value <= child value.
      The smallest element is always at index 0.

    Array representation:
      parent(i) = (i - 1) // 2
      left(i)   = 2*i + 1
      right(i)  = 2*i + 2
    """
    def __init__(self) -> None:
        # Internal dynamic array storing the heap elements.
        self._data: list[int] = []

    def insert(self, value: int) -> None:
        """
        Insert a value into the heap.

        Steps:
          1) Append value at the end (keeps tree complete).
          2) "Heapify up" (bubble up) by swapping with parent
             while heap property is violated.
        """
        # 1) Place new value at the bottom-rightmost position
        self._data.append(value)
        i = len(self._data) - 1  # index of newly inserted value

        # 2) Heapify up: while not at root, compare with parent
        while i > 0:
            parent = (i - 1) // 2
            # If parent <= child, heap property is satisfied
            if self._data[parent] <= self._data[i]:
                break
            # Otherwise swap child with parent and continue upward
            self._data[parent], self._data[i] = self._data[i], self._data[parent]
            i = parent


class MinHeapWithExtract:
    """
    Min-heap implementation with both insert and extract_min.

    extract_min removes and returns the smallest value (root).
    """
    def __init__(self) -> None:
        # Internal array storing heap elements.
        self._data: list[int] = []

    def insert(self, value: int) -> None:
        """
        Insert a value into the heap (same logic as MinHeap.insert).
        """
        self._data.append(value)
        i = len(self._data) - 1

        # Heapify up
        while i > 0:
            parent = (i - 1) // 2
            if self._data[parent] <= self._data[i]:
                break
            self._data[parent], self._data[i] = self._data[i], self._data[parent]
            i = parent

    def extract_min(self) -> int:
        """
        Remove and return the minimum element from the heap.

        Steps:
          1) Save root (min value).
          2) Move last element to root, shrink array by 1.
          3) "Heapify down" (bubble down) from root:
             swap with the smaller child while heap property is violated.
        """
        # Underflow: cannot extract from empty heap
        if not self._data:
            raise IndexError("heap underflow")

        # 1) Minimum is always at root (index 0)
        min_val = self._data[0]

        # 2) Pop last element (bottom-rightmost) to replace the root
        last = self._data.pop()
        if not self._data:
            # If heap is now empty, the popped element was the min
            return min_val

        # Put last element at the root, then restore heap property
        self._data[0] = last

        # 3) Heapify down from root
        n = len(self._data)
        i = 0
        while True:
            left = 2 * i + 1
            right = 2 * i + 2
            smallest = i  # assume current node is smallest

            # If left child exists and is smaller, update candidate
            if left < n and self._data[left] < self._data[smallest]:
                smallest = left

            # If right child exists and is smaller than current candidate, update
            if right < n and self._data[right] < self._data[smallest]:
                smallest = right

            # If neither child is smaller, heap property is restored
            if smallest == i:
                break

            # Swap current node with smallest child and continue
            self._data[i], self._data[smallest] = self._data[smallest], self._data[i]
            i = smallest

        return min_val


def heapify_down_array(a: list[int], n: int, i: int) -> None:
    """
    Heapify-down operation on a *raw array* segment a[0:n].

    This assumes:
      - Both subtrees rooted at children of i are already min-heaps.
    After this function:
      - The subtree rooted at i becomes a min-heap.

    Parameters:
      a: array being treated like a heap
      n: heap size (only indices < n are considered part of the heap)
      i: index to heapify down from
    """
    while True:
        left = 2 * i + 1
        right = 2 * i + 2
        smallest = i

        # Choose the smallest among node i and its children (within heap bounds)
        if left < n and a[left] < a[smallest]:
            smallest = left
        if right < n and a[right] < a[smallest]:
            smallest = right

        # If i is already smallest, heap property holds
        if smallest == i:
            break

        # Otherwise swap i with the smaller child and continue downward
        a[i], a[smallest] = a[smallest], a[i]
        i = smallest


def build_min_heap(a: list[int]) -> None:
    """
    Convert an arbitrary array into a min-heap in-place.

    Key idea:
      - Leaves are already heaps.
      - Start from the last parent node and heapify down each parent.

    Last parent index is (n//2 - 1).
    """
    n = len(a)
    # Iterate backwards over all internal nodes
    for i in range(n // 2 - 1, -1, -1):
        heapify_down_array(a, n, i)


def heapsort_desc_minheap(a: list[int]) -> None:
    """
    Heapsort using a min-heap that produces *descending* order in-place.

    Why descending?
      - In a min-heap, the minimum element is at the root (index 0).
      - Each iteration swaps the min element to the end of the array.
      - That places smaller elements at the back first, resulting in descending order.

    Steps:
      1) Build a min-heap over the entire array.
      2) Repeatedly swap root with the last element in the heap,
         shrink heap size, and heapify down to restore heap property.
    """
    build_min_heap(a)

    # end marks the last index of the current heap
    for end in range(len(a) - 1, 0, -1):
        # Move current min (root) to its final position at the end
        a[0], a[end] = a[end], a[0]

        # Restore min-heap property on the reduced heap [0:end)
        heapify_down_array(a, end, 0)


# -------- Tests --------

def print_array(a: list[int]) -> None:
    """Print array elements on one line."""
    print(" ".join(str(x) for x in a))


values_to_insert = [5, 3, 8, 1, 7, 2]

# --- Test MinHeap.insert (no extract) ---
print("=== Testing MinHeap.insert ===")

h1 = MinHeap()
print("Inserting into MinHeap (no extract available):", end=" ")
for v in values_to_insert:
    # We print to confirm the values inserted; MinHeap doesn't expose contents
    print(v, end=" ")
    h1.insert(v)  # just exercising insert
print("\n")

# --- Test MinHeapWithExtract.insert / extract_min ---
print("=== Testing MinHeapWithExtract insert / extract_min ===")

h2 = MinHeapWithExtract()
print("Inserting:", end=" ")
for v in values_to_insert:
    print(v, end=" ")
    h2.insert(v)
print()

# Repeatedly extract min until heap underflows
print("Extracting in sorted order:", end=" ")
while True:
    try:
        print(h2.extract_min(), end=" ")
    except IndexError:
        # When heap is empty, extract_min raises IndexError
        break
print("\n")

# --- Test build_min_heap ---
print("=== Testing build_min_heap ===")

arr1 = [9, 4, 7, 1, 0, 3, 2]
print("Original array:", end=" ")
print_array(arr1)

# Convert arr1 into a min-heap representation in-place
build_min_heap(arr1)
print("After build_min_heap (min-heap array):", end=" ")
print_array(arr1)
print()

# --- Test heapsort_desc_minheap ---
print("=== Testing heapsort_desc_minheap (descending sort) ===")

arr2 = [12, 3, 17, 8, 34, 25, 1]
print("Original array:", end=" ")
print_array(arr2)

# Sort in-place (descending order due to min-heap variant)
heapsort_desc_minheap(arr2)
print("After heapsort_desc_minheap (descending):", end=" ")
print_array(arr2)
