// #include <vector>
// using std::vector;

/*
    -------------------------------------------------------------------------
    COMMENTED-OUT (baseline) MERGE SORT
    -------------------------------------------------------------------------
    The large commented block at the top is a "plain" merge sort implementation
    without step counting. Keeping it around is useful for instruction:

      - It shows the clean algorithm first (merge + recursive split)
      - Then the active code below adds instrumentation (Stats) to measure work

    In production, you’d typically delete this to reduce noise, but in a lesson
    it provides a helpful comparison.
*/

// static void merge(vector<int>& arr, vector<int>& tmp, int left, int mid, int right) {
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

// static void mergeSortRec(vector<int>& arr, vector<int>& tmp, int left, int right) {
//     if (right - left <= 1) return;
//     int mid = left + (right - left) / 2;
//     mergeSortRec(arr, tmp, left, mid);
//     mergeSortRec(arr, tmp, mid, right);
//     merge(arr, tmp, left, mid, right);
// }

// void mergeSort(vector<int>& arr) {
//     vector<int> tmp(arr.size());
//     mergeSortRec(arr, tmp, 0, (int)arr.size());
// }

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// ------------------------------------------------------------
// Step counting struct
// ------------------------------------------------------------
/*
    Stats tracks the amount of “work” merge sort performs.

    comparisons:
        - Counts element-to-element comparisons during the merge step.
        - Specifically, each time we evaluate: arr[i] <= arr[j].

    writes:
        - Counts assignment writes into arrays during merging.
        - This implementation counts:
            1) Each write into the temp buffer tmp[k]
            2) Each write copying merged values back into arr[p]

    Why count writes?
        - Merge sort’s time cost is not just comparisons; it also performs
          significant memory movement (writes), and uses extra memory (tmp).
*/
struct Stats {
    long long comparisons = 0;
    long long writes = 0;
};

// ------------------------------------------------------------
// Merge Sort with step counting
// ------------------------------------------------------------
/*
    merge_vec()
    -----------
    Merge two adjacent sorted ranges in `arr` into one sorted range.

    Parameters:
        arr   : main array containing two sorted halves
        tmp   : temporary buffer used to assemble merged output
        left  : inclusive start of left half
        mid   : start of right half (and end of left half)
        right : exclusive end of right half
        stats : reference to Stats accumulator

    Range convention (half-open intervals):
        left half  = [left, mid)
        right half = [mid, right)
        output     = [left, right)

    Step counting:
        - comparisons increments once per decision between arr[i] and arr[j]
        - writes increments for every assignment to tmp[] or arr[]
*/
static void merge_vec(vector<int>& arr, vector<int>& tmp,
                      int left, int mid, int right, Stats& stats)
{
    // i walks the left half, j walks the right half, k writes into tmp
    int i = left, j = mid, k = left;

    /*
        Main merge loop:
          - while both halves still have elements,
            pick the smaller front element and write it to tmp[k]
    */
    while (i < mid && j < right) {
        stats.comparisons++;                   // compare arr[i] vs arr[j]

        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];              // write left element to tmp
        } else {
            tmp[k++] = arr[j++];              // write right element to tmp
        }

        stats.writes++;                        // count the write into tmp
    }

    /*
        If anything remains in the left half, copy it into tmp.
        (At this point, the right half is exhausted.)
    */
    while (i < mid) {
        tmp[k++] = arr[i++];
        stats.writes++;                        // write into tmp
    }

    /*
        If anything remains in the right half, copy it into tmp.
        (At this point, the left half is exhausted.)
    */
    while (j < right) {
        tmp[k++] = arr[j++];
        stats.writes++;                        // write into tmp
    }

    /*
        Copy merged range back into the original array.
        This is necessary because future merges expect sorted data in `arr`.

        Note: This implementation counts these copy-back writes too,
        because they are real assignments and part of merge sort’s overhead.
    */
    for (int p = left; p < right; p++) {
        arr[p] = tmp[p];
        stats.writes++;                        // write back into arr
    }
}

/*
    mergeSortRec()
    -------------
    Recursive merge sort on the half-open interval [left, right).

    Base case:
        - If the range has size 0 or 1, it's already sorted.

    Recursive case:
        - Split into two halves
        - Sort each half recursively
        - Merge the sorted halves
*/
static void mergeSortRec(vector<int>& arr, vector<int>& tmp,
                         int left, int right, Stats& stats)
{
    // If range length <= 1, nothing to sort
    if (right - left <= 1) return;

    // Compute midpoint safely
    int mid = left + (right - left) / 2;

    // Sort left half, then right half
    mergeSortRec(arr, tmp, left, mid, stats);
    mergeSortRec(arr, tmp, mid, right, stats);

    // Merge the two sorted halves
    merge_vec(arr, tmp, left, mid, right, stats);
}

/*
    mergeSort()
    ----------
    Public wrapper that:
        - Allocates one temp buffer (same size as arr)
        - Runs recursive merge sort
        - Leaves sorted result in arr
        - Accumulates stats

    Note:
        - `stats` is NOT reset inside this function; the caller provides a
          fresh Stats (as main() does) to start counts at zero.
*/
void mergeSort(vector<int>& arr, Stats& stats)
{
    // tmp buffer reused for all merges (better than allocating each merge)
    vector<int> tmp(arr.size());

    mergeSortRec(arr, tmp, 0, (int)arr.size(), stats);
}

// ------------------------------------------------------------
// Utility: load integers from a file
// ------------------------------------------------------------
/*
    loadFile()
    ----------
    Reads integers from a text file into a vector<int>.

    Assumptions:
        - File contains whitespace-delimited integers.
        - Returns an empty vector if the file cannot be opened.

    Note:
        - Using `while (in >> x)` naturally stops at EOF or invalid input.
*/
vector<int> loadFile(const string& path)
{
    ifstream in(path);
    vector<int> arr;

    if (!in.is_open()) {
        cout << "Error reading: " << path << "\n";
        return {};
    }

    int x;
    while (in >> x) arr.push_back(x);
    return arr;
}

// ------------------------------------------------------------
// Compare sorted output vs expected
// ------------------------------------------------------------
/*
    compareArrays()
    --------------
    Compares two equally-sized arrays element-by-element.

    Prints:
        - Up to the first 10 mismatches for readability.

    Returns:
        - Total mismatch count.

    Assumption:
        - Caller ensures a.size() == b.size().
        - This function uses `a.size()` as the loop bound.
*/
int compareArrays(const vector<int>& a, const vector<int>& b)
{
    int mismatches = 0;
    int n = (int)a.size();

    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            if (mismatches < 10) {
                cout << "Mismatch at index " << i
                     << ": got " << a[i]
                     << ", expected " << b[i] << "\n";
            }
            mismatches++;
        }
    }

    return mismatches;
}

// ------------------------------------------------------------
// Main Test Harness
// ------------------------------------------------------------
/*
    main()
    ------
    Test workflow:
      1) Load unordered.txt (input data)
      2) Load ordered.txt   (expected sorted output)
      3) Validate both loaded and same length
      4) Run merge sort with step counting
      5) Print comparisons + writes
      6) Compare actual sorted output to expected

    Paths used:
      - "..\\data\\unordered.txt"
      - "..\\data\\ordered.txt"

    These assume you run the executable from a lesson folder that is a sibling
    of the "data" folder (consistent with your other examples).
*/
int main()
{
    string unorderedPath = "..\\data\\unordered.txt";
    string orderedPath   = "..\\data\\ordered.txt";

    cout << "Attempting to read: " << unorderedPath << "\n";
    vector<int> arr = loadFile(unorderedPath);

    // Empty vector is treated as "failed to load" in this harness.
    // (This also treats a legitimately empty data file as an error.)
    if (arr.empty()) {
        cout << "Missing unordered input file — aborting.\n";
        return 1;
    }

    cout << "Attempting to read: " << orderedPath << "\n";
    vector<int> expected = loadFile(orderedPath);
    if (expected.empty()) {
        cout << "Missing ordered input file — aborting.\n";
        return 1;
    }

    // Must be same length to do a 1:1 verification
    if (arr.size() != expected.size()) {
        cout << "File lengths differ! unordered=" << arr.size()
             << ", ordered=" << expected.size() << "\n";
        return 1;
    }

    // Stats counters start at 0 by default member initializers
    Stats stats;

    cout << "Sorting " << arr.size() << " elements with merge sort...\n";

    // Sort in place and accumulate step counts in stats
    mergeSort(arr, stats);

    cout << "\n--- Merge Sort Step Counts ---\n";
    cout << "Comparisons: " << stats.comparisons << "\n";
    cout << "Writes:      " << stats.writes << "\n";

    cout << "\nChecking sorted output...\n";
    int mismatches = compareArrays(arr, expected);

    if (mismatches == 0) {
        cout << "SUCCESS — output matches expected sorted list!\n";
    } else {
        cout << "FAIL — mismatches found: " << mismatches << "\n";
    }

    return 0;
}
