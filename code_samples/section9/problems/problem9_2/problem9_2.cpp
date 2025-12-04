#include <iostream>
#include <vector>
using namespace std;

int knapSack(int W, const vector<int> &weight, const vector<int> &value) {
    vector<int> dp(W + 1, 0);
    int n = (int)weight.size();
    for (int i = 0; i < n; i++) {
        int wt = weight[i];
        int val = value[i];
        for (int w = W; w >= wt; w--) {
            dp[w] = max(dp[w], dp[w - wt] + val);
        }
    }
    return dp[W];
}

void test(const string &name, int W, const vector<int> &weight,
          const vector<int> &value, int expected) 
{
    int result = knapSack(W, weight, value);
    cout << name << "\n";
    cout << "Capacity = " << W << "\n";

    cout << "Weights: {";
    for (size_t i = 0; i < weight.size(); i++) {
        cout << weight[i] << (i + 1 == weight.size() ? "" : ",");
    }
    cout << "}\n";

    cout << "Values:  {";
    for (size_t i = 0; i < value.size(); i++) {
        cout << value[i] << (i + 1 == value.size() ? "" : ",");
    }
    cout << "}\n";

    cout << "knapSack = " << result 
         << " (expected " << expected << ")\n\n";
}

int main() {
    cout << "=== Testing knapSack ===\n\n";

    // Test 1: Classic example
    test("Test 1: Classic example",
         50,
         {10, 20, 30},
         {60, 100, 120},
         220);

    // Test 2: Small weights
    test("Test 2: Small weights",
         6,
         {1, 2, 3},
         {10, 15, 40},
         65);

    // Test 3: Capacity too small
    test("Test 3: Too small capacity",
         4,
         {5, 6, 7},
         {10, 20, 30},
         0);

    // Test 4: Zero capacity
    test("Test 4: Zero capacity",
         0,
         {2, 3, 4},
         {20, 30, 40},
         0);

    // Test 5: Mixed multi-choice
    test("Test 5: Larger mixed set",
         10,
         {2, 3, 5, 7},
         {20, 30, 50, 70},
         100);

    return 0;
}
