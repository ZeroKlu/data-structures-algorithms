"""
Problem 7.2 - Group Anagrams.

This module implements a hash-table-based solution for grouping strings
that are anagrams of each other, along with a simple test harness.
"""

from collections import defaultdict


def group_anagrams(strs: list[str]) -> list[list[str]]:
    """
    Groups strings that are anagrams of each other.

    Definition:
        Two strings are anagrams if they contain the same characters with
        the same frequencies, possibly in a different order.

    Args:
        strs:
            List of input strings to group.

    Returns:
        A list of groups, where each group is a list of strings that are
        anagrams of one another.

    Approach:
        - Use a defaultdict(list) to collect strings by a canonical key.
        - The canonical key is the sorted version of each string.
          All anagrams share the same sorted form.

    Example:
        "eat" -> sorted("eat") -> "aet"
        "tea" -> sorted("tea") -> "aet"  (same group)

    Complexity:
        Let:
            n = number of strings
            k = average string length

        - Time:  O(n · k log k)
            Sorting each string dominates the runtime.
        - Space: O(n · k)
            For storing keys and grouped strings.

    Notes:
        - The order of groups and the order of strings within each group
          is not guaranteed and may vary.
        - If deterministic ordering is required, you can sort the result
          or sort each group after grouping.
    """
    # defaultdict automatically creates an empty list for new keys
    groups: defaultdict[str, list[str]] = defaultdict(list)

    # Process each string and place it into its anagram group
    for s in strs:
        # Create canonical key by sorting characters
        key = "".join(sorted(s))
        groups[key].append(s)

    # Convert grouped values into a list of lists
    return list(groups.values())


# ============================
# Tests (top-level statements)
# ============================
#
# These statements execute immediately when the module is run.
#

input_words = ["eat", "tea", "tan", "ate", "nat", "bat"]

print("=== Test: group_anagrams ===\n")
print("Input:", input_words, "\n")

# Run the function under test
anagrams = group_anagrams(input_words)

# Print grouped anagrams
print("Anagram Groups:")
for g in anagrams:
    print("  [" + ", ".join(g) + "]")

# Expected output guidance
print("\nExpected groups (order may vary):")
print("  [eat, tea, ate]")
print("  [tan, nat]")
print("  [bat]")
