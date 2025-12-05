#nullable enable

static (int index, int steps) ExponentialSearch(int[] arr, int target)
{
    int steps = 0;
    int n = arr.Length;

    if (n == 0) return (-1, steps);

    // Step 0: check index 0
    steps++;
    if (arr[0] == target) return (0, steps);

    // Find range by doubling
    int bound = 1;
    while (bound < n)
    {
        steps++;
        if (arr[bound] < target)
            bound *= 2;
        else
            break;
    }

    int left = bound / 2;
    int right = Math.Min(bound, n - 1);

    return BinarySearchRange(arr, left, right, target, steps);
}

static (int index, int steps) BinarySearchRange(int[] arr, int lo, int hi, int target, int steps)
{
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;
        steps++;

        int val = arr[mid];

        if (val == target) return (mid, steps);
        if (val < target) lo = mid + 1;
        else hi = mid - 1;
    }

    return (-1, steps);
}

// ---------------------------------------------------------------------------
// File Loading (tries multiple possible paths)
// ---------------------------------------------------------------------------

static int[]? LoadOrderedFile()
{
    Console.WriteLine($"Working directory: {Directory.GetCurrentDirectory()}");

    string[] candidates =
    [
        @"..\data\ordered.txt",                       // typical case
        @"..\..\data\ordered.txt",                    // dotnet-script nested folder
        @"code_samples\section12\data\ordered.txt"    // when run from repo root
    ];

    foreach (var rel in candidates)
    {
        string full = Path.GetFullPath(rel);
        Console.WriteLine($"Attempting to read: {full}");

        if (File.Exists(full))
        {
            try
            {
                var arr = File.ReadAllText(full)
                    .Split(new char[] { ' ', '\t', '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries)
                    .Select(s => int.Parse(s))
                    .ToArray();

                Console.WriteLine($"Loaded {arr.Length} integers from {full}");
                return arr;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error reading ordered.txt:\n{ex.Message}");
            }
        }
        else
        {
            Console.WriteLine("  File does not exist here.");
        }
    }

    Console.WriteLine("Missing input file - aborting.");
    return null;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

void RunTests()
{
    var arr = LoadOrderedFile();
    if (arr == null)
        return;

    int[] testTargets = { -5000, 0, 5000, 6000 };

    foreach (var target in testTargets)
    {
        var result = ExponentialSearch(arr, target);
        Console.WriteLine($"Target {target} → index={result.index}, steps={result.steps}");
    }
}

RunTests();
