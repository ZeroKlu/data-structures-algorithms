// =======================================
// Jump Search (index-only)
// =======================================
static int JumpSearch(int[] arr, int target)
{
    int n = arr.Length;
    if (n == 0) return -1;

    int step = (int)Math.Sqrt(n);
    int prev = 0;
    int curr = step;

    // Jump phase: find block where target could be
    while (curr < n && arr[curr] < target)
    {
        prev = curr;
        curr += step;
    }

    if (curr >= n) curr = n - 1;

    // Linear scan within [prev, curr]
    for (int i = prev; i <= curr; i++)
    {
        if (arr[i] == target)
            return i;
    }

    return -1;
}

// =======================================
// Jump Search with step counting
// steps = number of comparisons
// =======================================
static (int index, int steps) JumpSearchSteps(int[] arr, int target)
{
    int n = arr.Length;
    int steps = 0;

    if (n == 0)
        return (-1, steps);

    int step = (int)Math.Sqrt(n);
    int prev = 0;
    int curr = step;

    // Jump phase
    while (curr < n)
    {
        steps++; // compare arr[curr] < target or arr[curr] >= target

        if (arr[curr] >= target)
            break;

        prev = curr;
        curr += step;
    }

    if (curr >= n) curr = n - 1;

    // Linear scan in [prev, curr]
    for (int i = prev; i <= curr; i++)
    {
        steps++; // compare arr[i] == target
        if (arr[i] == target)
            return (i, steps);
    }

    return (-1, steps);
}

// =======================================
// Load ordered.txt using CURRENT DIRECTORY
// =======================================
static int[] LoadOrderedFile()
{
    // When you run:
    //   cd code_samples\section12\lesson_3_jump_search
    //   dotnet script jump_search.csx
    // CurrentDirectory = that folder.
    // Data is in ..\data\ordered.txt
    string relativePath = @"..\data\ordered.txt";
    string fullPath = Path.GetFullPath(relativePath, Environment.CurrentDirectory);

    Console.WriteLine($"Attempting to read: {fullPath}");

    try
    {
        string text = File.ReadAllText(fullPath);
        return text
            .Split((char[])null, StringSplitOptions.RemoveEmptyEntries)
            .Select(int.Parse)
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

// =======================================
// MAIN (top-level for dotnet script)
// =======================================
var arr = LoadOrderedFile();
if (arr.Length == 0)
{
    Console.WriteLine("Missing input file - aborting.");
    return;
}

Console.WriteLine("=== Jump Search Tests (ordered data only) ===");
Console.WriteLine($"Loaded {arr.Length} integers\n");

// First element
var r = JumpSearchSteps(arr, arr[0]);
Console.WriteLine($"Search first  ({arr[0]}): index={r.index}, steps={r.steps}");

// Middle element
int midVal = arr[arr.Length / 2];
r = JumpSearchSteps(arr, midVal);
Console.WriteLine($"Search middle ({midVal}): index={r.index}, steps={r.steps}");

// Last element
int lastVal = arr[arr.Length - 1];
r = JumpSearchSteps(arr, lastVal);
Console.WriteLine($"Search last   ({lastVal}): index={r.index}, steps={r.steps}");

// Missing element
r = JumpSearchSteps(arr, 999_999);
Console.WriteLine($"Search missing (999999): index={r.index}, steps={r.steps}");
