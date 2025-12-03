package code_samples.section7.lesson;

import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Arrays;

class HashDemo {
    public static void main(String[] args) {
        Map<String, Integer> freq = new HashMap<>();
        freq.put("apple", 1);
        freq.put("banana", 2);
        int n = freq.getOrDefault("apple", 0);
        System.out.println("[HashDemo] apple count = " + n);
    }
}

public class section7 {
    Map<String, Integer> countWords(List<String> words) {
        Map<String, Integer> freq = new HashMap<>();
        for (String w : words) {
            freq.put(w, freq.getOrDefault(w, 0) + 1);
        }
        return freq;
    }

    Integer firstDuplicate(int[] arr) {
        Set<Integer> seen = new HashSet<>();
        for (int x : arr) {
            if (seen.contains(x)) {
                return x;
            }
            seen.add(x);
        }
        return null; // no duplicate
    }

    int[] twoSum(int[] nums, int target) {
        Map<Integer, Integer> indexOf = new HashMap<>(); // value -> index
        for (int i = 0; i < nums.length; i++) {
            int need = target - nums[i];
            if (indexOf.containsKey(need)) {
                return new int[] { indexOf.get(need), i };
            }
            indexOf.put(nums[i], i);
        }
        return new int[] { -1, -1 };
    }

    // ===== Helpers for testing =====

    private static void printMap(String label, Map<String, Integer> map, String... keys) {
        System.out.println(label);
        for (String k : keys) {
            Integer v = map.get(k);
            if (v != null) {
                System.out.println("  '" + k + "' -> " + v);
            } else {
                System.out.println("  '" + k + "' -> (not found)");
            }
        }
    }

    private static void printArray(String label, int[] arr) {
        System.out.println(label + Arrays.toString(arr));
    }

    // ===== Test runner =====

    public static void main(String[] args) {
        section7 s = new section7();

        // --- Test 1: countWords ---
        System.out.println("=== Test: countWords (word frequency) ===");
        List<String> words = Arrays.asList(
            "apple", "banana", "apple", "orange", "banana", "apple"
        );
        Map<String, Integer> freq = s.countWords(words);
        printMap("Frequencies:", freq, "apple", "banana", "orange", "pear");
        System.out.println("Expected: apple=3, banana=2, orange=1, pear not found\n");

        // --- Test 2: firstDuplicate ---
        System.out.println("=== Test: firstDuplicate ===");
        int[] arr1 = {2, 5, 1, 2, 3, 5, 1};
        printArray("Array 1: ", arr1);
        Integer dup1 = s.firstDuplicate(arr1);
        System.out.println("First duplicate: " + dup1 + " (expected 2)\n");

        int[] arr2 = {1, 2, 3, 4, 5};
        printArray("Array 2: ", arr2);
        Integer dup2 = s.firstDuplicate(arr2);
        System.out.println("First duplicate: " + dup2 + " (expected null = no duplicate)\n");

        // --- Test 3: twoSum ---
        System.out.println("=== Test: twoSum (hash map) ===");
        int[] nums = {2, 7, 11, 15};
        int target = 9;
        printArray("Array: ", nums);
        System.out.println("Target: " + target);
        int[] pair1 = s.twoSum(nums, target);
        System.out.println("twoSum result indices: " + Arrays.toString(pair1));
        if (pair1[0] != -1) {
            System.out.println("Values: " + nums[pair1[0]] + ", " + nums[pair1[1]]);
            System.out.println("Expected: indices [0,1] or [1,0] with values 2 and 7\n");
        } else {
            System.out.println("No pair found (unexpected for this test)\n");
        }

        // Negative test: no pair
        System.out.println("--- Negative case (no pair) ---");
        int[] nums2 = {1, 2, 3};
        int target2 = 100;
        printArray("Array: ", nums2);
        System.out.println("Target: " + target2);
        int[] pair2 = s.twoSum(nums2, target2);
        System.out.println("twoSum result indices: " + Arrays.toString(pair2));
        System.out.println("Expected: [-1, -1]\n");
    }
}
