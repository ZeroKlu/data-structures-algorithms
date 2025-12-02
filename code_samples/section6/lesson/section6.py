"""Section 6: Heaps and Priority Queues"""

#pylint: disable=too-few-public-methods

class MinHeap:
    """Min heap implementation"""
    def __init__(self) -> None:
        self._data: list[int] = []

    def insert(self, value: int) -> None:
        """Insert value into heap"""
        self._data.append(value)
        i = len(self._data) - 1
        # heapify up
        while i > 0:
            parent = (i - 1) // 2
            if self._data[parent] <= self._data[i]:
                break
            self._data[parent], self._data[i] = self._data[i], self._data[parent]
            i = parent

class MinHeapWithExtract:
    """Min heap implementation with extract min"""
    def __init__(self) -> None:
        self._data: list[int] = []

    def insert(self, value: int) -> None:
        """Insert value into heap"""
        self._data.append(value)
        i = len(self._data) - 1
        while i > 0:
            parent = (i - 1) // 2
            if self._data[parent] <= self._data[i]:
                break
            self._data[parent], self._data[i] = self._data[i], self._data[parent]
            i = parent

    def extract_min(self) -> int:
        """Extract min value from heap"""
        if not self._data:
            raise IndexError("heap underflow")
        min_val = self._data[0]
        last = self._data.pop()
        if not self._data:
            return min_val
        self._data[0] = last

        n = len(self._data)
        i = 0
        while True:
            left = 2 * i + 1
            right = 2 * i + 2
            smallest = i
            if left < n and self._data[left] < self._data[smallest]:
                smallest = left
            if right < n and self._data[right] < self._data[smallest]:
                smallest = right
            if smallest == i:
                break
            self._data[i], self._data[smallest] = self._data[smallest], self._data[i]
            i = smallest
        return min_val

def heapify_down_array(a: list[int], n: int, i: int) -> None:
    """Heapify down array"""
    while True:
        left = 2 * i + 1
        right = 2 * i + 2
        smallest = i
        if left < n and a[left] < a[smallest]:
            smallest = left
        if right < n and a[right] < a[smallest]:
            smallest = right
        if smallest == i:
            break
        a[i], a[smallest] = a[smallest], a[i]
        i = smallest

def build_min_heap(a: list[int]) -> None:
    """Build min heap"""
    n = len(a)
    for i in range(n // 2 - 1, -1, -1):
        heapify_down_array(a, n, i)

def heapsort_desc_minheap(a: list[int]) -> None:
    """Heap sort descending with min heap"""
    build_min_heap(a)
    for end in range(len(a) - 1, 0, -1):
        a[0], a[end] = a[end], a[0]
        heapify_down_array(a, end, 0)

# -------- Tests --------

def print_array(a: list[int]) -> None:
    """Print array"""
    print(" ".join(str(x) for x in a))

values_to_insert = [5, 3, 8, 1, 7, 2]

# --- Test MinHeap.insert (no extract) ---
print("=== Testing MinHeap.insert ===")

h1 = MinHeap()
print("Inserting into MinHeap (no extract available):", end=" ")
for v in values_to_insert:
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

print("Extracting in sorted order:", end=" ")
while True:
    try:
        print(h2.extract_min(), end=" ")
    except IndexError:
        break
print("\n")

# --- Test build_min_heap ---
print("=== Testing build_min_heap ===")

arr1 = [9, 4, 7, 1, 0, 3, 2]
print("Original array:", end=" ")
print_array(arr1)

build_min_heap(arr1)
print("After build_min_heap (min-heap array):", end=" ")
print_array(arr1)
print()

# --- Test heapsort_desc_minheap ---
print("=== Testing heapsort_desc_minheap (descending sort) ===")

arr2 = [12, 3, 17, 8, 34, 25, 1]
print("Original array:", end=" ")
print_array(arr2)

heapsort_desc_minheap(arr2)
print("After heapsort_desc_minheap (descending):", end=" ")
print_array(arr2)
