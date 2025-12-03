function twoSumIndices(nums, target) {
    const map = new Map(); // value -> index
    for (let i = 0; i < nums.length; i++) {
        const need = target - nums[i];
        if (map.has(need)) {
            return [map.get(need), i];
        }
        map.set(nums[i], i);
    }
    return []; // no solution
}

// ============================
// Tests (top-level)
// ============================

function printArray(arr) {
    return "[" + arr.join(", ") + "]";
}

const tests = [
    { nums: [2, 7, 11, 15], target: 9, expected: [0, 1] },
    { nums: [3, 2, 4],      target: 6, expected: [1, 2] },
    { nums: [1, 2, 3, 4],   target: 10, expected: [] }
];

console.log("=== Test: twoSumIndices ===\n");

for (const { nums, target, expected } of tests) {
    const result = twoSumIndices(nums, target);

    console.log(`Input nums = ${printArray(nums)}, target = ${target}`);
    console.log(`Result: ${printArray(result)} (expected ${printArray(expected)})\n`);
}
