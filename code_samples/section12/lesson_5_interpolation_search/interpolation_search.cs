// =======================================================
// Interpolation Search (index-only)
// =======================================================
static int InterpolationSearch(int[] arr, int target)
{
    int n = arr.Length;
    if (n == 0) return -1;

    int lo = 0, hi = n - 1;

    while (lo <= hi && target >= arr[lo] && target <= arr[hi])
    {
        // All values in this range identical
        if (arr[hi] == arr[lo])
        {
            return arr[lo] == target ? lo : -1;
        }

        // Estimate the likely position
        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                              (arr[hi] - arr[lo]));

        if (pos < lo || pos > hi)
            return -1;

        if (arr[pos] == target)
            return pos;
        else if (arr[pos] < target)
            lo = pos + 1;
        else
            hi = pos - 1;
    }

    return -1;
}

// =======================================================
// Interpolation Search with step counting
// =======================================================
static (int index, int steps) InterpolationSearchSteps(int[] arr, int target)
{
    int steps = 0;
    int n = arr.Length;
    if (n == 0) return (-1, steps);

    int lo = 0, hi = n - 1;

    while (lo <= hi && target >= arr[lo] && target <= arr[hi])
    {
        steps++; // arr[hi] == arr[lo]
        if (arr[hi] == arr[lo])
        {
            steps++; // arr[lo] == target
            return (arr[lo] == target ? lo : -1, steps);
        }

        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                              (arr[hi] - arr[lo]));

        if (pos < lo || pos > hi)
            return (-1, steps);

        steps++; // arr[pos] == target
        if (arr[pos] == target)
            return (pos, steps);

        steps++; // arr[pos] < target
        if (arr[pos] < target)
            lo = pos + 1;
        else
            hi = pos - 1;
    }

    return (-1, steps);
}

// =======================================================
// Load ordered.txt relative to current script directory
// =======================================================
static int[] LoadOrderedFile()
{
    string baseDir = Environment.CurrentDirectory;
    string path = Path.GetFullPath(
        Path.Combine(baseDir, "..", "data", "ordered.txt")
    );

    Console.WriteLine($"Attempting to read: {path}");

    try
    {
        // Read entire file and split on ANY whitespace
        string text = File.ReadAllText(path);

        return text
            .Split((char[])null, StringSplitOptions.RemoveEmptyEntries) // splits on all whitespace
            .Select(s => int.Parse(s))
            .ToArray();
    }
    catch (Exception ex)
    {
        Console.WriteLine("Error reading ordered.txt");
        Console.WriteLine(ex.Message);
        Console.WriteLine($"Working directory: {Environment.CurrentDirectory}");
        return Array.Empty<int>();
    }
}

// =======================================================
// MAIN (top-level for dotnet script)
// =======================================================
var arr = LoadOrderedFile();
if (arr.Length == 0)
{
    Console.WriteLine("Missing input file — aborting.");
    return;
}

Console.WriteLine("\n=== Interpolation Search Tests (sorted data only) ===");
Console.WriteLine($"Loaded {arr.Length} integers\n");

// First
var r = InterpolationSearchSteps(arr, arr[0]);
Console.WriteLine($"Search first  ({arr[0]}): index={r.index}, steps={r.steps}");

// Middle
int midVal = arr[arr.Length / 2];
r = InterpolationSearchSteps(arr, midVal);
Console.WriteLine($"Search middle ({midVal}): index={r.index}, steps={r.steps}");

// Last
int lastVal = arr[arr.Length - 1];
r = InterpolationSearchSteps(arr, lastVal);
Console.WriteLine($"Search last   ({lastVal}): index={r.index}, steps={r.steps}");

// Missing
r = InterpolationSearchSteps(arr, 999_999);
Console.WriteLine($"Search missing (999999): index={r.index}, steps={r.steps}");
