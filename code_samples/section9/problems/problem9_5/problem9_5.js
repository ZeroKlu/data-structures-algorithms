function minCost(n, cuts) {
    const pos = [0, ...cuts.slice(), n];
    pos.sort((a, b) => a - b);
    const m = pos.length;
    const dp = Array.from({ length: m }, () => new Array(m).fill(0));

    for (let len = 2; len < m; len++) {
        for (let i = 0; i + len < m; i++) {
            const j = i + len;
            let best = Number.POSITIVE_INFINITY;
            for (let k = i + 1; k < j; k++) {
                const cand = dp[i][k] + dp[k][j] + (pos[j] - pos[i]);
                if (cand < best) best = cand;
            }
            dp[i][j] = best === Number.POSITIVE_INFINITY ? 0 : best;
        }
    }
    return dp[0][m - 1];
}

// ===========================
// Test Harness
// ===========================

function test(name, n, cuts, expected) {
    const result = minCost(n, cuts);
    console.log(name);
    console.log(`n = ${n}, cuts = [${cuts.join(",")}]`);
    console.log(`minCost = ${result} (expected ${expected})\n`);
}

console.log("=== Testing minCost (Cutting Sticks DP) ===\n");

// Test 1: Classic example
test("Test 1: Classic example",
     7, [1, 3, 4, 5], 16);

// Test 2: Two cuts
test("Test 2: Two cuts",
     9, [5, 6], 13);

// Test 3: Single cut
test("Test 3: Single cut",
     10, [4], 10);

// Test 4: Even spacing (correct = 24)
test("Test 4: Even spacing",
     10, [2, 4, 6, 8], 24);

// Test 5: No cuts
test("Test 5: No cuts",
     100, [], 0);
