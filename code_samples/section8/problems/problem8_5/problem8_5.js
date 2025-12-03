function longestCommonSubsequence(text1, text2) {
    const m = text1.length;
    const n = text2.length;
    const dp = Array.from({ length: m + 1 }, () => new Array(n + 1).fill(0));

    for (let i = 1; i <= m; i++) {
        for (let j = 1; j <= n; j++) {
            if (text1[i - 1] === text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

// =============================
// Tests (top-level)
// =============================

const tests = [
    ["abcde", "ace", 3],
    ["abc", "abc", 3],
    ["abc", "def", 0],
    ["AGGTAB", "GXTXAYB", 4],
    ["bl", "yby", 1],
    ["aaaa", "aa", 2],
    ["", "", 0],
    ["abc", "", 0],
    ["", "abc", 0],
];

console.log("=== Test: longestCommonSubsequence ===\n");

for (const [s1, s2, expected] of tests) {
    const result = longestCommonSubsequence(s1, s2);
    console.log(`LCS("${s1}", "${s2}") = ${result} (expected ${expected})`);
}
