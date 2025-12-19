/**
 * longestConsecutive
 *
 * Purpose:
 *   Computes the length of the longest run of consecutive integers in an array,
 *   regardless of their original order.
 *
 * Example:
 *   nums = [100, 4, 200, 1, 3, 2]
 *   The longest consecutive sequence is [1, 2, 3, 4] → returns 4.
 *
 * Parameters:
 *   nums - Array of integers (may be empty and may contain duplicates).
 *
 * Returns:
 *   Length of the longest consecutive sequence (0 if the input is empty).
 *
 * Core idea:
 *   - Use a Set for O(1) average-time membership checks.
 *   - Only start counting a sequence from numbers that are the *start* of a run
 *     (i.e., x - 1 is not present). This avoids recounting the same sequence.
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       Each number is processed a constant number of times.
 *   - Space: O(n)
 *       The Set stores all unique values.
 *
 * Notes:
 *   - Duplicate values in the input do not affect correctness because Set
 *     automatically removes duplicates.
 *   - Iteration order of a Set is insertion order, but ordering does not matter
 *     for correctness here.
 */
function longestConsecutive(nums) {
    // Convert the array into a Set to:
    //  1) Remove duplicates
    //  2) Enable fast existence checks
    const set = new Set(nums);

    // Tracks the maximum sequence length found so far
    let best = 0;

    // Iterate over each unique number in the set
    for (const x of set) {
        // Only attempt to build a sequence starting at x if x-1 does NOT exist.
        // This ensures x is the smallest number in its consecutive run.
        if (!set.has(x - 1)) {
            let cur = x;
            let length = 1;

            // Extend the sequence forward as long as consecutive numbers exist
            while (set.has(cur + 1)) {
                cur++;
                length++;
            }

            // Update the best (maximum) length if this run is longer
            if (length > best) best = length;
        }
    }

    return best;
}

// ============================
// Tests (top-level)
// ============================

/**
 * printArray
 *
 * Helper function to format an array as "[a, b, c]".
 *
 * Parameters:
 *   arr - Array of numbers.
 *
 * Returns:
 *   String representation of the array.
 */
function printArray(arr) {
    return "[" + arr.join(", ") + "]";
}

// Test cases covering:
//  - typical mixed-order input
//  - duplicates
//  - empty input
//  - negative values
const tests = [
    [100, 4, 200, 1, 3, 2],            // expected 4: 1,2,3,4
    [0, 3, 7, 2, 5, 8, 4, 6, 0, 1],     // expected 9: 0..8
    [1, 2, 0, 1],                       // expected 3: 0,1,2
    [],                                 // expected 0
    [-2, -1, 0, 2, 3, 4, 5]             // expected 4: 2,3,4,5
];

// Expected outputs aligned with the test cases
const expected = [4, 9, 3, 0, 4];

console.log("=== Test: longestConsecutive ===\n");

// Run each test case and compare with expected result
tests.forEach((nums, i) => {
    const result = longestConsecutive(nums);

    console.log("Input: " + printArray(nums));
    console.log(
        "Longest consecutive length:",
        result,
        `(expected ${expected[i]})\n`
    );
});
