#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int lengthOfLIS(const vector<int> &nums) {
    vector<int> tails;
    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);
        } else {
            *it = x;
        }
    }
    return (int)tails.size();
}

int main() {
    cout << "=== Test 1: Basic Increasing Sequence ===\n";
    vector<int> arr1 = {1, 2, 3, 4, 5};
    cout << "Input: [1,2,3,4,5]\n";
    cout << "lengthOfLIS = " << lengthOfLIS(arr1) 
         << " (expected 5)\n\n";

    cout << "=== Test 2: Mixed Sequence ===\n";
    vector<int> arr2 = {10, 9, 2, 5, 3, 7, 101, 18};
    cout << "Input: [10,9,2,5,3,7,101,18]\n";
    cout << "lengthOfLIS = " << lengthOfLIS(arr2) 
         << " (expected 4)\n\n";

    cout << "=== Test 3: All Identical ===\n";
    vector<int> arr3 = {7, 7, 7, 7, 7};
    cout << "Input: [7,7,7,7,7]\n";
    cout << "lengthOfLIS = " << lengthOfLIS(arr3) 
         << " (expected 1)\n\n";

    cout << "=== Test 4: Empty Array ===\n";
    vector<int> arr4 = {};
    cout << "Input: []\n";
    cout << "lengthOfLIS = " << lengthOfLIS(arr4) 
         << " (expected 0)\n\n";

    cout << "=== Test 5: Decreasing Sequence ===\n";
    vector<int> arr5 = {9, 7, 5, 3, 1};
    cout << "Input: [9,7,5,3,1]\n";
    cout << "lengthOfLIS = " << lengthOfLIS(arr5) 
         << " (expected 1)\n\n";

    cout << "=== Test 6: Random Pattern ===\n";
    vector<int> arr6 = {4, 10, 4, 3, 8, 9};
    cout << "Input: [4,10,4,3,8,9]\n";
    cout << "lengthOfLIS = " << lengthOfLIS(arr6) 
         << " (expected 3)\n\n";

    return 0;
}
