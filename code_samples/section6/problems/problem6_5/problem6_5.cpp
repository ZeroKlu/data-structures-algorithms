#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class KthLargest {
    int k;
    priority_queue<int, vector<int>, greater<int>> pq; // min-heap

public:
    KthLargest(int k_, const vector<int> &nums) : k(k_) {
        for (int x : nums) add(x);
    }

    int add(int val) {
        if ((int)pq.size() < k) {
            pq.push(val);
        } else if (val > pq.top()) {
            pq.pop();
            pq.push(val);
        }
        return pq.top();
    }
};

// ---------- Test Helpers ----------

void printVector(const string &label, const vector<int> &v) {
    cout << label;
    for (int x : v) cout << x << " ";
    cout << "\n";
}

// ---------- Test Function ----------

void testKthLargest() {
    cout << "=== Test KthLargest ===\n";

    vector<int> nums = {4, 5, 8, 2};
    int k = 3;

    printVector("Initial nums: ", nums);
    cout << "k = " << k << "\n\n";

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
    */

    cout << endl;
}

// ---------- main ----------

int main() {
    testKthLargest();
    return 0;
}
