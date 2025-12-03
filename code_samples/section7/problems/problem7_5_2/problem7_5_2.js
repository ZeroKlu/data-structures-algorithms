function countUniquePairs(nums, target) {
    const seen = new Set();
    const pairs = new Set(); // store "a,b" with a <= b

    for (const x of nums) {
        const y = target - x;
        if (seen.has(y)) {
            const a = Math.min(x, y);
            const b = Math.max(x, y);
            pairs.add(a + "," + b);
        }
        seen.add(x);
    }
    return pairs.size;
}

// ============================
// Tests (top-level)
// ============================

function printArray(arr) {
    return "[" + arr.join(", ") + "]";
}

const tests = [
    { nums: [1, 1, 2, 2, 3, 3], target: 4, expected: 2, note: "{1,3} & {2,2}" },
    { nums: [2, 7, 11, 15],     target: 9, expected: 1, note: "{2,7}" },
    { nums: [3, 3, 3],          target: 6, expected: 1, note: "{3,3}" },
    { nums: [1, 2, 3, 4],       target: 10, expected: 0, note: "none" },
    { nums: [1, 5, 1, 5],       target: 6, expected: 1, note: "{1,5}" }
];

console.log("=== Test: countUniquePairs ===\n");

for (const t of tests) {
    const result = countUniquePairs(t.nums, t.target);

    console.log(`Input nums = ${printArray(t.nums)}, target = ${t.target}`);
    console.log(`Result:   ${result}`);
    console.log(`Expected: ${t.expected} (${t.note})\n`);
}
