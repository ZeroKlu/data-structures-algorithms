function tsp(cost) {
    const n = cost.length;
    const maxMask = 1 << n;
    const INF = 1e9;
    const dp = Array.from({ length: maxMask }, () => new Array(n).fill(INF));
    dp[1][0] = 0;

    for (let mask = 1; mask < maxMask; mask++) {
        for (let i = 0; i < n; i++) {
            if ((mask & (1 << i)) === 0) continue;
            const curCost = dp[mask][i];
            if (curCost === INF) continue;
            for (let j = 0; j < n; j++) {
                if (mask & (1 << j)) continue;
                const nextMask = mask | (1 << j);
                const cand = curCost + cost[i][j];
                if (cand < dp[nextMask][j]) {
                    dp[nextMask][j] = cand;
                }
            }
        }
    }

    const fullMask = maxMask - 1;
    let ans = INF;
    for (let i = 0; i < n; i++) {
        const cand = dp[fullMask][i] + cost[i][0];
        if (cand < ans) ans = cand;
    }
    return ans;
}

// ===========================
// Test Harness
// ===========================

function test(name, cost, expected) {
    const result = tsp(cost);
    console.log(name);
    console.log("Cost matrix:");
    cost.forEach(row => console.log("  " + JSON.stringify(row)));
    console.log(`tsp() = ${result} (expected ${expected})\n`);
}

console.log("=== Testing tsp (bitmask DP) ===\n");

// -----------------------------------------
// Test 1: Classic 4-city example
// -----------------------------------------
test(
    "Test 1: Classic 4-city example",
    [
        [0, 10, 15, 20],
        [10, 0, 35, 25],
        [15, 35, 0, 30],
        [20, 25, 30, 0]
    ],
    80
);

// -----------------------------------------
// Test 2: 3-city triangle
// -----------------------------------------
test(
    "Test 2: 3-city triangle",
    [
        [0, 1, 4],
        [1, 0, 2],
        [4, 2, 0]
    ],
    7
);

// -----------------------------------------
// Test 3: 4-city square loop
// -----------------------------------------
test(
    "Test 3: 4-city square",
    [
        [0, 1, 2, 1],
        [1, 0, 1, 2],
        [2, 1, 0, 1],
        [1, 2, 1, 0]
    ],
    4
);
