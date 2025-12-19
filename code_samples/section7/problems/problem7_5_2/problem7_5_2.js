/**
 * countUniquePairs
 *
 * Purpose:
 *   Counts the number of *unique value pairs* {a, b} such that:
 *       a + b === target
 *
 *   Uniqueness is by VALUE, not by index.
 *   That means duplicates in the input array do not create extra pairs
 *   if they represent the same two numbers.
 *
 * Examples:
 *   nums   = [1, 1, 2, 2, 3, 3], target = 4
 *   Valid unique pairs: {1,3}, {2,2} → result = 2
 *
 *   nums   = [1, 5, 1, 5], target = 6
 *   Valid unique pair: {1,5} → result = 1
 *
 * Parameters:
 *   nums   - Array of integers (may contain duplicates).
 *   target - Target sum.
 *
 * Returns:
 *   The number of distinct value pairs that sum to the target.
 *
 * Algorithm (single pass with sets):
 *   - Use `seen` to store values we have already processed.
 *   - Use `pairs` to store unique normalized pairs.
 *
 *   For each value x in nums:
 *     1) Compute y = target - x.
 *     2) If y is already in `seen`, then {x, y} is a valid pair.
 *     3) Normalize the pair so order does not matter:
 *          a = min(x, y), b = max(x, y)
 *     4) Encode the pair as a string "a,b" and insert into `pairs`.
 *     5) Add x to `seen`.
 *
 * Why normalization matters:
 *   - Without it, {1,3} and {3,1} would be treated as different pairs.
 *   - Enforcing a <= b guarantees one canonical representation.
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       Each element is processed once, with O(1) average-time Set operations.
 *       String creation adds some overhead but is acceptable for learning/demo.
 *   - Space: O(n)
 *       In the worst case, both sets may grow linearly.
 *
 * Notes / potential improvements:
 *   - Using strings ("a,b") is simple but not optimal.
 *     A more efficient alternative would be to store pairs as:
 *       - a custom object with a hash, or
 *       - a single packed integer / BigInt key.
 */
function countUniquePairs(nums, target) {
    // Set of values we have already seen while scanning left-to-right
    const seen = new Set();

    // Set of unique normalized pair keys ("a,b")
    const pairs = new Set();

    // Iterate through the array
    for (const x of nums) {
        // Complement needed to reach the target
        const y = target - x;

        // If y has already appeared, x and y form a valid pair
        if (seen.has(y)) {
            // Normalize order so (x,y) and (y,x) are treated the same
            const a = Math.min(x, y);
            const b = Math.max(x, y);

            // Encode normalized pair into a string key
            pairs.add(a + "," + b);
        }

        // Record current value as seen
        seen.add(x);
    }

    // Number of unique value pairs found
    return pairs.size;
}

// ============================
// Tests (top-level)
// ============================

/**
 * printArray
 *
 * Helper function to format arrays consistently.
 */
function printArray(arr) {
    return "[" + arr.join(", ") + "]";
}

// Test cases covering:
//  - duplicates that form multiple unique pairs
//  - standard two-sum case
//  - repeated single value case
//  - no-solution case
//  - same pair appearing multiple times
const tests = [
    { nums: [1, 1, 2, 2, 3, 3], target: 4, expected: 2, note: "{1,3} & {2,2}" },
    { nums: [2, 7, 11, 15],     target: 9, expected: 1, note: "{2,7}" },
    { nums: [3, 3, 3],          target: 6, expected: 1, note: "{3,3}" },
    { nums: [1, 2, 3, 4],       target: 10, expected: 0, note: "none" },
    { nums: [1, 5, 1, 5],       target: 6, expected: 1, note: "{1,5}" }
];

console.log("=== Test: countUniquePairs ===\n");

// Run each test case
for (const t of tests) {
    const result = countUniquePairs(t.nums, t.target);

    console.log(`Input nums = ${printArray(t.nums)}, target = ${t.target}`);
    console.log(`Result:   ${result}`);
    console.log(`Expected: ${t.expected} (${t.note})\n`);
}
