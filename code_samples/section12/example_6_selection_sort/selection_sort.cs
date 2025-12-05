#nullable enable

static int[]? LoadFile(string filename)
{
    try
    {
        string workDir = Directory.GetCurrentDirectory();
        string full = Path.GetFullPath(Path.Combine(workDir, "..", "data", filename));

        Console.WriteLine($"Attempting to read: {full}");

        string text = File.ReadAllText(full);

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

static void SelectionSort(int[] arr, out long comparisons, out long swaps)
{
    comparisons = 0;
    swaps = 0;

    int n = arr.Length;

    for (int i = 0; i < n; i++)
    {
        int minIdx = i;

        for (int j = i + 1; j < n; j++)
        {
            comparisons++;
            if (arr[j] < arr[minIdx])
            {
                minIdx = j;
            }
        }

        if (minIdx != i)
        {
            int tmp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = tmp;
            swaps++;
        }
    }
}

var unordered = LoadFile("unordered.txt");
if (unordered is null)
{
    Console.WriteLine("Missing unordered file - aborting.");
    return;
}

var ordered = LoadFile("ordered.txt");
if (ordered is null)
{
    Console.WriteLine("Missing ordered file - aborting.");
    return;
}

if (unordered.Length != ordered.Length)
{
    Console.WriteLine("Length mismatch between files.");
    return;
}

Console.WriteLine("\n--- Selection Sort (Step Counting) ---");

SelectionSort(unordered, out long comps, out long swaps);

Console.WriteLine($"Comparisons: {comps}");
Console.WriteLine($"Swaps:       {swaps}");

Console.WriteLine("\nComparing result to ordered.txt...");

int mismatches = 0;

for (int i = 0; i < unordered.Length; i++)
{
    if (unordered[i] != ordered[i])
    {
        if (mismatches < 10)
        {
            Console.WriteLine($"Mismatch @ {i}: got {unordered[i]}, expected {ordered[i]}");
        }
        mismatches++;
    }
}

if (mismatches == 0)
    Console.WriteLine("SUCCESS: Sorted output matches ordered.txt");
else
    Console.WriteLine($"FAIL: {mismatches} mismatches.");
