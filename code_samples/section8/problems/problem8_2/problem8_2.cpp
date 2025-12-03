#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int rob(const vector<int> &nums) {
    if (nums.empty()) return 0;
    if (nums.size() == 1) return nums[0];

    int prev2 = nums[0];
    int prev1 = max(nums[0], nums[1]);

    for (size_t i = 2; i < nums.size(); i++) {
        int take = prev2 + nums[i];
        int skip = prev1;
        int cur = max(take, skip);

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

int main() {
    cout << "=== Test: rob (House Robber) ===\n\n";

    struct TestCase {
        vector<int> nums;
        int expected;
    };

    vector<TestCase> tests = {
        {{}, 0},
        {{5}, 5},
        {{1, 2}, 2},
        {{2, 7, 9, 3, 1}, 12},       // classic example
        {{1, 2, 3, 1}, 4},
        {{2, 1, 1, 2}, 4},
        {{10, 5, 20, 15, 30}, 60},
        {{1, 3, 1, 3, 100}, 103}
    };

    for (size_t i = 0; i < tests.size(); i++) {
        int result = rob(tests[i].nums);

        cout << "Test " << (i + 1) << ": rob([";
        for (size_t j = 0; j < tests[i].nums.size(); j++) {
            cout << tests[i].nums[j];
            if (j + 1 < tests[i].nums.size()) cout << ", ";
        }
        cout << "]) = " << result
             << " (expected " << tests[i].expected << ")\n";
    }

    return 0;
}
