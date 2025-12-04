package code_samples.section9.problems.problem9_3;

public class problem9_3 {

    boolean canPartition(int[] nums) {
        int total = 0;
        for (int x : nums) total += x;
        if ((total & 1) == 1) return false;
        int target = total / 2;

        boolean[] dp = new boolean[target + 1];
        dp[0] = true;

        for (int x : nums) {
            for (int s = target; s >= x; s--) {
                if (dp[s - x]) dp[s] = true;
            }
        }
        return dp[target];
    }

    // ===========================
    // Test Harness
    // ===========================
    public static void main(String[] args) {
        problem9_3 solver = new problem9_3();

        runTest("Test 1: Classic example", new int[]{1, 5, 11, 5}, true, solver);
        runTest("Test 2: Cannot partition", new int[]{1, 2, 3, 5}, false, solver);
        runTest("Test 3: Single element", new int[]{7}, false, solver);
        runTest("Test 4: Two equal numbers", new int[]{4, 4}, true, solver);
        runTest("Test 5: Larger mixed set", new int[]{2, 2, 3, 5}, false, solver);
        runTest("Test 6: Multiple valid subsets", new int[]{2, 2, 1, 1}, true, solver);
    }

    private static void runTest(String name, int[] arr, boolean expected, problem9_3 solver) {
        boolean result = solver.canPartition(arr);

        System.out.println(name);
        System.out.print("Input: {");
        for (int i = 0; i < arr.length; i++) {
            System.out.print(arr[i]);
            if (i + 1 < arr.length) System.out.print(",");
        }
        System.out.println("}");

        System.out.println("canPartition = " + result + " (expected " + expected + ")\n");
    }
}
