#include <iostream>  // Provides standard input/output streams (cout)
#include <vector>    // Provides the std::vector container
#include <fstream>   // Provides file stream classes (ifstream)
#include <cmath>     // Provides mathematical functions such as sqrt

using namespace std; // Allows use of standard library names without std:: prefix

// ============================================================
// Jump Search (index only)
// ============================================================

/**
 * Performs jump search on a sorted vector of integers.
 *
 * Jump search works in two phases:
 *  1) Jump ahead by a fixed block size (typically sqrt(n)) to find a block
 *     where the target could exist.
 *  2) Perform a linear scan within that block.
 *
 * @param arr     Sorted vector to search
 * @param target  Value to locate
 *
 * @return Index of the target if found; otherwise -1
 */
int jumpSearch(const vector<int>& arr, int target) {
    int n = (int)arr.size(); // Number of elements in the vector
    if (n == 0) return -1;   // Empty dataset => not found

    // Choose block size (jump length) as sqrt(n)
    int step = (int)std::sqrt(n);

    int prev = 0;    // Start of the current block
    int curr = step; // End of the current block (initially one step ahead)

    // --------------------------------------------------------
    // Jump phase: find a block where target could be
    // --------------------------------------------------------
    // Keep jumping while:
    //  - curr is in bounds
    //  - the value at arr[curr] is still less than target
    while (curr < n && arr[curr] < target) {
        prev = curr;     // Move block start forward
        curr += step;    // Jump to next block end
    }

    // Clamp curr to last valid index if we jumped beyond the vector end
    if (curr >= n) curr = n - 1;

    // --------------------------------------------------------
    // Linear search phase: scan within [prev, curr]
    // --------------------------------------------------------
    for (int i = prev; i <= curr; i++) {
        if (arr[i] == target) {
            return i; // Target found
        }
    }

    // Target was not found in the identified block
    return -1;
}

// ============================================================
// Jump Search with step counting
// steps counts the number of comparisons
// ============================================================

/**
 * Performs jump search while counting the number of comparisons made.
 *
 * This function counts comparisons in both phases:
 *  - Jump phase comparisons against arr[curr]
 *  - Linear scan comparisons against arr[i]
 *
 * @param arr    Sorted vector to search
 * @param target Value to locate
 * @param steps  Output parameter used to record comparison count
 *
 * @return Index of the target if found; otherwise -1
 */
int jumpSearchSteps(const vector<int>& arr, int target, int& steps) {
    steps = 0;               // Reset step counter for this search
    int n = (int)arr.size(); // Number of elements
    if (n == 0) return -1;   // Empty dataset => not found

    // Choose block size (jump length) as sqrt(n)
    int step = (int)std::sqrt(n);

    int prev = 0;    // Start of the current block
    int curr = step; // End of the current block

    // --------------------------------------------------------
    // Jump phase
    // --------------------------------------------------------
    while (curr < n) {
        // Count the comparison involving arr[curr] and target
        // (conceptually, this corresponds to checking whether we should stop jumping)
        steps++;                // compare arr[curr] < target or arr[curr] >= target

        // If block end is >= target, then target (if present) must be in this block
        if (arr[curr] >= target)
            break;

        // Otherwise, jump to the next block
        prev = curr;
        curr += step;
    }

    // Clamp curr to last valid index if we jumped beyond the vector end
    if (curr >= n) curr = n - 1;

    // --------------------------------------------------------
    // Linear scan within the block [prev, curr]
    // --------------------------------------------------------
    for (int i = prev; i <= curr; i++) {
        steps++;                // compare arr[i] == target

        if (arr[i] == target) {
            return i;           // Target found
        }
    }

    // Target not found in the identified block
    return -1;
}

// ============================================================
// Load ordered.txt into vector<int>
// ============================================================

/**
 * Loads integers from a text file into a vector<int>.
 *
 * @param path Path to the input file
 *
 * @return Vector of integers read from the file,
 *         or an empty vector on failure
 */
vector<int> loadFile(const string& path) {
    ifstream in(path);  // Open file for reading
    vector<int> arr;    // Container for loaded integers

    // Validate file open
    if (!in) {
        cout << "Error reading: " << path << "\n";
        return arr;     // Return empty vector on failure
    }

    int x;  // Temporary variable for extracted integers

    // Read integers until EOF or invalid input
    while (in >> x) {
        arr.push_back(x);
    }

    return arr; // Returned by value (move semantics apply)
}

// ============================================================
// Run tests on ordered data
// ============================================================

/**
 * Executes a standardized set of jump search tests on ordered data.
 *
 * @param arr Sorted vector of integers to test
 */
void runTests(const vector<int>& arr) {
    cout << "=== Jump Search Tests (ordered data only) ===\n";
    cout << "Loaded " << arr.size() << " integers\n\n";

    int steps = 0; // Holds comparison count for each search
    int idx;       // Holds search result index

    // Test 1: Search for the first element
    idx = jumpSearchSteps(arr, arr.front(), steps);
    cout << "Search first  (" << arr.front() << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Test 2: Search for the middle element
    int midVal = arr[arr.size() / 2];
    idx = jumpSearchSteps(arr, midVal, steps);
    cout << "Search middle (" << midVal << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Test 3: Search for the last element
    idx = jumpSearchSteps(arr, arr.back(), steps);
    cout << "Search last   (" << arr.back() << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Test 4: Search for a value guaranteed not to exist
    idx = jumpSearchSteps(arr, 999999, steps);
    cout << "Search missing (999999): index=" << idx
         << ", steps=" << steps << "\n\n";
}

// ============================================================
// MAIN
// ============================================================

/**
 * Program entry point.
 *
 * Loads an ordered dataset from disk, runs jump search tests,
 * and exits.
 */
int main() {
    // Adjust path as needed relative to where you run the binary
    // Here: compiled & run from code_samples/section12/lesson_3_binary_search
    // Data: ..\data\ordered.txt
    string path = "..\\data\\ordered.txt";

    // Load ordered dataset from file
    vector<int> ordered = loadFile(path);

    // Validate successful file load
    if (ordered.empty()) {
        cout << "Missing input file or failed to load.\n";
        return 1; // Exit with error code
    }

    // Run jump search benchmarks
    runTests(ordered);
    return 0; // Successful program termination
}
