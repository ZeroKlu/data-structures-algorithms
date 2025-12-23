// =======================================
// Jump Search (index-only)
// =======================================

/**
 * Performs jump search on a sorted integer array.
 *
 * Jump search operates in two phases:
 *  1) Jump phase: advance in blocks of size sqrt(n) to find
 *     a range where the target could exist.
 *  2) Linear scan phase: linearly scan the identified block.
 *
 * @param arr     Sorted array to search
 * @param target  Value to locate
 *
 * @return Index of the target if found; otherwise -1
 */
static int JumpSearch(int[] arr, int target)
{
    int n = arr.Length;     // Number of elements in the array
    if (n == 0) return -1;  // Empty array => not found

    // Determine jump (block) size
    int step = (int)Math.Sqrt(n);

    int prev = 0;    // Start index of the current block
    int curr = step; // End index of the current block

    // -------------------------------------------------------
    // Jump phase: find block where target could be
    // -------------------------------------------------------
    while (curr < n && arr[curr] < target)
    {
        prev = curr;     // Move block start forward
        curr += step;    // Jump to the next block
    }

    // Clamp curr to last valid index if we overshoot the array
    if (curr >= n) curr = n - 1;

    // -------------------------------------------------------
    // Linear scan within [prev, curr]
    // -------------------------------------------------------
    for (int i = prev; i <= curr; i++)
    {
        if (arr[i] == target)
            return i;    // Target found
    }

    // Target not found
    return -1;
}

// =======================================
// Jump Search with step counting
// steps = number of comparisons
// =======================================

/**
 * Performs jump search while counting the number of comparisons made.
 *
 * Comparisons are counted in:
 *  - The jump phase (comparisons against arr[curr])
 *  - The linear scan phase (comparisons against arr[i])
 *
 * @param arr     Sorted array to search
 * @param target  Value to locate
 *
 * @return Tuple containing:
 *         - index: position of the target, or -1 if not found
 *         - steps: number of comparisons performed
 */
static (int index, int steps) JumpSearchSteps(int[] arr, int target)
{
    int n = arr.Length; // Number of elements
    int steps = 0;      // Counts number of comparisons

    // Handle empty array case
    if (n == 0)
        return (-1, steps);

    // Determine jump (block) size
    int step = (int)Math.Sqrt(n);

    int prev = 0;    // Start index of the current block
    int curr = step; // End index of the current block

    // -------------------------------------------------------
    // Jump phase
    // -------------------------------------------------------
    while (curr < n)
    {
        // Count comparison involving arr[curr] and target
        steps++; // compare arr[curr] < target or arr[curr] >= target

        // If current block end is >= target, stop jumping
        if (arr[curr] >= target)
            break;

        // Otherwise, jump to the next block
        prev = curr;
        curr += step;
    }

    // Clamp curr to last valid index if we overshoot the array
    if (curr >= n) curr = n - 1;

    // -------------------------------------------------------
    // Linear scan in [prev, curr]
    // -------------------------------------------------------
    for (int i = prev; i <= curr; i++)
    {
        steps++; // compare arr[i] == target

        if (arr[i] == target)
            return (i, steps); // Target found
    }

    // Target not found
    return (-1, steps);
}

// =======================================
// Load ordered.txt using CURRENT DIRECTORY
// =======================================

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
    //   cd code_samples\section12\lesson_3_jump_search
    //   dotnet script jump_search.csx
    //
    // Environment.CurrentDirectory will be:
    //   code_samples\section12\lesson_3_jump_search
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

        // Split on whitespace, parse tokens as integers,
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

// =======================================
// MAIN (top-level for dotnet script)
// =======================================

// Load ordered dataset from disk
var arr = LoadOrderedFile();

// Validate successful file load
if (arr.Length == 0)
{
    Console.WriteLine("Missing input file - aborting.");
    return; // Exit script early on failure
}

Console.WriteLine("=== Jump Search Tests (ordered data only) ===");
Console.WriteLine($"Loaded {arr.Length} integers\n");

// Test 1: Search for the first element
var r = JumpSearchSteps(arr, arr[0]);
Console.WriteLine($"Search first  ({arr[0]}): index={r.index}, steps={r.steps}");

// Test 2: Search for the middle element
int midVal = arr[arr.Length / 2];
r = JumpSearchSteps(arr, midVal);
Console.WriteLine($"Search middle ({midVal}): index={r.index}, steps={r.steps}");

// Test 3: Search for the last element
int lastVal = arr[^1];
r = JumpSearchSteps(arr, lastVal);
Console.WriteLine($"Search last   ({lastVal}): index={r.index}, steps={r.steps}");

// Test 4: Search for a value guaranteed not to exist
r = JumpSearchSteps(arr, 999_999);
Console.WriteLine($"Search missing (999999): index={r.index}, steps={r.steps}");
