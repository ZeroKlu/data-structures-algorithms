#include <iostream>   // Standard I/O streams (cout, cerr)
#include <fstream>    // File input stream (ifstream)
#include <vector>     // Dynamic array container
#include <string>     // std::string
#include <cmath>      // Mathematical utilities (not strictly required here)
using namespace std;

/* -------------------------------------------------------
   Binary Search helper (counts steps)
   Searches arr[left..right] inclusive.
   ------------------------------------------------------- */

/**
 * Performs a binary search on a sorted vector within the index
 * range [left, right], inclusive, while counting "steps".
 *
 * Step counting:
 *   - Each iteration of the while-loop increments the step counter.
 *   - Each iteration corresponds to one midpoint comparison.
 *
 * Important:
 *   - This function does NOT reset the step counter.
 *   - It assumes `steps` has already been initialized by the caller.
 *
 * @param arr    Sorted vector of integers
 * @param left   Left boundary index (inclusive)
 * @param right  Right boundary index (inclusive)
 * @param target Value to search for
 * @param steps  Reference to an integer used to accumulate step count
 *
 * @return Index of target if found; otherwise -1
 */
int binarySearchSteps(const vector<int>& arr, int left, int right, int target, int& steps) {
    // Standard binary search loop over a bounded range
    while (left <= right) {
        steps++;  // Count this binary-search probe

        // Compute midpoint safely
        int mid = left + (right - left) / 2;

        // Compare midpoint value against target
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1; // Search right half
        else right = mid - 1;                        // Search left half
    }

    // Target not found in the given range
    return -1;
}

/* -------------------------------------------------------
   Exponential Search (counts steps)
   ------------------------------------------------------- */

/**
 * Performs exponential search on a sorted vector while counting steps.
 *
 * Exponential search works in two phases:
 *   1) Exponential range expansion:
 *      Start at index 1 and repeatedly double the bound until
 *      arr[bound] >= target or bound exceeds array size.
 *   2) Binary search:
 *      Once a candidate range is identified, perform binary search
 *      within that range.
 *
 * Step counting includes:
 *   - The initial check of arr[0]
 *   - Each comparison during exponential range expansion
 *   - Each iteration performed by the binary-search helper
 *
 * @param arr    Sorted vector of integers
 * @param target Value to search for
 * @param steps  Reference to an integer that will receive the step count
 *
 * @return Index of target if found; otherwise -1
 */
int exponentialSearch(const vector<int>& arr, int target, int& steps) {
    steps = 0;                 // Reset step counter for this search
    int n = arr.size();        // Number of elements in the array
    if (n == 0) return -1;     // Empty array => not found

    // ---------------------------------------------------
    // Quick check of the first element
    // ---------------------------------------------------
    steps++;                  // Count check of arr[0]
    if (arr[0] == target) return 0;

    // ---------------------------------------------------
    // Phase 1: Exponential range expansion
    // ---------------------------------------------------
    int bound = 1;

    // Double the bound while:
    //  - bound is within array limits, and
    //  - arr[bound] is still less than the target
    while (bound < n && arr[bound] < target) {
        steps++;              // Count this range-expansion comparison
        bound *= 2;           // Exponentially increase the search bound
    }

    // Determine the final search range for binary search
    int left = bound / 2;
    int right = min(bound, n - 1);

    // ---------------------------------------------------
    // Phase 2: Binary search within the identified range
    // ---------------------------------------------------
    return binarySearchSteps(arr, left, right, target, steps);
}

/* -------------------------------------------------------
   Load integers from file
   ------------------------------------------------------- */

/**
 * Loads integers from a text file into a vector.
 *
 * Each integer is expected to be separated by whitespace.
 * The vector is appended to (not cleared), so the caller
 * should ensure it is empty if reuse is not desired.
 *
 * @param path Path to the input file
 * @param out  Vector to receive the integers
 *
 * @return true if file was successfully opened and read;
 *         false if the file could not be opened
 */
bool loadFile(const string& path, vector<int>& out) {
    ifstream in(path);
    if (!in.is_open()) {
        cerr << "Error reading: " << path << endl;
        return false;
    }

    int x;
    // Read integers until EOF or failure
    while (in >> x) out.push_back(x);
    return true;
}

/* -------------------------------------------------------
   MAIN TESTS
   ------------------------------------------------------- */

/**
 * Program entry point.
 *
 * Loads ordered data from disk and runs exponential search
 * on a series of test values, printing both the resulting
 * index and the number of steps required.
 */
int main() {
    // Path to ordered.txt relative to where this program is run
    string orderedPath = "..\\data\\ordered.txt";
    vector<int> arr;

    // Diagnostic output for file path
    cout << "Attempting to read: " << orderedPath << endl;

    // Load dataset from disk
    if (!loadFile(orderedPath, arr)) {
        cerr << "Missing input file - aborting.\n";
        return 1;
    }

    cout << "Loaded " << arr.size() << " integers.\n\n";

    int steps, index;

    // Test targets:
    // Includes values expected to exist and one likely missing (9999)
    vector<int> tests = { -5000, 0, 5000, 1234, -2500, 9999 };

    // Run exponential search for each target
    for (int target : tests) {
        index = exponentialSearch(arr, target, steps);
        cout << "Target " << target
             << " → index " << index
             << ", steps " << steps << endl;
    }

    return 0;
}
