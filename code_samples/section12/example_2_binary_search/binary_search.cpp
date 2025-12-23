#include <iostream>   // Provides standard input/output streams (cout)
#include <vector>     // Provides the std::vector container
#include <fstream>    // Provides file stream classes (ifstream)
#include <string>     // Provides the std::string class

using namespace std;  // Allows use of standard library names without std:: prefix

// =====================================================
// Given binarySearch (no changes)
// =====================================================

/**
 * Performs a standard binary search on a sorted vector of integers.
 *
 * @param arr     Sorted vector to search
 * @param target  Value to locate
 *
 * @return Index of the target if found; otherwise -1
 */
int binarySearch(const vector<int>& arr, int target) {
    int lo = 0;                        // Lower bound of search range (inclusive)
    int hi = (int)arr.size() - 1;      // Upper bound of search range (inclusive)

    // Continue searching while the range is valid
    while (lo <= hi) {
        // Compute midpoint safely to avoid overflow
        int mid = lo + (hi - lo) / 2;

        // Check for a match at the midpoint
        if (arr[mid] == target) {
            return mid;                // Target found
        }
        // Target is larger; discard left half
        else if (arr[mid] < target) {
            lo = mid + 1;
        }
        // Target is smaller; discard right half
        else {
            hi = mid - 1;
        }
    }

    // Target was not found in the vector
    return -1;
}

// Binary search with step counting

/**
 * Performs a binary search while counting the number of iterations performed.
 *
 * @param arr    Sorted vector to search
 * @param target Value to locate
 * @param steps  Output parameter used to record step count
 *
 * @return Index of the target if found; otherwise -1
 */
int binarySearchSteps(const vector<int>& arr, int target, int& steps) {
    steps = 0;                         // Reset step counter
    int lo = 0;                        // Lower bound of search range
    int hi = (int)arr.size() - 1;      // Upper bound of search range

    // Continue searching while the range is valid
    while (lo <= hi) {
        steps++;  // Count this iteration / comparison step

        // Compute midpoint of current search range
        int mid = lo + (hi - lo) / 2;

        // Check if midpoint element matches the target
        if (arr[mid] == target) return mid;

        // Decide which half of the array to continue searching
        if (arr[mid] < target)
            lo = mid + 1;              // Target lies in the right half
        else
            hi = mid - 1;              // Target lies in the left half
    }

    // Target was not found after exhausting the search range
    return -1;
}

// =====================================================
// File loader
// =====================================================

/**
 * Loads integers from a text file into a vector.
 *
 * @param path Path to the input file
 *
 * @return Vector containing integers read from the file,
 *         or an empty vector if the file cannot be opened
 */
vector<int> load_file(const string& path) {
    ifstream f(path);     // Open the file for reading
    vector<int> out;      // Container for loaded integers

    // Validate file open
    if (!f.is_open()) {
        cout << "Error reading: " << path << "\n";
        return out;       // Return empty vector on failure
    }

    int x;  // Temporary variable for input
    // Read integers until EOF or invalid input
    while (f >> x) out.push_back(x);

    return out;  // Vector is returned by value (move semantics apply)
}

// =====================================================
// Run binary search tests
// =====================================================

/**
 * Executes a standardized set of binary search tests on a dataset.
 *
 * @param arr Sorted vector of integers to test
 */
void run_tests(const vector<int>& arr) {
    cout << "=== Binary Search Tests ===\n";
    cout << "Loaded " << arr.size() << " integers\n\n";

    int idx;    // Stores index returned by search
    int steps;  // Stores number of steps taken

    // Test 1: Search for the first element (best-case scenario)
    idx = binarySearchSteps(arr, arr.front(), steps);
    cout << "Search first  (" << arr.front() << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Test 2: Search for the middle element
    int midVal = arr[arr.size() / 2];
    idx = binarySearchSteps(arr, midVal, steps);
    cout << "Search middle (" << midVal << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Test 3: Search for the last element (worst-case successful search)
    idx = binarySearchSteps(arr, arr.back(), steps);
    cout << "Search last   (" << arr.back() << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Test 4: Search for a value guaranteed not to exist
    idx = binarySearchSteps(arr, 999999, steps);
    cout << "Search missing (999999): index=" << idx
         << ", steps=" << steps << "\n\n";
}

// =====================================================
// MAIN
// =====================================================

/**
 * Program entry point.
 *
 * Loads an ordered dataset from disk,
 * runs binary search tests,
 * and exits.
 */
int main() {
    // Load sorted dataset from file
    vector<int> arr = load_file("..\\data\\ordered.txt");

    // Validate successful file load
    if (arr.empty()) {
        cout << "Failed to load ordered.txt\n";
        return 1;  // Exit with error code
    }

    // Run binary search benchmarks
    run_tests(arr);

    return 0;  // Successful program termination
}
