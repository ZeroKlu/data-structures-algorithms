// Example 11: Heap Sort with step counting (C++)
//
// Expects two data files containing space-separated integers:
//   - unordered.txt : unsorted integers
//   - ordered.txt   : the same integers in sorted order
//
// This program:
//   1. Loads both files (trying several likely relative paths).
//   2. Runs heap sort on the unordered data.
//   3. Counts comparisons and writes.
//   4. Verifies the result against ordered.txt.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Global counters for illustrating complexity
static long long g_comparisons = 0;
static long long g_writes      = 0;

// ---------------- Utility: robust file loader ----------------

static vector<int> loadFile(const string& filename) {
    // Candidate paths to try, in order
    string c0 = filename;                         // "unordered.txt"
    string c1 = "data/" + filename;               // "data/unordered.txt"
    string c2 = "../data/" + filename;            // "../data/unordered.txt"
    string c3 = "../../data/" + filename;         // "../../data/unordered.txt"
    string c4 = "../section12/data/" + filename;  // "../section12/data/unordered.txt"
    string c5 = "../../section12/data/" + filename;

    string candidates[6] = { c0, c1, c2, c3, c4, c5 };

    ifstream in;
    string usedPath;
    bool opened = false;

    for (const auto& path : candidates) {
        in.open(path);
        if (in) {
            usedPath = path;
            opened = true;
            break;
        }
        in.clear();
    }

    if (!opened) {
        cerr << "Error reading: " << filename << "\n";
        cerr << "Search paths attempted:\n";
        for (const auto& path : candidates) {
            cerr << "  " << path << "\n";
        }
        cerr << "Missing input file - aborting.\n";
        exit(1);
    }

    cout << "Loaded: " << usedPath << "\n";

    vector<int> data;
    int x;
    while (in >> x) {
        data.push_back(x);
    }
    in.close();
    return data;
}

// ---------------- Heap sort implementation with counting ----------------

static void swapInt(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
    // Three writes to the array locations
    g_writes += 3;
}

// Heapify subtree rooted at index i, in arr[0..n-1]
static void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left  = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        g_comparisons++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }
    if (right < n) {
        g_comparisons++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }

    if (largest != i) {
        swapInt(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    g_comparisons = 0;
    g_writes = 0;

    int n = static_cast<int>(arr.size());

    // Build max-heap
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }

    // Extract elements one by one
    for (int end = n - 1; end > 0; --end) {
        swapInt(arr[0], arr[end]);   // move max to end
        heapify(arr, end, 0);        // rebuild heap on prefix [0..end-1]
    }
}

// ---------------- Test harness ----------------

int main() {
    // Load data
    vector<int> unordered = loadFile("unordered.txt");
    vector<int> expected  = loadFile("ordered.txt");

    if (unordered.size() != expected.size()) {
        cerr << "Size mismatch: unordered=" << unordered.size()
             << ", ordered=" << expected.size() << "\n";
        return 1;
    }

    // Sort
    heapSort(unordered);

    // Verify
    bool ok = true;
    for (size_t i = 0; i < unordered.size(); ++i) {
        if (unordered[i] != expected[i]) {
            cout << "Mismatch at index " << i
                 << ": got " << unordered[i]
                 << ", expected " << expected[i] << "\n";
            ok = false;
            break;
        }
    }

    cout << "\nHeap Sort (C++)\n";
    cout << "---------------\n";
    cout << "Elements:     " << unordered.size() << "\n";
    cout << "Comparisons:  " << g_comparisons << "\n";
    cout << "Writes:       " << g_writes << "\n";
    cout << "Correct?      " << (ok ? "YES \u2713" : "NO \u2717") << "\n";

    return ok ? 0 : 1;
}
