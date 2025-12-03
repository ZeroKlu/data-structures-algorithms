"""Problem 6.3 - K Smallest Elements in an Array"""

import heapq

def k_smallest(nums: list[int], k: int) -> list[int]:
    """Find k smallest elements in an array."""
    if k == 0:
        return []
    # Python has min-heap, so store negatives to simulate max-heap
    heap: list[int] = []
    for x in nums:
        if len(heap) < k:
            heapq.heappush(heap, -x)
        elif x < -heap[0]:
            heapq.heapreplace(heap, -x)
    return [-x for x in heap]

# ===== Test Harness =====

def print_list(label: str, xs: list[int]) -> None:
    """Print list."""
    print(label + " ".join(str(x) for x in xs))

def test_k_smallest():
    """Test k_smallest."""
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


# If running normally:
if __name__ == "__main__":
    test_k_smallest()
