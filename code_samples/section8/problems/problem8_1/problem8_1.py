"""Problem 8.1. Climbing Stairs."""

#pylint: disable=pointless-string-statement

def climb_stairs(n: int) -> int:
    """
    Compute the number of distinct ways to climb a staircase with n steps.

    At each step, you may climb either 1 step or 2 steps.
    This leads to the recurrence relation:

        ways(n) = ways(n - 1) + ways(n - 2)

    This implementation uses dynamic programming with constant space
    by storing only the two most recent results instead of a full table.
    """

    """
    Base cases:
    - n = 0 → 0 ways (by convention in this implementation)
    - n = 1 → 1 way
    - n = 2 → 2 ways (1+1, 2)
    """
    if n <= 2:
        return n

    """
    prev2 represents the number of ways to reach step (i - 2).
    Initialized to ways(1) = 1.

    prev1 represents the number of ways to reach step (i - 1).
    Initialized to ways(2) = 2.
    """
    prev2, prev1 = 1, 2

    """
    Iterate from step 3 up to step n.
    At each iteration, compute the number of ways to reach the
    current step using the previous two results.
    """
    for _ in range(3, n + 1):

        """
        The number of ways to reach the current step is the sum
        of the ways to reach the previous two steps.
        """
        cur = prev1 + prev2

        """
        Advance the rolling window:
        - prev2 takes the old prev1 value
        - prev1 takes the newly computed value
        """
        prev2, prev1 = prev1, cur

    """
    After the loop completes, prev1 contains the number of
    distinct ways to reach step n.
    """
    return prev1


# ============================
# Tests for climb_stairs
# ============================

if __name__ == "__main__":

    print("=== Test: climb_stairs ===\n")

    """
    Test cases include:
    - Edge cases (0, 1)
    - Small values for clarity
    - Larger values to demonstrate efficiency
    """
    tests    = [0, 1, 2, 3, 4, 5, 6, 10, 20]

    """
    Expected results corresponding to each test case.
    These values follow the Fibonacci-style progression
    defined by the problem.
    """
    expected = [0, 1, 2, 3, 5, 8, 13, 89, 10946]

    """
    Execute each test and print the computed result
    alongside the expected value for verification.
    """
    for num, exp in zip(tests, expected):
        result = climb_stairs(num)
        print(f"climb_stairs({num}) = {result} (expected {exp})")
