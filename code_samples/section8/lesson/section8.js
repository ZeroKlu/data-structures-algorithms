function fibDp(n) {
    if (n <= 1) return n;
    let prev2 = 0; // F(0)
    let prev1 = 1; // F(1)
    for (let i = 2; i <= n; i++) {
        const curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

function climbStairs(n) {
    if (n <= 1) return 1;
    let prev2 = 1; // ways(0)
    let prev1 = 1; // ways(1)
    for (let i = 2; i <= n; i++) {
        const curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

function knapSack01(n, W, w, v) {
    const dp = new Array(W + 1).fill(0);
    for (let i = 0; i < n; i++) {
        for (let c = W; c >= w[i]; c--) {
            const withItem = dp[c - w[i]] + v[i];
            if (withItem > dp[c]) dp[c] = withItem;
        }
    }
    return dp[W];
}

function lcsLength(s, t) {
    const n = s.length;
    const m = t.length;
    const dp = Array.from({ length: n + 1 }, () =>
        new Array(m + 1).fill(0)
    );

    for (let i = 1; i <= n; i++) {
        for (let j = 1; j <= m; j++) {
            if (s[i - 1] === t[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n][m];
}

// ==========================================================
// Tests
// ==========================================================

// ---------- fibDp ----------
console.log("=== Test: fibDp ===\n");

const fibTests    = [0, 1, 2, 3, 4, 5, 10, 20, 30, 50];
const fibExpected = [0, 1, 1, 2, 3, 5, 55, 6765, 832040, 12586269025n];

for (let i = 0; i < fibTests.length; i++) {
    let result = fibDp(fibTests[i]);
    let resultBig = BigInt(result);
    console.log(`fibDp(${fibTests[i]}) = ${resultBig} (expected ${fibExpected[i]})`);
}

// ---------- climbStairs ----------
console.log("\n=== Test: climbStairs ===\n");

const climbTests     = [0, 1, 2, 3, 4, 5, 10];
const climbExpected  = [1, 1, 2, 3, 5, 8, 89];

for (let i = 0; i < climbTests.length; i++) {
    const result = climbStairs(climbTests[i]);
    console.log(`climbStairs(${climbTests[i]}) = ${result} (expected ${climbExpected[i]})`);
}

// ---------- knapSack01 ----------
console.log("\n=== Test: knapSack01 ===\n");

{
    const w = [2, 3, 4, 5];
    const v = [3, 4, 5, 6];
    const n = w.length;

    console.log(`knapSack01(W=5) = ${knapSack01(n, 5, w, v)} (expected 7)`);
    console.log(`knapSack01(W=7) = ${knapSack01(n, 7, w, v)} (expected 9)`);
    console.log(`knapSack01(W=3) = ${knapSack01(n, 3, w, v)} (expected 4)`);
}

// ---------- lcsLength ----------
console.log("\n=== Test: lcsLength ===\n");

const lcsTests = [
    ["abcde", "ace", 3],
    ["abc", "abc", 3],
    ["abc", "def", 0],
    ["", "abc", 0],
    ["AGGTAB", "GXTXAYB", 4]
];

for (const [s, t, expected] of lcsTests) {
    const result = lcsLength(s, t);
    console.log(`lcsLength("${s}", "${t}") = ${result} (expected ${expected})`);
}
