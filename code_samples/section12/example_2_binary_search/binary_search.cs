using System;
using System.IO;
using System.Linq;

// ================================
// Original binary search
// ================================
static int BinarySearch(int[] arr, int target)
{
    int lo = 0, hi = arr.Length - 1;
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target)
        {
            return mid;
        }
        else if (arr[mid] < target)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    return -1;
}

// Same, but counts the number of loop iterations (steps)
static (int index, int steps) BinarySearchSteps(int[] arr, int target)
{
    int lo = 0, hi = arr.Length - 1;
    int steps = 0;

    while (lo <= hi)
    {
        steps++;
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target)
            return (mid, steps);
        if (arr[mid] < target)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return (-1, steps);
}

// ================================
// Load ordered.txt using CURRENT DIRECTORY
// ================================
static int[] LoadOrderedFile()
{
    // When you run:
    //   cd code_samples\section12\lesson_3_binary_search
    //   dotnet script binary_search.csx
    // CurrentDirectory = that folder
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

// ================================
// MAIN (top-level for dotnet script)
// ================================
var arr = LoadOrderedFile();
if (arr.Length == 0)
{
    Console.WriteLine("Missing input file - aborting.");
    return;
}

Console.WriteLine("=== Binary Search Tests ===");
Console.WriteLine($"Loaded {arr.Length} integers\n");

// First element
var r = BinarySearchSteps(arr, arr[0]);
Console.WriteLine($"Search first  ({arr[0]}): index={r.index}, steps={r.steps}");

// Middle element
int midVal = arr[arr.Length / 2];
r = BinarySearchSteps(arr, midVal);
Console.WriteLine($"Search middle ({midVal}): index={r.index}, steps={r.steps}");

// Last element
int lastVal = arr[arr.Length - 1];
r = BinarySearchSteps(arr, lastVal);
Console.WriteLine($"Search last   ({lastVal}): index={r.index}, steps={r.steps}");

// Missing element
r = BinarySearchSteps(arr, 999_999);
Console.WriteLine($"Search missing (999999): index={r.index}, steps={r.steps}");
