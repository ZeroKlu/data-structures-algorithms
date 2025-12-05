#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// ===============================
// Linear Search with Step Counting
// ===============================
int linearSearchSteps(const vector<int>& arr, int target, long long& stepsOut) {
    long long steps = 0;
    for (int i = 0; i < (int)arr.size(); i++) {
        steps++; // count comparison
        if (arr[i] == target) {
            stepsOut = steps;
            return i;
        }
    }
    stepsOut = steps;
    return -1;
}

// ===============================
// Load integers from file
// ===============================
vector<int> loadFile(const string& path) {
    ifstream f(path);
    vector<int> arr;
    if (!f.is_open()) {
        cout << "Error: cannot open " << path << "\n";
        return {};
    }

    int x;
    while (f >> x) {
        arr.push_back(x);
    }
    return arr;
}

// ===============================
// Run search test suite
// ===============================
void runSearchTests(const string& name, const vector<int>& arr) {
    cout << "=== " << name << " ===\n";
    cout << "Loaded " << arr.size() << " integers.\n\n";

    long long steps;
    int index;

    // Test 1 - first element
    index = linearSearchSteps(arr, arr.front(), steps);
    cout << "Search first element (" << arr.front()
         << "): index=" << index
         << ", steps=" << steps << "\n";

    // Test 2 - middle element
    index = linearSearchSteps(arr, arr[arr.size()/2], steps);
    cout << "Search middle element (" << arr[arr.size()/2]
         << "): index=" << index
         << ", steps=" << steps << "\n";

    // Test 3 - last element
    index = linearSearchSteps(arr, arr.back(), steps);
    cout << "Search last element (" << arr.back()
         << "): index=" << index
         << ", steps=" << steps << "\n";

    // Test 4 - missing element
    index = linearSearchSteps(arr, 999999, steps);
    cout << "Search missing element (999999)"
         << ": index=" << index
         << ", steps=" << steps << "\n\n";
}

// ===============================
// Main
// ===============================
int main() {
    // Load both test datasets
    vector<int> ordered   = loadFile("..\\data\\ordered.txt");
    vector<int> unordered = loadFile("..\\data\\unordered.txt");

    if (ordered.empty() || unordered.empty()) {
        cout << "Missing or unreadable input files.\n";
        return 1;
    }

    runSearchTests("Ordered Data", ordered);
    runSearchTests("Unordered Data", unordered);

    return 0;
}
