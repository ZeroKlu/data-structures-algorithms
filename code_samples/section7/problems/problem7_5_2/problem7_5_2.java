package code_samples.section7.problems.problem7_5_2;

import java.util.*;

public class problem7_5_2 {

    int countUniquePairs(int[] nums, int target) {
        Set<Integer> seen = new HashSet<>();
        Set<String> pairs = new HashSet<>();

        for (int x : nums) {
            int y = target - x;
            if (seen.contains(y)) {
                int a = Math.min(x, y);
                int b = Math.max(x, y);
                String key = a + "," + b;
                pairs.add(key);
            }
            seen.add(x);
        }
        return pairs.size();
    }

    // ============================
    // Test Harness
    // ============================
    public static void main(String[] args) {
        problem7_5_2 solver = new problem7_5_2();

        int[][] testArrays = {
            {1, 1, 2, 2, 3, 3},
            {2, 7, 11, 15},
            {3, 3, 3},
            {1, 2, 3, 4},
            {1, 5, 1, 5}
        };

        int[] targets   = {4, 9, 6, 10, 6};
        int[] expected  = {2, 1, 1, 0, 1};
        String[] notes = {
            "{1,3} and {2,2}",
            "{2,7}",
            "{3,3}",
            "no pairs",
            "{1,5}"
        };

        System.out.println("=== Test: countUniquePairs ===\n");

        for (int i = 0; i < testArrays.length; i++) {
            int[] nums = testArrays[i];
            int target = targets[i];

            int result = solver.countUniquePairs(nums, target);

            System.out.println("Input nums = " + Arrays.toString(nums)
                + ", target = " + target);
            System.out.println("Result:   " + result);
            System.out.println("Expected: " + expected[i]
                + " (" + notes[i] + ")\n");
        }
    }
}
