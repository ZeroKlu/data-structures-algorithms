#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
using namespace std;

int countUniquePairs(const vector<int> &nums, int target) {
    unordered_set<int> seen;
    unordered_set<string> pairs;

    for (int x : nums) {
        int y = target - x;
        if (seen.count(y)) {
            int a = min(x, y);
            int b = max(x, y);
            string key = to_string(a) + "," + to_string(b);
            pairs.insert(key);
        }
        seen.insert(x);
    }
    return (int)pairs.size();
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
    cout << "=== Test: countUniquePairs ===\n\n";

    struct TestCase {
        vector<int> nums;
        int target;
        int expected;
        string description;
    };

    vector<TestCase> tests = {
        { {1,1,2,2,3,3}, 4, 2, "{1,3} and {2,2}" },
        { {2,7,11,15},   9, 1, "{2,7}" },
        { {3,3,3},       6, 1, "{3,3}" },
        { {1,2,3,4},    10, 0, "no pairs" },
        { {1,5,1,5},     6, 1, "{1,5}" },
    };

    for (const auto& t : tests) {
        cout << "Input: nums = ";
        printVec(t.nums);
        cout << ", target = " << t.target << "\n";

        int result = countUniquePairs(t.nums, t.target);

        cout << "Result:   " << result << "\n";
        cout << "Expected: " << t.expected 
             << " (" << t.description << ")\n\n";
    }

    return 0;
}
