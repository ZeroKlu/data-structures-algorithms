package code_samples.section12.example_4_interpolation_search;

import java.io.File;                 // Represents file and directory pathnames
import java.io.FileNotFoundException; // Thrown when a file cannot be found
import java.util.*;                   // Provides collections and Scanner

/**
 * Demonstrates Interpolation Search on ordered integer data,
 * including a version that counts comparison steps.
 */
public class interpolation_search {

    // ======================================================
    // Interpolation Search (index-only)
    // ======================================================

    /**
     * Performs interpolation search on a sorted integer array.
     *
     * Interpolation search estimates the likely position of the target
     * based on its value relative to the values at the current bounds.
     * This approach works best when the data is uniformly distributed.
     *
     * @param arr     Sorted array to search
     * @param target  Value to locate
     *
     * @return Index of the target if found; otherwise -1
     */
    int interpolationSearch(int[] arr, int target) {
        int n = arr.length;        // Number of elements in the array
        if (n == 0) return -1;     // Empty array => not found

        int lo = 0, hi = n - 1;    // Search bounds (inclusive)

        // Continue searching while:
        //  - bounds are valid, and
        //  - target lies within the value range [arr[lo], arr[hi]]
        while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {

            // --------------------------------------------------
            // Special case: all values in [lo, hi] are identical
            // --------------------------------------------------
            if (arr[hi] == arr[lo]) {
                // Only possible match is at index lo
                return (arr[lo] == target) ? lo : -1;
            }

            // --------------------------------------------------
            // Estimate the likely position using interpolation:
            //
            // pos = lo + (hi - lo) * (target - arr[lo]) / (arr[hi] - arr[lo])
            //
            // Casting to double avoids integer truncation during division.
            // --------------------------------------------------
            int pos = lo + (int)((double)(hi - lo) * (target - arr[lo])
                    / (arr[hi] - arr[lo]));

            // Guard against out-of-range estimates
            if (pos < lo || pos > hi) return -1;

            // Compare against the estimated position
            if (arr[pos] == target) return pos;
            else if (arr[pos] < target) lo = pos + 1; // Search right subrange
            else hi = pos - 1;                         // Search left subrange
        }

        // Target not found
        return -1;
    }

    // ======================================================
    // Interpolation Search WITH step counting
    // ======================================================

    /**
     * Performs interpolation search while counting selected comparisons.
     *
     * The step counter includes:
     *  - Checking if arr[hi] == arr[lo]
     *  - Checking if arr[lo] == target (uniform-range case)
     *  - Checking if arr[pos] == target
     *  - Checking if arr[pos] < target (to choose direction)
     *
     * Loop boundary checks are not counted—only the explicitly
     * labeled comparisons are included.
     *
     * @param arr       Sorted array to search
     * @param target    Value to locate
     * @param stepsOut  Single-element array used to return step count
     *
     * @return Index of the target if found; otherwise -1
     */
    int interpolationSearchSteps(int[] arr, int target, int[] stepsOut) {
        int steps = 0;        // Counts number of comparisons
        int n = arr.length;   // Number of elements

        // Handle empty array case
        if (n == 0) {
            stepsOut[0] = 0;
            return -1;
        }

        int lo = 0, hi = n - 1; // Search bounds

        // Continue searching while target remains within value bounds
        while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {

            steps++; // Compare arr[hi] == arr[lo]
            if (arr[hi] == arr[lo]) {
                steps++; // Compare arr[lo] == target
                stepsOut[0] = steps;
                return (arr[lo] == target) ? lo : -1;
            }

            // Estimate likely position
            int pos = lo + (int)((double)(hi - lo) * (target - arr[lo])
                    / (arr[hi] - arr[lo]));

            // Guard against invalid estimates
            if (pos < lo || pos > hi) {
                stepsOut[0] = steps;
                return -1;
            }

            steps++; // Compare arr[pos] == target
            if (arr[pos] == target) {
                stepsOut[0] = steps;
                return pos;
            }

            steps++; // Compare arr[pos] < target
            if (arr[pos] < target) lo = pos + 1;
            else hi = pos - 1;
        }

        // Target not found
        stepsOut[0] = steps;
        return -1;
    }

    // ======================================================
    // Load ordered.txt relative to project directory
    // ======================================================

    /**
     * Loads the ordered integer dataset from disk.
     *
     * Expected project structure:
     *   <root>/code_samples/section12/data/ordered.txt
     *
     * @return Array of integers read from ordered.txt,
     *         or an empty array on failure
     */
    static int[] loadOrderedData() {
        // Resolve project root directory
        String root = System.getProperty("user.dir");

        // Build full path to ordered.txt
        String path = root
                + File.separator + "code_samples"
                + File.separator + "section12"
                + File.separator + "data"
                + File.separator + "ordered.txt";

        System.out.println("Attempting to read: " + path);

        List<Integer> list = new ArrayList<>();

        // Use try-with-resources to ensure Scanner is closed automatically
        try (Scanner sc = new Scanner(new File(path))) {
            while (sc.hasNextInt()) {
                list.add(sc.nextInt());
            }
        } catch (FileNotFoundException e) {
            System.out.println("Error reading ordered.txt: " + e.getMessage());
            return new int[0];
        }

        // Convert List<Integer> → primitive int[]
        int[] arr = new int[list.size()];
        for (int i = 0; i < arr.length; i++) arr[i] = list.get(i);

        return arr;
    }

    // ======================================================
    // MAIN Test Harness
    // ======================================================

    /**
     * Program entry point.
     *
     * Loads ordered data from disk and runs interpolation search tests,
     * reporting step counts for different search scenarios.
     */
    public static void main(String[] args) {

        // Load ordered dataset
        int[] arr = loadOrderedData();
        if (arr.length == 0) {
            System.out.println("Missing input file - aborting.");
            return;
        }

        interpolation_search is = new interpolation_search(); // Search instance
        int[] steps = new int[1];                             // Step counter output

        System.out.println("\n=== Interpolation Search Tests (sorted data only) ===");
        System.out.println("Loaded " + arr.length + " integers\n");

        // Test 1: Search for the first element
        int idx = is.interpolationSearchSteps(arr, arr[0], steps);
        System.out.println("Search first  (" + arr[0] + "): index=" + idx + ", steps=" + steps[0]);

        // Test 2: Search for the middle element
        int midVal = arr[arr.length / 2];
        idx = is.interpolationSearchSteps(arr, midVal, steps);
        System.out.println("Search middle (" + midVal + "): index=" + idx + ", steps=" + steps[0]);

        // Test 3: Search for the last element
        int lastVal = arr[arr.length - 1];
        idx = is.interpolationSearchSteps(arr, lastVal, steps);
        System.out.println("Search last   (" + lastVal + "): index=" + idx + ", steps=" + steps[0]);

        // Test 4: Search for a value guaranteed not to exist
        idx = is.interpolationSearchSteps(arr, 999_999, steps);
        System.out.println("Search missing (999999): index=" + idx + ", steps=" + steps[0]);
    }
}
