// void MergeSort(int[] arr) {
//     int n = arr.Length;
//     int[] tmp = new int[n];
//     MergeSortRec(arr, tmp, 0, n);
// }

/*
    -------------------------------------------------------------------------
    COMMENTED-OUT (baseline) MERGE SORT
    -------------------------------------------------------------------------
    This top block is a “clean” merge sort without instrumentation.

    It’s kept here (commented) so you can compare:
      - the pure algorithm (merge + recursion)
      - versus the instrumented version below that counts comparisons + writes

    In a teaching context, this is useful scaffolding. In a production file,
    you’d likely remove it to reduce clutter.
*/

// void MergeSortRec(int[] arr, int[] tmp, int left, int right) {
//     if (right - left <= 1) return;
//     int mid = left + (right - left) / 2;
//     MergeSortRec(arr, tmp, left, mid);
//     MergeSortRec(arr, tmp, mid, right);
//     Merge(arr, tmp, left, mid, right);
// }

// void Merge(int[] arr, int[] tmp, int left, int mid, int right) {
//     int i = left, j = mid, k = left;
//     while (i < mid && j < right) {
//         if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
//         else                  tmp[k++] = arr[j++];
//     }
//     while (i < mid)  tmp[k++] = arr[i++];
//     while (j < right) tmp[k++] = arr[j++];
//     for (int p = left; p < right; p++) {
//         arr[p] = tmp[p];
//     }
// }

// -------------------------------------------------------------
// Step counters
// -------------------------------------------------------------
/*
    These globals count “work” done by merge sort.

    comparisons:
        - Counts comparisons between elements during the merge decision:
              arr[i] <= arr[j]

    writes:
        - Counts assignments into arrays (data movement), specifically:
            1) Each write into tmp[k]
            2) Each write copying tmp[p] back into arr[p]

    Why global counters?
        - Simplicity for a script-style (top-level) demo.
        - It avoids passing a Stats object through every call.

    Note:
        - Because these are global, always reset them before running another sort
          in the same process (as the harness does).
*/
long comparisons = 0;
long writes = 0;

// -------------------------------------------------------------
// Merge Sort implementation with step counting
// -------------------------------------------------------------
/*
    MergeSort()
    ----------
    Public wrapper:
      - Allocates a temporary buffer tmp (same size as arr)
      - Kicks off recursive merge sort over [0, n)

    Range convention:
      - Uses half-open interval [left, right)
        where right is exclusive.
*/
void MergeSort(int[] arr) {
    int n = arr.Length;
    int[] tmp = new int[n];            // temp buffer reused across merges
    MergeSortRec(arr, tmp, 0, n);
}

/*
    MergeSortRec()
    --------------
    Recursively sorts arr[left..right) using tmp as a buffer.

    Base case:
      - If the segment length is 0 or 1, it is already sorted.

    Recursive case:
      - Split into [left, mid) and [mid, right)
      - Sort each half
      - Merge them back together
*/
void MergeSortRec(int[] arr, int[] tmp, int left, int right) {
    if (right - left <= 1) return;     // base case: size 0 or 1

    int mid = left + (right - left) / 2;

    MergeSortRec(arr, tmp, left, mid);
    MergeSortRec(arr, tmp, mid, right);

    Merge(arr, tmp, left, mid, right);
}

/*
    Merge()
    -------
    Merges two sorted ranges into one sorted range.

    Inputs:
      - arr[left..mid)   sorted
      - arr[mid..right)  sorted

    Output:
      - arr[left..right) sorted

    How it works:
      1) Merge into tmp[left..right)
      2) Copy tmp[left..right) back into arr[left..right)

    Step counting in this function:
      - comparisons++ each time we compare arr[i] and arr[j]
      - writes++ each time we write into tmp or copy back into arr
*/
void Merge(int[] arr, int[] tmp, int left, int mid, int right) {
    int i = left;      // pointer into left half
    int j = mid;       // pointer into right half
    int k = left;      // pointer into tmp output region

    // Merge while both halves still have remaining elements
    while (i < mid && j < right) {
        // comparison for arr[i] <= arr[j]
        comparisons++;

        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];       // write from left half
        } else {
            tmp[k++] = arr[j++];       // write from right half
        }

        writes++;                      // wrote one element into tmp
    }

    // Copy any remaining left-half elements
    while (i < mid) {
        tmp[k++] = arr[i++];
        writes++;                      // wrote into tmp
    }

    // Copy any remaining right-half elements
    while (j < right) {
        tmp[k++] = arr[j++];
        writes++;                      // wrote into tmp
    }

    // Copy merged result back into arr
    for (int p = left; p < right; p++) {
        arr[p] = tmp[p];
        writes++;                      // wrote into arr
    }
}

// -------------------------------------------------------------
// File loading helpers
// -------------------------------------------------------------
/*
    LoadFile()
    ----------
    Attempts to locate and load an integer file (unordered.txt / ordered.txt).

    Strategy:
      1) Try a relative path: ../data/<fileName> from the current working dir.
         This matches the “run from lesson folder” workflow.

      2) If that fails, try an explicit Desktop-based path that matches your
         known project layout:
            Desktop/data-structures-algorithms/code_samples/section12/data/<fileName>

    If both fail:
      - Throws FileNotFoundException with a helpful “Tried:” message.
*/
int[] LoadFile(string fileName) {
    // 1) Try ../data/<fileName> relative to current working directory
    string cwd = Environment.CurrentDirectory;
    string path1 = Path.GetFullPath(Path.Combine(cwd, "..", "data", fileName));
    Console.WriteLine($"Attempting to read (relative): {path1}");
    if (File.Exists(path1)) {
        return ReadIntFile(path1);
    }

    // 2) Try explicit known project path on Desktop
    string desktop = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);
    string path2 = Path.Combine(desktop,
        "data-structures-algorithms",
        "code_samples",
        "section12",
        "data",
        fileName);
    Console.WriteLine($"Relative path not found, trying explicit path: {path2}");
    if (File.Exists(path2)) {
        return ReadIntFile(path2);
    }

    // If we got here, neither candidate existed.
    throw new FileNotFoundException(
        $"Could not find {fileName}.\nTried:\n  {path1}\n  {path2}");
}

/*
    ReadIntFile()
    ------------
    Reads the entire file as text, splits on whitespace, and parses ints.

    Notes:
      - Splitting on multiple whitespace characters allows the file to be
        space-separated, newline-separated, or mixed.
      - Any parse error will throw, which is fine for a demo; the outer harness
        catches exceptions and prints a friendly message.

    Error handling:
      - Logs the failing path + exception message and rethrows so the caller
        can handle it at a higher level.
*/
int[] ReadIntFile(string fullPath) {
    try {
        string text = File.ReadAllText(fullPath);

        // Treat spaces, tabs, and newlines as separators
        char[] sep = [' ', '\n', '\r', '\t'];

        var nums = text
            .Split(sep, StringSplitOptions.RemoveEmptyEntries)
            .Select(int.Parse)
            .ToArray();

        return nums;
    } catch (Exception ex) {
        Console.WriteLine($"Error reading: {fullPath}");
        Console.WriteLine(ex.Message);
        throw;
    }
}

// -------------------------------------------------------------
// Test harness (top-level)
// -------------------------------------------------------------
/*
    This is a script-style “top-level program” harness.

    Flow:
      1) Load unordered.txt and ordered.txt
      2) Print lengths for sanity
      3) Reset counters (important since they’re globals)
      4) MergeSort(unordered)
      5) Verify correctness by SequenceEqual(expected)
      6) Print comparisons and writes
      7) Print first 10 sorted elements for a quick glance

    The catch block:
      - Handles missing files and any parse/read failures.
      - Prints a simple message and aborts without crashing the runtime.
*/
try {
    int[] unordered = LoadFile("unordered.txt");
    int[] expected = LoadFile("ordered.txt");

    Console.WriteLine($"Unordered length: {unordered.Length}");
    Console.WriteLine($"Expected length:  {expected.Length}");

    // Because comparisons/writes are global, reset before measuring this run.
    comparisons = 0;
    writes = 0;

    // Sort in place
    MergeSort(unordered);

    // Verify sorted output matches the known-good ordered.txt
    bool ok = unordered.SequenceEqual(expected);
    Console.WriteLine($"Sorted correctly? {ok}");

    // Print instrumentation counters
    Console.WriteLine($"Comparisons: {comparisons}");
    Console.WriteLine($"Writes:      {writes}");

    // Quick peek at sorted result
    Console.WriteLine("First 10 sorted elements:");
    Console.WriteLine(string.Join(" ", unordered.Take(10)));
}
catch (Exception e) {
    // Any exception here likely means:
    //   - file not found
    //   - read error
    //   - parse error
    Console.WriteLine("Error reading input files:");
    Console.WriteLine(e.Message);
    Console.WriteLine("Missing input file — aborting.");
}
