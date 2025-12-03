"""Problem 7.5 part 1: Find two numbers in an array that sum to a target."""

def two_sum_indices(nums: list[int], target: int) -> list[int]:
    """Find two numbers in an array that sum to a target."""
    index_of = {}
    for i, x in enumerate(nums):
        need = target - x
        if need in index_of:
            return [index_of[need], i]
        index_of[x] = i
    return []

# ============================
# Tests (top-level statements)
# ============================

tests = [
    ([2, 7, 11, 15], 9,  [0, 1]),   # 2 + 7 = 9
    ([3, 2, 4],      6,  [1, 2]),   # 2 + 4 = 6
    ([1, 2, 3, 4],   10, []),        # no solution
]

print("=== Test: two_sum_indices ===\n")

for numbers, tgt, expected in tests:
    result = two_sum_indices(numbers, tgt)
    print(f"Input: nums={numbers}, target={tgt}")
    print(f"Result: {result} (expected {expected})\n")
