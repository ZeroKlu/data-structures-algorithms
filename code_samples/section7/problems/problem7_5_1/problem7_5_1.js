/**
 * twoSumIndices
 *
 * Purpose:
 *   Finds the indices of two distinct elements in the array `nums`
 *   whose values add up to the given `target`.
 *
 * Parameters:
 *   nums   - Array of integers.
 *   target - Target sum we want to achieve.
 *
 * Returns:
 *   An array [i, j] such that:
 *       nums[i] + nums[j] === target
 *   If no such pair exists, returns an empty array [].
 *
 * Approach (single-pass hash map):
 *   - Use a Map to store values we have already seen along with their indices.
 *   - As we iterate through the array:
 *       1) Compute the value needed to reach the target.
 *       2) Check if that value already exists in the map.
 *       3) If it does, we have found a valid pair.
 *       4) Otherwise, store the current value and its index for future lookups.
 *
 * Why this works:
 *   - At iteration i, the map contains only elements from indices < i.
 *   - This guarantees we never reuse the same element twice.
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       Each element is processed once with O(1) average-time Map operations.
 *   - Space: O(n)
 *       The map may store up to n elements.
 *
 * Notes:
 *   - If a value appears multiple times, the map keeps the most recently
 *     stored index for that value.
 *   - This implementation returns the first valid pair encountered.
 */
function twoSumIndices(nums, target) {
    // Map: value -> index where that value was last seen
    const map = new Map();

    // Iterate through the array
    for (let i = 0; i < nums.length; i++) {
        // Compute the complement needed to reach the target
        const need = target - nums[i];

        // If we've already seen the needed value, return the indices
        if (map.has(need)) {
            return [map.get(need), i];
        }

        // Otherwise, record the current value and its index
        map.set(nums[i], i);
    }

    // No valid pair found
    return [];
}

// ============================
// Tests (top-level)
// ============================

/**
 * printArray
 *
 * Helper function to format arrays for output.
 */
function printArray(arr) {
    return "[" + arr.join(", ") + "]";
}

// Test cases covering:
//  - standard success cases
//  - different index positions
//  - a negative case with no solution
const tests = [
    { nums: [2, 7, 11, 15], target: 9,  expected: [0, 1] },
    { nums: [3, 2, 4],      target: 6,  expected: [1, 2] },
    { nums: [1, 2, 3, 4],   target: 10, expected: [] }
];

console.log("=== Test: twoSumIndices ===\n");

// Run each test case
for (const { nums, target, expected } of tests) {
    // Execute the function under test
    const result = twoSumIndices(nums, target);

    // Display input and result
    console.log(`Input nums = ${printArray(nums)}, target = ${target}`);
    console.log(
        `Result: ${printArray(result)} (expected ${printArray(expected)})\n`
    );
}
