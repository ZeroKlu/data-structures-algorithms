"""Problem 7.3. - Longest Consecutive Sequence."""

def longest_consecutive(nums: list[int]) -> int:
    """Find the length of the longest consecutive sequence in an array."""
    s = set(nums)
    best = 0
    for x in s:
        if x - 1 not in s:
            cur = x
            length = 1
            while cur + 1 in s:
                cur += 1
                length += 1
            best = max(best, length)
    return best

# ============================
# Tests (top-level statements)
# ============================

tests = [
    [100, 4, 200, 1, 3, 2],             # expected 4
    [0, 3, 7, 2, 5, 8, 4, 6, 0, 1],      # expected 9
    [1, 2, 0, 1],                       # expected 3
    [],                                 # expected 0
    [-2, -1, 0, 2, 3, 4, 5]             # expected 4
]

expected = [4, 9, 3, 0, 4]

print("=== Test: longest_consecutive ===\n")

for numbers, exp in zip(tests, expected):
    result = longest_consecutive(numbers)
    print(f"Input: {numbers}")
    print(f"Longest consecutive length: {result} (expected {exp})\n")
