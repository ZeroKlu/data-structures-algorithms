/**
 * Computes the length of the Longest Increasing Subsequence (LIS)
 * in the given array of numbers.
 *
 * This implementation uses the classic O(n log n) "tails" method,
 * sometimes described via patience sorting.
 *
 * Core invariant:
 *   tails[i] is the smallest possible tail value of any strictly
 *   increasing subsequence of length (i + 1) encountered so far.
 *
 * Because `tails` is always kept sorted, we can binary-search it
 * to decide where each new value belongs.
 *
 * Algorithm overview:
 *   For each value x in nums:
 *     1) Binary search tails to find the first index `lo` such that
 *        tails[lo] >= x (lower_bound).
 *     2) Place x at tails[lo].
 *        - If lo equals the current length of tails, x extends the LIS.
 *        - Otherwise, x replaces an existing tail, improving (lowering)
 *          the tail value for subsequences of that length.
 *
 * Important notes:
 *   - This function returns only the length of the LIS, not the subsequence.
 *   - The subsequence is strictly increasing (duplicates do not extend it).
 *
 * Complexity:
 *   Time:  O(n log n)
 *   Space: O(n)
 *
 * @param {number[]} nums - Input array of numbers
 * @returns {number} Length of the longest strictly increasing subsequence
 */
function lengthOfLIS(nums) {
    // `tails` holds candidate tail values for increasing subsequences.
    const tails = [];

    // Process each number in the input.
    for (const x of nums) {
        // Binary search in the range [0, tails.length)
        let lo = 0, hi = tails.length;

        while (lo < hi) {
            // Midpoint calculation using bit shift for fast integer division by 2.
            const mid = (lo + hi) >> 1;

            // If tails[mid] < x, x must go to the right of mid.
            if (tails[mid] < x) {
                lo = mid + 1;
            } else {
                // Otherwise, mid is a candidate insertion/replacement position.
                hi = mid;
            }
        }

        // Place x at index lo.
        // If lo == tails.length, this effectively appends x and increases LIS length.
        // Otherwise, it replaces an existing value to improve the tail.
        tails[lo] = x;
    }

    // The number of elements in tails equals the LIS length.
    return tails.length;
}

// ===========================
// Tests
// ===========================

/**
 * Helper to run a test case and print results in a consistent format.
 *
 * @param {string} name - Test name/description
 * @param {number[]} arr - Input array
 * @param {number} expected - Expected LIS length
 */
function test(name, arr, expected) {
    const result = lengthOfLIS(arr);
    console.log(`${name}`);
    console.log(`Input: [${arr.join(",")}]`);
    console.log(`lengthOfLIS = ${result} (expected ${expected})\n`);
}

console.log("=== Testing lengthOfLIS ===\n");

// Strictly increasing sequence => LIS uses all elements.
test("Test 1: Increasing sequence", [1, 2, 3, 4, 5], 5);

// Classic mixed example => known LIS length is 4.
test("Test 2: Mixed sequence", [10, 9, 2, 5, 3, 7, 101, 18], 4);

// All identical values => strictly increasing disallows duplicates => LIS = 1.
test("Test 3: All identical", [7, 7, 7, 7, 7], 1);

// Empty input => LIS length is 0.
test("Test 4: Empty array", [], 0);

// Strictly decreasing sequence => best LIS is any single element.
test("Test 5: Decreasing sequence", [9, 7, 5, 3, 1], 1);

// Random pattern => one LIS is [4,8,9], length = 3.
test("Test 6: Random pattern", [4, 10, 4, 3, 8, 9], 3);
