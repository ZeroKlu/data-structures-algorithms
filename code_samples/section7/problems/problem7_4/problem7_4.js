function subarraySum(nums, k) {
    const freq = new Map();
    freq.set(0, 1);
    let prefix = 0;
    let count = 0;

    for (const x of nums) {
        prefix += x;
        const need = prefix - k;
        if (freq.has(need)) {
            count += freq.get(need);
        }
        freq.set(prefix, (freq.get(prefix) || 0) + 1);
    }
    return count;
}

// ============================
// Tests (top-level)
// ============================

function printArray(arr) {
    return `[${arr.join(", ")}]`;
}

const tests = [
    { nums: [1, 1, 1], k: 2, expected: 2 },
    { nums: [1, 2, 3], k: 3, expected: 2 },                 // [1,2], [3]
    { nums: [3, 4, 7, 2, -3, 1, 4, 2], k: 7, expected: 4 },
    { nums: [], k: 0, expected: 0 }
];

console.log("=== Test: subarraySum ===\n");

for (const { nums, k, expected } of tests) {
    const result = subarraySum(nums, k);
    console.log(`Input: nums = ${printArray(nums)}, k = ${k}`);
    console.log(`Result: ${result} (expected ${expected})\n`);
}
