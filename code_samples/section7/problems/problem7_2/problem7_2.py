"""Problem 7.2 - Group Anagrams."""

from collections import defaultdict

def group_anagrams(strs: list[str]) -> list[list[str]]:
    """Group anagrams."""
    groups: defaultdict[str, list[str]] = defaultdict(list)
    for s in strs:
        key = "".join(sorted(s))
        groups[key].append(s)
    return list(groups.values())

# ============================
# Tests (top-level statements)
# ============================

input_words = ["eat", "tea", "tan", "ate", "nat", "bat"]

print("=== Test: group_anagrams ===\n")
print("Input:", input_words, "\n")

anagrams = group_anagrams(input_words)

print("Anagram Groups:")
for g in anagrams:
    print("  [" + ", ".join(g) + "]")

print("\nExpected groups (order may vary):")
print("  [eat, tea, ate]")
print("  [tan, nat]")
print("  [bat]")
