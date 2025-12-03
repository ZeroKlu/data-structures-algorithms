function rob(nums) {
    if (nums.length === 0) return 0;
    if (nums.length === 1) return nums[0];

    let prev2 = nums[0];
    let prev1 = Math.max(nums[0], nums[1]);

    for (let i = 2; i < nums.length; i++) {
        const take = prev2 + nums[i];
        const skip = prev1;
        const cur = Math.max(take, skip);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

// ============================
// Tests
// ============================

const tests = [
    [],
    [5],
    [1, 2],
    [2, 7, 9, 3, 1],
    [1, 2, 3, 1],
    [2, 1, 1, 2],
    [10, 5, 20, 15, 30],
    [1, 3, 1, 3, 100]
];

const expected = [
    0,
    5,
    2,
    12,
    4,
    4,
    60,
    103
];

console.log("=== Test: rob (House Robber) ===\n");

for (let i = 0; i < tests.length; i++) {
    const nums = tests[i];
    const result = rob(nums);

    console.log(`Test ${i + 1}: rob([${nums.join(", ")}]) = ${result} (expected ${expected[i]})`);
}
