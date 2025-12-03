#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

int longestConsecutive(const vector<int> &nums) {
    unordered_set<int> set(nums.begin(), nums.end());
    int best = 0;

    for (int x : set) {
        if (!set.count(x - 1)) { // start of a sequence
            int cur = x;
            int length = 1;
            while (set.count(cur + 1)) {
                cur++;
                length++;
            }
            if (length > best) best = length;
        }
    }
    return best;
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
    cout << "=== Test: longestConsecutive ===\n\n";

    {
        vector<int> nums = {100, 4, 200, 1, 3, 2};
        cout << "Input: ";
        printVec(nums);
        cout << "\n";
        int r = longestConsecutive(nums);
        cout << "Longest consecutive length: " << r
             << " (expected 4: sequence 1,2,3,4)\n\n";
    }

    {
        vector<int> nums = {0,3,7,2,5,8,4,6,0,1};
        cout << "Input: ";
        printVec(nums);
        cout << "\n";
        int r = longestConsecutive(nums);
        cout << "Longest consecutive length: " << r
             << " (expected 9: sequence 0..8)\n\n";
    }

    {
        vector<int> nums = {1,2,0,1};
        cout << "Input: ";
        printVec(nums);
        cout << "\n";
        int r = longestConsecutive(nums);
        cout << "Longest consecutive length: " << r
             << " (expected 3: 0,1,2)\n\n";
    }

    {
        vector<int> nums; // empty
        cout << "Input: []\n";
        int r = longestConsecutive(nums);
        cout << "Longest consecutive length: " << r
             << " (expected 0)\n\n";
    }

    {
        vector<int> nums = {-2, -1, 0, 2, 3, 4, 5};
        cout << "Input: ";
        printVec(nums);
        cout << "\n";
        int r = longestConsecutive(nums);
        cout << "Longest consecutive length: " << r
             << " (expected 4: 2,3,4,5)\n\n";
    }

    return 0;
}
