#include <iostream>
#include <vector>
using namespace std;

/*
 * climbStairs
 *
 * Problem:
 *   Given n steps, you can climb either 1 step or 2 steps at a time.
 *   Return the number of distinct ways to reach the top.
 *
 * Definition used in THIS implementation:
 *   - n = 0 → 0 ways   (explicit convention chosen here)
 *   - n = 1 → 1 way
 *   - n = 2 → 2 ways
 *
 * Recurrence relation:
 *   ways(n) = ways(n - 1) + ways(n - 2)
 *
 * This is equivalent to a Fibonacci-style sequence:
 *   ways(1) = 1
 *   ways(2) = 2
 *
 * Dynamic Programming Optimization:
 *   Instead of storing a full dp[] array, we only keep:
 *     prev2 = ways(n - 2)
 *     prev1 = ways(n - 1)
 *
 * Complexity:
 *   Time:  O(n)
 *   Space: O(1)
 *
 * Notes:
 *   - Some definitions treat n = 0 as having 1 way (do nothing).
 *     This version deliberately returns 0 for n = 0.
 *     Either choice is valid as long as it is used consistently.
 */
int climbStairs(int n) {
    // Handle small n directly:
    // n = 0 → 0
    // n = 1 → 1
    // n = 2 → 2
    if (n <= 2) return n;

    /*
     * prev2 represents ways(1)
     * prev1 represents ways(2)
     */
    int prev2 = 1; // ways(1)
    int prev1 = 2; // ways(2)

    /*
     * Compute ways(3) through ways(n).
     * At each step:
     *   cur = ways(i) = ways(i-1) + ways(i-2)
     * Then shift the window forward.
     */
    for (int i = 3; i <= n; i++) {
        int cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    // prev1 now holds ways(n)
    return prev1;
}

int main() {

    cout << "=== Test: climbStairs ===\n\n";

    /*
     * Each test case specifies:
     *   n        → number of steps
     *   expected → expected number of ways (based on our chosen definition)
     */
    struct TestCase {
        int n;
        int expected;
    };

    vector<TestCase> tests = {
        {0, 0},      // definition choice: 0 → 0
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 5},
        {5, 8},
        {6, 13},
        {10, 89},
        {20, 10946}
    };

    /*
     * Run each test and print the result alongside the expected value.
     */
    for (const auto &t : tests) {
        int result = climbStairs(t.n);
        cout << "climbStairs(" << t.n << ") = " << result
             << " (expected " << t.expected << ")\n";
    }

    return 0;
}
