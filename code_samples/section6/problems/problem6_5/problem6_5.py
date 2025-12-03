"""Problem 6.5: Find the kth largest element in an array."""

import heapq

#pylint: disable=too-few-public-methods

class KthLargest:
    """Models the kth largest element in an array."""
    def __init__(self, k: int, nums: list[int]) -> None:
        self.k = k
        self.heap: list[int] = []
        for n in nums:
            self.add(n)

    def add(self, val: int) -> int:
        """Add element to heap."""
        if len(self.heap) < self.k:
            heapq.heappush(self.heap, val)
        elif val > self.heap[0]:
            heapq.heapreplace(self.heap, val)
        return self.heap[0]

# ===== Test Helpers =====

def print_list(label: str, arr: list[int]) -> None:
    """Print list."""
    print(label + " ".join(str(x) for x in arr))

# ===== Tests =====

def test_kth_largest():
    """Test kth_largest."""
    print("=== Test KthLargest ===")

    nums = [4, 5, 8, 2]
    k = 3

    print_list("Initial nums: ", nums)
    print("k =", k)
    print()

    kth = KthLargest(k, nums)

    to_add = [3, 5, 10, 9, 4]

    print("Adding values:")
    for val in to_add:
        kth_val = kth.add(val)
        print(f"add({val}) -> kth largest = {kth_val}")

    # Expected results for k = 3:

    #     add(3)  -> 4
    #     add(5)  -> 5
    #     add(10) -> 5
    #     add(9)  -> 8
    #     add(4)  -> 8

    # Explanation:
    #     - heap always stores the current k largest values
    #     - heap[0] is the kth largest (smallest in the k-sized heap)

# Only run tests when executed directly
if __name__ == "__main__":
    test_kth_largest()
