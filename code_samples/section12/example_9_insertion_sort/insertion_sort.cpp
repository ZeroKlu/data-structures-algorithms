// insertion_sort.cpp
//
// Insertion Sort with detailed step counting and file-based verification.
//
// This program:
//   1) Loads an unsorted list of integers from unordered.txt
//   2) Loads a correctly sorted version from ordered.txt
//   3) Sorts the unordered list using INSERTION SORT
//   4) Counts comparisons and writes (shifts + insertions)
//   5) Verifies the result against ordered.txt
//
// ------------------------------------------------------------
// Build (example, Windows with g++):
//   g++ -std=c++17 -O2 insertion_sort.cpp -o insertion_sort
//
// Run (from your lesson folder):
//   insertion_sort
// ------------------------------------------------------------

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <cstring>

// Platform-specific includes for getcwd()
#ifdef _WIN32
    #include <direct.h>
    #define GETCWD _getcwd
#else
    #include <unistd.h>
    #define GETCWD getcwd
#endif

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

// ============================================================
// INSERTION SORT WITH STEP COUNTING
// ============================================================
//
// Insertion sort builds a sorted prefix of the array one element
// at a time. At iteration i:
//
//   - arr[0..i-1] is already sorted
//   - arr[i] is inserted into its correct position in that prefix
//
// Time Complexity:
//   - Best case (already sorted): O(n)
//   - Average / Worst case:       O(n²)
//
// Step Counting Policy:
//   - comparisons: number of comparisons between arr[j] and key
//   - writes: number of array writes caused by shifting or inserting
//
void insertionSort(vector<int>& arr,
                   long long& comparisons,
                   long long& writes)
{
    int n = static_cast<int>(arr.size());

    // Start from the second element (index 1),
    // because a single-element prefix is already sorted.
    for (int i = 1; i < n; i++) {

        // The value we want to insert into the sorted prefix
        int key = arr[i];

        // Index of the last element in the sorted prefix
        int j = i - 1;

        // Shift elements to the right while they are greater than key
        while (j >= 0) {

            // Count the comparison arr[j] > key
            comparisons++;

            if (arr[j] > key) {
                // Shift arr[j] one position to the right
                arr[j + 1] = arr[j];
                writes++;          // count the write
                j--;
            } else {
                // Correct insertion point found
                break;
            }
        }

        // Insert key into its correct position
        arr[j + 1] = key;
        writes++;                  // count the insertion write
    }
}

// ============================================================
// FILE LOADING HELPERS
// ============================================================
//
// We try several possible locations to find the input files.
// This avoids hardcoding paths and makes the program robust
// when run from different directories.
//
// Search order:
//   1) Current working directory
//   2) ../data/
//   3) ../../data/
//   4) ~/Desktop/data/
//

// Try to open a file from several likely locations.
// If successful, returns an open ifstream.
// `outPath` records the last path attempted.
static std::ifstream openWithFallbacks(const string& name,
                                       string& outPath)
{
    std::ifstream in;

    // 1) Current directory
    outPath = name;
    in.open(outPath);
    if (in.is_open()) return in;
    in.close();

    // 2) ../data/<name>
    outPath = "../data/" + name;
    in.open(outPath);
    if (in.is_open()) return in;
    in.close();

    // 3) ../../data/<name>
    outPath = "../../data/" + name;
    in.open(outPath);
    if (in.is_open()) return in;
    in.close();

    // 4) Desktop/data/<name> (useful on Windows/macOS)
    const char* home = std::getenv("USERPROFILE");
    if (!home) home = std::getenv("HOME");
    if (home) {
        outPath = string(home) + "/Desktop/data/" + name;
        in.open(outPath);
        if (in.is_open()) return in;
        in.close();
    }

    // All attempts failed
    return in;
}

// Load all integers from a file into a vector<int>.
// Returns true on success, false on failure.
bool loadFile(const string& logicalName,
              vector<int>& out,
              string& usedPath)
{
    std::ifstream in = openWithFallbacks(logicalName, usedPath);

    if (!in.is_open()) {
        cerr << "Error reading: " << usedPath << "\n";

        // Print working directory to help diagnose path issues
        char cwd[1024];
        if (GETCWD(cwd, sizeof(cwd))) {
            cerr << "Working directory: " << cwd << "\n";
        }

        cerr << "errno " << errno << ": " << std::strerror(errno) << "\n";
        return false;
    }

    // Read integers until EOF
    out.clear();
    int x;
    while (in >> x) {
        out.push_back(x);
    }

    return true;
}

// ============================================================
// MAIN TEST HARNESS
// ============================================================
//
// Loads input files, runs insertion sort, prints step counts,
// and verifies correctness.
//
int main()
{
    vector<int> unordered;   // unsorted input
    vector<int> expected;    // correctly sorted reference
    string pathUnordered;
    string pathOrdered;

    // Load unordered.txt
    if (!loadFile("unordered.txt", unordered, pathUnordered)) {
        cerr << "Missing unordered input file — aborting.\n";
        return 1;
    }

    // Load ordered.txt
    if (!loadFile("ordered.txt", expected, pathOrdered)) {
        cerr << "Missing ordered input file — aborting.\n";
        return 1;
    }

    // Sanity check: both files must have the same number of elements
    if (unordered.size() != expected.size()) {
        cerr << "Size mismatch: unordered has " << unordered.size()
             << " numbers, ordered has " << expected.size() << ".\n";
        return 1;
    }

    // Make a working copy so original data remains unchanged
    vector<int> arr = unordered;

    // Step counters
    long long comparisons = 0;
    long long writes      = 0;

    // Run insertion sort
    insertionSort(arr, comparisons, writes);

    // Verify sorted output against expected
    int mismatchIndex = -1;
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] != expected[i]) {
            mismatchIndex = static_cast<int>(i);
            break;
        }
    }

    // --------------------------------------------------------
    // Report results
    // --------------------------------------------------------
    cout << "Insertion Sort (C++) Test\n";
    cout << "-------------------------\n";
    cout << "unordered.txt path: " << pathUnordered << "\n";
    cout << "ordered.txt   path: " << pathOrdered << "\n\n";

    cout << "Number of elements: " << arr.size() << "\n";
    cout << "Comparisons:        " << comparisons << "\n";
    cout << "Writes:             " << writes << "\n";

    if (mismatchIndex == -1) {
        cout << "Result:             PASS (sorted output matches ordered.txt)\n";
    } else {
        cout << "Result:             FAIL (first mismatch at index "
             << mismatchIndex << ": got " << arr[mismatchIndex]
             << ", expected " << expected[mismatchIndex] << ")\n";
    }

    return 0;
}
