package code_samples.section7.problems.problem7_5_1;

import java.util.*;

public class problem7_5_1 {

    int[] twoSumIndices(int[] nums, int target) {
        Map<Integer, Integer> mp = new HashMap<>(); // value -> index
        for (int i = 0; i < nums.length; i++) {
            int need = target - nums[i];
            if (mp.containsKey(need)) {
                return new int[]{mp.get(need), i};
            }
            mp.put(nums[i], i);
        }
        return new int[0]; // no solution
    }

    // ===============================
    // Test Harness
    // ===============================
    public static void main(String[] args) {
        problem7_5_1 sol = new problem7_5_1();

        int[][] tests = {
            {2, 7, 11, 15},   // expected [0,1]
            {3, 2, 4},        // expected [1,2]
            {1, 2, 3, 4}      // expected []
        };

        int[] targets = {9, 6, 10};
        int[][] expected = {
            {0, 1},
            {1, 2},
            {}                 // no result
        };

        System.out.println("=== Test: twoSumIndices ===\n");

        for (int t = 0; t < tests.length; t++) {
            int[] nums = tests[t];
            int target = targets[t];
            int[] result = sol.twoSumIndices(nums, target);

            System.out.println("Input nums = " + Arrays.toString(nums) + ", target = " + target);
            System.out.println("Result: " + Arrays.toString(result) +
                               " (expected " + Arrays.toString(expected[t]) + ")\n");
        }
    }
}
