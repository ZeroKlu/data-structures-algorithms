package code_samples.section8.lesson;

public class section8 {

    long fibDp(int n) {
        if (n <= 1) return n;
        long prev2 = 0; // F(0)
        long prev1 = 1; // F(1)
        for (int i = 2; i <= n; i++) {
            long curr = prev1 + prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }

    long climbStairs(int n) {
        if (n <= 1) return 1;
        long prev2 = 1; // ways(0)
        long prev1 = 1; // ways(1)
        for (int i = 2; i <= n; i++) {
            long curr = prev1 + prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }

    int knapSack01(int n, int W, int[] w, int[] v) {
        int[] dp = new int[W + 1];
        for (int i = 0; i < n; i++) {
            for (int c = W; c >= w[i]; c--) {
                int withItem = dp[c - w[i]] + v[i];
                if (withItem > dp[c]) {
                    dp[c] = withItem;
                }
            }
        }
        return dp[W];
    }

    int lcsLength(String s, String t) {
        int n = s.length();
        int m = t.length();
        int[][] dp = new int[n + 1][m + 1];
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (s.charAt(i - 1) == t.charAt(j - 1)) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[n][m];
    }

    // ============================
    // Test Harness
    // ============================
    public static void main(String[] args) {

        section8 solver = new section8();

        // -------- fibDp tests --------
        int[] fibTests =    {0, 1, 2, 3, 4, 5, 10, 20, 30, 50};
        long[] fibExpected = {0, 1, 1, 2, 3, 5, 55, 6765, 832040, 12586269025L};

        System.out.println("=== Test: fibDp ===\n");

        for (int i = 0; i < fibTests.length; i++) {
            int n = fibTests[i];
            long result = solver.fibDp(n);

            System.out.println("fibDp(" + n + ") = " + result +
                               " (expected " + fibExpected[i] + ")");
        }

        // -------- climbStairs tests --------
        System.out.println("\n=== Test: climbStairs ===\n");

        int[] climbTests =     {0, 1, 2, 3, 4, 5, 10};
        long[] climbExpected = {1, 1, 2, 3, 5, 8, 89};

        for (int i = 0; i < climbTests.length; i++) {
            int n = climbTests[i];
            long result = solver.climbStairs(n);

            System.out.println("climbStairs(" + n + ") = " + result +
                               " (expected " + climbExpected[i] + ")");
        }

        // -------- knapSack01 tests --------
        System.out.println("\n=== Test: knapSack01 ===\n");

        {
            int[] w = {2, 3, 4, 5};
            int[] v = {3, 4, 5, 6};
            int n = w.length;

            int W1 = 5;
            int expected1 = 7; // 2+3 weights -> 3+4 value
            int result1 = solver.knapSack01(n, W1, w, v);
            System.out.println("knapSack01(W=" + W1 + ") = " + result1 +
                               " (expected " + expected1 + ")");

            int W2 = 7;
            int expected2 = 9; // 2+5 weights -> 3+6 value
            int result2 = solver.knapSack01(n, W2, w, v);
            System.out.println("knapSack01(W=" + W2 + ") = " + result2 +
                               " (expected " + expected2 + ")");

            int W3 = 3;
            int expected3 = 4; // weight 3, value 4
            int result3 = solver.knapSack01(n, W3, w, v);
            System.out.println("knapSack01(W=" + W3 + ") = " + result3 +
                               " (expected " + expected3 + ")");
        }

        // -------- lcsLength tests --------
        System.out.println("\n=== Test: lcsLength ===\n");

        class LcsCase {
            String s, t;
            int expected;
            LcsCase(String s, String t, int expected) {
                this.s = s; this.t = t; this.expected = expected;
            }
        }

        LcsCase[] lcsTests = {
            new LcsCase("abcde", "ace", 3),        // "ace"
            new LcsCase("abc", "abc", 3),          // "abc"
            new LcsCase("abc", "def", 0),          // none
            new LcsCase("", "abc", 0),
            new LcsCase("AGGTAB", "GXTXAYB", 4)    // "GTAB"
        };

        for (LcsCase c : lcsTests) {
            int result = solver.lcsLength(c.s, c.t);
            System.out.println("lcsLength(\"" + c.s + "\", \"" + c.t +
                               "\") = " + result +
                               " (expected " + c.expected + ")");
        }
    }
}
