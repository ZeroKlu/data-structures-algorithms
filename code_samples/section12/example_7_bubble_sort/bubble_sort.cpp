#include <iostream>   // std::cout for console output
#include <fstream>    // std::ifstream for file input
#include <vector>     // std::vector dynamic array
#include <string>     // std::string paths/text
#include <sstream>    // (not used directly here; commonly used for parsing)
#include <algorithm>  // std::swap, std::min

// ------------------------------------------------------------
// Bubble Sort with step counting
// ------------------------------------------------------------
/*
    Bubble sort repeatedly scans the array and swaps adjacent elements
    that are out of order. After each full pass:

        - The largest remaining element "bubbles" to the end
          of the unsorted portion of the array.

    Optimization used here:
        - If we complete a pass with no swaps, the array is already sorted,
          so we can stop early.

    Step counting:
        - comparisons counts how many adjacent comparisons we perform:
              arr[i - 1] > arr[i]
        - swaps counts how many swaps are actually executed

    Parameters:
        - arr: array to sort (modified in place)
        - comparisons: output counter for comparisons
        - swaps: output counter for swaps
*/
void bubbleSort(std::vector<int>& arr, long long& comparisons, long long& swaps) {
    // Initialize counters
    comparisons = 0;
    swaps = 0;

    int n = (int)arr.size();

    // swapped controls whether another pass is needed
    bool swapped = true;

    // Continue until we complete a pass with no swaps
    while (swapped) {
        swapped = false;  // assume sorted unless a swap occurs this pass

        // One pass compares adjacent pairs up to index n-1
        for (int i = 1; i < n; i++) {
            comparisons++;  // count each adjacent comparison

            // If out of order, swap adjacent elements
            if (arr[i - 1] > arr[i]) {
                std::swap(arr[i - 1], arr[i]);
                swaps++;     // count actual swap
                swapped = true;
            }
        }

        // After one full pass, the last element is in its final position
        // so we can shrink the unsorted range.
        n--; // last element now fixed
    }
}

// ------------------------------------------------------------
// Load integers from a file into a vector<int>
// ------------------------------------------------------------
/*
    Loads whitespace-delimited integers from a file into `out`.

    Returns:
        - true  if the file opened successfully and integers were read
        - false if the file could not be opened

    Notes:
        - The file format expected is simple: integers separated by spaces
          and/or newlines (any whitespace).
        - out.clear() ensures the vector is empty before filling.
*/
bool loadFile(const std::string& path, std::vector<int>& out) {
    std::ifstream in(path);
    if (!in) {
        std::cout << "Error reading: " << path << "\n";
        return false;
    }

    // Ensure output vector is empty before reading
    out.clear();

    // Stream extraction reads ints until EOF or invalid token
    int x;
    while (in >> x) {
        out.push_back(x);
    }
    return true;
}

// ------------------------------------------------------------
// Compare vectors, print first few mismatches
// ------------------------------------------------------------
/*
    Compares two vectors element-by-element and counts mismatches.

    Behavior:
        - Compares only up to the smaller length of the two vectors
          (using std::min), although in this program we already ensure
          the sizes match before calling this function.
        - Prints up to the first 10 mismatches for readability.

    Returns:
        - Number of mismatched positions.
*/
int compareVectors(const std::vector<int>& a, const std::vector<int>& b) {
    int mismatches = 0;

    // Compare only within bounds of both vectors
    int n = std::min(a.size(), b.size());

    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            // Print only the first few mismatches
            if (mismatches < 10) {
                std::cout << "Mismatch at " << i
                          << ": got " << a[i]
                          << ", expected " << b[i] << "\n";
            }
            mismatches++;
        }
    }

    return mismatches;
}

// ------------------------------------------------------------
// Main Test Driver
// ------------------------------------------------------------
/*
    Test workflow:
        1) Load unordered.txt (unsorted input)
        2) Load ordered.txt   (reference sorted output)
        3) Run bubble sort on unordered data while counting steps
        4) Verify sorted output matches ordered.txt
        5) Print success/failure summary

    Note about paths:
        - Uses relative paths "../data/..."
        - These depend on where you run the compiled program from.
*/
int main() {
    std::string unorderedPath = "../data/unordered.txt";
    std::string orderedPath   = "../data/ordered.txt";

    // --------------------------------------------------
    // Load unordered input
    // --------------------------------------------------
    std::cout << "Attempting to read: " << unorderedPath << "\n";
    std::vector<int> unordered;
    if (!loadFile(unorderedPath, unordered)) {
        std::cout << "Missing unordered file - aborting.\n";
        return 1;
    }

    // --------------------------------------------------
    // Load ordered reference output
    // --------------------------------------------------
    std::cout << "Attempting to read: " << orderedPath << "\n";
    std::vector<int> ordered;
    if (!loadFile(orderedPath, ordered)) {
        std::cout << "Missing ordered file - aborting.\n";
        return 1;
    }

    // Sanity check: both lists should contain the same number of values
    if (unordered.size() != ordered.size()) {
        std::cout << "File size mismatch - aborting.\n";
        return 1;
    }

    // Step counters for bubble sort
    long long comparisons = 0;
    long long swaps = 0;

    std::cout << "\n--- Bubble Sort Step Count ---\n";

    // Sort in place and record statistics
    bubbleSort(unordered, comparisons, swaps);

    // Print statistics
    std::cout << "Comparisons: " << comparisons << "\n";
    std::cout << "Swaps:       " << swaps << "\n\n";

    // --------------------------------------------------
    // Verify correctness against ordered.txt
    // --------------------------------------------------
    std::cout << "Comparing result to ordered.txt...\n";

    int mismatches = compareVectors(unordered, ordered);
    if (mismatches == 0)
        std::cout << "SUCCESS: Output matches ordered.txt\n";
    else
        std::cout << "FAIL: " << mismatches << " mismatches found.\n";

    return 0;
}
