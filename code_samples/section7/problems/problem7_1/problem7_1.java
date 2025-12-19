package code_samples.section7.problems.problem7_1;

/**
 * problem7_1
 *
 * Solves the classic "First Unique Character in a String" problem.
 *
 * Given a string, return the index of the first character that appears
 * exactly once. If no such character exists, return -1.
 *
 * This implementation uses a fixed-size frequency array for performance
 * and simplicity when working with ASCII / extended single-byte characters.
 */
public class problem7_1 {

    /**
     * firstUniqChar
     *
     * @param s Input string to examine.
     * @return
     *   - The 0-based index of the first unique character
     *   - -1 if no unique character exists or the string is empty
     *
     * Approach:
     *   Two-pass algorithm:
     *     1) Count character frequencies.
     *     2) Scan again to find the first character with frequency == 1.
     *
     * Complexity:
     *   - Time:  O(n), where n = s.length()
     *   - Space: O(1), fixed array of size 256
     *
     * Important notes:
     *   - Characters are treated as raw char values (0-255).
     *   - This is suitable for ASCII or extended single-byte encodings.
     *   - Java char is UTF-16; characters outside the BMP or general Unicode
     *     code points above 255 may not behave as intended with this approach.
     */
    int firstUniqChar(String s) {
        // Frequency table for all possible byte values
        int[] freq = new int[256];

        // First pass: count how many times each character appears
        for (int i = 0; i < s.length(); i++) {
            // s.charAt(i) returns a char (UTF-16 code unit).
            // Using it directly as an index assumes values <= 255.
            freq[s.charAt(i)]++;
        }

        // Second pass: find the first index whose character appears exactly once
        for (int i = 0; i < s.length(); i++) {
            if (freq[s.charAt(i)] == 1) {
                return i;
            }
        }

        // No unique character found
        return -1;
    }

    // =====================
    // Test code (main)
    // =====================

    /**
     * Simple test harness that exercises firstUniqChar()
     * with several input strings and prints the results.
     *
     * This is not a formal unit test framework, but a
     * console-based sanity check.
     */
    public static void main(String[] args) {
        problem7_1 sol = new problem7_1();

        // Test cases with expected results documented inline
        String[] tests = {
            "leetcode",      // expected 0 ('l')
            "loveleetcode",  // expected 2 ('v')
            "aabbcc",        // expected -1
            "",              // expected -1 (empty string)
            "x",             // expected 0
            "aabbc",         // expected 4 ('c')
            "swiss"          // expected 1 ('w')
        };

        // Expected outputs aligned with the tests array
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

        // Run all test cases
        for (int i = 0; i < tests.length; i++) {
            int result = sol.firstUniqChar(tests[i]);

            System.out.println("Input: \"" + tests[i] + "\"");
            System.out.println(
                "firstUniqChar -> " + result +
                " (expected " + expected[i] + ")\n"
            );
        }
    }
}
