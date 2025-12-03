package code_samples.section8.problems.problem8_5;

public class problem8_5 {

    int longestCommonSubsequence(String text1, String text2) {
        int m = text1.length();
        int n = text2.length();
        int[][] dp = new int[m + 1][n + 1];

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (text1.charAt(i - 1) == text2.charAt(j - 1)) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[m][n];
    }

    // ==========================
    // Test Harness
    // ==========================
    public static void main(String[] args) {

        problem8_5 solver = new problem8_5();

        record Test(String a, String b, int expected) {}
        Test[] tests = {
            new Test("abcde", "ace", 3),
            new Test("abc", "abc", 3),
            new Test("abc", "def", 0),
            new Test("AGGTAB", "GXTXAYB", 4),
            new Test("bl", "yby", 1),
            new Test("aaaa", "aa", 2),
            new Test("", "", 0),
            new Test("abc", "", 0),
            new Test("", "abc", 0)
        };

        System.out.println("=== Test: longestCommonSubsequence ===\n");

        for (Test t : tests) {
            int result = solver.longestCommonSubsequence(t.a(), t.b());
            System.out.println(
                "LCS(\"" + t.a() + "\", \"" + t.b() + "\") = " + result +
                " (expected " + t.expected() + ")"
            );
        }
    }
}
