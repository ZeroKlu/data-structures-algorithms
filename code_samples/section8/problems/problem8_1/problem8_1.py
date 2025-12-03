"""Problem 8.1. Climbing Stairs."""

def climb_stairs(n: int) -> int:
    """Climb stairs."""
    if n <= 2:
        return n
    prev2, prev1 = 1, 2
    for _ in range(3, n + 1):
        cur = prev1 + prev2
        prev2, prev1 = prev1, cur
    return prev1

# ============================
# Tests for climb_stairs
# ============================

if __name__ == "__main__":

    print("=== Test: climb_stairs ===\n")

    tests    = [0, 1, 2, 3, 4, 5, 6, 10, 20]
    expected = [0, 1, 2, 3, 5, 8, 13, 89, 10946]

    for num, exp in zip(tests, expected):
        result = climb_stairs(num)
        print(f"climb_stairs({num}) = {result} (expected {exp})")
