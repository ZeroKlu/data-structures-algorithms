/*
    quick_sort.cpp
    -------------
    Quick Sort (C++) using Lomuto partitioning, with step counting and file-based tests.

    PURPOSE
    -------
    This program:
      1) loads integers from unordered.txt
      2) sorts them using Quick Sort (Lomuto partition scheme)
      3) compares the sorted output to ordered.txt (known-correct sorted list)
      4) prints step counts (comparisons + writes) to illustrate algorithm cost

    INPUT FILES
    -----------
    - unordered.txt : integers in arbitrary order
    - ordered.txt   : the same integers sorted ascending

    Integers can be separated by any whitespace (spaces/newlines/tabs).

    STEP COUNTING MODEL
    -------------------
    We count two things:

      - g_comparisons:
          In partition(), every time we evaluate the key comparison:
              arr[j] <= pivot

      - g_writes:
          We treat each swap as 3 "writes" (a common teaching approximation):
              tmp = a
              a = b
              b = tmp

          In this C++ version we use std::swap(), but we still *count* it
          as 3 writes to match the convention used throughout your examples.

    COMPLEXITY (Big-O)
    ------------------
    Average case:  O(n log n)
    Worst case:    O(n^2)     (can happen when pivot choice is poor)
    Best case:     O(n log n)

    IMPORTANT NOTE ABOUT PIVOT CHOICE
    --------------------------------
    This implementation always chooses the last element as the pivot:
        pivot = arr[right]

    If the input is already sorted (or nearly sorted), this can degrade to
    worst-case O(n^2). That’s OK for teaching, but in production you’d
    typically choose a better pivot strategy (random pivot, median-of-three, etc.).
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;

// ------------------------------------------------------------
// GLOBAL COUNTERS
// ------------------------------------------------------------
// These are reset by quickSort() for each run.
static long long g_comparisons = 0;  // counts comparisons: arr[j] <= pivot
static long long g_writes      = 0;  // counts "writes" (data movement), swap=3 writes

// ------------------------------------------------------------
// Quick Sort (Lomuto partition) with step counting
// ------------------------------------------------------------
/*
    partition()
    -----------
    Lomuto partition scheme:

      - Choose pivot = arr[right] (last element)
      - Maintain index i such that:
            arr[left .. i-1]  <= pivot
            arr[i    .. j-1]  >  pivot (in-progress scanning region)

      - Scan j from left to right-1:
            if arr[j] <= pivot:
                swap arr[i] and arr[j]
                i++

      - Finally swap pivot into position i:
            swap arr[i] and arr[right]

      - Return i (pivot’s final index)

    STEP COUNTING
    -------------
      - g_comparisons++ for each evaluation of (arr[j] <= pivot)
      - g_writes += 3 for each swap (teaching approximation)
*/
static int partition(vector<int>& arr, int left, int right) {
    // Pivot selection: last element in the current partition
    int pivot = arr[right];

    // i marks the boundary between <= pivot and > pivot regions
    int i = left;

    // Scan all elements except the pivot itself
    for (int j = left; j < right; ++j) {
        g_comparisons++; // compare arr[j] <= pivot

        // If current element belongs in the "small" (<= pivot) side
        if (arr[j] <= pivot) {
            // Swap arr[i] with arr[j] to grow the <= pivot region
            std::swap(arr[i], arr[j]);

            // Count the cost of swapping values (approx as 3 writes)
            g_writes += 3;

            // Move boundary forward
            ++i;
        }
    }

    // Put pivot in its final sorted position
    std::swap(arr[i], arr[right]);
    g_writes += 3;

    // i is now the pivot's index
    return i;
}

/*
    quickSortRec()
    --------------
    Recursively sorts arr[left..right] in-place.

    Base case:
      - if left >= right, the range has 0 or 1 element, so it is already sorted.

    Recursive case:
      1) partition arr[left..right] around pivot -> get pivot index p
      2) sort left side:  arr[left..p-1]
      3) sort right side: arr[p+1..right]
*/
static void quickSortRec(vector<int>& arr, int left, int right) {
    if (left < right) {
        int p = partition(arr, left, right);

        // Recursively sort the partitions around the pivot
        quickSortRec(arr, left, p - 1);
        quickSortRec(arr, p + 1, right);
    }
}

/*
    quickSort()
    ----------
    Public wrapper:
      - resets step counters
      - sorts the entire vector if it is not empty
*/
void quickSort(vector<int>& arr) {
    g_comparisons = 0;
    g_writes = 0;

    if (!arr.empty()) {
        quickSortRec(arr, 0, static_cast<int>(arr.size()) - 1);
    }
}

// ------------------------------------------------------------
// FILE LOADER (tries several relative paths)
// ------------------------------------------------------------
/*
    loadFile()
    ----------
    Loads integers from a whitespace-separated text file.

    WHY MULTIPLE PATHS?
    -------------------
    Students often run executables from different working directories.
    To reduce "file not found" issues, we try several likely locations:

      1) <CWD>/<filename>
      2) <CWD>/data/<filename>
      3) <CWD>/../data/<filename>       (common course layout)
      4) <CWD>/../../data/<filename>    (if run from deeper folders)

    On success:
      - prints which path was used
      - returns a vector<int> with all loaded integers

    On failure:
      - prints attempted paths and terminates the program
*/
vector<int> loadFile(const string& filename) {
    const char* prefixes[] = {
        "",             // filename in current directory
        "data/",        // ./data/filename
        "../data/",     // ../data/filename
        "../../data/",  // ../../data/filename
        nullptr
    };

    std::ifstream in;
    string full;

    // Try opening the file in each candidate location
    for (int i = 0; prefixes[i] != nullptr; ++i) {
        full = string(prefixes[i]) + filename;
        in.open(full);

        if (in.is_open()) {
            cout << "Loaded: " << full << "\n";
            break;
        }

        // Clear the stream state so we can reuse it for another open()
        in.clear();
    }

    // If still not open, none of the attempts worked
    if (!in.is_open()) {
        cout << "Error reading: " << filename << "\n";
        cout << "Search paths attempted:\n";
        for (int i = 0; prefixes[i] != nullptr; ++i) {
            cout << "  " << prefixes[i] << filename << "\n";
        }
        cout << "Missing input file — aborting.\n";
        std::exit(1);
    }

    // Read ints until EOF
    vector<int> arr;
    int x;
    while (in >> x) {
        arr.push_back(x);
    }

    return arr;
}

// ------------------------------------------------------------
// MAIN TEST
// ------------------------------------------------------------
/*
    main()
    ------
    1) Load unordered.txt and ordered.txt
    2) Ensure they contain the same number of integers
    3) Copy unordered into a working array
    4) Sort with quickSort()
    5) Verify sorted output equals ordered.txt
    6) Print step counts and a small output sample
*/
int main() {
    // Load data
    vector<int> unordered = loadFile("unordered.txt");
    vector<int> expected  = loadFile("ordered.txt");

    // Sanity check: both files must contain the same integers, thus same length
    if (unordered.size() != expected.size()) {
        cout << "Mismatch in file lengths ("
             << unordered.size() << " vs " << expected.size()
             << ") — aborting.\n";
        return 1;
    }

    // Copy and sort (we keep unordered intact by sorting a copy)
    vector<int> arr = unordered;
    quickSort(arr);

    // Validate sorted output against expected
    bool ok = true;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] != expected[i]) {
            ok = false;
            cout << "Mismatch at index " << i
                 << ": got " << arr[i]
                 << ", expected " << expected[i] << "\n";
            break;
        }
    }

    // Print results
    cout << "\nQuick Sort (C++)\n";
    cout << "----------------\n";
    cout << "Elements:     " << arr.size() << "\n";
    cout << "Comparisons:  " << g_comparisons << "\n";
    cout << "Writes:       " << g_writes << "\n";
    cout << "Correct?      " << (ok ? "YES \xE2\x9C\x94" : "NO \xE2\x9D\x8B") << "\n";

    // Show a small sample if correct
    if (ok) {
        cout << "\nFirst 10 sorted values:\n";
        for (size_t i = 0; i < 10 && i < arr.size(); ++i) {
            cout << arr[i] << " ";
        }
        cout << "\n";
    }

    return 0;
}
