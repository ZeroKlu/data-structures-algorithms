"""Lesson 3 - Binary Search."""

import os  # Provides operating system and file path utilities

# Disable pylint warning for catching broad exceptions
# (used here to give clear diagnostics when file loading fails)
# pylint: disable=broad-exception-caught


def binary_search(arr: list[int], target: int) -> int:
    """
    Perform a standard binary search on a sorted list of integers.

    :param arr: Sorted list of integers to search
    :param target: Value to locate
    :return: Index of the target if found; otherwise -1
    """
    lo, hi = 0, len(arr) - 1  # Initialize search bounds

    # Continue searching while the search range is valid
    while lo <= hi:
        # Compute midpoint safely using integer division
        mid = lo + (hi - lo) // 2

        # Check if the midpoint value matches the target
        if arr[mid] == target:
            return mid

        # Decide which half of the list to continue searching
        if arr[mid] < target:
            lo = mid + 1  # Target lies in the right half
        else:
            hi = mid - 1  # Target lies in the left half

    # Target was not found in the list
    return -1


def binary_search_steps(arr: list[int], target: int) -> tuple[int, int]:
    """
    Perform a binary search while counting the number of iterations.

    :param arr: Sorted list of integers to search
    :param target: Value to locate
    :return: Tuple containing:
             - index: position of the target, or -1 if not found
             - steps: number of loop iterations performed
    """
    lo, hi = 0, len(arr) - 1  # Initialize search bounds
    steps = 0                # Counts number of iterations

    # Continue searching while the search range is valid
    while lo <= hi:
        steps += 1  # Count this iteration / comparison step

        # Compute midpoint of the current search range
        mid = lo + (hi - lo) // 2

        # Check for a match at the midpoint
        if arr[mid] == target:
            return mid, steps

        # Decide which half of the list to search next
        if arr[mid] < target:
            lo = mid + 1  # Target lies in the right half
        else:
            hi = mid - 1  # Target lies in the left half

    # Target was not found after exhausting the search range
    return -1, steps


def load_ordered_file() -> list[int]:
    """
    Load ordered.txt located at:
        code_samples/section12/data/ordered.txt
    relative to this script's directory.

    :return: List of integers read from ordered.txt,
             or an empty list on failure
    """
    # Determine the directory containing this script file
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # Build a relative path that moves up one directory into section12/,
    # then into the data/ directory to locate ordered.txt
    file_path = os.path.join(script_dir, "..", "data", "ordered.txt")
    file_path = os.path.abspath(file_path)

    # Print resolved path for diagnostic purposes
    print(f"Attempting to read: {file_path}")

    try:
        # Open the file and read its entire contents as text
        with open(file_path, "r", encoding="utf-8") as f:
            text = f.read()

        # Split on whitespace and convert each token to an integer
        return [int(x) for x in text.split()]

    except Exception as e:
        # Provide diagnostic output if file loading fails
        print("Error reading ordered.txt")
        print("Working directory:", os.getcwd())
        print("Exception:", e)

        # Return empty list to signal failure
        return []


def run_tests(arr: list[int]) -> None:
    """
    Execute a standardized set of binary search tests on a dataset.

    :param arr: Sorted list of integers to test
    """
    print("\n=== Binary Search Tests ===")
    print(f"Loaded {len(arr)} integers\n")

    # Test 1: Search for the first element (best-case scenario)
    idx, steps = binary_search_steps(arr, arr[0])
    print(f"Search first ({arr[0]}): index={idx}, steps={steps}")

    # Test 2: Search for the middle element
    mid_val = arr[len(arr) // 2]
    idx, steps = binary_search_steps(arr, mid_val)
    print(f"Search middle ({mid_val}): index={idx}, steps={steps}")

    # Test 3: Search for the last element (worst-case successful search)
    idx, steps = binary_search_steps(arr, arr[-1])
    print(f"Search last ({arr[-1]}): index={idx}, steps={steps}")

    # Test 4: Search for a value guaranteed not to exist
    idx, steps = binary_search_steps(arr, 999_999)
    print(f"Search missing (999999): index={idx}, steps={steps}")


def main() -> None:
    """
    Program entry point.

    Loads the ordered dataset from disk and runs binary search tests.
    """
    # Load ordered dataset
    arr = load_ordered_file()

    # Validate successful file load
    if not arr:
        print("Missing input file - aborting.")
        return  # Exit early on failure

    # Run binary search benchmarks
    run_tests(arr)


# Execute main() only when this script is run directly
if __name__ == "__main__":
    main()
