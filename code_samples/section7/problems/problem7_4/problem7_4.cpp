#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

int subarraySum(const vector<int> &nums, int k) {
    unordered_map<int, int> freq;
    freq[0] = 1;
    int prefix = 0;
    int count = 0;
    for (int x : nums) {
        prefix += x;
        int need = prefix - k;
        if (freq.count(need)) {
            count += freq[need];
        }
        freq[prefix]++;
    }
    return count;
}

static void printVec(const vector<int>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

int main() {
    cout << "=== Test: subarraySum ===\n\n";

    {
        vector<int> nums = {1, 1, 1};
        int k = 2;
        cout << "Input: ";
        printVec(nums);
        cout << ", k = " << k << "\n";
        int r = subarraySum(nums, k);
        cout << "Result: " << r << " (expected 2)\n\n";
    }

    {
        vector<int> nums = {1, 2, 3};
        int k = 3;
        cout << "Input: ";
        printVec(nums);
        cout << ", k = " << k << "\n";
        int r = subarraySum(nums, k);
        cout << "Result: " << r << " (expected 2: [1,2], [3])\n\n";
    }

    {
        vector<int> nums = {3, 4, 7, 2, -3, 1, 4, 2};
        int k = 7;
        cout << "Input: ";
        printVec(nums);
        cout << ", k = " << k << "\n";
        int r = subarraySum(nums, k);
        cout << "Result: " << r << " (expected 4)\n\n";
    }

    {
        vector<int> nums;  // empty
        int k = 0;
        cout << "Input: []\n";
        int r = subarraySum(nums, k);
        cout << "Result: " << r << " (expected 0)\n\n";
    }

    return 0;
}
