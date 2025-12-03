static int LongestCommonSubsequence(string text1, string text2)
{
    int m = text1.Length;
    int n = text2.Length;
    int[,] dp = new int[m + 1, n + 1];

    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (text1[i - 1] == text2[j - 1])
            {
                dp[i, j] = dp[i - 1, j - 1] + 1;
            }
            else
            {
                dp[i, j] = Math.Max(dp[i - 1, j], dp[i, j - 1]);
            }
        }
    }
    return dp[m, n];
}

// ===============================
// Tests (top-level)
// ===============================

var tests = new (string s1, string s2, int expected)[]
{
    ("abcde", "ace", 3),
    ("abc", "abc", 3),
    ("abc", "def", 0),
    ("AGGTAB", "GXTXAYB", 4),
    ("bl", "yby", 1),
    ("aaaa", "aa", 2),
    ("", "", 0),
    ("abc", "", 0),
    ("", "abc", 0)
};

Console.WriteLine("=== Test: LongestCommonSubsequence ===\n");

foreach (var t in tests)
{
    int result = LongestCommonSubsequence(t.s1, t.s2);
    Console.WriteLine(
        $"LCS(\"{t.s1}\", \"{t.s2}\") = {result} (expected {t.expected})"
    );
}
