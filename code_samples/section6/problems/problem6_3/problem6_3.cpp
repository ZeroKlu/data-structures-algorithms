#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

/*
    kSmallest using a max-heap (std::priority_queue)

    Goal:
      Return the k smallest values from an input array (vector<int>).
      The result does NOT need to be sorted (the code returns "heap order",
      which is typically descending because we pop from a max-heap).

    Why a max-heap?
      - We want to keep track of the k smallest elements seen so far.
      - If we store those k elements in a *max-heap*, the root (top) is the
        largest among the currently-kept k smallest elements.
      - That makes it easy to decide whether a new element should replace it:
          * If x >= top, x is not among the k smallest so far → ignore it.
          * If x < top, x should be included → pop top, push x.

    Complexity:
      - Each push/pop on the heap is O(log k)
      - We process n elements → O(n log k) time
      - Heap stores at most k elements → O(k) extra space

    Important edge cases:
      - k == 0 → should return empty result immediately.
      - k > n  → heap never reaches size k; result will contain all n elements.
      - Duplicates are handled naturally (they may appear in the result multiple times).
*/

vector<int> kSmallest(const vector<int> &arr, int k) {
    /*
        Input:
          - arr: input vector
          - k: number of smallest elements to keep

        Output:
          - vector<int> containing up to k smallest elements (any order)

        NOTE:
          This implementation uses std::priority_queue<int>, which is a max-heap by default.
    */
    priority_queue<int> pq; // max-heap: pq.top() returns the current largest element in the heap

    // Optimization / correctness for edge case:
    // If k <= 0, there are no elements to return.
    // (Without this guard, the loop still works for k==0, but it will do useless checks.)
    if (k <= 0) return {};

    for (int x : arr) {
        if ((int)pq.size() < k) {
            // Fill heap until it contains k elements.
            pq.push(x);
        } else if (x < pq.top()) {
            // If the current element is smaller than the largest of our kept k,
            // replace that largest with this smaller element.
            pq.pop();
            pq.push(x);
        }
        // Else: x is not smaller than our current cutoff (pq.top()),
        // so it cannot belong to the k smallest seen so far.
    }

    // Extract elements from the heap into a result vector.
    // Because pq is a max-heap, popping yields elements in descending order.
    vector<int> result;
    result.reserve(pq.size()); // avoid repeated reallocations

    while (!pq.empty()) {
        result.push_back(pq.top());
        pq.pop();
    }

    // Any order is acceptable; as-written this will usually be descending.
    return result;
}

// ----- Helpers -----

/*
    printVector
    -----------
    Utility function to print a label followed by vector contents.
*/
static void printVector(const string &label, const vector<int> &v) {
    cout << label;
    for (int x : v) cout << x << " ";
    cout << "\n";
}

/*
    test_kSmallest
    --------------
    Print-based tests for kSmallest.

    Tests included:
      1) k <= n: expect exactly k smallest values
      2) k > n : expect all values
      3) k = 0 : expect empty result

    Note:
      The raw result is in "heap pop order" (typically descending),
      so we also sort a copy for easy visual verification.
*/
void test_kSmallest() {
    vector<int> arr = {7, 2, 9, 4, 1, 5, 8, 3, 6};
    int n = (int)arr.size();
    (void)n; // n is not strictly needed below, but kept to show intent

    cout << "=== Test 1: k <= n ===\n";
    printVector("Input array: ", arr);

    int k = 4;
    cout << "k = " << k << "\n";

    // Get the k smallest elements (unordered)
    vector<int> res1 = kSmallest(arr, k);
    printVector("kSmallest result (heap order): ", res1);

    // Sort for easier validation (expected: 1 2 3 4)
    vector<int> sorted1 = res1;
    sort(sorted1.begin(), sorted1.end());
    printVector("kSmallest result (sorted): ", sorted1);
    cout << "(Expected k smallest values: 1 2 3 4)\n\n";

    cout << "=== Test 2: k > n ===\n";
    k = 20;
    printVector("Input array: ", arr);
    cout << "k = " << k << "\n";

    // When k > n, we'll never reach size k; we end up keeping all elements.
    vector<int> res2 = kSmallest(arr, k);
    printVector("kSmallest result (heap order): ", res2);

    vector<int> sorted2 = res2;
    sort(sorted2.begin(), sorted2.end());
    printVector("kSmallest result (sorted): ", sorted2);
    cout << "(Expected: all elements of the array sorted)\n\n";

    // Optional: Test k = 0
    cout << "=== Test 3: k = 0 ===\n";
    k = 0;
    cout << "k = " << k << "\n";

    // With k==0, result should be empty.
    vector<int> res3 = kSmallest(arr, k);
    printVector("kSmallest result: ", res3);
    cout << "(Expected: empty result)\n";
}

int main() {
    test_kSmallest();
    return 0;
}
