#include <stdio.h>

int rob(int *nums, int numsSize) {
    if (numsSize == 0) return 0;
    if (numsSize == 1) return nums[0];
    int prev2 = nums[0];
    int prev1 = nums[0] > nums[1] ? nums[0] : nums[1];
    for (int i = 2; i < numsSize; i++) {
        int take = prev2 + nums[i];
        int skip = prev1;
        int cur = take > skip ? take : skip;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

int main(void) {

    printf("=== Test: rob (House Robber) ===\n\n");

    struct {
        int nums[10];
        int size;
        int expected;
    } tests[] = {
        {{}, 0, 0},
        {{5}, 1, 5},
        {{1, 2}, 2, 2},
        {{2, 7, 9, 3, 1}, 5, 12},     // classic example
        {{1, 2, 3, 1}, 4, 4},
        {{2, 1, 1, 2}, 4, 4},
        {{10, 5, 20, 15, 30}, 5, 60},
        {{1, 3, 1, 3, 100}, 5, 103}
    };

    int numTests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < numTests; i++) {
        int result = rob(tests[i].nums, tests[i].size);

        printf("Test %d: rob([", i+1);
        for (int j = 0; j < tests[i].size; j++) {
            printf("%d", tests[i].nums[j]);
            if (j + 1 < tests[i].size) printf(", ");
        }
        printf("]) = %d (expected %d)\n", result, tests[i].expected);
    }

    return 0;
}
