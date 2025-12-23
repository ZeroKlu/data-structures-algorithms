#include <iostream>  // Provides standard input/output streams (cout)
#include <vector>    // Provides the std::vector container
#include <fstream>   // Provides file stream classes (ifstream)

using namespace std; // Allows use of standard library names without std:: prefix

// ============================================================
// Interpolation Search (index-only)
// Requires: arr sorted in ascending order
// ============================================================

/**
 * Performs interpolation search on a sorted vector of integers.
 *
 * Interpolation search is similar to binary search, but instead of always
 * selecting the midpoint, it estimates the likely index of the target based
 * on the target's value relative to the values at the current bounds.
 *
 * This approach tends to work best when values are fairly uniformly distributed.
 *
 * @param arr     Sorted vector to search
 * @param target  Value to locate
 *
 * @return Index of the target if found; otherwise -1
 */
int interpolationSearch(const vector<int>& arr, int target) {
    int n = (int)arr.size(); // Number of elements
    if (n == 0) return -1;   // Empty dataset => not found

    int lo = 0;              // Lower bound of the search range (inclusive)
    int hi = n - 1;          // Upper bound of the search range (inclusive)

    // Continue searching while:
    //  - bounds are valid, and
    //  - target is within the value range [arr[lo], arr[hi]]
    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {

        // Special case: if arr[lo] == arr[hi], all values in [lo, hi] are equal.
        // If that shared value equals target, return lo; otherwise, target can't exist.
        if (arr[hi] == arr[lo]) {
            // All values in [lo, hi] are the same
            if (arr[lo] == target) return lo;
            return -1;
        }

        // --------------------------------------------------------
        // Estimate probable position using linear interpolation:
        //
        // pos = lo + (hi - lo) * (target - arr[lo]) / (arr[hi] - arr[lo])
        //
        // Casting to double avoids integer truncation during division,
        // then the result is converted back to int for indexing.
        // --------------------------------------------------------
        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                             (arr[hi] - arr[lo]));

        // Guard: pos should generally fall within [lo, hi], but rounding or
        // unusual distributions can produce out-of-range estimates.
        if (pos < lo || pos > hi) {
            return -1;
        }

        // Compare against the estimated position
        if (arr[pos] == target) {
            return pos;            // Target found
        } else if (arr[pos] < target) {
            lo = pos + 1;          // Target must be in the right subrange
        } else {
            hi = pos - 1;          // Target must be in the left subrange
        }
    }

    // Target not found (range exhausted or target out of bounds)
    return -1;
}

// ============================================================
// Interpolation Search WITH step counting
// steps = number of comparisons
// ============================================================

/**
 * Performs interpolation search while counting selected comparisons.
 *
 * Step counting here focuses on key comparisons in the algorithm:
 *  - Checking if arr[hi] == arr[lo] (uniform-range check)
 *  - Checking if arr[lo] == target when uniform-range
 *  - Checking if arr[pos] == target
 *  - Checking if arr[pos] < target (to decide which side to continue searching)
 *
 * Note: The while-loop boundary checks (lo<=hi and target range checks)
 * are not counted here; only the labeled comparisons are counted.
 *
 * @param arr    Sorted vector to search
 * @param target Value to locate
 * @param steps  Output parameter used to record comparison count
 *
 * @return Index of the target if found; otherwise -1
 */
int interpolationSearchSteps(const vector<int>& arr, int target, int& steps) {
    steps = 0;               // Reset step counter for this search
    int n = (int)arr.size(); // Number of elements
    if (n == 0) return -1;   // Empty dataset => not found

    int lo = 0;              // Lower bound of search range
    int hi = n - 1;          // Upper bound of search range

    // Continue searching while target remains within the current value bounds
    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {

        steps++;  // compare arr[hi] == arr[lo]
        if (arr[hi] == arr[lo]) {
            // All values in range are equal; only possible match is that value
            steps++;  // compare arr[lo] == target
            if (arr[lo] == target) {
                return lo;   // Target found
            } else {
                return -1;   // Target not present
            }
        }

        // Estimate the likely position of the target
        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                             (arr[hi] - arr[lo]));

        // Guard: reject out-of-range estimated positions
        if (pos < lo || pos > hi) {
            return -1;
        }

        steps++;  // compare arr[pos] == target
        if (arr[pos] == target) {
            return pos;      // Target found
        }

        steps++;  // compare arr[pos] < target
        if (arr[pos] < target) {
            lo = pos + 1;    // Search right subrange
        } else {
            hi = pos - 1;    // Search left subrange
        }
    }

    // Target not found
    return -1;
}

// ============================================================
// Load integers from file into vector<int>
// ============================================================

/**
 * Loads integers from a text file into a vector<int>.
 *
 * @param path Path to the input file
 *
 * @return Vector of integers read from the file,
 *         or an empty vector if the file cannot be opened
 */
vector<int> loadFile(const string& path) {
    ifstream in(path);  // Open file for reading
    vector<int> arr;    // Container for loaded integers

    // Validate file open
    if (!in) {
        cout << "Error reading: " << path << "\n";
        return arr;     // Return empty vector on failure
    }

    int x; // Temporary input variable

    // Read integers until EOF or invalid input
    while (in >> x) {
        arr.push_back(x);
    }

    return arr; // Returned by value (move semantics apply)
}

// ============================================================
// Run tests: first, middle, last, missing
// ============================================================

/**
 * Executes a standardized set of interpolation search tests on ordered data.
 *
 * @param arr Sorted vector of integers to test
 */
void runTests(const vector<int>& arr) {
    cout << "=== Interpolation Search Tests (ordered data only) ===\n";
    cout << "Loaded " << arr.size() << " integers\n\n";

    int steps = 0; // Holds comparison count for each search
    int idx;       // Holds returned index

    // Test 1: Search for the first element
    idx = interpolationSearchSteps(arr, arr.front(), steps);
    cout << "Search first  (" << arr.front() << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Test 2: Search for the middle element
    int midVal = arr[arr.size() / 2];
    idx = interpolationSearchSteps(arr, midVal, steps);
    cout << "Search middle (" << midVal << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Test 3: Search for the last element
    int lastVal = arr.back();
    idx = interpolationSearchSteps(arr, lastVal, steps);
    cout << "Search last   (" << lastVal << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Test 4: Search for a value guaranteed not to exist
    idx = interpolationSearchSteps(arr, 999999, steps);
    cout << "Search missing (999999): index=" << idx
         << ", steps=" << steps << "\n\n";
}

// ============================================================
// MAIN
// ============================================================

/**
 * Program entry point.
 *
 * Loads an ordered dataset from disk, runs interpolation search tests,
 * and exits.
 */
int main() {
    // If compiled & run from: code_samples/section12/lesson_3_interpolation_search
    // then this path points to: code_samples/section12/data/ordered.txt
    string path = "..\\data\\ordered.txt";

    // Load ordered dataset from file
    vector<int> arr = loadFile(path);

    // Validate successful file load
    if (arr.empty()) {
        cout << "Missing input file or failed to load.\n";
        return 1; // Exit with error code
    }

    // Run interpolation search benchmarks
    runTests(arr);

    return 0; // Successful program termination
}
