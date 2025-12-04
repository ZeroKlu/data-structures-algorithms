function canPartition(nums) {
    const total = nums.reduce((a, b) => a + b, 0);
    if (total % 2 !== 0) return false;
    const target = total / 2;
    const dp = new Array(target + 1).fill(false);
    dp[0] = true;

    for (const x of nums) {
        for (let s = target; s >= x; s--) {
            if (dp[s - x]) dp[s] = true;
        }
    }
    return dp[target];
}

// ===========================
// Test Harness
// ===========================

function test(name, arr, expected) {
    const result = canPartition(arr);
    console.log(name);
    console.log(`Input: [${arr.join(",")}]`);
    console.log(`canPartition = ${result} (expected ${expected})\n`);
}

console.log("=== Testing canPartition ===\n");

test("Test 1: Classic example", [1, 5, 11, 5], true);
test("Test 2: Cannot partition", [1, 2, 3, 5], false);
test("Test 3: Single element", [7], false);
test("Test 4: Two equal numbers", [4, 4], true);
test("Test 5: Larger mixed set", [2, 2, 3, 5], false);
test("Test 6: Multiple valid subsets", [2, 2, 1, 1], true);
