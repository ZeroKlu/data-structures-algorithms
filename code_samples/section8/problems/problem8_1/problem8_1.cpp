#include <iostream>
#include <vector>
using namespace std;

int climbStairs(int n) {
    if (n <= 2) return n;
    int prev2 = 1;
    int prev1 = 2;
    for (int i = 3; i <= n; i++) {
        int cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

int main() {

    cout << "=== Test: climbStairs ===\n\n";

    struct TestCase {
        int n;
        int expected;
    };

    vector<TestCase> tests = {
        {0, 0},      // depending on definition, 0→0 or 1; using 0 for consistency
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 5},
        {5, 8},
        {6, 13},
        {10, 89},
        {20, 10946}
    };

    for (const auto &t : tests) {
        int result = climbStairs(t.n);
        cout << "climbStairs(" << t.n << ") = " << result
             << " (expected " << t.expected << ")\n";
    }

    return 0;
}
