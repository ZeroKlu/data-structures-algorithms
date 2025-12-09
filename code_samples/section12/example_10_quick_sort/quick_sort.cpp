#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;

// ------------------------------------------------------------
// GLOBAL COUNTERS
// ------------------------------------------------------------
static long long g_comparisons = 0;
static long long g_writes      = 0;

// ------------------------------------------------------------
// Quick Sort (Lomuto partition) with step counting
// ------------------------------------------------------------
static int partition(vector<int>& arr, int left, int right) {
    int pivot = arr[right];
    int i = left;

    for (int j = left; j < right; ++j) {
        g_comparisons++; // compare arr[j] <= pivot
        if (arr[j] <= pivot) {
            std::swap(arr[i], arr[j]);
            g_writes += 3; // treat a swap as 3 writes
            ++i;
        }
    }

    std::swap(arr[i], arr[right]);
    g_writes += 3;
    return i;
}

static void quickSortRec(vector<int>& arr, int left, int right) {
    if (left < right) {
        int p = partition(arr, left, right);
        quickSortRec(arr, left, p - 1);
        quickSortRec(arr, p + 1, right);
    }
}

void quickSort(vector<int>& arr) {
    g_comparisons = 0;
    g_writes = 0;
    if (!arr.empty()) {
        quickSortRec(arr, 0, static_cast<int>(arr.size()) - 1);
    }
}

// ------------------------------------------------------------
// FILE LOADER (tries several relative paths)
// ------------------------------------------------------------
// Example calls:
//   auto unordered = loadFile("unordered.txt");
//   auto expected  = loadFile("ordered.txt");
vector<int> loadFile(const string& filename) {
    const char* prefixes[] = {
        "",             // unordered.txt in current dir
        "data/",        // ./data/unordered.txt
        "../data/",     // ../data/unordered.txt (your usual layout)
        "../../data/",  // ../../data/unordered.txt (if run deeper)
        nullptr
    };

    std::ifstream in;
    string full;
    for (int i = 0; prefixes[i] != nullptr; ++i) {
        full = prefixes[i] + filename;
        in.open(full);
        if (in) {
            cout << "Loaded: " << full << "\n";
            break;
        }
        in.clear();
    }

    if (!in) {
        cout << "Error reading: " << filename << "\n";
        cout << "Search paths attempted:\n";
        for (int i = 0; prefixes[i] != nullptr; ++i) {
            cout << "  " << prefixes[i] << filename << "\n";
        }
        cout << "Missing input file — aborting.\n";
        std::exit(1);
    }

    vector<int> arr;
    int x;
    while (in >> x) {
        arr.push_back(x);
    }
    return arr;
}

// ------------------------------------------------------------
// MAIN TEST
// ------------------------------------------------------------
int main() {
    // Load data
    vector<int> unordered = loadFile("unordered.txt");
    vector<int> expected  = loadFile("ordered.txt");

    if (unordered.size() != expected.size()) {
        cout << "Mismatch in file lengths (" 
             << unordered.size() << " vs " << expected.size()
             << ") — aborting.\n";
        return 1;
    }

    // Copy and sort
    vector<int> arr = unordered;
    quickSort(arr);

    // Validate
    bool ok = true;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] != expected[i]) {
            ok = false;
            cout << "Mismatch at index " << i 
                 << ": got " << arr[i] 
                 << ", expected " << expected[i] << "\n";
            break;
        }
    }

    cout << "\nQuick Sort (C++)\n";
    cout << "----------------\n";
    cout << "Elements:     " << arr.size() << "\n";
    cout << "Comparisons:  " << g_comparisons << "\n";
    cout << "Writes:       " << g_writes << "\n";
    cout << "Correct?      " << (ok ? "YES \xE2\x9C\x94" : "NO \xE2\x9D\x8B") << "\n";

    if (ok) {
        cout << "\nFirst 10 sorted values:\n";
        for (size_t i = 0; i < 10 && i < arr.size(); ++i) {
            cout << arr[i] << " ";
        }
        cout << "\n";
    }

    return 0;
}
