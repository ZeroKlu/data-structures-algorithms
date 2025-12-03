function longestConsecutive(nums) {
    const set = new Set(nums);
    let best = 0;

    for (const x of set) {
        if (!set.has(x - 1)) {
            let cur = x;
            let length = 1;
            while (set.has(cur + 1)) {
                cur++;
                length++;
            }
            if (length > best) best = length;
        }
    }
    return best;
}

// ============================
// Tests (top-level)
// ============================

function printArray(arr) {
    return "[" + arr.join(", ") + "]";
}

const tests = [
    [100, 4, 200, 1, 3, 2],            // expected 4
    [0, 3, 7, 2, 5, 8, 4, 6, 0, 1],     // expected 9
    [1, 2, 0, 1],                       // expected 3
    [],                                 // expected 0
    [-2, -1, 0, 2, 3, 4, 5]             // expected 4
];

const expected = [4, 9, 3, 0, 4];

console.log("=== Test: longestConsecutive ===\n");

tests.forEach((nums, i) => {
    const result = longestConsecutive(nums);
    console.log("Input: " + printArray(nums));
    console.log("Longest consecutive length:", result, `(expected ${expected[i]})\n`);
});
