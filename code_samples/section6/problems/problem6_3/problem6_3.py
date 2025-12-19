"""
Problem 6.3 - K Smallest Elements in an Array

This module demonstrates how to find the k smallest elements in an array
using a HEAP-BASED approach with O(n log k) time complexity.

Key idea:
  - We want to keep track of the k smallest elements seen so far.
  - Python's heapq implements a MIN-HEAP.
  - To efficiently discard large elements, we simulate a MAX-HEAP by
    storing NEGATIVE values in the heap.

Why a max-heap (simulated)?
  - The root of the heap should represent the LARGEST of the k smallest values.
  - That value acts as the "cutoff":
        * If a new number is larger → ignore it
        * If a new number is smaller → replace the cutoff

Complexity:
  - Time:  O(n log k)
  - Space: O(k)

Edge cases handled:
  - k == 0 → return empty list
  - k > len(nums) → return all elements (heap never reaches size k)
"""

import heapq


def k_smallest(nums: list[int], k: int) -> list[int]:
    """
    Return the k smallest elements from nums (in any order).

    Parameters:
        nums : list[int]
            Input list of integers.
        k : int
            Number of smallest elements to return.

    Returns:
        list[int]
            A list containing up to k smallest elements.
            The result is NOT sorted.

    Algorithm:
        1) If k == 0, immediately return an empty list.
        2) Maintain a heap of at most k elements.
        3) Because heapq is a min-heap, store NEGATED values
           to simulate a max-heap.
        4) For each element x in nums:
             - If heap size < k: push -x
             - Else if x < current maximum (-heap[0]):
                   replace the maximum with x
        5) Convert values back to positive before returning.

    Notes:
        - heap[0] always holds the NEGATED largest value among the k smallest.
        - heapreplace() is more efficient than pop()+push().
    """
    if k == 0:
        return []

    # Heap stores NEGATIVE values to simulate a max-heap
    heap: list[int] = []

    for x in nums:
        if len(heap) < k:
            # Heap not full yet → add element
            heapq.heappush(heap, -x)

        elif x < -heap[0]:
            # x is smaller than the largest of the k smallest so far
            # Replace the current maximum with x
            heapq.heapreplace(heap, -x)

        # Else:
        #   x is not among the k smallest elements → ignore it

    # Convert back to positive values before returning
    return [-x for x in heap]


# ===== Test Harness =====

def print_list(label: str, xs: list[int]) -> None:
    """
    Print a label followed by elements of a list.

    Example:
        print_list("Result: ", [1, 2, 3])
        → Result: 1 2 3
    """
    print(label + " ".join(str(x) for x in xs))


def test_k_smallest() -> None:
    """
    Test the k_smallest() function with several scenarios:

      1) k <= n
      2) k > n
      3) k == 0

    For clarity:
      - Results are printed in heap order
      - Then printed again in sorted order for verification
    """
    nums = [7, 2, 9, 4, 1, 5, 8, 3, 6]

    # --- Test 1: k <= n ---
    print("=== Test 1: k <= n ===")
    print_list("Input array: ", nums)
    k = 4
    print("k =", k)

    result1 = k_smallest(nums, k)
    print_list("k_smallest result (heap order): ", result1)
    print_list("k_smallest result (sorted): ", sorted(result1))
    print("(Expected: 1 2 3 4)\n")

    # --- Test 2: k > n ---
    print("=== Test 2: k > n ===")
    k = 20
    print_list("Input array: ", nums)
    print("k =", k)

    result2 = k_smallest(nums, k)
    print_list("k_smallest result (heap order): ", result2)
    print_list("k_smallest result (sorted): ", sorted(result2))
    print("(Expected: all elements sorted)\n")

    # --- Test 3: k = 0 ---
    print("=== Test 3: k = 0 ===")
    k = 0
    print("k =", k)

    result3 = k_smallest(nums, k)
    print_list("k_smallest result: ", result3)
    print("(Expected: empty list)\n")


# Run tests when executed as a script
if __name__ == "__main__":
    test_k_smallest()
