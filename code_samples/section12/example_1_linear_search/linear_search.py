"""Linear search with tests."""

import os  # Provides file system and path utilities

# Disable pylint warning for catching broad exceptions in file loading
# (used here to provide detailed diagnostics on any file-related error)
# pylint: disable=broad-exception-caught


def linear_search(arr: list[int], target: int) -> int:
    """
    Perform a basic linear search on a list of integers.

    :param arr: List of integers to search
    :param target: Value to locate
    :return: Index of the target if found; otherwise -1
    """
    # Iterate through the list with both index and value
    for i, value in enumerate(arr):
        # Check whether the current value matches the target
        if value == target:
            return i  # Return index immediately when found

    # Target was not found in the list
    return -1


def linear_search_steps(arr: list[int], target: int) -> tuple[int, int]:
    """
    Perform a linear search while counting comparison steps.

    :param arr: List of integers to search
    :param target: Value to locate
    :return: Tuple containing:
             - index: position of the target, or -1 if not found
             - steps: number of comparisons performed
    """
    steps = 0  # Counts how many element comparisons are made

    # Iterate through the list with index and value
    for i, value in enumerate(arr):
        steps += 1  # Count the comparison value == target

        # Check for a match
        if value == target:
            return i, steps  # Return index and step count

    # Target was not found after scanning entire list
    return -1, steps


def load_file(relative_path: str) -> list[int]:
    """
    Load integers from a text file relative to the script location.

    :param relative_path: Relative path to the input data file
    :return: List of integers read from the file,
             or an empty list on error
    """
    # Determine the directory where this script resides
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # Build a full absolute path to the data file
    full_path = os.path.join(script_dir, relative_path)

    try:
        # Open the file and read its entire contents as text
        with open(full_path, "r", encoding="utf-8") as f:
            text = f.read()

        # Split on whitespace and convert each token to an integer
        return [int(x) for x in text.split()]

    except Exception as e:
        # Provide detailed diagnostics to help locate file path issues
        print(f"Error reading: {full_path}")
        print("Working directory:", os.getcwd())
        print("Exception:", e)

        # Return empty list to signal failure
        return []


def run_dataset(name: str, arr: list[int]) -> None:
    """
    Run a standard set of linear search tests on a dataset.

    :param name: Descriptive name of the dataset (used in output)
    :param arr: List of integers containing the dataset
    """
    print(f"=== {name} ===")
    print(f"Loaded {len(arr)} integers\n")

    # Test 1: Search for the first value (best-case scenario)
    idx, steps = linear_search_steps(arr, arr[0])
    print(f"Search first ({arr[0]}): index={idx}, steps={steps}")

    # Test 2: Search for the middle value (average-case scenario)
    mid_value = arr[len(arr) // 2]
    idx, steps = linear_search_steps(arr, mid_value)
    print(f"Search middle ({mid_value}): index={idx}, steps={steps}")

    # Test 3: Search for the last value (worst-case successful search)
    last_value = arr[-1]
    idx, steps = linear_search_steps(arr, last_value)
    print(f"Search last ({last_value}): index={idx}, steps={steps}")

    # Test 4: Search for a value guaranteed not to exist
    idx, steps = linear_search_steps(arr, 999_999)
    print(f"Search missing (999999): index={idx}, steps={steps}\n")


def main() -> None:
    """
    Program entry point.

    Loads ordered and unordered datasets from disk,
    then runs linear search tests on each.
    """
    # Data directory lives at: ../data/ relative to this script
    ordered   = load_file("../data/ordered.txt")
    unordered = load_file("../data/unordered.txt")

    # Validate that both datasets were loaded successfully
    if not ordered or not unordered:
        print("Missing input files - see error messages above.")
        return  # Exit early on failure

    # Run linear search benchmarks on each dataset
    run_dataset("Ordered Data", ordered)
    run_dataset("Unordered Data", unordered)


# Execute main() only when this file is run directly
if __name__ == "__main__":
    main()
