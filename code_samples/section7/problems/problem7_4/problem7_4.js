/**
 * subarraySum
 *
 * Purpose:
 *   Counts the number of contiguous subarrays whose elements sum exactly to k.
 *   This is the classic "Subarray Sum Equals K" problem.
 *
 * Parameters:
 *   nums - Array of integers (may be empty; may include negative values).
 *   k    - Target sum.
 *
 * Returns:
 *   Number of contiguous subarrays whose sum is exactly k.
 *
 * Core idea (prefix sums + frequency map):
 *   Let prefix be the running sum of elements seen so far.
 *   The sum of subarray (j+1..i) is:
 *
 *       prefix[i] - prefix[j]
 *
 *   We want:
 *
 *       prefix[i] - prefix[j] == k
 *       => prefix[j] == prefix[i] - k
 *
 *   So while scanning:
 *     - Maintain a map freq where freq[P] = how many times prefix sum P has occurred.
 *     - For current prefix sum P, the number of subarrays ending here with sum k is:
 *         freq[P - k]
 *     - Then increment freq[P].
 *
 * Why freq starts with (0 -> 1):
 *   - Represents an “empty prefix” before processing any elements.
 *   - Allows subarrays that start at index 0 to be counted correctly.
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       One pass; each step does O(1) average-time Map operations.
 *   - Space: O(n)
 *       In the worst case, all prefix sums are distinct.
 *
 * Notes:
 *   - Works with negative numbers (unlike many sliding-window approaches).
 *   - Using Map avoids prototype pitfalls that can happen with plain objects.
 */
function subarraySum(nums, k) {
    // Map: prefixSum -> frequency count
    const freq = new Map();

    // Base case: prefix sum 0 occurs once before reading any elements
    freq.set(0, 1);

    let prefix = 0; // running prefix sum
    let count = 0;  // number of qualifying subarrays found

    // Scan through the array once
    for (const x of nums) {
        // Update running sum
        prefix += x;

        // We need prior prefix sums equal to (prefix - k)
        const need = prefix - k;

        // If we've seen that needed prefix sum before, each occurrence forms
        // a subarray ending at the current position with sum k.
        if (freq.has(need)) {
            count += freq.get(need);
        }

        // Record that we've now seen this prefix sum.
        // If absent, treat prior count as 0.
        freq.set(prefix, (freq.get(prefix) || 0) + 1);
    }

    return count;
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
    return `[${arr.join(", ")}]`;
}

// Test cases include:
//  - simple repeated numbers
//  - different-length matching subarrays
//  - negatives (to show this works beyond sliding-window methods)
//  - empty input
const tests = [
    { nums: [1, 1, 1], k: 2, expected: 2 },
    { nums: [1, 2, 3], k: 3, expected: 2 },                 // [1,2], [3]
    { nums: [3, 4, 7, 2, -3, 1, 4, 2], k: 7, expected: 4 },
    { nums: [], k: 0, expected: 0 }
];

console.log("=== Test: subarraySum ===\n");

// Run each test case and compare output
for (const { nums, k, expected } of tests) {
    const result = subarraySum(nums, k);

    console.log(`Input: nums = ${printArray(nums)}, k = ${k}`);
    console.log(`Result: ${result} (expected ${expected})\n`);
}
