/**
 * canPartition
 * ------------
 * Determines whether the given array of numbers can be partitioned
 * into two subsets such that the sum of each subset is equal.
 *
 * This is the classic "Partition Equal Subset Sum" problem.
 * It can be reduced to a 0/1 subset-sum decision problem.
 *
 * Key observation:
 *   - Let total be the sum of all elements.
 *   - If total is odd, it is impossible to split it into two equal sums.
 *   - If total is even, we only need to check whether there exists a subset
 *     that sums to total / 2.
 *
 * Dynamic Programming formulation (1D boolean DP):
 *   dp[s] = true if we can form sum `s` using some subset of the elements
 *           processed so far.
 *
 * Initialization:
 *   dp[0] = true
 *   (Sum 0 is always achievable by choosing no elements.)
 *
 * Transition:
 *   For each element x:
 *     for s from target down to x:
 *       if dp[s - x] is true, then dp[s] can be set to true
 *
 * Why iterate s backwards?
 *   - Backward iteration ensures each element is used at most once (0/1 choice).
 *   - Forward iteration would allow the same element to be reused multiple times,
 *     turning this into an unbounded subset-sum problem.
 *
 * Complexity:
 *   Time:  O(n * target), where target = total / 2
 *   Space: O(target)
 *
 * @param {number[]} nums - Array of positive integers
 * @returns {boolean} true if nums can be partitioned into two equal-sum subsets
 */
function canPartition(nums) {
    // Compute the total sum of all elements
    const total = nums.reduce((a, b) => a + b, 0);

    // If total sum is odd, equal partition is impossible
    if (total % 2 !== 0) return false;

    // Target sum for one subset
    const target = total / 2;

    // dp[s] indicates whether sum s is achievable
    const dp = new Array(target + 1).fill(false);

    // Base case: sum 0 is always achievable
    dp[0] = true;

    // Process each number in the input array
    for (const x of nums) {
        // Traverse sums backwards to enforce 0/1 usage
        for (let s = target; s >= x; s--) {
            // If we could previously make (s - x),
            // then we can make s by including x
            if (dp[s - x]) dp[s] = true;
        }
    }

    // If target sum is achievable, a valid partition exists
    return dp[target];
}

// ===========================
// Test Harness
// ===========================

/**
 * Runs a single test case for canPartition() and prints:
 *   - test name
 *   - input array
 *   - computed result
 *   - expected result
 *
 * @param {string} name - Descriptive test name
 * @param {number[]} arr - Input array
 * @param {boolean} expected - Expected boolean result
 */
function test(name, arr, expected) {
    const result = canPartition(arr);
    console.log(name);
    console.log(`Input: [${arr.join(",")}]`);
    console.log(`canPartition = ${result} (expected ${expected})\n`);
}

// Header for all tests
console.log("=== Testing canPartition ===\n");

// Test 1: Classic true example
// {1,5,11,5} => total = 22, target = 11 => partition exists
test("Test 1: Classic example", [1, 5, 11, 5], true);

// Test 2: Cannot partition (total sum is odd)
test("Test 2: Cannot partition", [1, 2, 3, 5], false);

// Test 3: Single element cannot be partitioned
test("Test 3: Single element", [7], false);

// Test 4: Two equal numbers can be partitioned trivially
test("Test 4: Two equal numbers", [4, 4], true);

// Test 5: Larger mixed set with no valid partition
test("Test 5: Larger mixed set", [2, 2, 3, 5], false);

// Test 6: Multiple valid subsets exist
// Example partition: {2,1} and {2,1}
test("Test 6: Multiple valid subsets", [2, 2, 1, 1], true);
