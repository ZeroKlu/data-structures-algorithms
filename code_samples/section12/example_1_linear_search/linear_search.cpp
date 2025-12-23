#include <iostream>   // Provides standard input/output streams (cout, cin)
#include <fstream>    // Provides file stream classes (ifstream)
#include <vector>     // Provides the std::vector container
#include <string>     // Provides the std::string class

using namespace std;  // Allows use of standard library names without std:: prefix

// ===============================
// Linear Search with Step Counting
// ===============================

/**
 * Performs a linear search on a vector of integers while counting comparisons.
 *
 * @param arr       Reference to the input vector to search
 * @param target    Value to search for
 * @param stepsOut  Output parameter to record the number of comparisons made
 *
 * @return Index of the target if found, otherwise -1
 */
int linearSearchSteps(const vector<int>& arr, int target, long long& stepsOut) {
    long long steps = 0;  // Tracks the number of element comparisons

    // Iterate sequentially through the vector
    for (int i = 0; i < (int)arr.size(); i++) {
        steps++; // Count the comparison arr[i] == target

        // Check if the current element matches the target
        if (arr[i] == target) {
            stepsOut = steps; // Output total steps taken
            return i;         // Return index where target was found
        }
    }

    // Target was not found after scanning entire vector
    stepsOut = steps;         // Total comparisons equals vector size
    return -1;                // Indicate unsuccessful search
}

// ===============================
// Load integers from file
// ===============================

/**
 * Reads integers from a text file into a vector.
 *
 * @param path  Path to the input file
 *
 * @return Vector containing all integers read from the file.
 *         Returns an empty vector if the file cannot be opened.
 */
vector<int> loadFile(const string& path) {
    // Attempt to open the file for reading
    ifstream f(path);

    vector<int> arr;  // Dynamic array to store integers from file

    // Check for file open failure
    if (!f.is_open()) {
        cout << "Error: cannot open " << path << "\n";
        return {};    // Return empty vector on failure
    }

    int x;  // Temporary variable to store each integer read

    // Read integers until EOF or invalid input
    while (f >> x) {
        arr.push_back(x);  // Append each value to the vector
    }

    // Vector is returned by value (move semantics apply)
    return arr;
}

// ===============================
// Run search test suite
// ===============================

/**
 * Executes a standardized set of linear search tests on a dataset.
 *
 * @param name  Descriptive name of the dataset (used for output labeling)
 * @param arr   Vector containing the dataset to be tested
 */
void runSearchTests(const string& name, const vector<int>& arr) {
    cout << "=== " << name << " ===\n";
    cout << "Loaded " << arr.size() << " integers.\n\n";

    long long steps; // Stores number of comparisons made per search
    int index;       // Stores index result of the search

    // Test 1: Search for the first element (best-case scenario)
    index = linearSearchSteps(arr, arr.front(), steps);
    cout << "Search first element (" << arr.front()
         << "): index=" << index
         << ", steps=" << steps << "\n";

    // Test 2: Search for the middle element (average-case scenario)
    index = linearSearchSteps(arr, arr[arr.size() / 2], steps);
    cout << "Search middle element (" << arr[arr.size() / 2]
         << "): index=" << index
         << ", steps=" << steps << "\n";

    // Test 3: Search for the last element (worst-case successful search)
    index = linearSearchSteps(arr, arr.back(), steps);
    cout << "Search last element (" << arr.back()
         << "): index=" << index
         << ", steps=" << steps << "\n";

    // Test 4: Search for a value guaranteed not to exist
    index = linearSearchSteps(arr, 999999, steps);
    cout << "Search missing element (999999)"
         << ": index=" << index
         << ", steps=" << steps << "\n\n";
}

// ===============================
// Main
// ===============================

/**
 * Program entry point.
 *
 * Loads ordered and unordered datasets from disk,
 * runs linear search performance tests on each,
 * and reports comparison counts for different cases.
 */
int main() {
    // Load both test datasets from disk
    vector<int> ordered   = loadFile("..\\data\\ordered.txt");
    vector<int> unordered = loadFile("..\\data\\unordered.txt");

    // Validate that both files were successfully loaded
    if (ordered.empty() || unordered.empty()) {
        cout << "Missing or unreadable input files.\n";
        return 1;  // Exit with error code
    }

    // Run search benchmarks on each dataset
    runSearchTests("Ordered Data", ordered);
    runSearchTests("Unordered Data", unordered);

    return 0;  // Successful program termination
}
