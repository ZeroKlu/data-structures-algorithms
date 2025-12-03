#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

vector<int> twoSumIndices(const vector<int> &nums, int target) {
    unordered_map<int, int> mp; // value -> index
    for (int i = 0; i < (int)nums.size(); i++) {
        int need = target - nums[i];
        auto it = mp.find(need);
        if (it != mp.end()) {
            return {it->second, i};
        }
        mp[nums[i]] = i;
    }
    return {}; // no solution
}

static void printVec(const vector<int> &v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

int main() {
    cout << "=== Test: twoSumIndices ===\n\n";

    {
        vector<int> nums = {2, 7, 11, 15};
        int target = 9;

        cout << "Input: nums = ";
        printVec(nums);
        cout << ", target = " << target << "\n";

        auto result = twoSumIndices(nums, target);
        cout << "Result: ";
        printVec(result);
        cout << " (expected [0, 1])\n\n";
    }

    {
        vector<int> nums = {3, 2, 4};
        int target = 6;

        cout << "Input: nums = ";
        printVec(nums);
        cout << ", target = " << target << "\n";

        auto result = twoSumIndices(nums, target);
        cout << "Result: ";
        printVec(result);
        cout << " (expected [1, 2])\n\n";
    }

    {
        vector<int> nums = {1, 2, 3, 4};
        int target = 10;

        cout << "Input: nums = ";
        printVec(nums);
        cout << ", target = " << target << "\n";

        auto result = twoSumIndices(nums, target);
        cout << "Result: ";
        printVec(result);
        cout << " (expected [])\n\n";
    }

    return 0;
}
