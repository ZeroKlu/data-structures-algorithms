// ============================================================================
// Example 11 - Heap Sort with Step Counting (C++)
// ============================================================================
//
// PURPOSE
// -------
// Demonstrate Heap Sort on a list of integers while counting "steps" to help
// visualize time complexity.
//
// INPUT FILES
// -----------
// This program expects TWO text files containing the SAME integers:
//   - unordered.txt : unsorted integers
//   - ordered.txt   : the same integers sorted in ascending order
//
// Integers may be separated by any whitespace (spaces, tabs, newlines).
//
// PROGRAM FLOW
// ------------
// 1) Load unordered.txt and ordered.txt using a robust "try many paths" loader.
// 2) Heap-sort the unordered data IN PLACE (mutates the array).
// 3) Track:
//      - g_comparisons : number of data comparisons (arr[a] vs arr[b])
//      - g_writes      : number of array writes (swap counted as 3 writes)
// 4) Verify the sorted result matches ordered.txt exactly.
// 5) Print summary stats and PASS/FAIL.
//
// STEP COUNTING MODEL
// -------------------
// comparisons:
//   - Counted each time we compare two *data values* while restoring heap order:
//       arr[left]  > arr[largest]
//       arr[right] > arr[largest]
//
//   - We do NOT count index bounds checks (left < n, right < n) as comparisons,
//     because those are not comparisons between array values.
//
// writes:
//   - We treat a swap of two array elements as 3 writes, matching the convention
//     used in your other examples (tmp + two assignments).
//
// HEAP SORT NOTES
// --------------
// - This implementation builds a MAX-HEAP (largest element at the root).
// - Then it repeatedly swaps the root (max) to the end and heapifies the prefix.
//
// Complexity (typical):
//   - Build heap: O(n)
//   - Extract n times: O(n log n)
//   - Total: O(n log n) time, O(1) extra space (in-place)
//
// ============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>   // std::exit

using namespace std;

// ---------------------------------------------------------------------------
// GLOBAL COUNTERS (illustrate complexity)
// ---------------------------------------------------------------------------
// We keep these global so that heapify/swap can update them without needing
// to pass them down through every helper function call.
static long long g_comparisons = 0;  // counts data-to-data comparisons
static long long g_writes      = 0;  // counts array writes (swap counted as 3)

// ---------------------------------------------------------------------------
// Utility: robust file loader
// ---------------------------------------------------------------------------
// loadFile("unordered.txt") tries several likely relative locations:
//   1) "unordered.txt"                 (current working directory)
//   2) "data/unordered.txt"            (./data)
//   3) "../data/unordered.txt"         (sibling data folder)
//   4) "../../data/unordered.txt"      (if running from a deeper folder)
//   5) "../section12/data/unordered.txt"
//   6) "../../section12/data/unordered.txt"
//
// On success:
//   - Prints the actual file path used
//   - Returns the loaded integers as vector<int>
//
// On failure:
//   - Prints diagnostics and exits the program
static vector<int> loadFile(const string& filename) {
    // Candidate paths to try, in order
    string c0 = filename;                         // "unordered.txt"
    string c1 = "data/" + filename;               // "data/unordered.txt"
    string c2 = "../data/" + filename;            // "../data/unordered.txt"
    string c3 = "../../data/" + filename;         // "../../data/unordered.txt"
    string c4 = "../section12/data/" + filename;  // "../section12/data/unordered.txt"
    string c5 = "../../section12/data/" + filename; // "../../section12/data/unordered.txt"

    string candidates[6] = { c0, c1, c2, c3, c4, c5 };

    ifstream in;
    string usedPath;
    bool opened = false;

    // Try each candidate path until we successfully open the file
    for (const auto& path : candidates) {
        in.open(path);
        if (in.is_open()) {
            usedPath = path;
            opened = true;
            break;
        }

        // Important: clear fail flags so the stream can be reused
        in.clear();
    }

    // If we never opened anything, print what we tried and abort
    if (!opened) {
        cerr << "Error reading: " << filename << "\n";
        cerr << "Search paths attempted:\n";
        for (const auto& path : candidates) {
            cerr << "  " << path << "\n";
        }
        cerr << "Missing input file - aborting.\n";
        std::exit(1);
    }

    cout << "Loaded: " << usedPath << "\n";

    // Read all integers from the file
    vector<int> data;
    int x;
    while (in >> x) {
        data.push_back(x);
    }

    in.close();
    return data;
}

// ---------------------------------------------------------------------------
// Heap sort implementation with step counting
// ---------------------------------------------------------------------------

// swapInt(a, b)
// -------------
// Swaps two integers by reference.
// Step counting:
//   - We treat a swap as 3 writes (tmp + a + b), consistent with your other
//     sorting examples.
//
// Note:
//   - In reality, you could count writes differently (e.g., only assignments
//     to array indices). Here we keep it consistent across the lesson set.
static void swapInt(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;

    // Count swap as "3 writes"
    g_writes += 3;
}

// heapify(arr, n, i)
// ------------------
// Restores the MAX-HEAP property for the subtree rooted at index i within
// arr[0 .. n-1].
//
// MAX-HEAP property:
//   Every parent node >= its children.
//
// How heapify works:
//   1) Identify the largest value among:
//        - current node i
//        - left child  (2*i + 1)
//        - right child (2*i + 2)
//   2) If one of the children is larger than arr[i], swap and recurse down.
//
// Step counting:
//   - Each time we compare arr[left] to arr[largest], that's a comparison.
//   - Each time we compare arr[right] to arr[largest], that's a comparison.
//   - We do NOT count "left < n" or "right < n" as a comparison.
static void heapify(vector<int>& arr, int n, int i) {
    int largest = i;         // assume root is largest initially
    int left  = 2 * i + 1;   // left child index
    int right = 2 * i + 2;   // right child index

    // If left child exists, compare it to current largest
    if (left < n) {
        g_comparisons++;               // compare arr[left] vs arr[largest]
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }

    // If right child exists, compare it to current largest
    if (right < n) {
        g_comparisons++;               // compare arr[right] vs arr[largest]
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }

    // If the largest isn't the root, swap and keep heapifying downward
    if (largest != i) {
        swapInt(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// heapSort(arr)
// -------------
// In-place heap sort using a MAX-HEAP.
//
// Steps:
//   1) Build a max-heap from the array
//      - Start from the last parent node and heapify each node down to 0
//   2) Repeatedly extract the max element
//      - Swap root with the last element in the unsorted region
//      - Reduce heap size by 1
//      - Heapify root again to restore heap property
//
// Step counters are reset at the start so each run reports clean stats.
void heapSort(vector<int>& arr) {
    g_comparisons = 0;
    g_writes = 0;

    int n = static_cast<int>(arr.size());
    if (n <= 1) return; // already sorted (or empty)

    // 1) Build max-heap
    // The last parent is at index (n/2 - 1). Leaves start at (n/2).
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }

    // 2) Extract elements one by one from heap
    for (int end = n - 1; end > 0; --end) {
        // Move current max (root) to the end of the array
        swapInt(arr[0], arr[end]);

        // Restore heap property for the reduced heap [0 .. end-1]
        heapify(arr, end, 0);
    }
}

// ---------------------------------------------------------------------------
// Test harness
// ---------------------------------------------------------------------------
// - Load unordered + ordered (expected) arrays
// - Heap-sort unordered in place
// - Verify equality element-by-element
// - Print step counts and PASS/FAIL
int main() {
    // Load test data
    vector<int> unordered = loadFile("unordered.txt");
    vector<int> expected  = loadFile("ordered.txt");

    // Validate that both files contain the same count of integers
    if (unordered.size() != expected.size()) {
        cerr << "Size mismatch: unordered=" << unordered.size()
             << ", ordered=" << expected.size() << "\n";
        return 1;
    }

    // Sort unordered data in place
    heapSort(unordered);

    // Verify output matches expected exactly
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

    // Print summary results
    cout << "\nHeap Sort (C++)\n";
    cout << "---------------\n";
    cout << "Elements:     " << unordered.size() << "\n";
    cout << "Comparisons:  " << g_comparisons << "\n";
    cout << "Writes:       " << g_writes << "\n";
    cout << "Correct?      " << (ok ? "YES \u2713" : "NO \u2717") << "\n";

    // Optional: show a tiny sample if correct (useful sanity check)
    if (ok) {
        cout << "\nFirst 10 sorted values:\n";
        for (size_t i = 0; i < 10 && i < unordered.size(); ++i) {
            cout << unordered[i] << " ";
        }
        cout << "\n";
    }

    return ok ? 0 : 1;
}
