#nullable enable

static void BubbleSort(int[] arr) {
    int n = arr.Length;
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (int i = 1; i < n; i++) {
            if (arr[i - 1] > arr[i]) {
                (arr[i], arr[i - 1]) = (arr[i - 1], arr[i]);
                swapped = true;
            }
        }
        n--;
    }
}

// -------------------------------------------------------------
// Bubble Sort with step counting
// -------------------------------------------------------------
void BubbleSortSteps(int[] arr, StepCounter stats)
{
    int n = arr.Length;
    bool swapped = true;

    while (swapped)
    {
        swapped = false;
        for (int i = 1; i < n; i++)
        {
            stats.Comparisons++;
            if (arr[i - 1] > arr[i])
            {
                int tmp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = tmp;

                stats.Swaps++;
                swapped = true;
            }
        }
        n--; // Last element is now in the correct place
    }
}

// -------------------------------------------------------------
// Auto-detect project root (fixes dotnet-script path problems)
// -------------------------------------------------------------
string FindProjectRoot()
{
    string cur = Directory.GetCurrentDirectory();

    while (cur != Directory.GetDirectoryRoot(cur))
    {
        if (Directory.Exists(Path.Combine(cur, "code_samples")))
        {
            return cur;
        }
        cur = Directory.GetParent(cur)!.FullName;
    }

    // fallback if nothing found
    return Directory.GetCurrentDirectory();
}

// -------------------------------------------------------------
// Load a file from code_samples/section12/data
// -------------------------------------------------------------
int[]? LoadFile(string filename)
{
    try
    {
        string root = FindProjectRoot();

        string fullPath = Path.Combine(
            root,
            "code_samples",
            "section12",
            "data",
            filename
        );

        Console.WriteLine($"Attempting to read: {fullPath}");

        string text = File.ReadAllText(fullPath);
        return text
            .Split((char[])null!, StringSplitOptions.RemoveEmptyEntries)
            .Select(int.Parse)
            .ToArray();
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
int CompareArrays(int[] a, int[] b)
{
    int mismatches = 0;
    int n = Math.Min(a.Length, b.Length);

    for (int i = 0; i < n; i++)
    {
        if (a[i] != b[i])
        {
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
var unordered = LoadFile("unordered.txt");
var expected  = LoadFile("ordered.txt");

if (unordered is null || expected is null)
{
    Console.WriteLine("Missing input file — aborting.");
    return;
}

if (unordered.Length != expected.Length)
{
    Console.WriteLine("File length mismatch — aborting.");
    Console.WriteLine($"unordered: {unordered.Length}, expected: {expected.Length}");
    return;
}

var stats = new StepCounter();

Console.WriteLine("\n--- Bubble Sort Step Count ---");
BubbleSortSteps(unordered, stats);

Console.WriteLine($"Comparisons: {stats.Comparisons}");
Console.WriteLine($"Swaps:       {stats.Swaps}");

Console.WriteLine("\nChecking sorted output...");
int mismatchesFound = CompareArrays(unordered, expected);

if (mismatchesFound == 0)
    Console.WriteLine("SUCCESS — output matches expected sorted list!");
else
    Console.WriteLine($"FAIL — mismatches found: {mismatchesFound}");

record StepCounter
{
    public long Comparisons { get; set; }
    public long Swaps { get; set; }
}
