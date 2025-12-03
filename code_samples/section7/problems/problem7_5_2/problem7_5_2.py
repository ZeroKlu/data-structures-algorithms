"""Problem 7.5 part 2: Count unique pairs that sum to a target."""

def count_unique_pairs(nums: list[int], target: int) -> int:
    """Count unique pairs that sum to a target."""
    seen: set[int] = set()
    pairs: set[tuple[int, int]] = set()

    for x in nums:
        y = target - x
        if y in seen:
            a = min(x, y)
            b = max(x, y)
            pairs.add((a, b))
        seen.add(x)
    return len(pairs)

# ============================
# Tests (top-level statements)
# ============================

tests = [
    ([1, 1, 2, 2, 3, 3], 4, 2, "{1,3} and {2,2}"),
    ([2, 7, 11, 15],     9, 1, "{2,7}"),
    ([3, 3, 3],          6, 1, "{3,3}"),
    ([1, 2, 3, 4],      10, 0, "none"),
    ([1, 5, 1, 5],       6, 1, "{1,5}")
]

print("=== Test: count_unique_pairs ===\n")

for numbers, tgt, expected, note in tests:
    result = count_unique_pairs(numbers, tgt)
    print(f"Input nums = {numbers}, target = {tgt}")
    print(f"Result:   {result}")
    print(f"Expected: {expected} ({note})\n")
