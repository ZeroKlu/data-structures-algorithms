function climbStairs(n) {
    if (n <= 2) return n;
    let prev2 = 1;
    let prev1 = 2;
    for (let i = 3; i <= n; i++) {
        const cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

// ============================
// Tests (top-level)
// ============================

const tests    = [0, 1, 2, 3, 4, 5, 6, 10, 20];
const expected = [0, 1, 2, 3, 5, 8, 13, 89, 10946];

console.log("=== Test: climbStairs ===\n");

for (let i = 0; i < tests.length; i++) {
    const n = tests[i];
    const result = climbStairs(n);

    console.log(`climbStairs(${n}) = ${result} (expected ${expected[i]})`);
}
