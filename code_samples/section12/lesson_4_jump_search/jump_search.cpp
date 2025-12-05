#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

// ============================================================
// Jump Search (index only)
// ============================================================
int jumpSearch(const vector<int>& arr, int target) {
    int n = (int)arr.size();
    if (n == 0) return -1;

    int step = (int)std::sqrt(n);
    int prev = 0;
    int curr = step;

    // Jump phase: find block where target could be
    while (curr < n && arr[curr] < target) {
        prev = curr;
        curr += step;
    }

    if (curr >= n) curr = n - 1;

    // Linear search in [prev, curr]
    for (int i = prev; i <= curr; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

// ============================================================
// Jump Search with step counting
// steps counts the number of comparisons
// ============================================================
int jumpSearchSteps(const vector<int>& arr, int target, int& steps) {
    steps = 0;
    int n = (int)arr.size();
    if (n == 0) return -1;

    int step = (int)std::sqrt(n);
    int prev = 0;
    int curr = step;

    // Jump phase
    while (curr < n) {
        steps++;                // compare arr[curr] < target or arr[curr] >= target
        if (arr[curr] >= target)
            break;
        prev = curr;
        curr += step;
    }

    if (curr >= n) curr = n - 1;

    // Linear scan within the block
    for (int i = prev; i <= curr; i++) {
        steps++;                // compare arr[i] == target
        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}

// ============================================================
// Load ordered.txt into vector<int>
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
// Run tests on ordered data
// ============================================================
void runTests(const vector<int>& arr) {
    cout << "=== Jump Search Tests (ordered data only) ===\n";
    cout << "Loaded " << arr.size() << " integers\n\n";

    int steps = 0;
    int idx;

    // First element
    idx = jumpSearchSteps(arr, arr.front(), steps);
    cout << "Search first  (" << arr.front() << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Middle element
    int midVal = arr[arr.size() / 2];
    idx = jumpSearchSteps(arr, midVal, steps);
    cout << "Search middle (" << midVal << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Last element
    idx = jumpSearchSteps(arr, arr.back(), steps);
    cout << "Search last   (" << arr.back() << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Missing value
    idx = jumpSearchSteps(arr, 999999, steps);
    cout << "Search missing (999999): index=" << idx
         << ", steps=" << steps << "\n\n";
}

// ============================================================
// MAIN
// ============================================================
int main() {
    // Adjust path as needed relative to where you run the binary
    // Here: compiled & run from code_samples/section12/lesson_3_binary_search
    // Data: ..\data\ordered.txt
    string path = "..\\data\\ordered.txt";
    vector<int> ordered = loadFile(path);

    if (ordered.empty()) {
        cout << "Missing input file or failed to load.\n";
        return 1;
    }

    runTests(ordered);
    return 0;
}
