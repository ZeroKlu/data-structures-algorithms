#nullable enable

// =============================================================
// Bubble Sort (baseline / no step counting)
// =============================================================
//
// This is a plain bubble sort implementation that:
//   - Repeatedly sweeps through the array,
//   - Swaps adjacent out-of-order elements,
//   - Stops early when a full pass completes with no swaps.
//
// Notes:
//   - The variable `n` shrinks each pass because the largest element in
//     the current unsorted range "bubbles" to the end of that range.
//   - This function sorts the array IN PLACE.
//
static void BubbleSort(int[] arr) {
    int n = arr.Length;
    bool swapped = true;

    // Keep running passes until a pass makes no swaps
    while (swapped) {
        swapped = false;

        // Compare adjacent pairs within the current unsorted range [0..n-1]
        for (int i = 1; i < n; i++) {
            if (arr[i - 1] > arr[i]) {
                // Tuple swap syntax (compact swap of two array elements)
                (arr[i], arr[i - 1]) = (arr[i - 1], arr[i]);
                swapped = true;
            }
        }

        // After each full pass, the element at index (n-1) is finalized
        n--;
    }
}

// -------------------------------------------------------------
// Bubble Sort with step counting
// -------------------------------------------------------------
//
// This version adds a StepCounter object to track work performed.
//
// Step definitions used here:
//   - Comparisons: number of adjacent comparisons performed
//       arr[i - 1] > arr[i]
//   - Swaps: number of actual swaps executed
//
// Important behavior notes:
//   - Sorts IN PLACE.
//   - Uses the same early-exit optimization:
//       if no swaps occur in a pass, the array is already sorted.
//   - Shrinks the effective range by 1 after each pass (n--).
//
void BubbleSortSteps(int[] arr, StepCounter stats)
{
    int n = arr.Length;
    bool swapped = true;

    // Repeat passes until the array is sorted (no swaps in a pass)
    while (swapped)
    {
        swapped = false;

        // One pass through the current unsorted portion
        for (int i = 1; i < n; i++)
        {
            // Count the adjacent comparison performed each iteration
            stats.Comparisons++;

            // Swap if the pair is out of order
            if (arr[i - 1] > arr[i])
            {
                // Manual swap using a temp variable
                int tmp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = tmp;

                // Count the swap and mark that a swap occurred this pass
                stats.Swaps++;
                swapped = true;
            }
        }

        // Last element in the current range is now in the correct place,
        // so we don't need to include it in subsequent passes.
        n--; // Last element is now in the correct place
    }
}

// -------------------------------------------------------------
// Auto-detect project root (fixes dotnet-script path problems)
// -------------------------------------------------------------
//
// dotnet-script / top-level scripts often run with a working directory
// that isn't the repo root. This helper walks up parent directories
// until it finds a folder that contains "code_samples".
//
// Returns:
//   - Path to detected project root if found
//   - Current directory as a fallback if not found
//
string FindProjectRoot()
{
    string cur = Directory.GetCurrentDirectory();

    // Climb upward until we reach the filesystem root
    while (cur != Directory.GetDirectoryRoot(cur))
    {
        // Heuristic: repo root is assumed to contain "code_samples"
        if (Directory.Exists(Path.Combine(cur, "code_samples")))
        {
            return cur;
        }

        // Move one directory up
        cur = Directory.GetParent(cur)!.FullName;
    }

    // fallback if nothing found
    return Directory.GetCurrentDirectory();
}

// -------------------------------------------------------------
// Load a file from code_samples/section12/data
// -------------------------------------------------------------
//
// Loads a whitespace-delimited integer file from:
//   <project-root>/code_samples/section12/data/<filename>
//
// Returns:
//   - int[] on success
//   - null on failure (file missing, unreadable, parse error, etc.)
//
// Also prints the exact path being attempted to help debug path issues.
//
int[]? LoadFile(string filename)
{
    try
    {
        // Determine the repo/project root robustly
        string root = FindProjectRoot();

        // Build the full path to the requested data file
        string fullPath = Path.Combine(
            root,
            "code_samples",
            "section12",
            "data",
            filename
        );

        Console.WriteLine($"Attempting to read: {fullPath}");

        // Read entire file and parse ints split on whitespace
        string text = File.ReadAllText(fullPath);
        return [.. text
            .Split((char[])null!, StringSplitOptions.RemoveEmptyEntries)
            .Select(int.Parse)];
    }
    catch (Exception ex)
    {
        Console.WriteLine($"Error reading: {filename}");
        Console.WriteLine(ex.Message);
        return null;
    }
}

// -------------------------------------------------------------
// Compare arrays for mismatches
// -------------------------------------------------------------
//
// Compares two arrays element-by-element up to the smaller length
// (Math.Min is used as a defensive measure, even though the caller
// typically checks for equal lengths first).
//
// Prints up to the first 10 mismatches for readability.
//
// Returns:
//   - number of mismatched positions found
//
int CompareArrays(int[] a, int[] b)
{
    int mismatches = 0;
    int n = Math.Min(a.Length, b.Length);

    for (int i = 0; i < n; i++)
    {
        if (a[i] != b[i])
        {
            // Print only the first few mismatches to avoid flooding output
            if (mismatches < 10)
                Console.WriteLine($"Mismatch @ {i}: got {a[i]}, expected {b[i]}");

            mismatches++;
        }
    }

    return mismatches;
}

// -------------------------------------------------------------
// Top-level program (NO Main function)
// -------------------------------------------------------------
//
// Workflow:
//   1) Load unordered.txt (input to sort)
//   2) Load ordered.txt   (expected sorted output reference)
//   3) Run bubble sort with step counting on unordered data
//   4) Compare result to expected output and report success/failure
//
var unordered = LoadFile("unordered.txt");
var expected  = LoadFile("ordered.txt");

// Validate file loading
if (unordered is null || expected is null)
{
    Console.WriteLine("Missing input file — aborting.");
    return;
}

// Validate lengths match (so element-by-element comparison is meaningful)
if (unordered.Length != expected.Length)
{
    Console.WriteLine("File length mismatch — aborting.");
    Console.WriteLine($"unordered: {unordered.Length}, expected: {expected.Length}");
    return;
}

// Create a counter object to record comparisons and swaps
var stats = new StepCounter();

Console.WriteLine("\n--- Bubble Sort Step Count ---");

// Sort IN PLACE and accumulate step counts
BubbleSortSteps(unordered, stats);

// Print step-count results
Console.WriteLine($"Comparisons: {stats.Comparisons}");
Console.WriteLine($"Swaps:       {stats.Swaps}");

// Verify correctness by comparing to expected sorted output
Console.WriteLine("\nChecking sorted output...");
int mismatchesFound = CompareArrays(unordered, expected);

if (mismatchesFound == 0)
    Console.WriteLine("SUCCESS — output matches expected sorted list!");
else
    Console.WriteLine($"FAIL — mismatches found: {mismatchesFound}");

// -------------------------------------------------------------
// StepCounter record type
// -------------------------------------------------------------
//
// Simple container for step-count statistics.
// Using a record here provides a compact syntax while still allowing
// mutable properties.
//
record StepCounter
{
    public long Comparisons { get; set; }
    public long Swaps { get; set; }
}
