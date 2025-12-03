package code_samples.section7.problems.problem7_2;

import java.util.*;

public class problem7_2 {
    List<List<String> > groupAnagrams(String[] strs) {
        Map<String, List<String>> map = new HashMap<>();
        for (String s : strs) {
            char[] chars = s.toCharArray();
            Arrays.sort(chars);
            String key = new String(chars);
            map.computeIfAbsent(key, k -> new ArrayList<>()).add(s);
        }
        return new ArrayList<>(map.values());
    }

    // ================================
    // Test Code
    // ================================
    public static void main(String[] args) {
        problem7_2 sol = new problem7_2();

        String[] input = {"eat", "tea", "tan", "ate", "nat", "bat"};

        System.out.println("=== Test: groupAnagrams ===\n");
        System.out.println("Input: " + Arrays.toString(input) + "\n");

        List<List<String>> groups = sol.groupAnagrams(input);

        System.out.println("Anagram Groups:");
        for (List<String> group : groups) {
            System.out.println("  " + group);
        }

        System.out.println("\nExpected groups (order may vary):");
        System.out.println("  [eat, tea, ate]");
        System.out.println("  [tan, nat]");
        System.out.println("  [bat]");
    }
}
