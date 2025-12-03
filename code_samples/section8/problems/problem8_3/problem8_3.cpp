#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int coinChange(const vector<int> &coins, int amount) {
    if (amount == 0) return 0;
    const int INF = amount + 1;
    vector<int> dp(amount + 1, INF);
    dp[0] = 0;
    for (int coin : coins) {
        for (int x = coin; x <= amount; x++) {
            dp[x] = min(dp[x], dp[x - coin] + 1);
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}

int main() {

    cout << "=== Test: coinChange (Minimum Coins) ===\n\n";

    struct TestCase {
        vector<int> coins;
        int amount;
        int expected;
    };

    vector<TestCase> tests = {
        {{1, 2, 5}, 11, 3},        // 5 + 5 + 1
        {{2}, 3, -1},              // impossible
        {{1}, 0, 0},               // zero amount
        {{1}, 2, 2},               // 1+1
        {{1, 3, 4}, 6, 2},         // 3+3 or 4+1+1
        {{2, 5, 10, 1}, 27, 4},    // 10 + 10 + 5 + 2
        {{186, 419, 83, 408}, 6249, 20}  // known LC example
    };

    for (size_t i = 0; i < tests.size(); i++) {
        int result = coinChange(tests[i].coins, tests[i].amount);

        cout << "Test " << i + 1 << ": coins=[";
        for (size_t j = 0; j < tests[i].coins.size(); j++) {
            cout << tests[i].coins[j];
            if (j + 1 < tests[i].coins.size()) cout << ", ";
        }
        cout << "], amount=" << tests[i].amount
             << " -> " << result
             << " (expected " << tests[i].expected << ")\n";
    }

    return 0;
}
