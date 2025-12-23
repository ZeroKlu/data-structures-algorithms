// ============================================================
// Insertion Sort with Step Counting (C#)
// ============================================================
//
// This program demonstrates INSERTION SORT while explicitly
// counting operations to illustrate time complexity.
//
// What we count:
//   • comparisons → comparisons between arr[j] and the key
//   • writes      → writes to the array (shifts + final insertion)
//
// The program:
//   1) Loads integers from unordered.txt and ordered.txt
//   2) Sorts the unordered list using insertion sort
//   3) Counts comparisons and writes
//   4) Verifies correctness against ordered.txt
//
// ============================================================

#nullable enable

// ------------------------------------------------------------
// Insertion Sort with step counting
// ------------------------------------------------------------
//
// Returns:
//   (comparisons, writes)
//
// Algorithm overview:
//   - Treat the left portion of the array as sorted
//   - Take the next element ("key")
//   - Shift larger elements right until the correct position is found
//   - Insert the key
//
// Time Complexity:
//   Best case (already sorted): O(n)
//   Average / Worst case:       O(n²)
//
(int comparisons, int writes) InsertionSort(int[] arr)
{
    int comparisons = 0;
    int writes = 0;

    // Start at index 1; a single-element prefix is already sorted
    for (int i = 1; i < arr.Length; i++)
    {
        // Value to be inserted into the sorted prefix
        int key = arr[i];

        // Index of the last element in the sorted prefix
        int j = i - 1;

        // Shift elements right while they are greater than key
        while (j >= 0)
        {
            comparisons++; // compare arr[j] with key

            if (arr[j] > key)
            {
                // Shift element to the right
                arr[j + 1] = arr[j];
                writes++;
                j--;
            }
            else
            {
                // Found correct insertion point
                break;
            }
        }

        // Insert key into its final position
        arr[j + 1] = key;
        writes++;
    }

    return (comparisons, writes);
}

// ------------------------------------------------------------
// File loader with multiple fallback directory attempts
// ------------------------------------------------------------
//
// Tries several possible locations so the program works
// regardless of the current working directory.
//
int[] LoadFile(string filename)
{
    string wd = Directory.GetCurrentDirectory();

    string[] attempts =
    [
        // 1) Same directory as executable
        Path.Combine(wd, filename),

        // 2) ./data/
        Path.Combine(wd, "data", filename),

        // 3) ../data/
        Path.Combine(wd, "..", "data", filename),

        // 4) ../../data/
        Path.Combine(wd, "..", "..", "data", filename),

        // 5) Explicit known project location (your environment)
        Path.Combine(
            Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
            "data-structures-algorithms",
            "code_samples",
            "section12",
            "data",
            filename
        )
    ];

    foreach (var path in attempts)
    {
        try
        {
            if (File.Exists(path))
            {
                string text = File.ReadAllText(path);

                // Split on all whitespace and parse integers
                char[] sep = [' ', '\n', '\r', '\t'];
                return text
                    .Split(sep, StringSplitOptions.RemoveEmptyEntries)
                    .Select(int.Parse)
                    .ToArray();
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error reading: {path}");
            Console.WriteLine($"Exception: {ex.Message}");
        }
    }

    // If all attempts failed, print diagnostics and exit
    Console.WriteLine($"Error reading {filename}");
    Console.WriteLine("Search paths attempted:");
    foreach (var p in attempts)
        Console.WriteLine("  " + p);

    Console.WriteLine("Missing input file — aborting.");
    Environment.Exit(1);
    return []; // unreachable, but keeps compiler happy
}

// ------------------------------------------------------------
// Run test harness
// ------------------------------------------------------------

// Load test files
var unordered = LoadFile("unordered.txt");
var expected  = LoadFile("ordered.txt");

// Work on a copy so original data remains unchanged
var arr = (int[])unordered.Clone();

// Run insertion sort
var (comparisons, writes) = InsertionSort(arr);

// Verify correctness
bool ok = arr.SequenceEqual(expected);

// ------------------------------------------------------------
// Output results
// ------------------------------------------------------------
Console.WriteLine("\nInsertion Sort (C#)");
Console.WriteLine("-------------------");
Console.WriteLine($"Elements:     {arr.Length}");
Console.WriteLine($"Comparisons:  {comparisons}");
Console.WriteLine($"Writes:       {writes}");
Console.WriteLine($"Correct?      {(ok ? "YES ✔" : "NO ✘")}");

// If incorrect, show first mismatch
if (!ok)
{
    for (int i = 0; i < arr.Length; i++)
    {
        if (arr[i] != expected[i])
        {
            Console.WriteLine(
                $"Mismatch at index {i}: got {arr[i]}, expected {expected[i]}"
            );
            break;
        }
    }
}
else
{
    // Show a small sample of sorted output
    Console.WriteLine("\nFirst 10 sorted values:");
    Console.WriteLine(string.Join(" ", arr.Take(10)));
}
