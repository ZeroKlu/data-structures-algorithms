// =======================================================
// Interpolation Search (index-only)
// =======================================================

/**
 * Performs interpolation search on a sorted integer array.
 *
 * Interpolation search improves on binary search by estimating
 * where the target might be located based on its value relative
 * to the values at the current bounds.
 *
 * This algorithm performs best when the data is uniformly distributed.
 *
 * @param arr     Sorted array to search
 * @param target  Value to locate
 *
 * @return Index of the target if found; otherwise -1
 */
static int InterpolationSearch(int[] arr, int target)
{
    int n = arr.Length;        // Number of elements in the array
    if (n == 0) return -1;     // Empty array => not found

    int lo = 0, hi = n - 1;    // Search bounds (inclusive)

    // Continue searching while:
    //  - bounds are valid, and
    //  - target is within the value range [arr[lo], arr[hi]]
    while (lo <= hi && target >= arr[lo] && target <= arr[hi])
    {
        // --------------------------------------------------
        // Special case: all values in [lo, hi] are identical
        // --------------------------------------------------
        if (arr[hi] == arr[lo])
        {
            // Only possible match is at lo
            return arr[lo] == target ? lo : -1;
        }

        // --------------------------------------------------
        // Estimate the likely position using interpolation:
        //
        // pos = lo + (hi - lo) * (target - arr[lo]) / (arr[hi] - arr[lo])
        //
        // Casting to double avoids integer truncation during division,
        // then the result is cast back to int for indexing.
        // --------------------------------------------------
        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                              (arr[hi] - arr[lo]));

        // Guard against out-of-range estimates
        if (pos < lo || pos > hi)
            return -1;

        // Compare against the estimated position
        if (arr[pos] == target)
            return pos;            // Target found
        else if (arr[pos] < target)
            lo = pos + 1;          // Search right subrange
        else
            hi = pos - 1;          // Search left subrange
    }

    // Target not found
    return -1;
}

// =======================================================
// Interpolation Search with step counting
// =======================================================

/**
 * Performs interpolation search while counting selected comparisons.
 *
 * The step counter includes:
 *  - Checking if arr[hi] == arr[lo]
 *  - Checking if arr[lo] == target (uniform-range case)
 *  - Checking if arr[pos] == target
 *  - Checking if arr[pos] < target (to choose direction)
 *
 * Note: Loop boundary checks are not counted here—only the
 * explicitly labeled comparison steps are included.
 *
 * @param arr     Sorted array to search
 * @param target  Value to locate
 *
 * @return Tuple containing:
 *         - index: position of the target, or -1 if not found
 *         - steps: number of comparisons performed
 */
static (int index, int steps) InterpolationSearchSteps(int[] arr, int target)
{
    int steps = 0;             // Counts number of comparisons
    int n = arr.Length;        // Number of elements
    if (n == 0) return (-1, steps);

    int lo = 0, hi = n - 1;    // Search bounds

    // Continue searching while target remains within value bounds
    while (lo <= hi && target >= arr[lo] && target <= arr[hi])
    {
        steps++; // compare arr[hi] == arr[lo]
        if (arr[hi] == arr[lo])
        {
            steps++; // compare arr[lo] == target
            return (arr[lo] == target ? lo : -1, steps);
        }

        // Estimate the likely position
        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                              (arr[hi] - arr[lo]));

        // Guard against invalid estimates
        if (pos < lo || pos > hi)
            return (-1, steps);

        steps++; // compare arr[pos] == target
        if (arr[pos] == target)
            return (pos, steps);

        steps++; // compare arr[pos] < target
        if (arr[pos] < target)
            lo = pos + 1;      // Search right subrange
        else
            hi = pos - 1;      // Search left subrange
    }

    // Target not found
    return (-1, steps);
}

// =======================================================
// Load ordered.txt relative to current script directory
// =======================================================

/**
 * Loads the ordered integer dataset from disk.
 *
 * The file path is resolved relative to the current working directory,
 * then navigates up to ../data/ordered.txt.
 *
 * @return Array of integers read from ordered.txt,
 *         or an empty array on failure
 */
static int[] LoadOrderedFile()
{
    // Base directory where the script is executed
    string baseDir = Environment.CurrentDirectory;

    // Resolve full path to ../data/ordered.txt
    string path = Path.GetFullPath(
        Path.Combine(baseDir, "..", "data", "ordered.txt")
    );

    Console.WriteLine($"Attempting to read: {path}");

    try
    {
        // Read entire file contents as a single string
        string text = File.ReadAllText(path);

        // Split on any whitespace, parse tokens as integers,
        // and convert the result to an array
        return [.. text
            .Split((char[])null, StringSplitOptions.RemoveEmptyEntries)
            .Select(s => int.Parse(s))];
    }
    catch (Exception ex)
    {
        // Provide diagnostic output if file loading fails
        Console.WriteLine("Error reading ordered.txt");
        Console.WriteLine(ex.Message);
        Console.WriteLine($"Working directory: {Environment.CurrentDirectory}");
        return [];
    }
}

// =======================================================
// MAIN (top-level for dotnet script)
// =======================================================

// Load ordered dataset from disk
var arr = LoadOrderedFile();

// Validate successful file load
if (arr.Length == 0)
{
    Console.WriteLine("Missing input file - aborting.");
    return;
}

Console.WriteLine("\n=== Interpolation Search Tests (sorted data only) ===");
Console.WriteLine($"Loaded {arr.Length} integers\n");

// Test 1: Search for the first element
var r = InterpolationSearchSteps(arr, arr[0]);
Console.WriteLine($"Search first  ({arr[0]}): index={r.index}, steps={r.steps}");

// Test 2: Search for the middle element
int midVal = arr[arr.Length / 2];
r = InterpolationSearchSteps(arr, midVal);
Console.WriteLine($"Search middle ({midVal}): index={r.index}, steps={r.steps}");

// Test 3: Search for the last element
int lastVal = arr[^1];
r = InterpolationSearchSteps(arr, lastVal);
Console.WriteLine($"Search last   ({lastVal}): index={r.index}, steps={r.steps}");

// Test 4: Search for a value guaranteed not to exist
r = InterpolationSearchSteps(arr, 999_999);
Console.WriteLine($"Search missing (999999): index={r.index}, steps={r.steps}");
