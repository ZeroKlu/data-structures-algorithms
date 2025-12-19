package code_samples.section7.problems.problem7_2;

import java.util.*;

/**
 * problem7_2
 *
 * Solves the classic "Group Anagrams" problem.
 *
 * Given an array of strings, group the strings that are anagrams of each other.
 * Two strings are anagrams if they contain the same characters with the same
 * frequencies, possibly in a different order.
 *
 * This implementation uses:
 *  - HashMap for grouping by a canonical key
 *  - Sorted-character strings as the anagram key
 */
public class problem7_2 {

    /**
     * groupAnagrams
     *
     * @param strs Array of input strings to be grouped.
     * @return A list of anagram groups, where each group is a list of strings.
     *
     * Approach:
     *   - For each string:
     *       1) Convert it to a char array.
     *       2) Sort the characters.
     *       3) Convert the sorted characters back into a string.
     *          This sorted string becomes the canonical key for all its anagrams.
     *       4) Insert the original string into a map keyed by this canonical form.
     *
     * Example:
     *   "eat" -> ['e','a','t'] -> ['a','e','t'] -> "aet"
     *   "tea" -> "aet"  (same key → same group)
     *
     * Complexity:
     *   Let:
     *     n = number of strings
     *     k = average length of a string
     *
     *   - Time:  O(n · k log k)
     *       Sorting each string dominates the cost.
     *   - Space: O(n · k)
     *       For storing keys and grouped strings.
     *
     * Notes:
     *   - The order of groups and the order of strings within each group
     *     are not guaranteed, because HashMap does not preserve insertion order.
     */
    List<List<String>> groupAnagrams(String[] strs) {
        // Map: sorted string (anagram key) -> list of original strings
        Map<String, List<String>> map = new HashMap<>();

        // Process each input string
        for (String s : strs) {
            // Convert the string into a mutable char array
            char[] chars = s.toCharArray();

            // Sort characters alphabetically to form the canonical key
            Arrays.sort(chars);

            // Convert sorted characters back into a string key
            String key = new String(chars);

            // Fetch the existing group for this key, or create it if absent,
            // then add the original string to that group.
            map.computeIfAbsent(key, k -> new ArrayList<>()).add(s);
        }

        // Return all grouped anagram lists as a List<List<String>>
        return new ArrayList<>(map.values());
    }

    // ================================
    // Test Code
    // ================================

    /**
     * main
     *
     * Simple console-based test harness to demonstrate groupAnagrams().
     * This is not a formal unit test (e.g., JUnit), but it clearly shows
     * expected behavior and output.
     */
    public static void main(String[] args) {
        problem7_2 sol = new problem7_2();

        // Input words to group
        String[] input = {"eat", "tea", "tan", "ate", "nat", "bat"};

        System.out.println("=== Test: groupAnagrams ===\n");
        System.out.println("Input: " + Arrays.toString(input) + "\n");

        // Run the solution
        List<List<String>> groups = sol.groupAnagrams(input);

        // Print the resulting groups
        System.out.println("Anagram Groups:");
        for (List<String> group : groups) {
            System.out.println("  " + group);
        }

        // Expected output guidance (ordering may differ)
        System.out.println("\nExpected groups (order may vary):");
        System.out.println("  [eat, tea, ate]");
        System.out.println("  [tan, nat]");
        System.out.println("  [bat]");
    }
}
