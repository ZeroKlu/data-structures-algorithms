package code_samples.section12.example_3_jump_search;

import java.io.File;                 // Represents file and directory pathnames
import java.io.FileNotFoundException; // Thrown when a file cannot be found
import java.util.*;                   // Provides collections and Scanner

/**
 * Demonstrates Jump Search on ordered integer data,
 * including a version that counts comparison steps.
 */
public class jump_search {

    // ============================================================
    // Jump Search (index only)
    // ============================================================

    /**
     * Performs jump search on a sorted integer array.
     *
     * Jump search operates in two phases:
     *  1) Jump phase: move forward in fixed-size blocks (sqrt(n))
     *     until a block that may contain the target is found.
     *  2) Linear search phase: scan linearly within that block.
     *
     * @param arr     Sorted array to search
     * @param target  Value to locate
     *
     * @return Index of the target if found; otherwise -1
     */
    int jumpSearch(int[] arr, int target) {
        int n = arr.length;        // Number of elements in the array
        if (n == 0) return -1;     // Empty array => not found

        // Determine jump (block) size
        int step = (int)Math.sqrt(n);

        int prev = 0;              // Start index of the current block
        int curr = step;           // End index of the current block

        // --------------------------------------------------------
        // Jump phase: move block-by-block until target may be inside
        // --------------------------------------------------------
        // Continue jumping while:
        //  - curr is within bounds
        //  - the value at arr[curr] is still less than the target
        while (curr < n && arr[curr] < target) {
            prev = curr;           // Advance block start
            curr += step;          // Jump to next block end
        }

        // Clamp curr to last valid index if we overshoot the array
        if (curr >= n) curr = n - 1;

        // --------------------------------------------------------
        // Linear search phase within [prev, curr]
        // --------------------------------------------------------
        for (int i = prev; i <= curr; i++) {
            if (arr[i] == target) return i; // Target found
        }

        // Target not found
        return -1;
    }

    // ============================================================
    // Jump Search WITH step counting
    // stepsOut[0] = number of comparisons
    // ============================================================

    /**
     * Performs jump search while counting the number of comparisons made.
     *
     * Comparisons are counted in both phases:
     *  - Jump phase comparisons against arr[curr]
     *  - Linear scan comparisons against arr[i]
     *
     * @param arr       Sorted array to search
     * @param target    Value to locate
     * @param stepsOut  Single-element array used to return step count
     *
     * @return Index of the target if found; otherwise -1
     */
    int jumpSearchSteps(int[] arr, int target, int[] stepsOut) {
        int n = arr.length;   // Number of elements
        int steps = 0;        // Counts total number of comparisons

        // Handle empty array case
        if (n == 0) {
            stepsOut[0] = 0;
            return -1;
        }

        // Determine jump (block) size
        int step = (int)Math.sqrt(n);

        int prev = 0;         // Start index of the current block
        int curr = step;      // End index of the current block

        // --------------------------------------------------------
        // Jump phase
        // --------------------------------------------------------
        while (curr < n) {
            steps++; // Count comparison involving arr[curr] and target

            // If current block end is >= target, stop jumping
            if (arr[curr] >= target)
                break;

            // Otherwise, jump to the next block
            prev = curr;
            curr += step;
        }

        // Clamp curr to last valid index if we overshoot the array
        if (curr >= n) curr = n - 1;

        // --------------------------------------------------------
        // Linear search phase within [prev, curr]
        // --------------------------------------------------------
        for (int i = prev; i <= curr; i++) {
            steps++; // Count comparison arr[i] == target

            if (arr[i] == target) {
                stepsOut[0] = steps; // Store step count
                return i;            // Target found
            }
        }

        // Target not found
        stepsOut[0] = steps;
        return -1;
    }

    // ============================================================
    // Load ordered.txt relative to project directory
    // ============================================================

    /**
     * Loads the ordered integer dataset from disk using the project root.
     *
     * This method assumes the project structure:
     *   <root>/code_samples/section12/data/ordered.txt
     *
     * @return Array of integers read from ordered.txt,
     *         or an empty array on failure
     */
    static int[] loadOrderedData() {
        // Resolve project root directory
        String root = System.getProperty("user.dir");

        // Build path to ordered.txt using platform-independent separators
        String path = root
                + File.separator + "code_samples"
                + File.separator + "section12"
                + File.separator + "data"
                + File.separator + "ordered.txt";

        System.out.println("Attempting to read: " + path);

        List<Integer> list = new ArrayList<>(); // Temporary dynamic storage

        // Use try-with-resources to ensure Scanner is closed automatically
        try (Scanner sc = new Scanner(new File(path))) {
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
        for (int i = 0; i < arr.length; i++) arr[i] = list.get(i);

        return arr;
    }

    // ============================================================
    // MAIN TESTS
    // ============================================================

    /**
     * Program entry point.
     *
     * Loads ordered data from disk and runs jump search tests,
     * reporting step counts for different search scenarios.
     */
    public static void main(String[] args) {
        // Load ordered dataset
        int[] arr = loadOrderedData();

        // Validate successful file load
        if (arr.length == 0) {
            System.out.println("Missing input file - aborting.");
            return;
        }

        jump_search js = new jump_search(); // Instance to run searches

        int[] steps = new int[1]; // Used to return step count
        int idx;                  // Stores search result index

        System.out.println("=== Jump Search Tests ===");
        System.out.println("Loaded " + arr.length + " integers\n");

        // Test 1: Search for the first element
        idx = js.jumpSearchSteps(arr, arr[0], steps);
        System.out.println("Search first  (" + arr[0] + "): index=" + idx + ", steps=" + steps[0]);

        // Test 2: Search for the middle element
        int midVal = arr[arr.length / 2];
        idx = js.jumpSearchSteps(arr, midVal, steps);
        System.out.println("Search middle (" + midVal + "): index=" + idx + ", steps=" + steps[0]);

        // Test 3: Search for the last element
        int lastVal = arr[arr.length - 1];
        idx = js.jumpSearchSteps(arr, lastVal, steps);
        System.out.println("Search last   (" + lastVal + "): index=" + idx + ", steps=" + steps[0]);

        // Test 4: Search for a value guaranteed not to exist
        idx = js.jumpSearchSteps(arr, 999_999, steps);
        System.out.println("Search missing (999999): index=" + idx + ", steps=" + steps[0]);
    }
}
