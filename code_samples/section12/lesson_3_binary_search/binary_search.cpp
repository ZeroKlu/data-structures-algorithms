#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// =====================================================
// Given binarySearch (no changes)
// =====================================================
int binarySearch(const vector<int>& arr, int target) {
    int lo = 0, hi = (int)arr.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

// Binary search with step counting
int binarySearchSteps(const vector<int>& arr, int target, int& steps) {
    steps = 0;
    int lo = 0, hi = (int)arr.size() - 1;

    while (lo <= hi) {
        steps++;
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target) return mid;
        if (arr[mid] < target)  lo = mid + 1;
        else                     hi = mid - 1;
    }
    return -1;
}

// =====================================================
// File loader
// =====================================================
vector<int> load_file(const string& path) {
    ifstream f(path);
    vector<int> out;

    if (!f.is_open()) {
        cout << "Error reading: " << path << "\n";
        return out;
    }

    int x;
    while (f >> x) out.push_back(x);
    return out;
}

// =====================================================
// Run binary search tests
// =====================================================
void run_tests(const vector<int>& arr) {
    cout << "=== Binary Search Tests ===\n";
    cout << "Loaded " << arr.size() << " integers\n\n";

    int idx, steps;

    // First
    idx = binarySearchSteps(arr, arr.front(), steps);
    cout << "Search first  (" << arr.front() << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Middle
    int midVal = arr[arr.size() / 2];
    idx = binarySearchSteps(arr, midVal, steps);
    cout << "Search middle (" << midVal << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Last
    idx = binarySearchSteps(arr, arr.back(), steps);
    cout << "Search last   (" << arr.back() << "): index=" << idx
         << ", steps=" << steps << "\n";

    // Missing
    idx = binarySearchSteps(arr, 999999, steps);
    cout << "Search missing (999999): index=" << idx
         << ", steps=" << steps << "\n\n";
}

// =====================================================
// MAIN
// =====================================================
int main() {
    vector<int> arr = load_file("..\\data\\ordered.txt");

    if (arr.empty()) {
        cout << "Failed to load ordered.txt\n";
        return 1;
    }

    run_tests(arr);
    return 0;
}
