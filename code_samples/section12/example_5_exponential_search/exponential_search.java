package code_samples.section12.example_5_exponential_search;

import java.io.File;                 // Represents file and directory pathnames
import java.io.FileNotFoundException; // Thrown when a file cannot be found
import java.nio.file.Files;          // File existence checks
import java.nio.file.Path;           // File path abstraction
import java.nio.file.Paths;          // Path construction utilities
import java.util.ArrayList;           // Dynamic list for file loading
import java.util.List;
import java.util.Scanner;             // Token-based file reading

public class exponential_search {

    // ----------------------------------------------------
    // Result holder for search index + step count
    // ----------------------------------------------------

    /**
     * Simple immutable container used to return both:
     *  - index: where the target was found (or -1)
     *  - steps: number of array probes performed
     *
     * This avoids using out-parameters and keeps the API clear.
     */
    static class Result {
        final int index;
        final int steps;

        Result(int index, int steps) {
            this.index = index;
            this.steps = steps;
        }
    }

    // ----------------------------------------------------
    // Exponential search with step counting
    // ----------------------------------------------------

    /**
     * Performs exponential search on a sorted integer array.
     *
     * Exponential search operates in two phases:
     *
     *  1) Exponential range expansion:
     *     Starting from index 1, repeatedly double the index
     *     (1, 2, 4, 8, ...) until either:
     *       - the target is less than or equal to arr[bound], or
     *       - the bound exceeds the array size.
     *
     *  2) Binary search:
     *     Once a candidate range is found, perform binary search
     *     within that range.
     *
     * Step counting:
     *   - Counts how many times the algorithm probes the array
     *     (i.e., reads arr[i]).
     *
     * @param arr     Sorted array of integers
     * @param target  Value to locate
     *
     * @return Result object containing index and step count
     */
    static Result exponentialSearchWithSteps(int[] arr, int target) {
        int n = arr.length;
        int steps = 0;

        // Handle empty array
        if (n == 0) {
            return new Result(-1, steps);
        }

        // ------------------------------------------------
        // Step 0: Check the first element directly
        // ------------------------------------------------
        int val0 = arr[0];
        steps++;                   // One array probe
        if (val0 == target) {
            return new Result(0, steps);
        }

        // ------------------------------------------------
        // Phase 1: Find range by repeated doubling
        // ------------------------------------------------
        int bound = 1;

        // Expand the bound until:
        //  - bound exceeds array length, or
        //  - arr[bound] >= target
        while (bound < n) {
            int v = arr[bound];
            steps++;               // One array probe

            if (v < target) {
                bound *= 2;        // Exponentially increase range
            } else {
                break;             // Found upper bound
            }
        }

        // Determine the final search interval
        int left = bound / 2;
        int right = Math.min(bound, n - 1);

        // ------------------------------------------------
        // Phase 2: Binary search within [left, right]
        // ------------------------------------------------
        return binarySearchRangeWithSteps(arr, left, right, target, steps);
    }

    // ----------------------------------------------------
    // Binary search helper with step counting
    // ----------------------------------------------------

    /**
     * Performs binary search on a subrange [lo, hi] of the array,
     * continuing the step count accumulated during exponential search.
     *
     * @param arr        Sorted array of integers
     * @param lo         Lower bound index (inclusive)
     * @param hi         Upper bound index (inclusive)
     * @param target     Value to locate
     * @param stepsSoFar Step count accumulated so far
     *
     * @return Result object with final index and step count
     */
    static Result binarySearchRangeWithSteps(
            int[] arr,
            int lo,
            int hi,
            int target,
            int stepsSoFar) {

        int steps = stepsSoFar;

        // Standard binary search loop
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;

            int val = arr[mid];
            steps++;               // One array probe

            if (val == target) {
                return new Result(mid, steps);
            } else if (val < target) {
                lo = mid + 1;      // Search right half
            } else {
                hi = mid - 1;      // Search left half
            }
        }

        // Target not found in this range
        return new Result(-1, steps);
    }

    // ----------------------------------------------------
    // File loading helpers
    // ----------------------------------------------------

    /**
     * Attempts to load ordered.txt from multiple candidate paths.
     *
     * This makes the program robust when run from different
     * working directories (lesson folder vs workspace root).
     *
     * Candidate paths:
     *   1) ../data/ordered.txt
     *   2) code_samples/section12/data/ordered.txt
     *
     * @return Array of integers if found; otherwise null
     */
    static int[] loadOrderedFile() {
        String[] candidates = new String[] {
            ".." + File.separator + "data" + File.separator + "ordered.txt",
            "code_samples" + File.separator + "section12"
                + File.separator + "data"
                + File.separator + "ordered.txt"
        };

        // Print current working directory for diagnostics
        System.out.println("Working directory: " + System.getProperty("user.dir"));

        // Try each candidate path
        for (String rel : candidates) {
            Path p = Paths.get(rel).normalize().toAbsolutePath();
            System.out.println("Trying: " + p);

            if (Files.exists(p)) {
                try (Scanner sc = new Scanner(new File(p.toString()))) {
                    List<Integer> list = new ArrayList<>();

                    // Read integers from file
                    while (sc.hasNextInt()) {
                        list.add(sc.nextInt());
                    }

                    // Convert List<Integer> → int[]
                    int[] arr = new int[list.size()];
                    for (int i = 0; i < list.size(); i++) {
                        arr[i] = list.get(i);
                    }

                    System.out.println("Loaded " + arr.length
                            + " integers from " + p);
                    return arr;

                } catch (FileNotFoundException e) {
                    // Should not occur after exists(), but guarded anyway
                    System.out.println(
                        "Unexpected FileNotFound after exists() for: " + p);
                }
            } else {
                System.out.println("  Not found at: " + p);
            }
        }

        // All candidate paths failed
        System.out.println(
            "Missing input file - could not locate ordered.txt via any candidate path.");
        return null;
    }

    // ----------------------------------------------------
    // Demo / tests
    // ----------------------------------------------------

    /**
     * Program entry point.
     *
     * Loads ordered data from disk and runs exponential search
     * on representative target values, printing index and step count.
     */
    public static void main(String[] args) {
        int[] arr = loadOrderedFile();
        if (arr == null) {
            // Diagnostics already printed
            return;
        }

        // Representative test targets:
        int[] testTargets = new int[] {
            -5000,   // first element (best case)
            0,       // middle element
            5000,    // last element
            6000     // value not present
        };

        // Run exponential search for each target
        for (int target : testTargets) {
            Result res = exponentialSearchWithSteps(arr, target);
            System.out.printf(
                "Target %d -> index = %d, steps = %d%n",
                target, res.index, res.steps);
        }
    }
}
