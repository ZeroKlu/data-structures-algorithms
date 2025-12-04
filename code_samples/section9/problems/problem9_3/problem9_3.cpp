#include <iostream>
#include <vector>
using namespace std;

bool canPartition(const vector<int> &nums) {
    int total = 0;
    for (int x : nums) total += x;
    if (total % 2 != 0) return false;
    int target = total / 2;

    vector<bool> dp(target + 1, false);
    dp[0] = true;
    for (int x : nums) {
        for (int s = target; s >= x; s--) {
            if (dp[s - x]) dp[s] = true;
        }
    }
    return dp[target];
}

void test(const string &name, const vector<int> &arr, bool expected) {
    bool result = canPartition(arr);

    cout << name << "\n";
    cout << "Input: {";
    for (size_t i = 0; i < arr.size(); i++) {
        cout << arr[i] << (i + 1 == arr.size() ? "" : ",");
    }
    cout << "}\n";

    cout << "canPartition = " 
         << (result ? "true" : "false")
         << " (expected " << (expected ? "true" : "false") << ")\n\n";
}

int main() {
    cout << "=== Testing canPartition ===\n\n";

    // Test 1: Classic true example
    test("Test 1: Classic example", {1, 5, 11, 5}, true);

    // Test 2: Cannot partition
    test("Test 2: Cannot partition", {1, 2, 3, 5}, false);

    // Test 3: Single element
    test("Test 3: Single element", {7}, false);

    // Test 4: Two equal numbers
    test("Test 4: Two equal numbers", {4, 4}, true);

    // Test 5: Larger mixed set
    test("Test 5: Larger mixed set", {2, 2, 3, 5}, false);

    // Test 6: Multiple partitions possible
    test("Test 6: Multiple partitions", {2, 2, 1, 1}, true);

    return 0;
}
