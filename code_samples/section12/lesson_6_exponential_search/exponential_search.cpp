#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

/* -------------------------------------------------------
   Binary Search helper (counts steps)
   Searches arr[left..right] inclusive.
   ------------------------------------------------------- */
int binarySearchSteps(const vector<int>& arr, int left, int right, int target, int& steps) {
    while (left <= right) {
        steps++;  // each binary-search check

        int mid = left + (right - left) / 2;

        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

/* -------------------------------------------------------
   Exponential Search (counts steps)
   ------------------------------------------------------- */
int exponentialSearch(const vector<int>& arr, int target, int& steps) {
    steps = 0;
    int n = arr.size();
    if (n == 0) return -1;

    steps++;  // check arr[0]
    if (arr[0] == target) return 0;

    int bound = 1;

    // Exponentially expand range
    while (bound < n && arr[bound] < target) {
        steps++;
        bound *= 2;
    }

    int left = bound / 2;
    int right = min(bound, n - 1);

    return binarySearchSteps(arr, left, right, target, steps);
}

/* -------------------------------------------------------
   Load integers from file
   ------------------------------------------------------- */
bool loadFile(const string& path, vector<int>& out) {
    ifstream in(path);
    if (!in.is_open()) {
        cerr << "Error reading: " << path << endl;
        return false;
    }

    int x;
    while (in >> x) out.push_back(x);
    return true;
}

/* -------------------------------------------------------
   MAIN TESTS
   ------------------------------------------------------- */
int main() {
    string orderedPath = "..\\data\\ordered.txt";
    vector<int> arr;

    cout << "Attempting to read: " << orderedPath << endl;

    if (!loadFile(orderedPath, arr)) {
        cerr << "Missing input file — aborting.\n";
        return 1;
    }

    cout << "Loaded " << arr.size() << " integers.\n\n";

    int steps, index;

    vector<int> tests = { -5000, 0, 5000, 1234, -2500, 9999 };

    for (int target : tests) {
        index = exponentialSearch(arr, target, steps);
        cout << "Target " << target
             << " → index " << index
             << ", steps " << steps << endl;
    }

    return 0;
}
