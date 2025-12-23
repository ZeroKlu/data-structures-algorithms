#nullable enable

// ============================================================
// File Loading Helper
// ============================================================
//
// Loads a whitespace-delimited list of integers from a file located at:
//   <working directory>\..\data\<filename>
//
// Why this path logic?
// - In these lessons, scripts are typically run from a "lesson" folder.
// - The shared data folder lives one level up in: ..\data\
// - Using Directory.GetCurrentDirectory() makes the behavior depend on
//   where you *run* the script from (not where the script file lives).
//
// Return value:
// - int[] of parsed integers on success
// - null on failure (file missing, unreadable, parse error, etc.)
//
static int[]? LoadFile(string filename)
{
    try
    {
        // Current working directory (the folder you ran the script from)
        string workDir = Directory.GetCurrentDirectory();

        // Build an absolute path to ..\data\<filename>
        // Example:
        //   workDir = C:\...\code_samples\section12\lesson_X_selection_sort
        //   full    = C:\...\code_samples\section12\data\unordered.txt
        string full = Path.GetFullPath(Path.Combine(workDir, "..", "data", filename));

        // Helpful diagnostic: prints the exact path being attempted
        Console.WriteLine($"Attempting to read: {full}");

        // Read entire file contents as one string
        string text = File.ReadAllText(full);

        // Split on any whitespace (spaces, tabs, newlines),
        // parse each token as an int, and return as an array.
        //
        // Notes:
        // - Split((char[])null!, ...) means "split on all whitespace"
        // - The list/array spread syntax ([.. ...]) builds a new array
        //   from the LINQ result.
        return [.. text
            .Split((char[])null!, StringSplitOptions.RemoveEmptyEntries)
            .Select(int.Parse)];
    }
    catch (Exception ex)
    {
        // If anything goes wrong (missing file, permission, parsing, etc.)
        // print diagnostics and return null so the caller can abort cleanly.
        Console.WriteLine($"Error reading: {filename}");
        Console.WriteLine(ex.Message);
        return null;
    }
}

// ============================================================
// Selection Sort with Step Counting
// ============================================================
//
// Selection sort idea:
// - For each position i, find the smallest element in the range [i..n-1].
// - Swap that smallest element into position i.
// - Repeat until the array is sorted.
//
// Step counting:
// - comparisons: increments once for each comparison of arr[j] < arr[minIdx]
// - swaps: increments once for each actual swap performed
//
// Behavior:
// - Sorts the array IN PLACE (modifies the original array).
//
static void SelectionSort(int[] arr, out long comparisons, out long swaps)
{
    // Initialize counters
    comparisons = 0;
    swaps = 0;

    int n = arr.Length;

    // Outer loop picks the next position i to fill with the minimum
    for (int i = 0; i < n; i++)
    {
        // Assume the first unsorted element is the minimum
        int minIdx = i;

        // Scan the unsorted suffix to find the true minimum element
        for (int j = i + 1; j < n; j++)
        {
            comparisons++; // count every comparison
            if (arr[j] < arr[minIdx])
            {
                // Found a new minimum candidate
                minIdx = j;
            }
        }

        // Swap only if we found a smaller element elsewhere
        if (minIdx != i)
        {
            // Tuple swap is a concise way to swap values in C#
            (arr[minIdx], arr[i]) = (arr[i], arr[minIdx]);
            swaps++; // count actual swaps
        }
    }
}

// ============================================================
// MAIN (top-level statements)
// ============================================================
//
// Workflow:
//  1) Load unordered.txt (unsorted input)
//  2) Load ordered.txt   (reference "correct sorted" output)
//  3) Run selection sort on unordered data and count steps
//  4) Compare sorted output to ordered reference and report mismatches
//

// Load the unsorted dataset
var unordered = LoadFile("unordered.txt");
if (unordered is null)
{
    Console.WriteLine("Missing unordered file - aborting.");
    return;
}

// Load the expected sorted dataset (reference for verification)
var ordered = LoadFile("ordered.txt");
if (ordered is null)
{
    Console.WriteLine("Missing ordered file - aborting.");
    return;
}

// Sanity check: both files should contain the same number of integers
if (unordered.Length != ordered.Length)
{
    Console.WriteLine("Length mismatch between files.");
    return;
}

Console.WriteLine("\n--- Selection Sort (Step Counting) ---");

// Sort in place, capturing the number of comparisons and swaps
SelectionSort(unordered, out long comps, out long swaps);

// Print sorting statistics
Console.WriteLine($"Comparisons: {comps}");
Console.WriteLine($"Swaps:       {swaps}");

Console.WriteLine("\nComparing result to ordered.txt...");

// Verify correctness by comparing the sorted output to ordered.txt
int mismatches = 0;

for (int i = 0; i < unordered.Length; i++)
{
    if (unordered[i] != ordered[i])
    {
        // Print only the first few mismatches to keep output readable
        if (mismatches < 10)
        {
            Console.WriteLine($"Mismatch @ {i}: got {unordered[i]}, expected {ordered[i]}");
        }
        mismatches++;
    }
}

// Final verdict
if (mismatches == 0)
    Console.WriteLine("SUCCESS: Sorted output matches ordered.txt");
else
    Console.WriteLine($"FAIL: {mismatches} mismatches.");
