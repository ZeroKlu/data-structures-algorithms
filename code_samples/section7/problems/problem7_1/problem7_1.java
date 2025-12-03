package code_samples.section7.problems.problem7_1;

public class problem7_1 {
    int firstUniqChar(String s) {
        int[] freq = new int[256];
        for (int i = 0; i < s.length(); i++) {
            freq[s.charAt(i)]++;
        }
        for (int i = 0; i < s.length(); i++) {
            if (freq[s.charAt(i)] == 1) {
                return i;
            }
        }
        return -1;
    }

    // =====================
    // Test code (main)
    // =====================
    public static void main(String[] args) {
        problem7_1 sol = new problem7_1();

        String[] tests = {
            "leetcode",      // expected 0
            "loveleetcode",  // expected 2
            "aabbcc",        // expected -1
            "",              // expected -1
            "x",             // expected 0
            "aabbc",         // expected 4
            "swiss"          // expected 1 ('w')
        };

        int[] expected = {
            0,   // "leetcode"
            2,   // "loveleetcode"
            -1,  // "aabbcc"
            -1,  // ""
            0,   // "x"
            4,   // "aabbc"
            1    // "swiss"
        };

        System.out.println("=== Testing firstUniqChar ===\n");

        for (int i = 0; i < tests.length; i++) {
            int result = sol.firstUniqChar(tests[i]);
            System.out.println("Input: \"" + tests[i] + "\"");
            System.out.println("firstUniqChar -> " + result
                + " (expected " + expected[i] + ")\n");
        }
    }
}
