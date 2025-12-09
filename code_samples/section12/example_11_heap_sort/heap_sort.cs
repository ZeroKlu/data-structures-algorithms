#nullable enable

// -------------------------------------------------
// Heap Sort with Comparisons & Writes (C# script)
// -------------------------------------------------

static void Heapify(int[] arr, int size, int i, ref long comparisons, ref long writes)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size)
    {
        comparisons++;
        if (arr[left] > arr[largest])
            largest = left;
    }

    if (right < size)
    {
        comparisons++;
        if (arr[right] > arr[largest])
            largest = right;
    }

    if (largest != i)
    {
        int tmp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = tmp;
        writes += 3;

        Heapify(arr, size, largest, ref comparisons, ref writes);
    }
}

static void HeapSort(int[] arr, ref long comparisons, ref long writes)
{
    int n = arr.Length;

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        Heapify(arr, n, i, ref comparisons, ref writes);

    // Extract elements from heap
    for (int end = n - 1; end > 0; end--)
    {
        int tmp = arr[0];
        arr[0] = arr[end];
        arr[end] = tmp;
        writes += 3;

        Heapify(arr, end, 0, ref comparisons, ref writes);
    }
}

// -------------------------------------------------
// File loader with multiple search paths
// -------------------------------------------------

static int[] LoadFile(string filename)
{
    string cwd = Environment.CurrentDirectory;

    string[] attempts = new[]
    {
        Path.Combine(cwd, filename),
        Path.Combine(cwd, "data",    filename),
        Path.Combine(cwd, "..", "data", filename),
        Path.Combine(cwd, "..", "..", "data", filename),
        Path.Combine(cwd, "..", "section12", "data", filename),
        Path.Combine(cwd, "..", "..", "section12", "data", filename),
    };

    foreach (var path in attempts)
    {
        if (File.Exists(path))
        {
            Console.WriteLine($"Loaded: {path}");
            string text = File.ReadAllText(path);
            var tokens = text
                .Split((char[]?)null, StringSplitOptions.RemoveEmptyEntries);
            return tokens.Select(int.Parse).ToArray();
        }
    }

    Console.WriteLine($"Error reading input file: {filename}");
    Console.WriteLine($"Working directory: {cwd}");
    Console.WriteLine("Search paths attempted:");
    foreach (var p in attempts)
        Console.WriteLine("  " + p);
    Console.WriteLine("Missing input file — aborting.");
    Environment.Exit(1);
    return Array.Empty<int>(); // unreachable, but keeps compiler happy
}

// -------------------------------------------------
// Top-level test harness
// -------------------------------------------------

var unordered = LoadFile("unordered.txt");
var expected  = LoadFile("ordered.txt");

if (unordered.Length != expected.Length)
{
    Console.WriteLine("Length mismatch between unordered and ordered arrays.");
    Console.WriteLine($"unordered.Length = {unordered.Length}");
    Console.WriteLine($"expected.Length  = {expected.Length}");
    Environment.Exit(1);
}

var arr = (int[])unordered.Clone();
long comparisons = 0;
long writes = 0;

HeapSort(arr, ref comparisons, ref writes);

// Validate
bool ok = true;
for (int i = 0; i < arr.Length; i++)
{
    if (arr[i] != expected[i])
    {
        Console.WriteLine($"Mismatch at index {i}: got {arr[i]}, expected {expected[i]}");
        ok = false;
        break;
    }
}

Console.WriteLine();
Console.WriteLine("Heap Sort (C#)");
Console.WriteLine("--------------");
Console.WriteLine($"Elements:     {arr.Length}");
Console.WriteLine($"Comparisons:  {comparisons}");
Console.WriteLine($"Writes:       {writes}");
Console.WriteLine($"Correct?      {(ok ? "YES ✓" : "NO ✗")}");
