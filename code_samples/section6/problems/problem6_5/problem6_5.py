"""
Problem 6.5: Find the k-th largest element in a stream / array.

This file implements the classic "Kth Largest Element in a Stream" pattern
using a MIN-HEAP.

Key idea:
---------
Maintain a min-heap of size at most k that stores the k largest elements
seen so far. The smallest element in this heap is the k-th largest overall.

Why a min-heap?
---------------
- We want fast access to the *smallest* element among the top k values.
- If a new value is larger than that smallest element, it belongs in the
  top k and should replace it.
- Python's `heapq` module implements a min-heap natively, making this approach
  efficient and simple.

Complexity:
-----------
- Time per insertion: O(log k)
- Space complexity: O(k)
"""

import heapq

# pylint: disable=too-few-public-methods


class KthLargest:
    """
    Models the k-th largest element in a dynamically growing array (stream).

    Invariant:
    ----------
    - `self.heap` is a MIN-HEAP containing the k largest elements seen so far
      (or fewer than k if not enough elements have been added yet).
    - The smallest element in the heap (`heap[0]`) is the k-th largest overall.
    """

    def __init__(self, k: int, nums: list[int]) -> None:
        """
        Initialize the KthLargest structure.

        Parameters:
        ----------
        k : int
            The rank to track (k-th largest element).
        nums : list[int]
            Initial list of numbers.

        Strategy:
        ---------
        - Store k.
        - Initialize an empty heap.
        - Insert each number using the same logic as streaming updates
          (via self.add), ensuring consistent behavior.
        """
        self.k = k
        self.heap: list[int] = []

        # Process initial numbers as if they arrived in a stream
        for n in nums:
            self.add(n)

    def add(self, val: int) -> int:
        """
        Add a new value to the stream and return the current k-th largest element.

        Algorithm:
        ----------
        1) If the heap contains fewer than k elements:
               - Push val into the heap unconditionally.
        2) Else if val is larger than the smallest element in the heap:
               - Replace the smallest element with val.
        3) Else:
               - Ignore val (it does not belong in the top k).

        Returns:
        --------
        int
            The current k-th largest element.

        Notes:
        ------
        - `heapq.heappush` inserts in O(log k).
        - `heapq.heapreplace` pops and pushes in one O(log k) operation.
        - This method assumes k > 0; otherwise, the concept of "k-th largest"
          is undefined.
        """
        if len(self.heap) < self.k:
            # Still collecting the first k elements
            heapq.heappush(self.heap, val)
        elif val > self.heap[0]:
            # val belongs among the top k; replace the smallest
            heapq.heapreplace(self.heap, val)

        # The smallest element in the k-sized heap is the k-th largest overall
        return self.heap[0]


# ===== Test Helpers =====

def print_list(label: str, arr: list[int]) -> None:
    """
    Print a list of integers with a label.

    Parameters:
    ----------
    label : str
        Text printed before the list.
    arr : list[int]
        List of integers to print.
    """
    print(label + " ".join(str(x) for x in arr))


# ===== Tests =====

def test_kth_largest() -> None:
    """
    Test harness for the KthLargest class.

    Scenario:
    ---------
    - Initial numbers: [4, 5, 8, 2]
    - k = 3
    - Add values sequentially: [3, 5, 10, 9, 4]

    Expected output:
    ----------------
        add(3)  -> 4
        add(5)  -> 5
        add(10) -> 5
        add(9)  -> 8
        add(4)  -> 8

    Explanation:
    ------------
    - The heap always stores the current 3 largest values.
    - The smallest of those values is the 3rd largest overall.
    """
    print("=== Test KthLargest ===")

    nums = [4, 5, 8, 2]
    k = 3

    print_list("Initial nums: ", nums)
    print("k =", k)
    print()

    # Initialize the KthLargest tracker
    kth = KthLargest(k, nums)

    to_add = [3, 5, 10, 9, 4]

    print("Adding values:")
    for val in to_add:
        kth_val = kth.add(val)
        print(f"add({val}) -> kth largest = {kth_val}")

    # Expected results for k = 3:
    #
    #     add(3)  -> 4
    #     add(5)  -> 5
    #     add(10) -> 5
    #     add(9)  -> 8
    #     add(4)  -> 8
    #
    # Explanation:
    #     - The heap always stores the current k largest values.
    #     - heap[0] is the k-th largest (the smallest in the k-sized heap).


# Only run tests when executed directly
if __name__ == "__main__":
    test_kth_largest()
