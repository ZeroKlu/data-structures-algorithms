"""Linear search with tests."""

import os

#pylint: disable=broad-exception-caught

def linear_search(arr: list[int], target: int) -> int:
    """Basic linear search."""
    for i, value in enumerate(arr):
        if value == target:
            return i
    return -1


def linear_search_steps(arr: list[int], target: int) -> tuple[int, int]:
    """Linear search that returns (index, steps)."""
    steps = 0
    for i, value in enumerate(arr):
        steps += 1
        if value == target:
            return i, steps
    return -1, steps


def load_file(relative_path: str) -> list[int]:
    """Load integers from file relative to script location."""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    full_path = os.path.join(script_dir, relative_path)

    try:
        with open(full_path, "r", encoding="utf-8") as f:
            text = f.read()
        return [int(x) for x in text.split()]
    except Exception as e:
        print(f"Error reading: {full_path}")
        print("Working directory:", os.getcwd())
        print("Exception:", e)
        return []


def run_dataset(name: str, arr: list[int]) -> None:
    """Run a dataset."""
    print(f"=== {name} ===")
    print(f"Loaded {len(arr)} integers\n")

    # First value
    idx, steps = linear_search_steps(arr, arr[0])
    print(f"Search first ({arr[0]}): index={idx}, steps={steps}")

    # Middle value
    mid_value = arr[len(arr) // 2]
    idx, steps = linear_search_steps(arr, mid_value)
    print(f"Search middle ({mid_value}): index={idx}, steps={steps}")

    # Last value
    last_value = arr[-1]
    idx, steps = linear_search_steps(arr, last_value)
    print(f"Search last ({last_value}): index={idx}, steps={steps}")

    # Missing value
    idx, steps = linear_search_steps(arr, 999_999)
    print(f"Search missing (999999): index={idx}, steps={steps}\n")


def main() -> None:
    """Run tests."""
    # Data folder lives at:  ../data/
    ordered   = load_file("../data/ordered.txt")
    unordered = load_file("../data/unordered.txt")

    if not ordered or not unordered:
        print("Missing input files — see error messages above.")
        return

    run_dataset("Ordered Data", ordered)
    run_dataset("Unordered Data", unordered)


if __name__ == "__main__":
    main()
