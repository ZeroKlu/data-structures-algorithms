// ======================================
// Given linear search
// ======================================

static int LinearSearch(int[] arr, int target)
{
    for (int i = 0; i < arr.Length; i++)
    {
        if (arr[i] == target)
        {
            return i;
        }
    }
    return -1;
}

// Same search, but counts comparison steps
static (int index, int steps) LinearSearchSteps(int[] arr, int target)
{
    int steps = 0;
    for (int i = 0; i < arr.Length; i++)
    {
        steps++;
        if (arr[i] == target)
        {
            return (i, steps);
        }
    }
    return (-1, steps);
}

// ======================================
// Load a file of integers relative to *current directory*
// ======================================

static int[] LoadFile(string relativePath)
{
    try
    {
        // Resolve to an absolute path using the current working directory
        string fullPath = Path.GetFullPath(relativePath, Environment.CurrentDirectory);

        string text = File.ReadAllText(fullPath);
        return text
            .Split((char[])null, StringSplitOptions.RemoveEmptyEntries)
            .Select(int.Parse)
            .ToArray();
    }
    catch (Exception ex)
    {
        Console.WriteLine($"Error reading: {relativePath}");
        Console.WriteLine($"Resolved full path: {Path.GetFullPath(relativePath, Environment.CurrentDirectory)}");
        Console.WriteLine($"Working directory: {Environment.CurrentDirectory}");
        Console.WriteLine($"Exception: {ex.Message}");
        return Array.Empty<int>();
    }
}

// ======================================
// Run dataset tests
// ======================================

static void RunTestsOnDataset(string name, int[] arr)
{
    Console.WriteLine($"=== {name} ===");
    Console.WriteLine($"Loaded {arr.Length} integers.\n");

    // first element
    var r1 = LinearSearchSteps(arr, arr[0]);
    Console.WriteLine($"Search first  ({arr[0]}): index={r1.index}, steps={r1.steps}");

    // middle element
    int midVal = arr[arr.Length / 2];
    var r2 = LinearSearchSteps(arr, midVal);
    Console.WriteLine($"Search middle ({midVal}): index={r2.index}, steps={r2.steps}");

    // last element
    int lastVal = arr[arr.Length - 1];
    var r3 = LinearSearchSteps(arr, lastVal);
    Console.WriteLine($"Search last   ({lastVal}): index={r3.index}, steps={r3.steps}");

    // missing
    var r4 = LinearSearchSteps(arr, 999999);
    Console.WriteLine($"Search missing (999999): index={r4.index}, steps={r4.steps}");

    Console.WriteLine();
}

// ======================================
// MAIN (top-level)
// ======================================

// From:  C:\...\code_samples\section12\lesson_2_linear_search
// Data:  ..\data\ordered.txt and ..\data\unordered.txt
var ordered   = LoadFile(@"..\data\ordered.txt");
var unordered = LoadFile(@"..\data\unordered.txt");

if (ordered.Length == 0 || unordered.Length == 0)
{
    Console.WriteLine("Missing input files - see error details above.");
    return;
}

RunTestsOnDataset("Ordered Data", ordered);
RunTestsOnDataset("Unordered Data", unordered);
