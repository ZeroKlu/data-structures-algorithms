package code_samples.section9.problems.problem9_1;

public class problem9_1 {

    int lengthOfLIS(int[] nums) {
        int[] tails = new int[nums.length];
        int len = 0;
        for (int x : nums) {
            int lo = 0, hi = len;
            while (lo < hi) {
                int mid = (lo + hi) / 2;
                if (tails[mid] < x) {
                    lo = mid + 1;
                } else {
                    hi = mid;
                }
            }
            tails[lo] = x;
            if (lo == len) {
                len++;
            }
        }
        return len;
    }

    // ===========================
    // Test Harness (main)
    // ===========================
    public static void main(String[] args) {
        problem9_1 solver = new problem9_1();

        System.out.println("=== Test 1: Basic Increasing Sequence ===");
        int[] arr1 = {1, 2, 3, 4, 5};
        System.out.println("Input: [1,2,3,4,5]");
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr1) + " (expected 5)\n");

        System.out.println("=== Test 2: Mixed Sequence ===");
        int[] arr2 = {10, 9, 2, 5, 3, 7, 101, 18};
        System.out.println("Input: [10,9,2,5,3,7,101,18]");
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr2) + " (expected 4)\n");

        System.out.println("=== Test 3: All Identical ===");
        int[] arr3 = {7, 7, 7, 7, 7};
        System.out.println("Input: [7,7,7,7,7]");
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr3) + " (expected 1)\n");

        System.out.println("=== Test 4: Empty Array ===");
        int[] arr4 = {};
        System.out.println("Input: []");
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr4) + " (expected 0)\n");

        System.out.println("=== Test 5: Decreasing Sequence ===");
        int[] arr5 = {9, 7, 5, 3, 1};
        System.out.println("Input: [9,7,5,3,1]");
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr5) + " (expected 1)\n");

        System.out.println("=== Test 6: Random Pattern ===");
        int[] arr6 = {4, 10, 4, 3, 8, 9};
        System.out.println("Input: [4,10,4,3,8,9]");
        System.out.println("lengthOfLIS = " + solver.lengthOfLIS(arr6) + " (expected 3)\n");
    }
}
