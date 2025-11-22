"""Section 2 - Arrays & Strings"""

# Note: In python, there is a built-in sum() function
def sum_array(arr: list[int]) -> int:
    """Return the sum of the values in the array."""
    total = 0
    for x in arr:
        total += x
    return total

# Note: In python, there is a built-in reverse() function
def reverse_array(arr: list[int]) -> None:
    """Reverse the order of the values in the array."""
    left = 0
    right = len(arr) - 1
    while left < right:
        arr[left], arr[right] = arr[right], arr[left]
        left += 1
        right -= 1

def has_pair_with_sum(arr: list[int], target: int) -> bool:
    """Return True if the array contains two values that sum to the target."""
    left = 0
    right = len(arr) - 1
    while left < right:
        s = arr[left] + arr[right]
        if s == target:
            return True
        if s < target:
            left += 1
        else:
            right -= 1
    return False

def reverse_string(s: str) -> str:
    """Return the reverse of the string."""
    return s[::-1]

lst = [1, 2, 3, 4, 5]
print("Sum of array:", sum_array(lst))
reverse_array(lst)
print("Reversed array:", lst)
reverse_array(lst)
print("Has pair with sum 9:", has_pair_with_sum(lst, 9))
print("Reversed string:", reverse_string("hello"))
