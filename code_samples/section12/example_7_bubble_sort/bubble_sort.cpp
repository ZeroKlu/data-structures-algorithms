#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// ------------------------------------------------------------
// Bubble Sort with step counting
// ------------------------------------------------------------
void bubbleSort(std::vector<int>& arr, long long& comparisons, long long& swaps) {
    comparisons = 0;
    swaps = 0;

    int n = (int)arr.size();
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (int i = 1; i < n; i++) {
            comparisons++;  // count comparison
            if (arr[i - 1] > arr[i]) {
                std::swap(arr[i - 1], arr[i]);
                swaps++;     // count swap
                swapped = true;
            }
        }
        n--; // last element now fixed
    }
}

// ------------------------------------------------------------
// Load integers from a file into a vector<int>
// ------------------------------------------------------------
bool loadFile(const std::string& path, std::vector<int>& out) {
    std::ifstream in(path);
    if (!in) {
        std::cout << "Error reading: " << path << "\n";
        return false;
    }

    out.clear();
    int x;
    while (in >> x) {
        out.push_back(x);
    }
    return true;
}

// ------------------------------------------------------------
// Compare vectors, print first few mismatches
// ------------------------------------------------------------
int compareVectors(const std::vector<int>& a, const std::vector<int>& b) {
    int mismatches = 0;
    int n = std::min(a.size(), b.size());

    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            if (mismatches < 10) {
                std::cout << "Mismatch at " << i
                          << ": got " << a[i]
                          << ", expected " << b[i] << "\n";
            }
            mismatches++;
        }
    }

    return mismatches;
}

// ------------------------------------------------------------
// Main Test Driver
// ------------------------------------------------------------
int main() {
    std::string unorderedPath = "../data/unordered.txt";
    std::string orderedPath   = "../data/ordered.txt";

    std::cout << "Attempting to read: " << unorderedPath << "\n";
    std::vector<int> unordered;
    if (!loadFile(unorderedPath, unordered)) {
        std::cout << "Missing unordered file - aborting.\n";
        return 1;
    }

    std::cout << "Attempting to read: " << orderedPath << "\n";
    std::vector<int> ordered;
    if (!loadFile(orderedPath, ordered)) {
        std::cout << "Missing ordered file - aborting.\n";
        return 1;
    }

    if (unordered.size() != ordered.size()) {
        std::cout << "File size mismatch - aborting.\n";
        return 1;
    }

    long long comparisons = 0;
    long long swaps = 0;

    std::cout << "\n--- Bubble Sort Step Count ---\n";

    bubbleSort(unordered, comparisons, swaps);

    std::cout << "Comparisons: " << comparisons << "\n";
    std::cout << "Swaps:       " << swaps << "\n\n";

    std::cout << "Comparing result to ordered.txt...\n";

    int mismatches = compareVectors(unordered, ordered);
    if (mismatches == 0)
        std::cout << "SUCCESS: Output matches ordered.txt\n";
    else
        std::cout << "FAIL: " << mismatches << " mismatches found.\n";

    return 0;
}
