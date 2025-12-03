package code_samples.section7.problems.problem7_4;

import java.util.*;

public class problem7_4 {

    int subarraySum(int[] nums, int k) {
        Map<Integer, Integer> freq = new HashMap<>();
        freq.put(0, 1);
        int prefix = 0;
        int count = 0;

        for (int x : nums) {
            prefix += x;
            int need = prefix - k;
            count += freq.getOrDefault(need, 0);
            freq.put(prefix, freq.getOrDefault(prefix, 0) + 1);
        }
        return count;
    }

    // ============================
    // Test harness
    // ============================
    public static void main(String[] args) {
        problem7_4 sol = new problem7_4();

        int[][] tests = {
            {1, 1, 1},                    // expected 2
            {1, 2, 3},                    // expected 2
            {3, 4, 7, 2, -3, 1, 4, 2},    // expected 4
            {},                           // expected 0
        };

        int[] kVals = {2, 3, 7, 0};
        int[] expected = {2, 2, 4, 0};

        System.out.println("=== Test: subarraySum ===\n");

        for (int t = 0; t < tests.length; t++) {
            int[] nums = tests[t];
            int k = kVals[t];

            System.out.println("Input: " + Arrays.toString(nums) + ", k = " + k);
            int result = sol.subarraySum(nums, k);

            System.out.println("Result: " + result +
                    " (expected " + expected[t] + ")\n");
        }
    }
}
