// ------------------------------------------------------------
// Example 10 - Quick Sort (C#)
// ------------------------------------------------------------

// Step counters
long comparisons = 0;
long writes = 0;

// ------------------------------------------------------------
// Quick Sort Implementation
// ------------------------------------------------------------

void QuickSort(int[] arr)
{
    QuickSortRec(arr, 0, arr.Length - 1);
}

void QuickSortRec(int[] arr, int left, int right)
{
    if (left >= right) return;

    int p = Partition(arr, left, right);
    QuickSortRec(arr, left, p - 1);
    QuickSortRec(arr, p + 1, right);
}

int Partition(int[] arr, int left, int right)
{
    int pivot = arr[right];
    int i = left;

    for (int j = left; j < right; j++)
    {
        comparisons++;
        if (arr[j] < pivot)
        {
            // swap arr[i], arr[j]
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            writes += 3; // three assignments
            i++;
        }
    }

    // final swap arr[i], arr[right]
    int tmp2 = arr[i];
    arr[i] = arr[right];
    arr[right] = tmp2;
    writes += 3;

    return i;
}

// ------------------------------------------------------------
// File Loader with Robust Path Searching
// ------------------------------------------------------------

int[] LoadFile(string filename)
{
    string cwd = Environment.CurrentDirectory;

    string[] searchPaths = new[]
    {
        Path.Combine(cwd, filename),
        Path.Combine(cwd, "data", filename),
        Path.Combine(cwd, "..", "data", filename),
        Path.Combine(cwd, "..", "section12", "data", filename),
        Path.Combine(cwd, "..", "..", "section12", "data", filename),
        Path.Combine(cwd, "..", "..", "data", filename),
    };

    string[] tried = new string[searchPaths.Length];
    int idx = 0;

    foreach (var p in searchPaths)
    {
        tried[idx++] = p;
        if (File.Exists(p))
        {
            Console.WriteLine($"Loaded: {p}");
            string text = File.ReadAllText(p);
            var tokens = text.Split(Array.Empty<char>(),
                                    StringSplitOptions.RemoveEmptyEntries);
            int[] arr = Array.ConvertAll(tokens, int.Parse);
            return arr;
        }
    }

    Console.WriteLine($"Error reading: {filename}");
    Console.WriteLine($"Working directory: {cwd}");
    Console.WriteLine("Search paths attempted:");
    foreach (var p in tried)
    {
        if (!string.IsNullOrEmpty(p))
            Console.WriteLine("  " + p);
    }
    Console.WriteLine("Missing input file — aborting.");
    Environment.Exit(1);
    return Array.Empty<int>(); // unreachable, but keeps compiler happy
}

// ------------------------------------------------------------
// Top-level "main"
// ------------------------------------------------------------

var unordered = LoadFile("unordered.txt");
var expected  = LoadFile("ordered.txt");

// Reset counters
comparisons = 0;
writes = 0;

// Sort a copy so we don't mutate original input
var arrToSort = (int[])unordered.Clone();
QuickSort(arrToSort);

// Verify correctness
bool correct = arrToSort.Length == expected.Length;
if (correct)
{
    for (int i = 0; i < arrToSort.Length; i++)
    {
        if (arrToSort[i] != expected[i])
        {
            correct = false;
            Console.WriteLine($"Mismatch at index {i}: got {arrToSort[i]}, expected {expected[i]}");
            break;
        }
    }
}

Console.WriteLine();
Console.WriteLine("Quick Sort (C#)");
Console.WriteLine("----------------");
Console.WriteLine($"Elements:     {arrToSort.Length}");
Console.WriteLine($"Comparisons:  {comparisons}");
Console.WriteLine($"Writes:       {writes}");
Console.WriteLine($"Correct?      {(correct ? "YES ✔" : "NO ✘")}");
