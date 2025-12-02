"""Problem 6.1 - Implement a Min-Heap (Array-Based)"""

from typing import Optional

class MinHeap:
    """Min heap implementation"""
    def __init__(self) -> None:
        self.data: list[int] = []

    def _bubble_up(self, i: int) -> None:
        """Bubble up element"""
        while i > 0:
            p = (i - 1) // 2
            if self.data[i] >= self.data[p]:
                break
            self.data[i], self.data[p] = self.data[p], self.data[i]
            i = p

    def _heapify(self, i: int) -> None:
        """Heapify element"""
        n = len(self.data)
        while True:
            smallest = i
            left = 2 * i + 1
            right = 2 * i + 2
            if left < n and self.data[left] < self.data[smallest]:
                smallest = left
            if right < n and self.data[right] < self.data[smallest]:
                smallest = right
            if smallest == i:
                break
            self.data[i], self.data[smallest] = self.data[smallest], self.data[i]
            i = smallest

    def insert(self, val: int) -> None:
        """Insert element into heap"""
        self.data.append(val)
        self._bubble_up(len(self.data) - 1)

    def peek(self) -> Optional[int]:
        """Peek min element"""
        return self.data[0] if self.data else None

    def pop(self) -> Optional[int]:
        """Pop top element"""
        if not self.data:
            return None
        if len(self.data) == 1:
            return self.data.pop()
        root = self.data[0]
        self.data[0] = self.data.pop()
        self._heapify(0)
        return root

# ===== Tests =====

def print_array(label: str, arr: list[int]) -> None:
    """Print array"""
    print(label + " ".join(str(x) for x in arr))


print("=== Testing MinHeap ===")

h = MinHeap()
values = [5, 3, 8, 1, 7, 2]

print("Inserting values:", end=" ")
for v in values:
    print(v, end=" ")
    h.insert(v)
    print(f"\n  After insert {v} -> peek (min): {h.peek()}")
print()

print("Extracting in sorted order:", end=" ")
while True:
    x = h.pop()
    if x is None:
        break
    print(x, end=" ")
print("\n")

print("Testing peek/pop on empty heap:")
print("peek():", h.peek())
print("pop():", h.pop())
print()

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
