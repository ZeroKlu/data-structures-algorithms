package code_samples.section8.problems.problem8_1;

public class problem8_1 {

    int climbStairs(int n) {
        if (n <= 2) return n;
        int prev2 = 1;
        int prev1 = 2;
        for (int i = 3; i <= n; i++) {
            int cur = prev1 + prev2;
            prev2 = prev1;
            prev1 = cur;
        }
        return prev1;
    }

    // ============================
    // Tests
    // ============================
    public static void main(String[] args) {

        problem8_1 solver = new problem8_1();

        int[] tests    = { 0, 1, 2, 3, 4, 5, 6, 10, 20 };
        int[] expected = { 0, 1, 2, 3, 5, 8, 13, 89, 10946 };

        System.out.println("=== Test: climbStairs ===\n");

        for (int i = 0; i < tests.length; i++) {
            int n = tests[i];
            int result = solver.climbStairs(n);

            System.out.println(
                "climbStairs(" + n + ") = " + result +
                " (expected " + expected[i] + ")"
            );
        }
    }
}
