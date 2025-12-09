"""Example 8: Merge sort."""

# def merge_sort(arr: list[int]) -> list[int]:
#     """Merge sort."""
#     if len(arr) <= 1:
#         return arr[:]
#     mid = len(arr) // 2
#     left = merge_sort(arr[:mid])
#     right = merge_sort(arr[mid:])
#     return _merge(left, right)

# def _merge(left: list[int], right: list[int]) -> list[int]:
#     result: list[int] = []
#     i = j = 0
#     while i < len(left) and j < len(right):
#         if left[i] <= right[j]:
#             result.append(left[i])
#             i += 1
#         else:
#             result.append(right[j])
#             j += 1
#     result.extend(left[i:])
#     result.extend(right[j:])
#     return result

import os

#pylint: disable=global-statement
#pylint: disable=broad-exception-caught

comparisons = 0
writes = 0

# ------------------------------------------------------------
# Merge Sort Algorithm
# ------------------------------------------------------------

def merge_sort(arr: list[int]) -> list[int]:
    """Merge sort with step counting."""
    if len(arr) <= 1:
        return arr[:]  # copy

    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])
    return _merge(left, right)


def _merge(left: list[int], right: list[int]) -> list[int]:
    """Merge two sorted lists using global step counters."""
    global comparisons, writes
    result: list[int] = []
    i = j = 0

    while i < len(left) and j < len(right):
        comparisons += 1   # compare left[i] <= right[j]
        if left[i] <= right[j]:
            result.append(left[i])   # write
            writes += 1
            i += 1
        else:
            result.append(right[j])  # write
            writes += 1
            j += 1

    # extend remainder from left
    if i < len(left):
        result.extend(left[i:])
        writes += len(left) - i

    # extend remainder from right
    if j < len(right):
        result.extend(right[j:])
        writes += len(right) - j

    return result


# ------------------------------------------------------------
# File Loading Utilities
# ------------------------------------------------------------

def load_file(file_name: str) -> list[int]:
    """
    Tries:
        - ../data/<file_name>
        - Desktop/data-structures-algorithms/code_samples/section12/data/<file_name>
    """
    cwd = os.getcwd()
    path1 = os.path.abspath(os.path.join(cwd, "..", "data", file_name))
    print(f"Attempting to read (relative): {path1}")

    if os.path.exists(path1):
        return read_int_file(path1)

    # explicit backup path for your environment
    desktop = os.path.join(os.path.expanduser("~"), "Desktop")
    path2 = os.path.join(desktop,
        "data-structures-algorithms",
        "code_samples",
        "section12",
        "data",
        file_name)

    print(f"Relative path not found, trying explicit path: {path2}")

    if os.path.exists(path2):
        return read_int_file(path2)

    raise FileNotFoundError(f"Missing {file_name}. Tried:\n  {path1}\n  {path2}")


def read_int_file(full_path: str) -> list[int]:
    """Load integers from file."""
    try:
        print(f"Reading file: {full_path}")
        with open(full_path, "r", encoding="utf-8") as f:
            text = f.read()
        nums = [int(x) for x in text.split()]
        return nums
    except Exception as ex:
        print(f"Error reading: {full_path}")
        print(ex)
        raise


# ------------------------------------------------------------
# Test Harness
# ------------------------------------------------------------

if __name__ == "__main__":
    try:
        unordered = load_file("unordered.txt")
        expected = load_file("ordered.txt")

        comparisons = 0
        writes = 0

        sorted_result = merge_sort(unordered)

        ok = sorted_result == expected
        print(f"\nCorrectly sorted? {ok}")

        print(f"Comparisons: {comparisons}")
        print(f"Writes:      {writes}")

        print("\nFirst 10 sorted values:")
        print(sorted_result[:10])

    except Exception as e:
        print("Missing input file — aborting.")
        print(e)
