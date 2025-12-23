// ================================
// Original binary search
// ================================

/**
 * Performs a standard binary search on a sorted integer array.
 *
 * @param arr     Sorted array to search
 * @param target  Value to locate
 *
 * @return Index of the target if found; otherwise -1
 */
static int BinarySearch(int[] arr, int target)
{
    int lo = 0;                 // Lower bound of the search range (inclusive)
    int hi = arr.Length - 1;    // Upper bound of the search range (inclusive)

    // Continue searching while the range is valid
    while (lo <= hi)
    {
        // Compute midpoint safely to avoid integer overflow
        int mid = lo + (hi - lo) / 2;

        // Check if the midpoint element matches the target
        if (arr[mid] == target)
        {
            return mid;         // Target found
        }
        // Target is larger; discard left half
        else if (arr[mid] < target)
        {
            lo = mid + 1;
        }
        // Target is smaller; discard right half
        else
        {
            hi = mid - 1;
        }
    }

    // Target was not found in the array
    return -1;
}

// Same, but counts the number of loop iterations (steps)

/**
 * Performs a binary search while counting how many iterations occur.
 *
 * @param arr     Sorted array to search
 * @param target  Value to locate
 *
 * @return Tuple containing:
 *         - index: position of the target, or -1 if not found
 *         - steps: number of loop iterations performed
 */
static (int index, int steps) BinarySearchSteps(int[] arr, int target)
{
    int lo = 0;                 // Lower bound of search range
    int hi = arr.Length - 1;    // Upper bound of search range
    int steps = 0;              // Counts number of iterations performed

    // Continue searching while the range is valid
    while (lo <= hi)
    {
        steps++;  // Count this iteration / comparison step

        // Compute midpoint of the current search range
        int mid = lo + (hi - lo) / 2;

        // Check if midpoint matches the target
        if (arr[mid] == target)
            return (mid, steps);

        // Decide which half of the array to search next
        if (arr[mid] < target)
            lo = mid + 1;       // Target lies in the right half
        else
            hi = mid - 1;       // Target lies in the left half
    }

    // Target was not found after exhausting search range
    return (-1, steps);
}

// ================================
// Load ordered.txt using CURRENT DIRECTORY
// ================================

/**
 * Loads the ordered integer dataset from disk.
 *
 * The file path is resolved relative to the current working directory,
 * which depends on where the script is executed from.
 *
 * @return Array of integers read from ordered.txt,
 *         or an empty array on failure
 */
static int[] LoadOrderedFile()
{
    // When you run:
    //   cd code_samples\section12\lesson_3_binary_search
    //   dotnet script binary_search.csx
    //
    // Environment.CurrentDirectory will be:
    //   code_samples\section12\lesson_3_binary_search
    //
    // The data file lives at:
    //   ..\data\ordered.txt
    string relativePath = @"..\data\ordered.txt";

    // Resolve relative path into a full absolute path
    string fullPath = Path.GetFullPath(relativePath, Environment.CurrentDirectory);

    Console.WriteLine($"Attempting to read: {fullPath}");

    try
    {
        // Read entire file contents as a single string
        string text = File.ReadAllText(fullPath);

        // Split on any whitespace, parse tokens as integers,
        // and convert the result to an array
        return [.. text
            .Split((char[])null, StringSplitOptions.RemoveEmptyEntries)
            .Select(int.Parse)];
    }
    catch (Exception ex)
    {
        // Provide diagnostic output to help troubleshoot file issues
        Console.WriteLine("Error reading ordered.txt");
        Console.WriteLine(ex.Message);
        Console.WriteLine($"Working directory: {Environment.CurrentDirectory}");

        // Return empty array to signal failure
        return [];
    }
}

// ================================
// MAIN (top-level for dotnet script)
// ================================

// Load the ordered dataset from disk
var arr = LoadOrderedFile();

// Validate successful file load
if (arr.Length == 0)
{
    Console.WriteLine("Missing input file - aborting.");
    return; // Exit script early on failure
}

Console.WriteLine("=== Binary Search Tests ===");
Console.WriteLine($"Loaded {arr.Length} integers\n");

// Test 1: Search for the first element (best-case scenario)
var r = BinarySearchSteps(arr, arr[0]);
Console.WriteLine($"Search first  ({arr[0]}): index={r.index}, steps={r.steps}");

// Test 2: Search for the middle element
int midVal = arr[arr.Length / 2];
r = BinarySearchSteps(arr, midVal);
Console.WriteLine($"Search middle ({midVal}): index={r.index}, steps={r.steps}");

// Test 3: Search for the last element (worst-case successful search)
int lastVal = arr[arr.Length - 1];
r = BinarySearchSteps(arr, lastVal);
Console.WriteLine($"Search last   ({lastVal}): index={r.index}, steps={r.steps}");

// Test 4: Search for a value guaranteed not to exist
r = BinarySearchSteps(arr, 999_999);
Console.WriteLine($"Search missing (999999): index={r.index}, steps={r.steps}");
