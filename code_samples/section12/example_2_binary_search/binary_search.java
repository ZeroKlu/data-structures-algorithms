package code_samples.section12.example_2_binary_search;

import java.io.File;                // Represents file and directory pathnames
import java.io.FileNotFoundException; // Thrown when a file cannot be found
import java.util.ArrayList;          // Resizable list implementation
import java.util.List;               // Interface for ordered collections
import java.util.Scanner;            // Reads formatted input from files

/**
 * Demonstrates binary search on a sorted dataset,
 * including a version that counts search steps.
 */
public class binary_search {

    // ----------------------------------------------------
    // Given binary search
    // ----------------------------------------------------

    /**
     * Performs a standard binary search on a sorted integer array.
     *
     * @param arr     Sorted array to search
     * @param target  Value to locate
     *
     * @return Index of the target if found; otherwise -1
     */
    int binarySearch(int[] arr, int target) {
        int lo = 0;                 // Lower bound of the search range (inclusive)
        int hi = arr.length - 1;    // Upper bound of the search range (inclusive)

        // Continue searching while the range is valid
        while (lo <= hi) {
            // Compute midpoint safely to avoid integer overflow
            int mid = lo + (hi - lo) / 2;

            // Check if midpoint element matches the target
            if (arr[mid] == target) {
                return mid;         // Target found
            }
            // Target is larger; discard left half
            else if (arr[mid] < target) {
                lo = mid + 1;
            }
            // Target is smaller; discard right half
            else {
                hi = mid - 1;
            }
        }

        // Target was not found in the array
        return -1;
    }

    // Same algorithm, but counts how many iterations (steps)

    /**
     * Performs a binary search while counting the number of iterations.
     *
     * @param arr       Sorted array to search
     * @param target    Value to locate
     * @param stepsOut  Single-element array used to return step count
     *
     * @return Index of the target if found; otherwise -1
     */
    int binarySearchSteps(int[] arr, int target, int[] stepsOut) {
        int lo = 0;                 // Lower bound of search range
        int hi = arr.length - 1;    // Upper bound of search range
        int steps = 0;              // Counts how many iterations occur

        // Continue searching while the range is valid
        while (lo <= hi) {
            steps++;  // Count this iteration / comparison step

            // Compute midpoint of the current search range
            int mid = lo + (hi - lo) / 2;

            // Check for a match at the midpoint
            if (arr[mid] == target) {
                stepsOut[0] = steps; // Store step count
                return mid;          // Target found
            }
            // Target lies in the right half
            else if (arr[mid] < target) {
                lo = mid + 1;
            }
            // Target lies in the left half
            else {
                hi = mid - 1;
            }
        }

        // Target was not found after exhausting search range
        stepsOut[0] = steps;
        return -1;
    }

    // ----------------------------------------------------
    // Load integers from file, using project root (user.dir)
    // ----------------------------------------------------

    /**
     * Loads the ordered integer dataset using the project root directory.
     *
     * This method assumes the project structure:
     *   <root>/code_samples/section12/data/ordered.txt
     *
     * @return Array of integers read from ordered.txt,
     *         or an empty array on failure
     */
    static int[] loadOrderedFromProjectRoot() {
        // Resolve project root directory
        String root = System.getProperty("user.dir");

        // Build path to ordered.txt using platform-independent separators
        File file = new File(root
                + File.separator + "code_samples"
                + File.separator + "section12"
                + File.separator + "data"
                + File.separator + "ordered.txt");

        System.out.println("Attempting to read: " + file.getAbsolutePath());

        List<Integer> list = new ArrayList<>(); // Temporary dynamic storage

        // Use try-with-resources to ensure Scanner is closed automatically
        try (Scanner sc = new Scanner(file)) {
            // Read integers until no more are available
            while (sc.hasNextInt()) {
                list.add(sc.nextInt());
            }
        } catch (FileNotFoundException e) {
            // Provide diagnostic output if file cannot be found
            System.out.println("Error reading ordered.txt");
            e.printStackTrace();
            return new int[0];
        }

        // Convert List<Integer> to primitive int[] array
        int[] arr = new int[list.size()];
        for (int i = 0; i < list.size(); i++) {
            arr[i] = list.get(i);
        }

        return arr;
    }

    // ----------------------------------------------------
    // MAIN TESTS
    // ----------------------------------------------------

    /**
     * Program entry point.
     *
     * Loads ordered data from disk and runs binary search tests,
     * reporting step counts for different search scenarios.
     */
    public static void main(String[] args) {
        // Load ordered dataset from project root
        int[] arr = loadOrderedFromProjectRoot();

        // Validate successful file load
        if (arr.length == 0) {
            System.out.println("Missing or empty ordered.txt, aborting.");
            return;
        }

        System.out.println("=== Binary Search Tests ===");
        System.out.println("Loaded " + arr.length + " integers\n");

        binary_search bs = new binary_search(); // Instance to run searches
        int[] steps = new int[1];               // Used to return step count

        // Test 1: Search for the first element (best-case scenario)
        int idx = bs.binarySearchSteps(arr, arr[0], steps);
        System.out.println("Search first  (" + arr[0] + "): index=" + idx + ", steps=" + steps[0]);

        // Test 2: Search for the middle element
        int midVal = arr[arr.length / 2];
        idx = bs.binarySearchSteps(arr, midVal, steps);
        System.out.println("Search middle (" + midVal + "): index=" + idx + ", steps=" + steps[0]);

        // Test 3: Search for the last element (worst-case successful search)
        int lastVal = arr[arr.length - 1];
        idx = bs.binarySearchSteps(arr, lastVal, steps);
        System.out.println("Search last   (" + lastVal + "): index=" + idx + ", steps=" + steps[0]);

        // Test 4: Search for a value guaranteed not to exist
        idx = bs.binarySearchSteps(arr, 999_999, steps);
        System.out.println("Search missing (999999): index=" + idx + ", steps=" + steps[0]);
    }
}
