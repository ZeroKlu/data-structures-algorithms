"""Problem 7.4 - Subarray Sum equals K."""

def subarray_sum(nums: list[int], k: int) -> int:
    """Subarray Sum equals K."""
    count = 0
    prefix = 0
    freq: dict[int, int] = {0: 1}
    for x in nums:
        prefix += x
        need = prefix - k
        count += freq.get(need, 0)
        freq[prefix] = freq.get(prefix, 0) + 1
    return count

# ============================
# Tests (top-level statements)
# ============================

tests = [
    ([1, 1, 1], 2, 2),            # expected 2
    ([1, 2, 3], 3, 2),            # expected 2: [1,2], [3]
    ([3, 4, 7, 2, -3, 1, 4, 2], 7, 4),  # expected 4
    ([], 0, 0),                  # expected 0
]

print("=== Test: subarray_sum ===\n")

for numbers, k_value, expected in tests:
    result = subarray_sum(numbers, k_value)
    print(f"Input: nums={numbers}, k={k_value}")
    print(f"Result: {result} (expected {expected})\n")
