"""
Problem 7.4 - Subarray Sum Equals K.

This module implements an efficient solution to count how many contiguous
subarrays sum to a given value k, using prefix sums and a hash table.
"""

def subarray_sum(nums: list[int], k: int) -> int:
    """
    Counts the number of contiguous subarrays whose sum equals k.

    Definition:
        A subarray is a contiguous slice of the array. The task is to count
        how many such slices have a total sum exactly equal to k.

    Args:
        nums:
            List of integers (may be empty and may contain negative values).
        k:
            Target sum.

    Returns:
        Number of contiguous subarrays whose sum is exactly k.

    Core idea (prefix sums + frequency map):
        Let prefix be the running sum of elements seen so far.

        The sum of a subarray from index (j+1) to i is:
            prefix[i] - prefix[j]

        We want:
            prefix[i] - prefix[j] == k
        Rearranging:
            prefix[j] == prefix[i] - k

        So, for each new prefix sum:
          - Count how many times (prefix - k) has appeared before.
          - Add that count to the answer.
          - Record the current prefix sum in a frequency dictionary.

    Why freq starts with {0: 1}:
        - This represents an "empty prefix" before reading any elements.
        - It allows subarrays that start at index 0 to be counted correctly
          when prefix == k.

    Complexity (expected / average):
        - Time:  O(n)
            Single pass through the array; dictionary lookups are O(1) on average.
        - Space: O(n)
            In the worst case, all prefix sums are distinct.

    Notes:
        - Works correctly with negative numbers (unlike sliding window approaches).
        - Python integers do not overflow, so no special handling is required.
    """
    count = 0           # Total number of valid subarrays found
    prefix = 0          # Running prefix sum

    # Frequency map: prefix_sum -> number of times it has appeared so far
    freq: dict[int, int] = {0: 1}

    # Process each element in the array
    for x in nums:
        # Update running sum
        prefix += x

        # We are looking for prior prefix sums equal to (prefix - k)
        need = prefix - k

        # Add how many times that needed prefix sum has appeared
        count += freq.get(need, 0)

        # Record that we've now seen this prefix sum one more time
        freq[prefix] = freq.get(prefix, 0) + 1

    return count


# ============================
# Tests (top-level statements)
# ============================
#
# These statements execute immediately when the module is run.
# No explicit main() function is required.
#

tests = [
    ([1, 1, 1], 2, 2),                 # expected 2: [1,1] twice
    ([1, 2, 3], 3, 2),                 # expected 2: [1,2], [3]
    ([3, 4, 7, 2, -3, 1, 4, 2], 7, 4), # expected 4
    ([], 0, 0),                        # expected 0 (empty input)
]

print("=== Test: subarray_sum ===\n")

# Run each test case and compare the result with the expected value
for numbers, k_value, expected in tests:
    result = subarray_sum(numbers, k_value)
    print(f"Input: nums={numbers}, k={k_value}")
    print(f"Result: {result} (expected {expected})\n")
