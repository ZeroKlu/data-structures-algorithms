#nullable enable

// ============================================================================
// Example 11 - Heap Sort with Comparisons & Writes (C# script)
// ============================================================================
//
// PURPOSE
// -------
// Demonstrate Heap Sort on an integer array while counting "steps" to help
// illustrate time complexity.
//
// INPUT FILES
// -----------
// This script expects TWO text files containing the SAME integers:
//
//   - unordered.txt : unsorted integers
//   - ordered.txt   : the same integers in sorted ascending order
//
// Integers can be separated by any whitespace (spaces, tabs, newlines).
//
// PROGRAM FLOW
// ------------
// 1) Load unordered.txt and ordered.txt using a robust multi-path loader.
// 2) Heap-sort a COPY of the unordered data (so original remains unchanged).
// 3) Track:
//      - comparisons: number of data-to-data comparisons (arr[a] vs arr[b])
//      - writes:      number of array writes (swap counted as 3 writes)
// 4) Verify the sorted output matches ordered.txt exactly.
// 5) Print a summary report and PASS/FAIL.
//
// STEP COUNTING MODEL
// -------------------
// comparisons:
//   - We count only comparisons between array VALUES, e.g.:
//       arr[left]  > arr[largest]
//       arr[right] > arr[largest]
//
//   - We do NOT count index bounds checks (left < size, right < size).
//
// writes:
//   - We treat a swap as 3 writes (tmp + two assignments) to match the
//     convention used in your other sorting examples.
//
// HEAP SORT NOTES
// --------------
// - This implementation builds a MAX-HEAP (largest element at the root).
// - Then it repeatedly swaps the root (max) to the end and heapifies the
//   remaining prefix.
// - Time:  O(n log n)
// - Space: O(1) extra (in-place), aside from the copy we make for testing.
//
// ============================================================================

using System;
using System.IO;
using System.Linq;

// -------------------------------------------------
// Heapify: restore MAX-HEAP property for subtree at index i
// -------------------------------------------------
// A MAX-HEAP has the property that every parent node is >= its children.
// heapify() assumes that the left/right subtrees of i are already heaps,
// and it fixes the node i by "sifting down" until the heap property holds.
//
// Parameters:
//   arr         : array being heap-sorted (mutated in place)
//   size        : current heap size (only indices [0..size-1] are in the heap)
//   i           : root index of the subtree we want to heapify
//   comparisons : step counter (by ref) for data comparisons
//   writes      : step counter (by ref) for array writes
//
// Counting rules:
//   - comparisons++ when we compare arr[left]  vs arr[largest]
//   - comparisons++ when we compare arr[right] vs arr[largest]
//   - writes += 3 when we swap two elements (swap counted as 3 writes)
static void Heapify(int[] arr, int size, int i, ref long comparisons, ref long writes)
{
    // Start by assuming the current root is the largest
    int largest = i;

    // Children indices in a 0-based array heap:
    //   left  child = 2*i + 1
    //   right child = 2*i + 2
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // If left child exists, compare it to current "largest"
    if (left < size)
    {
        comparisons++; // compare arr[left] > arr[largest]
        if (arr[left] > arr[largest])
            largest = left;
    }

    // If right child exists, compare it to current "largest"
    if (right < size)
    {
        comparisons++; // compare arr[right] > arr[largest]
        if (arr[right] > arr[largest])
            largest = right;
    }

    // If either child was larger, swap root with the largest child
    // and continue heapifying down the affected subtree.
    if (largest != i)
    {
        // Swap arr[i] <-> arr[largest]
        (arr[largest], arr[i]) = (arr[i], arr[largest]);

        // Count swap as 3 array writes (convention used across your lessons)
        writes += 3;

        // Recursively restore heap property below the swapped child
        Heapify(arr, size, largest, ref comparisons, ref writes);
    }
}

// -------------------------------------------------
// HeapSort: in-place heap sort with step counting
// -------------------------------------------------
// Steps:
//   1) Build a max heap from the array.
//   2) Repeatedly swap max element at root with the last element in the heap,
//      shrink the heap size, then heapify from root.
//
// Parameters:
//   arr         : array to sort IN PLACE
//   comparisons : step counter (by ref)
//   writes      : step counter (by ref)
static void HeapSort(int[] arr, ref long comparisons, ref long writes)
{
    int n = arr.Length;

    // NOTE: We do NOT reset comparisons/writes here, because the caller
    // might want to accumulate counts across multiple runs. In this script
    // we reset them in the test harness before calling HeapSort.

    // -------------------------
    // 1) Build max heap
    // -------------------------
    // The last parent node is at index n/2 - 1.
    // All nodes beyond that are leaves and already satisfy heap property.
    for (int i = n / 2 - 1; i >= 0; i--)
        Heapify(arr, n, i, ref comparisons, ref writes);

    // -------------------------
    // 2) Extract elements
    // -------------------------
    // Move current max (arr[0]) to the end, then restore heap on [0..end-1].
    for (int end = n - 1; end > 0; end--)
    {
        // Swap the root (max) with the last element in the current heap.
        (arr[end], arr[0]) = (arr[0], arr[end]);
        writes += 3; // swap counted as 3 writes

        // Restore heap property on the reduced heap prefix [0..end-1]
        Heapify(arr, end, 0, ref comparisons, ref writes);
    }
}

// -------------------------------------------------
// File loader with multiple search paths
// -------------------------------------------------
// We try several likely file locations to reduce "where did my file go?"
//
// attempts include:
//   - <cwd>/<filename>
//   - <cwd>/data/<filename>
//   - <cwd>/../data/<filename>
//   - <cwd>/../../data/<filename>
//   - <cwd>/../section12/data/<filename>
//   - <cwd>/../../section12/data/<filename>
//
// On success: prints the path used and returns the loaded integers.
// On failure: prints diagnostics and exits the program.
static int[] LoadFile(string filename)
{
    string cwd = Environment.CurrentDirectory;

    // NOTE: Using C# list/collection expression for readability.
    // These are all "full" paths (after Path.Combine).
    string[] attempts =
    [
        Path.Combine(cwd, filename),
        Path.Combine(cwd, "data", filename),
        Path.Combine(cwd, "..", "data", filename),
        Path.Combine(cwd, "..", "..", "data", filename),
        Path.Combine(cwd, "..", "section12", "data", filename),
        Path.Combine(cwd, "..", "..", "section12", "data", filename),
    ];

    // Try each candidate path until one exists
    foreach (var path in attempts)
    {
        if (File.Exists(path))
        {
            Console.WriteLine($"Loaded: {path}");

            // Read entire file and split by ANY whitespace.
            // Passing null for separators means "split on whitespace".
            string text = File.ReadAllText(path);
            var tokens = text.Split((char[]?)null, StringSplitOptions.RemoveEmptyEntries);

            // Convert tokens to int[]
            return [.. tokens.Select(int.Parse)];
        }
    }

    // If we get here, none of the paths worked. Print good diagnostics.
    Console.WriteLine($"Error reading input file: {filename}");
    Console.WriteLine($"Working directory: {cwd}");
    Console.WriteLine("Search paths attempted:");
    foreach (var p in attempts)
        Console.WriteLine("  " + p);

    Console.WriteLine("Missing input file — aborting.");
    Environment.Exit(1);

    return []; // unreachable, but keeps compiler happy
}

// -------------------------------------------------
// Top-level test harness
// -------------------------------------------------
// 1) Load unordered + expected
// 2) Sort a copy with HeapSort
// 3) Validate matches expected
// 4) Print step counts
var unordered = LoadFile("unordered.txt");
var expected  = LoadFile("ordered.txt");

// Basic sanity check: both inputs should have same length
if (unordered.Length != expected.Length)
{
    Console.WriteLine("Length mismatch between unordered and ordered arrays.");
    Console.WriteLine($"unordered.Length = {unordered.Length}");
    Console.WriteLine($"expected.Length  = {expected.Length}");
    Environment.Exit(1);
}

// Sort a COPY so unordered remains intact for debugging if needed
var arr = (int[])unordered.Clone();

// Reset counters for this run
long comparisons = 0;
long writes = 0;

// Sort in place
HeapSort(arr, ref comparisons, ref writes);

// -------------------------------------------------
// Validate output vs expected
// -------------------------------------------------
// We do an element-by-element comparison so we can print the first mismatch.
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

// -------------------------------------------------
// Print summary report
// -------------------------------------------------
Console.WriteLine();
Console.WriteLine("Heap Sort (C#)");
Console.WriteLine("--------------");
Console.WriteLine($"Elements:     {arr.Length}");
Console.WriteLine($"Comparisons:  {comparisons}");
Console.WriteLine($"Writes:       {writes}");
Console.WriteLine($"Correct?      {(ok ? "YES ✓" : "NO ✗")}");

// Optional sanity sample
if (ok)
{
    Console.WriteLine("\nFirst 10 sorted values:");
    Console.WriteLine(string.Join(" ", arr.Take(Math.Min(10, arr.Length))));
}
