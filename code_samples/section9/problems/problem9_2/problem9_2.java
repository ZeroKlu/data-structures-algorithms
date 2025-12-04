package code_samples.section9.problems.problem9_2;

public class problem9_2 {

    int knapSack(int W, int[] weight, int[] value) {
        int[] dp = new int[W + 1];
        for (int i = 0; i < weight.length; i++) {
            int wt = weight[i];
            int val = value[i];
            for (int w = W; w >= wt; w--) {
                dp[w] = Math.max(dp[w], dp[w - wt] + val);
            }
        }
        return dp[W];
    }

    // ===========================
    // Test Harness
    // ===========================
    public static void main(String[] args) {
        problem9_2 solver = new problem9_2();

        runTest("Test 1: Classic example",
                50,
                new int[]{10, 20, 30},
                new int[]{60, 100, 120},
                220,
                solver);

        runTest("Test 2: Small weights",
                6,
                new int[]{1, 2, 3},
                new int[]{10, 15, 40},
                65,
                solver);

        runTest("Test 3: Too small capacity",
                4,
                new int[]{5, 6, 7},
                new int[]{10, 20, 30},
                0,
                solver);

        runTest("Test 4: Zero capacity",
                0,
                new int[]{2, 3, 4},
                new int[]{20, 30, 40},
                0,
                solver);

        runTest("Test 5: Larger mixed set",
                10,
                new int[]{2, 3, 5, 7},
                new int[]{20, 30, 50, 70},
                100,
                solver);
    }

    private static void runTest(String name, int W, int[] weight, int[] value,
                                int expected, problem9_2 solver) {
        int result = solver.knapSack(W, weight, value);

        System.out.println(name);
        System.out.println("Capacity = " + W);

        System.out.print("Weights: {");
        for (int i = 0; i < weight.length; i++)
            System.out.print(weight[i] + (i + 1 == weight.length ? "" : ","));
        System.out.println("}");

        System.out.print("Values:  {");
        for (int i = 0; i < value.length; i++)
            System.out.print(value[i] + (i + 1 == value.length ? "" : ","));
        System.out.println("}");

        System.out.println("knapSack = " + result + " (expected " + expected + ")\n");
    }
}
