package code_samples.section7.lesson;

import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Arrays;

/**
 * HashDemo
 *
 * Small standalone demo showing a basic HashMap lookup with a default value.
 *
 * Notes:
 * - HashMap is an unordered key/value store with average O(1) inserts/reads.
 * - getOrDefault(key, defaultValue) is a safe way to read a key that may not exist.
 */
class HashDemo {
    public static void main(String[] args) {
        // Map from word -> frequency (count)
        Map<String, Integer> freq = new HashMap<>();

        // Insert a couple of entries manually
        freq.put("apple", 1);
        freq.put("banana", 2);

        // Read with a fallback default (0) if the key isn't present
        int n = freq.getOrDefault("apple", 0);

        System.out.println("[HashDemo] apple count = " + n);
    }
}

/**
 * section7
 *
 * This class groups three classic hashing-based problems:
 *  1) countWords   - frequency counting using HashMap
 *  2) firstDuplicate - first repeated element detection using HashSet
 *  3) twoSum       - index-pair lookup using HashMap
 *
 * Educational intent:
 * - Emphasizes the "expected O(1)" lookup behavior of hash-based collections.
 * - Includes a simple test runner in main() to demonstrate behavior and outputs.
 *
 * Naming note:
 * - Java style conventions typically use PascalCase for class names (e.g., Section7).
 *   This file keeps your original name `section7` unchanged.
 */
public class section7 {

    /**
     * Counts the frequency of each word in the provided list.
     *
     * @param words List of strings (words/tokens) to count.
     * @return Map from word -> frequency.
     *
     * Complexity:
     * - Time:  O(n) average, where n is the number of words
     * - Space: O(k), where k is the number of distinct words
     *
     * Implementation details:
     * - getOrDefault(w, 0) returns 0 if the word has not been seen yet.
     * - We then store (existingCount + 1) back in the map.
     */
    Map<String, Integer> countWords(List<String> words) {
        // Frequency table: word -> count
        Map<String, Integer> freq = new HashMap<>();

        // Iterate over every word and increment its counter
        for (String w : words) {
            // If absent, treat as 0; otherwise, use current value and add 1
            freq.put(w, freq.getOrDefault(w, 0) + 1);
        }

        return freq;
    }

    /**
     * Finds the first duplicate value encountered when scanning left-to-right.
     *
     * Example:
     * - [2, 5, 1, 2, 3, 5, 1] -> returns 2
     *
     * @param arr Array of ints to scan.
     * @return The first duplicate, or null if none exists.
     *
     * Complexity:
     * - Time:  O(n) average
     * - Space: O(n) worst-case (when all values are unique)
     *
     * Notes:
     * - Returning Integer (nullable) avoids ambiguous sentinel values like 0 or -1.
     * - A more concise variant could be:
     *     if (!seen.add(x)) return x;
     *   because HashSet.add returns false if x was already present.
     */
    Integer firstDuplicate(int[] arr) {
        // Set of values we've already seen
        Set<Integer> seen = new HashSet<>();

        for (int x : arr) {
            // If x has been seen before, it's the first duplicate encountered so far
            if (seen.contains(x)) {
                return x;
            }
            // Otherwise record that we have now seen x
            seen.add(x);
        }

        // No duplicates found
        return null; // no duplicate
    }

    /**
     * Solves the classic Two Sum problem:
     * Find indices (i, j) such that nums[i] + nums[j] == target.
     *
     * @param nums Input array of integers.
     * @param target Desired sum.
     * @return int[2] containing the indices if found; otherwise [-1, -1].
     *
     * Complexity:
     * - Time:  O(n) average
     * - Space: O(n)
     *
     * Approach:
     * - Maintain a HashMap from number value -> index where it was last seen.
     * - For each nums[i], compute need = target - nums[i].
     *   If need exists in the map, we found a prior index j with nums[j] == need.
     *
     * Note about duplicates:
     * - indexOf.put(nums[i], i) overwrites previous indices for the same value.
     *   This is fine for typical Two Sum requirements (return any valid pair).
     */
    int[] twoSum(int[] nums, int target) {
        // Map: value -> index (where we last saw this value)
        Map<Integer, Integer> indexOf = new HashMap<>(); // value -> index

        for (int i = 0; i < nums.length; i++) {
            int need = target - nums[i];

            // Check if we've previously seen the needed value
            if (indexOf.containsKey(need)) {
                // Return the indices: previous index for need, and current index i
                return new int[] { indexOf.get(need), i };
            }

            // Record the current value with its index for future lookups
            indexOf.put(nums[i], i);
        }

        // No pair found
        return new int[] { -1, -1 };
    }

    // ===== Helpers for testing =====

    /**
     * Prints a subset of keys from a map, showing "(not found)" when missing.
     *
     * @param label Heading printed before the key/value lines.
     * @param map The map to inspect.
     * @param keys Varargs list of keys to attempt to print.
     *
     * Note:
     * - map.get(k) returns null if the key is missing.
     * - This works here because our values are Integer; however, note that a map could
     *   *also* store a key with a null value. If you need to distinguish "missing key"
     *   vs "present but null", use containsKey(k).
     */
    private static void printMap(String label, Map<String, Integer> map, String... keys) {
        System.out.println(label);

        for (String k : keys) {
            // get() returns the value, or null if the key doesn't exist
            Integer v = map.get(k);

            if (v != null) {
                System.out.println("  '" + k + "' -> " + v);
            } else {
                System.out.println("  '" + k + "' -> (not found)");
            }
        }
    }

    /**
     * Prints an integer array with a label using Arrays.toString().
     *
     * @param label Prefix to display before the array.
     * @param arr The array to print.
     */
    private static void printArray(String label, int[] arr) {
        System.out.println(label + Arrays.toString(arr));
    }

    // ===== Test runner =====

    /**
     * Main test harness.
     *
     * Runs three "tests" by printing:
     *  1) Frequency counts from countWords
     *  2) First duplicate from firstDuplicate (including a no-duplicate case)
     *  3) Two-sum indices from twoSum (including a no-solution case)
     *
     * This is not a formal unit test framework (like JUnit), but a simple
     * console-driven sanity check.
     */
    public static void main(String[] args) {
        // Create an instance to call the instance methods (countWords/firstDuplicate/twoSum)
        section7 s = new section7();

        // --- Test 1: countWords ---
        System.out.println("=== Test: countWords (word frequency) ===");

        // Build a sample list of words; expected counts:
        // apple=3, banana=2, orange=1
        List<String> words = Arrays.asList(
            "apple", "banana", "apple", "orange", "banana", "apple"
        );

        // Call the function under test
        Map<String, Integer> freq = s.countWords(words);

        // Print selected keys, including a missing one ("pear")
        printMap("Frequencies:", freq, "apple", "banana", "orange", "pear");
        System.out.println("Expected: apple=3, banana=2, orange=1, pear not found\n");

        // --- Test 2: firstDuplicate ---
        System.out.println("=== Test: firstDuplicate ===");

        // Case 1: duplicates exist; first duplicate encountered is 2
        int[] arr1 = {2, 5, 1, 2, 3, 5, 1};
        printArray("Array 1: ", arr1);

        Integer dup1 = s.firstDuplicate(arr1);
        System.out.println("First duplicate: " + dup1 + " (expected 2)\n");

        // Case 2: no duplicates; should return null
        int[] arr2 = {1, 2, 3, 4, 5};
        printArray("Array 2: ", arr2);

        Integer dup2 = s.firstDuplicate(arr2);
        System.out.println("First duplicate: " + dup2 + " (expected null = no duplicate)\n");

        // --- Test 3: twoSum ---
        System.out.println("=== Test: twoSum (hash map) ===");

        // Positive case: 2 + 7 = 9
        int[] nums = {2, 7, 11, 15};
        int target = 9;

        printArray("Array: ", nums);
        System.out.println("Target: " + target);

        int[] pair1 = s.twoSum(nums, target);
        System.out.println("twoSum result indices: " + Arrays.toString(pair1));

        if (pair1[0] != -1) {
            // Use returned indices to show the actual values used
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
