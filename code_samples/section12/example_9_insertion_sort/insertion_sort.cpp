// insertion_sort.cpp
// Insertion sort with step counting and tests against unordered.txt / ordered.txt.
//
// Build (example, Windows with g++):
//   g++ -std=c++17 -O2 insertion_sort.cpp -o insertion_sort
//
// Run from your lesson folder:
//   insertion_sort

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <cstring>

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

// ---------------- Insertion Sort with step counting ----------------

void insertionSort(vector<int>& arr, long long& comparisons, long long& writes) {
    int n = static_cast<int>(arr.size());
    for (int i = 1; i < n; i++) {
        int key = arr[i];   // not counting this as an array write
        int j = i - 1;

        while (j >= 0) {
            comparisons++;  // comparing arr[j] vs key
            if (arr[j] > key) {
                arr[j + 1] = arr[j];  // shift
                writes++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
        writes++;
    }
}

// ---------------- File Loading Helpers ----------------

// Try to open a file from several likely locations.
// Returns an ifstream opened in text mode; `outPath` contains the path it actually tried last.
static std::ifstream openWithFallbacks(const string& name, string& outPath) {
    std::ifstream in;

    // 1) Current directory
    outPath = name;
    in.open(outPath);
    if (in.is_open()) return in;
    in.close();

    // 2) ../data/name
    outPath = "../data/" + name;
    in.open(outPath);
    if (in.is_open()) return in;
    in.close();

    // 3) ../../data/name
    outPath = "../../data/" + name;
    in.open(outPath);
    if (in.is_open()) return in;
    in.close();

    // 4) <HOME or USERPROFILE>/Desktop/data/name
    const char* home = std::getenv("USERPROFILE");
    if (!home) home = std::getenv("HOME");
    if (home) {
        outPath = string(home) + "/Desktop/data/" + name;
        in.open(outPath);
        if (in.is_open()) return in;
        in.close();
    }

    // Return a closed ifstream with outPath set to the last attempt.
    return in;
}

// Load all ints from a file into a vector<int>.
// Returns true on success, false on failure.
bool loadFile(const string& logicalName, vector<int>& out, string& usedPath) {
    std::ifstream in = openWithFallbacks(logicalName, usedPath);
    if (!in.is_open()) {
        cerr << "Error reading: " << usedPath << "\n";
        char cwd[1024];
        if (GETCWD(cwd, sizeof(cwd))) {
            cerr << "Working directory: " << cwd << "\n";
        }
        cerr << "errno " << errno << ": " << std::strerror(errno) << "\n";
        return false;
    }

    out.clear();
    int x;
    while (in >> x) {
        out.push_back(x);
    }
    return true;
}

// ---------------- Main Test Harness ----------------

int main() {
    vector<int> unordered;
    vector<int> expected;
    string pathUnordered, pathOrdered;

    if (!loadFile("unordered.txt", unordered, pathUnordered)) {
        cerr << "Missing unordered input file — aborting.\n";
        return 1;
    }
    if (!loadFile("ordered.txt", expected, pathOrdered)) {
        cerr << "Missing ordered input file — aborting.\n";
        return 1;
    }

    if (unordered.size() != expected.size()) {
        cerr << "Size mismatch: unordered has " << unordered.size()
             << " numbers, ordered has " << expected.size() << ".\n";
        return 1;
    }

    // Copy unordered into a working array
    vector<int> arr = unordered;

    long long comparisons = 0;
    long long writes      = 0;

    insertionSort(arr, comparisons, writes);

    // Verify
    int mismatchIndex = -1;
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] != expected[i]) {
            mismatchIndex = static_cast<int>(i);
            break;
        }
    }

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
