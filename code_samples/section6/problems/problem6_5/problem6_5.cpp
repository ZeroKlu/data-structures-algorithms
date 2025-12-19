#include <iostream>
#include <queue>
#include <vector>
using namespace std;

/*
    This program implements the "Kth Largest Element in a Stream" pattern.

    Core idea:
      - Maintain a MIN-HEAP of size at most k containing the k largest values seen so far.
      - The smallest value inside that heap (the root) is the current k-th largest overall.

    Why a min-heap (not a max-heap)?
      - We want fast access to the "cutoff" element: the smallest among the kept top-k values.
      - If a new value is <= cutoff, it doesn't belong in the top k, so we ignore it.
      - If it is > cutoff, it replaces the cutoff.

    Complexity:
      - Each add(val) operation performs at most one push and one pop → O(log k).
      - Space usage is O(k) for the heap.
*/

class KthLargest {
    // Desired rank: "k-th largest"
    int k;

    /*
        priority_queue<int, vector<int>, greater<int>> pq;

        In C++, std::priority_queue is a MAX-HEAP by default.
        Using 'greater<int>' makes it behave like a MIN-HEAP:
          - pq.top() is the smallest element in the heap.

        We keep only the k largest elements in this min-heap.
        Therefore, pq.top() is the k-th largest element overall.
    */
    priority_queue<int, vector<int>, greater<int>> pq; // min-heap

public:
    /*
        Constructor
        -----------
        Initializes KthLargest with:
          - k_: the desired k-th rank
          - nums: initial numbers in the stream

        Strategy:
          - Reuse add(x) logic so that initialization and streaming updates
            share the same behavior (single source of truth).
    */
    KthLargest(int k_, const vector<int> &nums) : k(k_) {
        for (int x : nums) add(x);
    }

    /*
        add(val)
        --------
        Adds a new value to the stream and returns the current k-th largest.

        Behavior:
          - If heap has fewer than k elements: push val unconditionally.
          - Else if val is bigger than the smallest of the top-k (pq.top()):
                pop the smallest, then push val.
          - Else: ignore val (it's not in the top k).

        Return:
          - pq.top(), which represents the k-th largest element.

        Assumption:
          - k > 0. If k == 0, pq.top() would be undefined behavior.
            (In production code, you'd validate k in the constructor.)
    */
    int add(int val) {
        if ((int)pq.size() < k) {
            // Haven't collected k elements yet; just include it.
            pq.push(val);
        } else if (val > pq.top()) {
            /*
                Heap already contains k elements.

                pq.top() is the current cutoff (the k-th largest).
                If val is larger, it belongs among the top k:
                  - remove the cutoff
                  - insert the new value
            */
            pq.pop();
            pq.push(val);
        }

        // The smallest value in the top-k set is the k-th largest overall.
        return pq.top();
    }
};

// ---------- Test Helpers ----------

/*
    printVector
    -----------
    Prints a vector of ints with a leading label.
*/
void printVector(const string &label, const vector<int> &v) {
    cout << label;
    for (int x : v) cout << x << " ";
    cout << "\n";
}

// ---------- Test Function ----------

/*
    testKthLargest
    --------------
    Demonstrates the KthLargest class using a standard example.

    Example:
      - initial nums = {4, 5, 8, 2}
      - k = 3
      - then add values: {3, 5, 10, 9, 4}

    Expected results:
        add(3)  -> 4
        add(5)  -> 5
        add(10) -> 5
        add(9)  -> 8
        add(4)  -> 8

    Explanation:
      - The min-heap always stores the current 3 largest values.
      - The root of that min-heap is the 3rd largest.
*/
void testKthLargest() {
    cout << "=== Test KthLargest ===\n";

    vector<int> nums = {4, 5, 8, 2};
    int k = 3;

    printVector("Initial nums: ", nums);
    cout << "k = " << k << "\n\n";

    // Build KthLargest using initial numbers.
    KthLargest kth(k, nums);

    vector<int> addVals = {3, 5, 10, 9, 4};

    cout << "Adding values:\n";
    for (int x : addVals) {
        int result = kth.add(x);
        cout << "add(" << x << ") -> kth largest = " << result << "\n";
    }

    /*
      Expected output sequence for k = 3 and initial nums {4,5,8,2}:

        add(3)  -> 4
        add(5)  -> 5
        add(10) -> 5
        add(9)  -> 8
        add(4)  -> 8

      Explanation:
        The min-heap always stores the current 3 largest values.
        The smallest of those 3 is the "3rd largest" overall.
    */

    cout << endl;
}

// ---------- main ----------

/*
    Program entry point: run the test harness.
*/
int main() {
    testKthLargest();
    return 0;
}
