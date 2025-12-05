#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

struct SortStats {
    long long comparisons = 0;
    long long swaps = 0;
};

// ==============================
// Load file into vector<int>
// ==============================
std::vector<int> loadFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        std::cerr << "Error reading: " << path << "\n";
        return {};
    }

    std::vector<int> arr;
    int x;
    while (f >> x) arr.push_back(x);
    return arr;
}

// ==============================
// Selection Sort with step count
// ==============================
void selectionSort(std::vector<int>& arr, SortStats& stats) {
    int n = (int)arr.size();
    for (int i = 0; i < n; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            stats.comparisons++;  // each comparison counted
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            std::swap(arr[i], arr[minIdx]);
            stats.swaps++;
        }
    }
}

// ==============================
// Test Harness
// ==============================
int main() {
    std::string unorderedPath = "..\\data\\unordered.txt";
    std::string orderedPath   = "..\\data\\ordered.txt";

    std::cout << "Attempting to read: " << unorderedPath << "\n";
    auto unorderedArr = loadFile(unorderedPath);
    if (unorderedArr.empty()) {
        std::cout << "Missing unordered input file - aborting.\n";
        return 1;
    }

    std::cout << "Attempting to read: " << orderedPath << "\n";
    auto orderedArr = loadFile(orderedPath);
    if (orderedArr.empty()) {
        std::cout << "Missing ordered input file - aborting.\n";
        return 1;
    }

    if (unorderedArr.size() != orderedArr.size()) {
        std::cout << "File lengths differ! unordered="
                  << unorderedArr.size()
                  << " ordered=" << orderedArr.size() << "\n";
        return 1;
    }

    std::cout << "\n--- Selection Sort (Step Counting) ---\n";
    SortStats stats;
    selectionSort(unorderedArr, stats);

    std::cout << "Comparisons: " << stats.comparisons << "\n";
    std::cout << "Swaps:       " << stats.swaps << "\n\n";

    // ==================================
    // Compare sorted result to ordered.txt
    // ==================================
    std::cout << "Comparing sorted result with ordered.txt...\n";

    int mismatches = 0;
    for (size_t i = 0; i < unorderedArr.size(); i++) {
        if (unorderedArr[i] != orderedArr[i]) {
            if (mismatches < 10) {
                std::cout << "Mismatch at index " << i
                          << ": got " << unorderedArr[i]
                          << " expected " << orderedArr[i] << "\n";
            }
            mismatches++;
        }
    }

    if (mismatches == 0) {
        std::cout << "SUCCESS: Sorted output matches ordered.txt.\n";
    } else {
        std::cout << "FAIL: " << mismatches
                  << " mismatches detected (first 10 shown).\n";
    }

    return 0;
}
