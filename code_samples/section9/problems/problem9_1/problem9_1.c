#include <stdio.h>
#include <stdlib.h>

static int lower_bound_int(int *arr, int len, int x) {
    int lo = 0, hi = len; // [lo, hi)
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < x) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int lengthOfLIS(int *nums, int numsSize) {
    if (numsSize == 0) return 0;
    int *tails = (int *)malloc(numsSize * sizeof(int));
    int len = 0; // current length of tails
    for (int i = 0; i < numsSize; i++) {
        int x = nums[i];
        int pos = lower_bound_int(tails, len, x);
        if (pos == len) {
            tails[len++] = x;
        } else {
            tails[pos] = x;
        }
    }
    free(tails);
    return len;
}

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
    printf("lengthOfLIS = %d (expected 1)\n\n", len5);

    printf("=== Test 6: Random Pattern ===\n");
    int arr6[] = {4, 10, 4, 3, 8, 9};
    int len6 = lengthOfLIS(arr6, 6);
    printf("Input: [4,10,4,3,8,9]\n");
    printf("lengthOfLIS = %d (expected 3)\n\n", len6);

    return 0;
}
