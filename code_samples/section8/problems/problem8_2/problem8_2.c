#include <stdio.h>

/*
 * Problem: House Robber
 *
 * Given an array of non-negative integers where each element represents
 * the amount of money in a house, determine the maximum amount of money
 * you can rob without robbing two adjacent houses.
 *
 * This solution uses dynamic programming with O(1) extra space.
 */
int rob(int *nums, int numsSize) {

    /*
     * Edge case: no houses → no money.
     */
    if (numsSize == 0) return 0;

    /*
     * Edge case: exactly one house → must take it.
     */
    if (numsSize == 1) return nums[0];

    /*
     * prev2 represents the maximum money that can be robbed
     * from houses up to index (i - 2).
     * Initially, this is just the money from the first house.
     */
    int prev2 = nums[0];

    /*
     * prev1 represents the maximum money that can be robbed
     * from houses up to index (i - 1).
     * For the second house, we choose the larger of:
     *   - robbing house 0
     *   - robbing house 1
     */
    int prev1 = nums[0] > nums[1] ? nums[0] : nums[1];

    /*
     * Iterate from the third house (index 2) to the last house.
     * At each step, decide whether to:
     *   - take the current house (add its value to prev2)
     *   - skip the current house (keep prev1)
     */
    for (int i = 2; i < numsSize; i++) {

        /*
         * If we rob the current house, we must skip the previous one.
         * So we add nums[i] to prev2.
         */
        int take = prev2 + nums[i];

        /*
         * If we skip the current house, the total remains prev1.
         */
        int skip = prev1;

        /*
         * Choose the better of the two options.
         */
        int cur = take > skip ? take : skip;

        /*
         * Slide the window forward:
         * - prev2 becomes the old prev1
         * - prev1 becomes the newly computed result
         */
        prev2 = prev1;
        prev1 = cur;
    }

    /*
     * prev1 now contains the maximum amount that can be robbed
     * from all houses.
     */
    return prev1;
}

int main(void) {

    printf("=== Test: rob (House Robber) ===\n\n");

    /*
     * Each test case includes:
     * - an array of house values
     * - the number of houses
     * - the expected maximum robbery amount
     */
    struct {
        int nums[10];
        int size;
        int expected;
    } tests[] = {
        {{0}, 0, 0},
        {{5}, 1, 5},
        {{1, 2}, 2, 2},
        {{2, 7, 9, 3, 1}, 5, 12},     // classic example
        {{1, 2, 3, 1}, 4, 4},
        {{2, 1, 1, 2}, 4, 4},
        {{10, 5, 20, 15, 30}, 5, 60},
        {{1, 3, 1, 3, 100}, 5, 103}
    };

    int numTests = sizeof(tests) / sizeof(tests[0]);

    /*
     * Run all test cases and print results.
     */
    for (int i = 0; i < numTests; i++) {
        int result = rob(tests[i].nums, tests[i].size);

        printf("Test %d: rob([", i + 1);
        for (int j = 0; j < tests[i].size; j++) {
            printf("%d", tests[i].nums[j]);
            if (j + 1 < tests[i].size) printf(", ");
        }
        printf("]) = %d (expected %d)\n", result, tests[i].expected);
    }

    return 0;
}
