// #include <vector>
// using std::vector;

// static void merge(vector<int>& arr, vector<int>& tmp, int left, int mid, int right) {
//     int i = left, j = mid, k = left;
//     while (i < mid && j < right) {
//         if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
//         else                  tmp[k++] = arr[j++];
//     }
//     while (i < mid)  tmp[k++] = arr[i++];
//     while (j < right) tmp[k++] = arr[j++];
//     for (int p = left; p < right; p++) {
//         arr[p] = tmp[p];
//     }
// }

// static void mergeSortRec(vector<int>& arr, vector<int>& tmp, int left, int right) {
//     if (right - left <= 1) return;
//     int mid = left + (right - left) / 2;
//     mergeSortRec(arr, tmp, left, mid);
//     mergeSortRec(arr, tmp, mid, right);
//     merge(arr, tmp, left, mid, right);
// }

// void mergeSort(vector<int>& arr) {
//     vector<int> tmp(arr.size());
//     mergeSortRec(arr, tmp, 0, (int)arr.size());
// }

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// ------------------------------------------------------------
// Step counting struct
// ------------------------------------------------------------
struct Stats {
    long long comparisons = 0;
    long long writes = 0;
};

// ------------------------------------------------------------
// Merge Sort with step counting
// ------------------------------------------------------------
static void merge_vec(vector<int>& arr, vector<int>& tmp,
                      int left, int mid, int right, Stats& stats)
{
    int i = left, j = mid, k = left;

    while (i < mid && j < right) {
        stats.comparisons++;                   // compare arr[i] vs arr[j]
        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
        }
        stats.writes++;                        // write into tmp
    }
    while (i < mid) {
        tmp[k++] = arr[i++];
        stats.writes++;
    }
    while (j < right) {
        tmp[k++] = arr[j++];
        stats.writes++;
    }

    for (int p = left; p < right; p++) {
        arr[p] = tmp[p];
        stats.writes++;
    }
}

static void mergeSortRec(vector<int>& arr, vector<int>& tmp,
                         int left, int right, Stats& stats)
{
    if (right - left <= 1) return;
    int mid = left + (right - left) / 2;

    mergeSortRec(arr, tmp, left, mid, stats);
    mergeSortRec(arr, tmp, mid, right, stats);
    merge_vec(arr, tmp, left, mid, right, stats);
}

void mergeSort(vector<int>& arr, Stats& stats)
{
    vector<int> tmp(arr.size());
    mergeSortRec(arr, tmp, 0, (int)arr.size(), stats);
}

// ------------------------------------------------------------
// Utility: load integers from a file
// ------------------------------------------------------------
vector<int> loadFile(const string& path)
{
    ifstream in(path);
    vector<int> arr;

    if (!in.is_open()) {
        cout << "Error reading: " << path << "\n";
        return {};
    }

    int x;
    while (in >> x) arr.push_back(x);
    return arr;
}

// ------------------------------------------------------------
// Compare sorted output vs expected
// ------------------------------------------------------------
int compareArrays(const vector<int>& a, const vector<int>& b)
{
    int mismatches = 0;
    int n = a.size();
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            if (mismatches < 10) {
                cout << "Mismatch at index " << i
                     << ": got " << a[i]
                     << ", expected " << b[i] << "\n";
            }
            mismatches++;
        }
    }
    return mismatches;
}

// ------------------------------------------------------------
// Main Test Harness
// ------------------------------------------------------------
int main()
{
    string unorderedPath = "..\\data\\unordered.txt";
    string orderedPath   = "..\\data\\ordered.txt";

    cout << "Attempting to read: " << unorderedPath << "\n";
    vector<int> arr = loadFile(unorderedPath);
    if (arr.empty()) {
        cout << "Missing unordered input file — aborting.\n";
        return 1;
    }

    cout << "Attempting to read: " << orderedPath << "\n";
    vector<int> expected = loadFile(orderedPath);
    if (expected.empty()) {
        cout << "Missing ordered input file — aborting.\n";
        return 1;
    }

    if (arr.size() != expected.size()) {
        cout << "File lengths differ! unordered=" << arr.size()
             << ", ordered=" << expected.size() << "\n";
        return 1;
    }

    Stats stats;
    cout << "Sorting " << arr.size() << " elements with merge sort...\n";
    mergeSort(arr, stats);

    cout << "\n--- Merge Sort Step Counts ---\n";
    cout << "Comparisons: " << stats.comparisons << "\n";
    cout << "Writes:      " << stats.writes << "\n";

    cout << "\nChecking sorted output...\n";
    int mismatches = compareArrays(arr, expected);

    if (mismatches == 0) {
        cout << "SUCCESS — output matches expected sorted list!\n";
    } else {
        cout << "FAIL — mismatches found: " << mismatches << "\n";
    }

    return 0;
}
