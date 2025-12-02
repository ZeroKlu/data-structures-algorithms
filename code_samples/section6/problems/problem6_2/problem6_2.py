"""Problem 6.2: Implement a min-heap."""

def heapify(arr: list[int], n: int, i: int) -> None:
    """Heapify element."""
    smallest = i
    left = 2 * i + 1
    right = 2 * i + 2

    if left < n and arr[left] < arr[smallest]:
        smallest = left
    if right < n and arr[right] < arr[smallest]:
        smallest = right

    if smallest != i:
        arr[i], arr[smallest] = arr[smallest], arr[i]
        heapify(arr, n, smallest)

def build_heap(arr: list[int]) -> None:
    """Build min heap."""
    n = len(arr)
    start = n // 2 - 1
    for i in range(start, -1, -1):
        heapify(arr, n, i)

# ===== Tests =====

def print_array(label: str, arr: list[int]) -> None:
    """Print array."""
    print(label + " ".join(str(x) for x in arr))

def pop_min(arr: list[int], size_ref: list[int]) -> int:
    """Pop the min element from an array-based min-heap using prefix size."""
    size = size_ref[0]
    if size == 0:
        return None

    root = arr[0]
    size -= 1
    size_ref[0] = size

    if size > 0:
        arr[0] = arr[size]
        # heapify-down using prefix size
        heapify(arr, size, 0)

    return root

# ===== Run Tests =====

array = [9, 4, 7, 1, 0, 3, 2]

print_array("Original array: ", array)

build_heap(array)

print_array("After build_heap (min-heap array): ", array)

print("Extracting elements in sorted order:")
heap_copy = array[:]     # clone heap
size_r = [len(heap_copy)]

output = []
while size_r[0] > 0:
    output.append(pop_min(heap_copy, size_r))

print_array("", output)
