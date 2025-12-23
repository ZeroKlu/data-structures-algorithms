#nullable enable

/**
 * Performs exponential search on a sorted integer array.
 *
 * Exponential search works in two phases:
 *   1) Exponentially expand a search bound to find a candidate range.
 *   2) Perform binary search within that range.
 *
 * Step counting:
 *   - Counts the initial check of arr[0]
 *   - Counts each comparison during range expansion
 *   - Counts each iteration of the binary search
 *
 * @param arr     Sorted array of integers
 * @param target  Value to locate
 *
 * @return Tuple:
 *   - index: position of target, or -1 if not found
 *   - steps: number of comparison/probe steps performed
 */
static (int index, int steps) ExponentialSearch(int[] arr, int target)
{
    int steps = 0;            // Total step counter
    int n = arr.Length;       // Number of elements in the array

    // Handle empty array
    if (n == 0) return (-1, steps);

    // -------------------------------------------------------
    // Step 0: Check the first element directly
    // -------------------------------------------------------
    steps++;
    if (arr[0] == target) return (0, steps);

    // -------------------------------------------------------
    // Phase 1: Find a range by exponential doubling
    // -------------------------------------------------------
    int bound = 1;

    // Double the bound until:
    //  - we reach/exceed array size, or
    //  - arr[bound] >= target (meaning target must be in this range)
    while (bound < n)
    {
        steps++;  // Count comparison at arr[bound]

        if (arr[bound] < target)
            bound *= 2;   // Exponentially expand search range
        else
            break;        // Found upper bound for target
    }

    // Compute final search range for binary search
    int left = bound / 2;
    int right = Math.Min(bound, n - 1);

    // -------------------------------------------------------
    // Phase 2: Binary search within the identified range
    // -------------------------------------------------------
    return BinarySearchRange(arr, left, right, target, steps);
}

/**
 * Performs binary search within a specified index range [lo, hi],
 * continuing to count steps started by exponential search.
 *
 * @param arr     Sorted array of integers
 * @param lo      Lower bound index (inclusive)
 * @param hi      Upper bound index (inclusive)
 * @param target  Value to locate
 * @param steps   Step counter carried over from exponential search
 *
 * @return Tuple:
 *   - index: position of target, or -1 if not found
 *   - steps: updated step count after binary search
 */
static (int index, int steps) BinarySearchRange(
    int[] arr,
    int lo,
    int hi,
    int target,
    int steps)
{
    // Standard binary search within restricted range
    while (lo <= hi)
    {
        // Compute midpoint safely
        int mid = lo + (hi - lo) / 2;

        steps++;  // Count this midpoint comparison

        int val = arr[mid];

        // Compare midpoint value against target
        if (val == target) return (mid, steps);
        if (val < target) lo = mid + 1;
        else hi = mid - 1;
    }

    // Target not found in this range
    return (-1, steps);
}

// ---------------------------------------------------------------------------
// File Loading (tries multiple possible paths)
// ---------------------------------------------------------------------------

/**
 * Attempts to load ordered.txt from several possible relative paths.
 *
 * This approach makes the script robust when run from:
 *   - lesson folders
 *   - nested dotnet-script environments
 *   - the repository root
 *
 * @return Array of integers if successful; otherwise null
 */
static int[]? LoadOrderedFile()
{
    // Print current working directory for diagnostics
    Console.WriteLine($"Working directory: {Directory.GetCurrentDirectory()}");

    // Candidate relative paths where ordered.txt may exist
    string[] candidates =
    [
        @"..\data\ordered.txt",                       // typical lesson folder
        @"..\..\data\ordered.txt",                    // nested dotnet-script folder
        @"code_samples\section12\data\ordered.txt"    // repo root execution
    ];

    // Try each candidate path
    foreach (var rel in candidates)
    {
        string full = Path.GetFullPath(rel);
        Console.WriteLine($"Attempting to read: {full}");

        if (File.Exists(full))
        {
            try
            {
                // Read entire file, split on whitespace, parse to integers
                var arr = File.ReadAllText(full)
                    .Split([' ', '\t', '\r', '\n'],
                           StringSplitOptions.RemoveEmptyEntries)
                    .Select(s => int.Parse(s))
                    .ToArray();

                Console.WriteLine($"Loaded {arr.Length} integers from {full}");
                return arr;
            }
            catch (Exception ex)
            {
                // File exists but could not be read or parsed
                Console.WriteLine($"Error reading ordered.txt:\n{ex.Message}");
            }
        }
        else
        {
            // File does not exist at this candidate location
            Console.WriteLine("  File does not exist here.");
        }
    }

    // All attempts failed
    Console.WriteLine("Missing input file - aborting.");
    return null;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

/**
 * Loads ordered data from disk and runs exponential search
 * on a small set of representative target values.
 */
static void RunTests()
{
    var arr = LoadOrderedFile();
    if (arr == null)
        return;

    // Representative test targets:
    //  - values expected near the beginning, middle, and end
    //  - one value guaranteed not to exist
    int[] testTargets = [-5000, 0, 5000, 6000];

    // Execute exponential search for each target
    foreach (var target in testTargets)
    {
        var (index, steps) = ExponentialSearch(arr, target);
        Console.WriteLine($"Target {target} → index={index}, steps={steps}");
    }
}

// Run test harness
RunTests();
