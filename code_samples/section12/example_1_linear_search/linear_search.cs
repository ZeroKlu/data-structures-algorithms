// ======================================
// Given linear search
// ======================================

/**
 * Performs a basic linear search on an integer array.
 *
 * @param arr     Array to search
 * @param target  Value to locate
 *
 * @return Index of the target if found; otherwise -1
 */
static int LinearSearch(int[] arr, int target)
{
    // Iterate through the array sequentially
    for (int i = 0; i < arr.Length; i++)
    {
        // Check if the current element matches the target
        if (arr[i] == target)
        {
            return i; // Return index immediately when found
        }
    }

    // Target was not found in the array
    return -1;
}

// ======================================
// Same search, but counts comparison steps
// ======================================

/**
 * Performs a linear search while counting how many comparisons are made.
 *
 * @param arr     Array to search
 * @param target  Value to locate
 *
 * @return A tuple containing:
 *         - index: position of the target, or -1 if not found
 *         - steps: number of element comparisons performed
 */
static (int index, int steps) LinearSearchSteps(int[] arr, int target)
{
    int steps = 0; // Counts how many comparisons are performed

    // Iterate through each element in order
    for (int i = 0; i < arr.Length; i++)
    {
        steps++; // Count the comparison arr[i] == target

        // Check for a match
        if (arr[i] == target)
        {
            // Return both the index and the number of steps taken
            return (i, steps);
        }
    }

    // Target was not found after scanning entire array
    return (-1, steps);
}

// ======================================
// Load a file of integers relative to *current directory*
// ======================================

/**
 * Loads a text file containing integers separated by whitespace.
 *
 * The file path is resolved relative to the current working directory.
 *
 * @param relativePath  Relative path to the input file
 *
 * @return An array of integers read from the file, or an empty array on error
 */
static int[] LoadFile(string relativePath)
{
    try
    {
        // Resolve the relative path into a full absolute path
        string fullPath = Path.GetFullPath(relativePath, Environment.CurrentDirectory);

        // Read the entire file as a single string
        string text = File.ReadAllText(fullPath);

        // Split on any whitespace, parse each token as an integer,
        // and convert the result into an array
        return [.. text
            .Split((char[])null, StringSplitOptions.RemoveEmptyEntries)
            .Select(int.Parse)];
    }
    catch (Exception ex)
    {
        // Provide detailed diagnostics to help locate file path issues
        Console.WriteLine($"Error reading: {relativePath}");
        Console.WriteLine($"Resolved full path: {Path.GetFullPath(relativePath, Environment.CurrentDirectory)}");
        Console.WriteLine($"Working directory: {Environment.CurrentDirectory}");
        Console.WriteLine($"Exception: {ex.Message}");

        // Return an empty array to signal failure
        return [];
    }
}

// ======================================
// Run dataset tests
// ======================================

/**
 * Executes a standardized set of linear search tests on a dataset.
 *
 * @param name  Descriptive name of the dataset (used in output)
 * @param arr   Integer array containing the dataset
 */
static void RunTestsOnDataset(string name, int[] arr)
{
    Console.WriteLine($"=== {name} ===");
    Console.WriteLine($"Loaded {arr.Length} integers.\n");

    // Test 1: Search for the first element (best-case scenario)
    var r1 = LinearSearchSteps(arr, arr[0]);
    Console.WriteLine($"Search first  ({arr[0]}): index={r1.index}, steps={r1.steps}");

    // Test 2: Search for the middle element (average-case scenario)
    int midVal = arr[arr.Length / 2];
    var r2 = LinearSearchSteps(arr, midVal);
    Console.WriteLine($"Search middle ({midVal}): index={r2.index}, steps={r2.steps}");

    // Test 3: Search for the last element (worst-case successful search)
    int lastVal = arr[^1];
    var r3 = LinearSearchSteps(arr, lastVal);
    Console.WriteLine($"Search last   ({lastVal}): index={r3.index}, steps={r3.steps}");

    // Test 4: Search for a value guaranteed not to exist
    var (index, steps) = LinearSearchSteps(arr, 999999);
    Console.WriteLine($"Search missing (999999): index={index}, steps={steps}");

    Console.WriteLine();
}

// ======================================
// MAIN (top-level)
// ======================================

// Expected execution context:
//   From:  C:\...\code_samples\section12\lesson_2_linear_search
//   Data:  ..\data\ordered.txt and ..\data\unordered.txt

// Load datasets from disk using relative paths
var ordered   = LoadFile(@"..\data\ordered.txt");
var unordered = LoadFile(@"..\data\unordered.txt");

// Validate that both datasets were successfully loaded
if (ordered.Length == 0 || unordered.Length == 0)
{
    Console.WriteLine("Missing input files - see error details above.");
    return; // Exit program early on failure
}

// Run linear search benchmarks on each dataset
RunTestsOnDataset("Ordered Data", ordered);
RunTestsOnDataset("Unordered Data", unordered);
