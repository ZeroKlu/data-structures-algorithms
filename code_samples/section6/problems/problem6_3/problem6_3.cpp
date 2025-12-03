#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> kSmallest(const vector<int> &arr, int k) {
    priority_queue<int> pq; // max-heap
    for (int x : arr) {
        if ((int)pq.size() < k) {
            pq.push(x);
        } else if (x < pq.top()) {
            pq.pop();
            pq.push(x);
        }
    }
    vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top());
        pq.pop();
    }
    return result; // any order is acceptable (usually descending due to max-heap)
}

// ----- Helpers -----

static void printVector(const string &label, const vector<int> &v) {
    cout << label;
    for (int x : v) cout << x << " ";
    cout << "\n";
}

// This is the function that tests kSmallest:
void test_kSmallest() {
    vector<int> arr = {7, 2, 9, 4, 1, 5, 8, 3, 6};
    int n = (int)arr.size();

    cout << "=== Test 1: k <= n ===\n";
    printVector("Input array: ", arr);
    int k = 4;
    cout << "k = " << k << "\n";

    vector<int> res1 = kSmallest(arr, k);
    printVector("kSmallest result (heap order): ", res1);

    // Sort to show them in ascending order for easier verification
    vector<int> sorted1 = res1;
    sort(sorted1.begin(), sorted1.end());
    printVector("kSmallest result (sorted): ", sorted1);
    cout << "(Expected k smallest values: 1 2 3 4)\n\n";

    cout << "=== Test 2: k > n ===\n";
    k = 20;
    printVector("Input array: ", arr);
    cout << "k = " << k << "\n";

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
    vector<int> res3 = kSmallest(arr, k);
    printVector("kSmallest result: ", res3);
    cout << "(Expected: empty result)\n";
}

int main() {
    test_kSmallest();
    return 0;
}
