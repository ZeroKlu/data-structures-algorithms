// ------------------------------------------------------------
// Insertion sort with step counting
// ------------------------------------------------------------
(int comparisons, int writes) InsertionSort(int[] arr)
{
    int comparisons = 0;
    int writes = 0;

    for (int i = 1; i < arr.Length; i++)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0)
        {
            comparisons++;

            if (arr[j] > key)
            {
                arr[j + 1] = arr[j];
                writes++;
                j--;
            }
            else
            {
                break;
            }
        }

        arr[j + 1] = key;
        writes++;
    }

    return (comparisons, writes);
}

// ------------------------------------------------------------
// File loader with multiple fallback directory attempts
// ------------------------------------------------------------
int[] LoadFile(string filename)
{
    string wd = Directory.GetCurrentDirectory();

    string[] attempts = new[]
    {
        Path.Combine(wd, filename),
        Path.Combine(wd, "data", filename),
        Path.Combine(wd, "..", "data", filename),
        Path.Combine(wd, "..", "..", "data", filename),

        // Your known real location:
        Path.Combine(
            Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
            "data-structures-algorithms",
            "code_samples",
            "section12",
            "data",
            filename
        )
    };

    foreach (var path in attempts)
    {
        try
        {
            if (File.Exists(path))
            {
                string text = File.ReadAllText(path);
                char[] sep = [' ', '\n', '\r', '\t'];
                var nums = text
                    .Split(sep, StringSplitOptions.RemoveEmptyEntries)
                    .Select(int.Parse)
                    .ToArray();
                return nums;
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error reading: {path}");
            Console.WriteLine($"Exception: {ex.Message}");
        }
    }

    Console.WriteLine($"Error reading {filename}");
    Console.WriteLine("Search paths attempted:");
    foreach (var p in attempts) Console.WriteLine("  " + p);
    Console.WriteLine("Missing input file — aborting.");
    Environment.Exit(1);
    return Array.Empty<int>();
}

// ------------------------------------------------------------
// Run test
// ------------------------------------------------------------
var unordered = LoadFile("unordered.txt");
var expected  = LoadFile("ordered.txt");

var arr = (int[])unordered.Clone();

var (comparisons, writes) = InsertionSort(arr);

// Check correctness
bool ok = arr.SequenceEqual(expected);

// ------------------------------------------------------------
// Output results
// ------------------------------------------------------------
Console.WriteLine("\nInsertion Sort (C#)");
Console.WriteLine("-------------------");
Console.WriteLine($"Elements:     {arr.Length}");
Console.WriteLine($"Comparisons:  {comparisons}");
Console.WriteLine($"Writes:       {writes}");
Console.WriteLine($"Correct?      {(ok ? "YES ✔" : "NO ✘")}");

if (!ok)
{
    for (int i = 0; i < arr.Length; i++)
    {
        if (arr[i] != expected[i])
        {
            Console.WriteLine($"Mismatch at index {i}: got {arr[i]}, expected {expected[i]}");
            break;
        }
    }
}
else
{
    Console.WriteLine("\nFirst 10 sorted values:");
    Console.WriteLine(string.Join(" ", arr.Take(10)));
}
