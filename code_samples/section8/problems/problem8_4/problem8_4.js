function uniquePaths(m, n) {
    const dp = Array.from({ length: m }, () => new Array(n).fill(1));
    for (let i = 1; i < m; i++) {
        for (let j = 1; j < n; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }
    return dp[m - 1][n - 1];
}

// ====================================
// Tests (top-level)
// ====================================

const tests = [
    [1, 1, 1],
    [1, 5, 1],
    [3, 3, 6],
    [3, 7, 28],
    [3, 2, 3],
    [7, 3, 28],
    [10, 10, 48620]
];

console.log("=== Test: uniquePaths ===\n");

for (const [m, n, expected] of tests) {
    const result = uniquePaths(m, n);
    console.log(`uniquePaths(${m}, ${n}) = ${result} (expected ${expected})`);
}
