#include <iostream>   // Console I/O (cout, cerr)
#include <fstream>    // File I/O (ifstream)
#include <vector>     // std::vector
#include <string>     // std::string
#include <algorithm>  // std::swap

// ============================================================
// Structure to track sorting statistics
// ============================================================
//
// comparisons:
//   Counts how many element-to-element comparisons are performed
//
// swaps:
//   Counts how many swaps are actually executed
//
struct SortStats {
    long long comparisons = 0;
    long long swaps = 0;
};

// ==============================
// Load file into vector<int>
// ==============================
//
// Reads a whitespace-separated list of integers from a file
// and returns them in a std::vector<int>.
//
// If the file cannot be opened, an empty vector is returned
// and an error message is printed.
//
std::vector<int> loadFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        std::cerr << "Error reading: " << path << "\n";
        return {};
    }

    std::vector<int> arr;
    int x;

    // Read integers until EOF
    while (f >> x)
        arr.push_back(x);

    return arr;
}

// ==============================
// Selection Sort with step count
// ==============================
//
// Implements classic selection sort:
//
//   For each position i:
//     - Find the smallest element in the range [i, n-1]
//     - Swap it into position i
//
// Step counting:
//   - stats.comparisons is incremented once per comparison
//     between arr[j] and arr[minIdx]
//   - stats.swaps is incremented only when a swap actually occurs
//
void selectionSort(std::vector<int>& arr, SortStats& stats) {
    int n = (int)arr.size();

    // Outer loop selects the position to fill
    for (int i = 0; i < n; i++) {
        int minIdx = i;

        // Inner loop finds the minimum element
        // in the unsorted suffix [i+1, n-1]
        for (int j = i + 1; j < n; j++) {
            stats.comparisons++;  // count every comparison
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }

        // Swap only if a new minimum was found
        if (minIdx != i) {
            std::swap(arr[i], arr[minIdx]);
            stats.swaps++;        // count actual swaps
        }
    }
}

// ==============================
// Test Harness
// ==============================
//
// Loads:
//   - unordered.txt (unsorted input)
//   - ordered.txt   (correct sorted reference)
//
// Then:
//   1) Runs selection sort on unordered data
//   2) Prints comparison & swap counts
//   3) Verifies result against ordered.txt
//
int main() {
    std::string unorderedPath = "..\\data\\unordered.txt";
    std::string orderedPath   = "..\\data\\ordered.txt";

    // --------------------------------------------------
    // Load unordered input
    // --------------------------------------------------
    std::cout << "Attempting to read: " << unorderedPath << "\n";
    auto unorderedArr = loadFile(unorderedPath);
    if (unorderedArr.empty()) {
        std::cout << "Missing unordered input file - aborting.\n";
        return 1;
    }

    // --------------------------------------------------
    // Load ordered reference data
    // --------------------------------------------------
    std::cout << "Attempting to read: " << orderedPath << "\n";
    auto orderedArr = loadFile(orderedPath);
    if (orderedArr.empty()) {
        std::cout << "Missing ordered input file - aborting.\n";
        return 1;
    }

    // Ensure both files have the same number of elements
    if (unorderedArr.size() != orderedArr.size()) {
        std::cout << "File lengths differ! unordered="
                  << unorderedArr.size()
                  << " ordered=" << orderedArr.size() << "\n";
        return 1;
    }

    // --------------------------------------------------
    // Run selection sort
    // --------------------------------------------------
    std::cout << "\n--- Selection Sort (Step Counting) ---\n";
    SortStats stats;
    selectionSort(unorderedArr, stats);

    std::cout << "Comparisons: " << stats.comparisons << "\n";
    std::cout << "Swaps:       " << stats.swaps << "\n\n";

    // ==================================
    // Compare sorted result to ordered.txt
    // ==================================
    //
    // This acts as a correctness check to ensure
    // selection sort produced the expected output.
    //
    std::cout << "Comparing sorted result with ordered.txt...\n";

    int mismatches = 0;
    for (size_t i = 0; i < unorderedArr.size(); i++) {
        if (unorderedArr[i] != orderedArr[i]) {
            // Print only the first few mismatches
            if (mismatches < 10) {
                std::cout << "Mismatch at index " << i
                          << ": got " << unorderedArr[i]
                          << " expected " << orderedArr[i] << "\n";
            }
            mismatches++;
        }
    }

    // Final verification result
    if (mismatches == 0) {
        std::cout << "SUCCESS: Sorted output matches ordered.txt.\n";
    } else {
        std::cout << "FAIL: " << mismatches
                  << " mismatches detected (first 10 shown).\n";
    }

    return 0;
}
