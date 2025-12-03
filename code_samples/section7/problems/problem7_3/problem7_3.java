package code_samples.section7.problems.problem7_3;

import java.util.*;

public class problem7_3 {
    int longestConsecutive(int[] nums) {
        Set<Integer> set = new HashSet<>();
        for (int x : nums) set.add(x);

        int best = 0;
        for (int x : set) {
            if (!set.contains(x - 1)) { // start of sequence
                int cur = x;
                int length = 1;
                while (set.contains(cur + 1)) {
                    cur++;
                    length++;
                }
                best = Math.max(best, length);
            }
        }
        return best;
    }

    // ============================
    // Test harness
    // ============================
    public static void main(String[] args) {
        problem7_3 sol = new problem7_3();

        int[][] tests = {
            {100, 4, 200, 1, 3, 2},
            {0, 3, 7, 2, 5, 8, 4, 6, 0, 1},
            {1, 2, 0, 1},
            {},                 // empty
            {-2, -1, 0, 2, 3, 4, 5}
        };

        int[] expected = {
            4,   // 1,2,3,4
            9,   // 0..8
            3,   // 0,1,2
            0,   // empty
            4    // 2,3,4,5
        };

        System.out.println("=== Test: longestConsecutive ===\n");

        for (int t = 0; t < tests.length; t++) {
            int[] nums = tests[t];

            System.out.print("Input: " + Arrays.toString(nums) + "\n");
            int result = sol.longestConsecutive(nums);
            System.out.println("Longest consecutive length: " + result +
                               " (expected " + expected[t] + ")\n");
        }
    }
}
