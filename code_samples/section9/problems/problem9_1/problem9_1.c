#include <stdio.h>
#include <stdlib.h>

/**
 * lower_bound_int
 * ---------------
 * Finds the first index in a sorted array `arr` (of current length `len`)
 * where the value is >= x.
 *
 * This is the classic "lower_bound" binary search on a half-open interval [lo, hi):
 *   - lo is inclusive
 *   - hi is exclusive
 *
 * Why this helper exists in LIS:
 * In the O(n log n) LIS algorithm, we maintain an array `tails` that is kept
 * sorted. For each new value x, we binary search for the position where x
 * should go to either extend the current best LIS length (append) or improve
 * a tail value (replace).
 *
 * Parameters:
 *   arr - pointer to the start of the array (assumed sorted in ascending order
 *         for indices [0, len))
 *   len - number of valid elements in arr
 *   x   - target value to locate insertion position for
 *
 * Returns:
 *   The smallest index i in [0, len] such that arr[i] >= x.
 *   If all values in arr are < x, returns len (meaning "insert at end").
 */
static int lower_bound_int(int *arr, int len, int x) {
    int lo = 0, hi = len; // search interval is [lo, hi)
    while (lo < hi) {
        // mid is chosen this way to avoid overflow from (lo + hi) in other languages;
        // in C it's still a good habit (and reads clearly).
        int mid = lo + (hi - lo) / 2;

        // If arr[mid] is strictly less than x, discard the left half including mid.
        // We need the first index where arr[i] >= x, so anything < x can't be the answer.
        if (arr[mid] < x) {
            lo = mid + 1;
        } else {
            // arr[mid] >= x means mid could be the answer; keep searching left.
            hi = mid;
        }
    }
    // lo == hi is the insertion position.
    return lo;
}

/**
 * lengthOfLIS
 * -----------
 * Computes the length of the Longest Increasing Subsequence (LIS) in `nums`.
 *
 * This implementation uses the classic "patience sorting" / tails method:
 *
 * Key invariant:
 *   tails[i] = the smallest possible tail value of an increasing subsequence
 *              of length (i + 1) found so far.
 *
 * Properties:
 *   - tails is kept sorted in ascending order across [0, len).
 *   - len is the number of active tails entries and equals the length of the
 *     longest subsequence found so far.
 *
 * For each element x in nums:
 *   - Find the first position pos where tails[pos] >= x (lower_bound).
 *   - If pos == len: x extends the longest subsequence, so append it.
 *   - Else: replace tails[pos] with x to improve (lower) the tail value for
 *           subsequences of that length, enabling better future extensions.
 *
 * Important nuance:
 *   This algorithm returns only the LENGTH of the LIS, not the subsequence itself.
 *
 * Complexity:
 *   - Time:  O(n log n) due to binary search per element
 *   - Space: O(n) for the tails array
 *
 * Parameters:
 *   nums     - input array
 *   numsSize - number of elements in nums
 *
 * Returns:
 *   Length of the longest strictly increasing subsequence.
 */
int lengthOfLIS(int *nums, int numsSize) {
    // Edge case: empty input has LIS length 0.
    if (numsSize == 0) return 0;

    // tails will store candidate tail values; at most numsSize long.
    // NOTE: In production code you might check malloc result for NULL.
    int *tails = (int *)malloc(numsSize * sizeof(int));

    // len = number of valid entries in tails (also current best LIS length).
    int len = 0;

    // Process each number and update tails accordingly.
    for (int i = 0; i < numsSize; i++) {
        int x = nums[i];

        // Find position where x should be placed to maintain sorted tails.
        int pos = lower_bound_int(tails, len, x);

        // If x is larger than all existing tails, it extends the best subsequence.
        if (pos == len) {
            tails[len++] = x;
        } else {
            // Otherwise, replace an existing tail with x to lower that tail value.
            // This does not change len, but can enable longer subsequences later.
            tails[pos] = x;
        }
    }

    // Clean up heap allocation now that we're done.
    free(tails);

    // len holds the LIS length.
    return len;
}

/**
 * main
 * ----
 * Basic test harness for lengthOfLIS().
 *
 * Each test prints:
 *  - a label
 *  - the input
 *  - the computed LIS length
 *  - the expected LIS length
 *
 * Note:
 * These tests validate correctness for several common edge patterns:
 *  - strictly increasing (LIS = n)
 *  - typical mixed array (classic LIS example)
 *  - all equal values (LIS = 1 because "increasing" is strict)
 *  - empty array (LIS = 0)
 *  - strictly decreasing (LIS = 1)
 *  - random pattern with repeats and inversions
 */
int main(void) {
    printf("=== Test 1: Basic Increasing Sequence ===\n");
    int arr1[] = {1, 2, 3, 4, 5};
    int len1 = lengthOfLIS(arr1, 5);
    printf("Input: [1,2,3,4,5]\n");
    printf("lengthOfLIS = %d (expected 5)\n\n", len1);

    printf("=== Test 2: Mixed Sequence ===\n");
    int arr2[] = {10, 9, 2, 5, 3, 7, 101, 18};
    int len2 = lengthOfLIS(arr2, 8);
    printf("Input: [10,9,2,5,3,7,101,18]\n");
    printf("lengthOfLIS = %d (expected 4)\n\n", len2);

    printf("=== Test 3: All Identical ===\n");
    int arr3[] = {7, 7, 7, 7, 7};
    int len3 = lengthOfLIS(arr3, 5);
    printf("Input: [7,7,7,7,7]\n");
    // Because the subsequence must be strictly increasing, duplicates cannot extend it.
    printf("lengthOfLIS = %d (expected 1)\n\n", len3);

    printf("=== Test 4: Empty Array ===\n");
    int *arr4 = NULL;
    int len4 = lengthOfLIS(arr4, 0);
    printf("Input: []\n");
    printf("lengthOfLIS = %d (expected 0)\n\n", len4);

    printf("=== Test 5: Decreasing Sequence ===\n");
    int arr5[] = {9, 7, 5, 3, 1};
    int len5 = lengthOfLIS(arr5, 5);
    printf("Input: [9,7,5,3,1]\n");
    // In a strictly decreasing array, the best increasing subsequence is any single element.
    printf("lengthOfLIS = %d (expected 1)\n\n", len5);

    printf("=== Test 6: Random Pattern ===\n");
    int arr6[] = {4, 10, 4, 3, 8, 9};
    int len6 = lengthOfLIS(arr6, 6);
    printf("Input: [4,10,4,3,8,9]\n");
    printf("lengthOfLIS = %d (expected 3)\n\n", len6);

    return 0;
}
