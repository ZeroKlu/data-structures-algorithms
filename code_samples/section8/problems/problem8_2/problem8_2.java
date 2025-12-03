package code_samples.section8.problems.problem8_2;

public class problem8_2 {

    int rob(int[] nums) {
        if (nums.length == 0) return 0;
        if (nums.length == 1) return nums[0];

        int prev2 = nums[0];
        int prev1 = Math.max(nums[0], nums[1]);

        for (int i = 2; i < nums.length; i++) {
            int take = prev2 + nums[i];
            int skip  = prev1;
            int cur = Math.max(take, skip);

            prev2 = prev1;
            prev1 = cur;
        }

        return prev1;
    }

    // ============================
    // Tests
    // ============================
    public static void main(String[] args) {

        problem8_2 solver = new problem8_2();

        int[][] tests = {
            {},                 // 0
            {5},                // 5
            {1, 2},             // 2
            {2, 7, 9, 3, 1},    // 12
            {1, 2, 3, 1},       // 4
            {2, 1, 1, 2},       // 4
            {10, 5, 20, 15, 30},// 60
            {1, 3, 1, 3, 100}   // 103
        };

        int[] expected = {
            0,
            5,
            2,
            12,
            4,
            4,
            60,
            103
        };

        System.out.println("=== Test: rob (House Robber) ===\n");

        for (int i = 0; i < tests.length; i++) {
            int result = solver.rob(tests[i]);
            System.out.print("Test " + (i + 1) + ": rob([");

            for (int j = 0; j < tests[i].length; j++) {
                System.out.print(tests[i][j]);
                if (j + 1 < tests[i].length) System.out.print(", ");
            }

            System.out.println("]) = " + result +
                               " (expected " + expected[i] + ")");
        }
    }
}
