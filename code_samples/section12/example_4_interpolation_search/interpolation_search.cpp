#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// ============================================================
// Interpolation Search (index-only)
// Requires: arr sorted in ascending order
// ============================================================
int interpolationSearch(const vector<int>& arr, int target) {
    int n = (int)arr.size();
    if (n == 0) return -1;

    int lo = 0;
    int hi = n - 1;

    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {
        if (arr[hi] == arr[lo]) {
            // All values in [lo, hi] are the same
            if (arr[lo] == target) return lo;
            return -1;
        }

        // Estimate probable position
        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                             (arr[hi] - arr[lo]));

        if (pos < lo || pos > hi) {
            return -1;
        }

        if (arr[pos] == target) {
            return pos;
        } else if (arr[pos] < target) {
            lo = pos + 1;
        } else {
            hi = pos - 1;
        }
    }

    return -1;
}

// ============================================================
// Interpolation Search WITH step counting
// steps = number of comparisons
// ============================================================
int interpolationSearchSteps(const vector<int>& arr, int target, int& steps) {
    steps = 0;
    int n = (int)arr.size();
    if (n == 0) return -1;

    int lo = 0;
    int hi = n - 1;

    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {
        steps++;  // compare arr[hi] == arr[lo]
        if (arr[hi] == arr[lo]) {
            steps++;  // compare arr[lo] == target
            if (arr[lo] == target) {
                return lo;
            } else {
                return -1;
            }
        }

        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                             (arr[hi] - arr[lo]));

        if (pos < lo || pos > hi) {
            return -1;
        }

        steps++;  // compare arr[pos] == target
        if (arr[pos] == target) {
            return pos;
        }

        steps++;  // compare arr[pos] < target
        if (arr[pos] < target) {
            lo = pos + 1;
        } else {
            hi = pos - 1;
        }
    }

    return -1;
}

// ============================================================
// Load integers from file into vector<int>
// ============================================================
vector<int> loadFile(const string& path) {
    ifstream in(path);
    vector<int> arr;
    if (!in) {
        cout << "Error reading: " << path << "\n";
        return arr;
    }
    int x;
    while (in >> x) {
        arr.push_back(x);
    }
    return arr;
}

// ============================================================
// Run tests: first, middle, last, missing
// ============================================================
void runTests(const vector<int>& arr) {
    cout << "=== Interpolation Search Tests (ordered data only) ===\n";
    cout << "Loaded " << arr.size() << " integers\n\n";

    int steps = 0;
    int idx;

    // First element
    idx = interpolationSearchSteps(arr, arr.front(), steps);
    cout << "Search first  (" << arr.front() << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Middle element
    int midVal = arr[arr.size() / 2];
    idx = interpolationSearchSteps(arr, midVal, steps);
    cout << "Search middle (" << midVal << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Last element
    int lastVal = arr.back();
    idx = interpolationSearchSteps(arr, lastVal, steps);
    cout << "Search last   (" << lastVal << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Missing element
    idx = interpolationSearchSteps(arr, 999999, steps);
    cout << "Search missing (999999): index=" << idx
         << ", steps=" << steps << "\n\n";
}

// ============================================================
// MAIN
// ============================================================
int main() {
    // If compiled & run from: code_samples/section12/lesson_3_interpolation_search
    // then this path points to: code_samples/section12/data/ordered.txt
    string path = "..\\data\\ordered.txt";
    vector<int> arr = loadFile(path);

    if (arr.empty()) {
        cout << "Missing input file or failed to load.\n";
        return 1;
    }

    runTests(arr);
    return 0;
}
