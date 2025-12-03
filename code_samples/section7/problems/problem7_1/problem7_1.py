"""Problem 7.1 - First Unique Character in a String."""

def first_uniq_char(s: str) -> int:
    """Find the first non-repeating character in a string."""
    freq: dict[str, int] = {}
    for ch in s:
        freq[ch] = freq.get(ch, 0) + 1
    for i, ch in enumerate(s):
        if freq[ch] == 1:
            return i
    return -1

# ============================
# Tests (top-level statements)
# ============================

tests = [
    "leetcode",       # expected 0
    "loveleetcode",   # expected 2
    "aabbcc",         # expected -1
    "",               # expected -1
    "x",              # expected 0
    "aabbc",          # expected 4
    "swiss"           # expected 1 ('w')
]

expected = [
    0,
    2,
    -1,
    -1,
    0,
    4,
    1
]

print("=== Testing first_uniq_char ===\n")

for val, exp in zip(tests, expected):
    result = first_uniq_char(val)
    print(f"Input: \"{val}\"")
    print(f"first_uniq_char -> {result} (expected {exp})\n")
