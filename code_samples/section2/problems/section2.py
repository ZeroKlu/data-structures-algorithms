"""Section 2 - Problems"""

def reverse_array(arr: list[int]) -> None:
    """Reverses an array in place."""
    i = 0
    j = len(arr) - 1
    while i < j:
        arr[i], arr[j] = arr[j], arr[i]
        i += 1
        j -= 1

def two_sum_bruteforce(nums: list[int], target: int) -> bool:
    """Brute force O(n^2)"""
    n = len(nums)
    for i in range(n):
        for j in range(i + 1, n):
            if nums[i] + nums[j] == target:
                return True
    return False

def two_sum_hash(nums: list[int], target: int) -> bool:
    """Hash table O(n)"""
    seen: set[int] = set()
    for x in nums:
        need = target - x
        if need in seen:
            return True
        seen.add(x)
    return False

def move_zeroes(nums: list[int]) -> None:
    """Move zeroes to the end."""
    write = 0
    #pylint: disable=consider-using-enumerate
    for read in range(len(nums)):
        if nums[read] != 0:
            nums[write] = nums[read]
            write += 1
    while write < len(nums):
        nums[write] = 0
        write += 1

def is_anagram(s: str, t: str) -> bool:
    """Check if two strings are anagrams."""
    if len(s) != len(t):
        return False
    freq = [0] * 26
    for ch_s, ch_t in zip(s, t):
        freq[ord(ch_s) - ord('a')] += 1
        freq[ord(ch_t) - ord('a')] -= 1
    return all(f == 0 for f in freq)

def length_of_longest_substring(s: str) -> int:
    """Find the length of the longest substring without repeating characters."""
    last_pos: dict[str, int] = {}
    left = 0
    best = 0
    for right, ch in enumerate(s):
        if ch in last_pos and last_pos[ch] >= left:
            left = last_pos[ch] + 1
        last_pos[ch] = right
        window_len = right - left + 1
        best = max(best, window_len)
    return best

def rotate(nums: list[int], k: int) -> None:
    """Rotate the array k positions."""
    n = len(nums)
    if n == 0:
        return
    k %= n
    if k == 0:
        return

    reverse_range(nums, 0, n - 1)
    reverse_range(nums, 0, k - 1)
    reverse_range(nums, k, n - 1)

def reverse_range(nums: list[int], n: int, r: int) -> None:
    """Reverse a range of the array."""
    while n < r:
        nums[n], nums[r] = nums[r], nums[n]
        n += 1
        r -= 1

lst = [1, 2, 3, 4, 5]

reverse_array(lst)
print(f"Reversed array: {lst}")
reverse_array(lst)

print(f"Has pair with sum 9 (brute force): {two_sum_bruteforce(lst, 9)}")
print(f"Has pair with sum 9 (hash table): {two_sum_hash(lst, 9)}")

ints = [0, 1, 0, 3, 12]
move_zeroes(ints)
print(f"Moved zeroes: {ints}")

s1 = "listen"
s2 = "silent"
print(f"Is \"{s1}\" an anagram of \"{s2}\": {is_anagram(s1, s2)}")

#pylint: disable=line-too-long
print(f"Length of longest substring without repeating characters: {length_of_longest_substring('abcabcbb')}")

rotate(lst, 2)
print(f"Array rotated 2 positions: {lst}")
