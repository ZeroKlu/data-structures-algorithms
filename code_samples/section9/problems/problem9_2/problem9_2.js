function knapSack(W, weight, value) {
    const dp = new Array(W + 1).fill(0);
    for (let i = 0; i < weight.length; i++) {
        const wt = weight[i];
        const val = value[i];
        for (let w = W; w >= wt; w--) {
            dp[w] = Math.max(dp[w], dp[w - wt] + val);
        }
    }
    return dp[W];
}

// ===========================
// Test Harness
// ===========================

function test(name, W, weight, value, expected) {
    const result = knapSack(W, weight, value);
    console.log(name);
    console.log(`Capacity = ${W}`);
    console.log(`Weights: [${weight.join(",")}]`);
    console.log(`Values:  [${value.join(",")}]`);
    console.log(`knapSack = ${result} (expected ${expected})\n`);
}

console.log("=== Testing knapSack ===\n");

test("Test 1: Classic example",
     50,
     [10, 20, 30],
     [60, 100, 120],
     220);

test("Test 2: Small weights",
     6,
     [1, 2, 3],
     [10, 15, 40],
     65);

test("Test 3: Too small capacity",
     4,
     [5, 6, 7],
     [10, 20, 30],
     0);

test("Test 4: Zero capacity",
     0,
     [2, 3, 4],
     [20, 30, 40],
     0);

test("Test 5: Larger mixed set",
     10,
     [2, 3, 5, 7],
     [20, 30, 50, 70],
     100);
