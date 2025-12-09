// void MergeSort(int[] arr) {
//     int n = arr.Length;
//     int[] tmp = new int[n];
//     MergeSortRec(arr, tmp, 0, n);
// }

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
long comparisons = 0;
long writes = 0;

// -------------------------------------------------------------
// Merge Sort implementation with step counting
// -------------------------------------------------------------
void MergeSort(int[] arr) {
    int n = arr.Length;
    int[] tmp = new int[n];
    MergeSortRec(arr, tmp, 0, n);
}

void MergeSortRec(int[] arr, int[] tmp, int left, int right) {
    if (right - left <= 1) return;
    int mid = left + (right - left) / 2;
    MergeSortRec(arr, tmp, left, mid);
    MergeSortRec(arr, tmp, mid, right);
    Merge(arr, tmp, left, mid, right);
}

void Merge(int[] arr, int[] tmp, int left, int mid, int right) {
    int i = left, j = mid, k = left;
    while (i < mid && j < right) {
        // comparison for arr[i] <= arr[j]
        comparisons++;
        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
        }
        writes++; // wrote to tmp
    }
    while (i < mid) {
        tmp[k++] = arr[i++];
        writes++;
    }
    while (j < right) {
        tmp[k++] = arr[j++];
        writes++;
    }
    for (int p = left; p < right; p++) {
        arr[p] = tmp[p];
        writes++; // copy back to arr
    }
}

// -------------------------------------------------------------
// File loading helpers
// -------------------------------------------------------------
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

    throw new FileNotFoundException(
        $"Could not find {fileName}.\nTried:\n  {path1}\n  {path2}");
}

int[] ReadIntFile(string fullPath) {
    try {
        string text = File.ReadAllText(fullPath);
        char[] sep = new[] { ' ', '\n', '\r', '\t' };
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
try {
    int[] unordered = LoadFile("unordered.txt");
    int[] expected = LoadFile("ordered.txt");

    Console.WriteLine($"Unordered length: {unordered.Length}");
    Console.WriteLine($"Expected length:  {expected.Length}");

    comparisons = 0;
    writes = 0;
    MergeSort(unordered);

    bool ok = unordered.SequenceEqual(expected);
    Console.WriteLine($"Sorted correctly? {ok}");

    Console.WriteLine($"Comparisons: {comparisons}");
    Console.WriteLine($"Writes:      {writes}");

    Console.WriteLine("First 10 sorted elements:");
    Console.WriteLine(string.Join(" ", unordered.Take(10)));
}
catch (Exception e) {
    Console.WriteLine("Error reading input files:");
    Console.WriteLine(e.Message);
    Console.WriteLine("Missing input file — aborting.");
}
