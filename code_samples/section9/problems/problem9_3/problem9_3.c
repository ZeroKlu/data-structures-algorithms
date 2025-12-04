#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool canPartition(int *nums, int numsSize) {
    int total = 0;
    for (int i = 0; i < numsSize; i++) {
        total += nums[i];
    }
    if (total % 2 != 0) return false;
    int target = total / 2;

    bool *dp = (bool *)calloc(target + 1, sizeof(bool));
    dp[0] = true;
    for (int i = 0; i < numsSize; i++) {
        int x = nums[i];
        for (int s = target; s >= x; s--) {
            if (dp[s - x]) dp[s] = true;
        }
    }
    bool ans = dp[target];
    free(dp);
    return ans;
}

void test(const char *name, int *arr, int n, bool expected) {
    bool result = canPartition(arr, n);

    printf("%s\n", name);
    printf("Input: {");
    for (int i = 0; i < n; i++) {
        printf("%d%s", arr[i], i + 1 == n ? "" : ",");
    }
    printf("}\n");

    printf("canPartition = %s (expected %s)\n\n",
           result ? "true" : "false",
           expected ? "true" : "false");
}

int main(void) {
    printf("=== Testing canPartition ===\n\n");

    // Test 1: Classic example (sum=11, cannot split into equal halves)
    int arr1[] = {1, 5, 11, 5};
    test("Test 1: Classic true example", arr1, 4, true);

    // Test 2: Cannot partition
    int arr2[] = {1, 2, 3, 5};
    test("Test 2: Cannot partition", arr2, 4, false);

    // Test 3: Single element cannot partition
    int arr3[] = {7};
    test("Test 3: Single element", arr3, 1, false);

    // Test 4: Two equal elements can partition
    int arr4[] = {4, 4};
    test("Test 4: Two equal numbers", arr4, 2, true);

    // Test 5: Larger set
    int arr5[] = {2, 2, 3, 5};
    test("Test 5: Larger mixed set", arr5, 4, false);

    // Test 6: Multiple ways to partition
    int arr6[] = {2, 2, 1, 1};
    test("Test 6: Multiple partitions", arr6, 4, true);

    return 0;
}
