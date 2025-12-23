// ------------------------------------------------------------
// Example 10 - Quick Sort (C#)
// ------------------------------------------------------------
//
// PURPOSE
// -------
// This script demonstrates Quick Sort (Lomuto-style partitioning) on an array
// of integers, while counting "steps" to illustrate time complexity.
//
// It loads two files that should contain the SAME integers:
//   - unordered.txt : numbers in random/unsorted order
//   - ordered.txt   : the same numbers sorted ascending
//
// The program:
//   1) loads both files (using robust path searching)
//   2) sorts a COPY of unordered[] with Quick Sort
//   3) compares the sorted output to ordered[]
//   4) prints step counts (comparisons + writes) and correctness
//
// INPUT FORMAT
// ------------
// Files should contain integers separated by whitespace (spaces/newlines/tabs).
//
// STEP COUNTING MODEL
// -------------------
// We count two kinds of operations:
//
//   comparisons:
//     - incremented once per "arr[j] < pivot" comparison inside Partition()
//
//   writes:
//     - we count swaps as 3 writes (teaching approximation), because swapping
//       commonly uses three assignments:
//           tmp = a
//           a = b
//           b = tmp
//
// NOTE ON PIVOT CHOICE
// --------------------
// This implementation uses the last element as the pivot: pivot = arr[right].
// That is easy to teach, but can degrade to O(n^2) if the data is already
// sorted or nearly sorted. (Random pivot or median-of-three are common
// improvements in real-world code.)

// ------------------------------------------------------------
// Step counters (global for simplicity)
// ------------------------------------------------------------
//
// Keeping these at top-level makes it easy to reuse them across
// QuickSortRec() and Partition() without passing them around.
long comparisons = 0;
long writes = 0;

// ------------------------------------------------------------
// Quick Sort Implementation (Lomuto partition scheme)
// ------------------------------------------------------------

/// <summary>
/// Public wrapper: sorts the entire array in place.
/// (Caller typically passes a COPY of data to keep original input intact.)
/// </summary>
void QuickSort(int[] arr)
{
    // If the array is empty, right becomes -1, and the recursion will exit immediately.
    QuickSortRec(arr, 0, arr.Length - 1);
}

/// <summary>
/// Recursively sorts arr[left..right] (inclusive).
///
/// Base case:
///   - if left >= right, range has 0 or 1 element and is already sorted.
///
/// Recursive case:
///   1) partition around a pivot and get pivot index p
///   2) sort left partition  [left .. p-1]
///   3) sort right partition [p+1 .. right]
/// </summary>
void QuickSortRec(int[] arr, int left, int right)
{
    if (left >= right) return;

    int p = Partition(arr, left, right);
    QuickSortRec(arr, left, p - 1);
    QuickSortRec(arr, p + 1, right);
}

/// <summary>
/// Partition arr[left..right] using Lomuto scheme:
///
/// - pivot = arr[right] (last element)
/// - i tracks the boundary of the "less than pivot" region
/// - j scans from left to right-1
///     if arr[j] < pivot:
///        swap arr[i] and arr[j]
///        i++
/// - finally swap pivot into position i
///
/// Returns:
///   - final pivot index i
///
/// Step counting:
///   - comparisons++ for each arr[j] < pivot check
///   - writes += 3 for each swap (3 assignments approximation)
/// </summary>
int Partition(int[] arr, int left, int right)
{
    int pivot = arr[right]; // pivot value (last element)
    int i = left;           // boundary for "values < pivot"

    // Scan everything except the pivot
    for (int j = left; j < right; j++)
    {
        comparisons++; // we are about to evaluate: arr[j] < pivot

        // NOTE:
        // Using < pivot (not <= pivot) means duplicates equal to pivot
        // will stay on the right side. Either is valid, but affects
        // stability/partition balance for duplicates.
        if (arr[j] < pivot)
        {
            // Swap arr[i] and arr[j] to expand the "< pivot" region
            (arr[j], arr[i]) = (arr[i], arr[j]);

            // Count a swap as 3 writes (tmp, arr[i], arr[j])
            writes += 3;

            i++;
        }
    }

    // Put pivot into its final correct place
    (arr[right], arr[i]) = (arr[i], arr[right]);
    writes += 3;

    return i;
}

// ------------------------------------------------------------
// File Loader with Robust Path Searching
// ------------------------------------------------------------
//
// Why path searching?
// Students often run scripts from different working directories.
// These attempts reduce "file not found" headaches.
///
/// Strategy:
///  - build a list of candidate full paths
///  - return the first one that exists
///  - print the chosen path
///  - if none exist, print diagnostics and exit
int[] LoadFile(string filename)
{
    string cwd = Environment.CurrentDirectory;

    // Candidate locations, relative to where the program is run.
    // Adjust/add patterns here if your folder structure changes.
    string[] searchPaths =
    {
        Path.Combine(cwd, filename),                      // ./unordered.txt
        Path.Combine(cwd, "data", filename),              // ./data/unordered.txt
        Path.Combine(cwd, "..", "data", filename),        // ../data/unordered.txt (common)
        Path.Combine(cwd, "..", "section12", "data", filename),
        Path.Combine(cwd, "..", "..", "section12", "data", filename),
        Path.Combine(cwd, "..", "..", "data", filename),  // ../../data/unordered.txt
    };

    // Keep a record of what we tried (for error messages).
    string[] tried = new string[searchPaths.Length];
    int idx = 0;

    foreach (var p in searchPaths)
    {
        tried[idx++] = p;

        if (File.Exists(p))
        {
            Console.WriteLine($"Loaded: {p}");

            // Read all text, split on ANY whitespace, parse to int[]
            string text = File.ReadAllText(p);
            var tokens = text.Split(Array.Empty<char>(), StringSplitOptions.RemoveEmptyEntries);

            return Array.ConvertAll(tokens, int.Parse);
        }
    }

    // If we reach here, none of the candidate paths existed.
    Console.WriteLine($"Error reading: {filename}");
    Console.WriteLine($"Working directory: {cwd}");
    Console.WriteLine("Search paths attempted:");
    foreach (var p in tried)
    {
        if (!string.IsNullOrEmpty(p))
            Console.WriteLine("  " + p);
    }
    Console.WriteLine("Missing input file — aborting.");

    Environment.Exit(1);
    return []; // unreachable, but keeps compiler happy
}

// ------------------------------------------------------------
// Top-level "main"
// ------------------------------------------------------------
//
// This is written for a top-level program (C# 9+),
// so there is no explicit static void Main().

var unordered = LoadFile("unordered.txt");
var expected  = LoadFile("ordered.txt");

// Reset counters before the run
comparisons = 0;
writes = 0;

// Sort a copy (so unordered[] remains unchanged if you want to reuse it)
var arrToSort = (int[])unordered.Clone();
QuickSort(arrToSort);

// ------------------------------------------------------------
// Verify correctness
// ------------------------------------------------------------
//
// We first check length equality, then scan for the first mismatch.
// If a mismatch is found, we report it (first mismatch only).
bool correct = arrToSort.Length == expected.Length;

if (correct)
{
    for (int i = 0; i < arrToSort.Length; i++)
    {
        if (arrToSort[i] != expected[i])
        {
            correct = false;
            Console.WriteLine($"Mismatch at index {i}: got {arrToSort[i]}, expected {expected[i]}");
            break;
        }
    }
}

// ------------------------------------------------------------
// Output results
// ------------------------------------------------------------
Console.WriteLine();
Console.WriteLine("Quick Sort (C#)");
Console.WriteLine("----------------");
Console.WriteLine($"Elements:     {arrToSort.Length}");
Console.WriteLine($"Comparisons:  {comparisons}");
Console.WriteLine($"Writes:       {writes}");
Console.WriteLine($"Correct?      {(correct ? "YES ✔" : "NO ✘")}");

// Optional: show a small sample when correct (matches your pattern in other lessons)
if (correct)
{
    Console.WriteLine("\nFirst 10 sorted values:");
    Console.WriteLine(string.Join(" ", arrToSort.Take(10)));
}
