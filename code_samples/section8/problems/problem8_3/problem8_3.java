package code_samples.section8.problems.problem8_3;

public class problem8_3 {

    int coinChange(int[] coins, int amount) {
        if (amount == 0) return 0;
        int INF = amount + 1;
        int[] dp = new int[amount + 1];
        for (int i = 0; i <= amount; i++) {
            dp[i] = INF;
        }
        dp[0] = 0;
        for (int coin : coins) {
            for (int x = coin; x <= amount; x++) {
                dp[x] = Math.min(dp[x], dp[x - coin] + 1);
            }
        }
        return dp[amount] > amount ? -1 : dp[amount];
    }

    // ============================
    // Test Harness
    // ============================
    public static void main(String[] args) {

        problem8_3 solver = new problem8_3();

        int[][] coinSets = {
            {1, 2, 5},
            {2},
            {1},
            {1, 3, 4},
            {2, 5, 10, 1},
            {186, 419, 83, 408}
        };

        int[] amounts = {
            11,     // -> 3
            3,      // -> -1
            0,      // -> 0
            6,      // -> 2
            27,     // -> 4
            6249    // -> 20 (LeetCode heavy test)
        };

        int[] expected = {
            3,
            -1,
            0,
            2,
            4,
            20
        };

        System.out.println("=== Test: coinChange (Minimum Coins) ===\n");

        for (int i = 0; i < amounts.length; i++) {

            int result = solver.coinChange(coinSets[i], amounts[i]);

            System.out.print("Test " + (i + 1) + ": coins = [");
            for (int j = 0; j < coinSets[i].length; j++) {
                System.out.print(coinSets[i][j]);
                if (j + 1 < coinSets[i].length) System.out.print(", ");
            }

            System.out.println("], amount = " + amounts[i] +
                               " -> " + result +
                               " (expected " + expected[i] + ")");
        }
    }
}
