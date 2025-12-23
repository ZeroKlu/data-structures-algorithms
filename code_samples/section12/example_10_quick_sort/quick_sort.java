package code_samples.section12.example_10_quick_sort;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/*
    ============================================================
    Example 10 - Quick Sort (Java) with Step Counting + File Tests
    ============================================================

    PURPOSE
    -------
    This program demonstrates Quick Sort on a set of integers, while
    counting "steps" to illustrate time complexity.

    The program expects TWO files containing the SAME integers:
        unordered.txt  - numbers in random order
        ordered.txt    - the same numbers sorted ascending

    Workflow:
        1) Load unordered.txt and ordered.txt (using multiple fallback paths)
        2) Make a copy of unordered data
        3) Quick-sort the copy
        4) Compare the sorted output to ordered.txt
        5) Print:
            - number of elements
            - comparisons
            - writes
            - correctness (PASS/FAIL)
            - sample output (first 10 values) if correct

    INPUT FORMAT
    ------------
    Files should contain integers separated by whitespace
    (spaces, newlines, tabs, etc.).

    STEP COUNTING MODEL
    -------------------
    We count two metrics:

      comparisons:
        - incremented once per "arr[j] <= pivot" check in partition()

      writes:
        - incremented by 3 for each swap (teaching approximation)
          because a swap is typically 3 assignments:
              tmp = a
              a = b
              b = tmp

    ALGORITHM NOTES
    ---------------
    - This is Lomuto partitioning using the LAST element as the pivot.
    - Quick Sort is O(n log n) average-case, but O(n^2) worst-case
      (especially if the pivot choice is poor, e.g., already-sorted input).
*/

public class quick_sort {

    // ------------------------------------------------------------
    // Global counters for illustrating complexity
    // ------------------------------------------------------------
    //
    // Using static globals keeps the counting simple:
    // - partition() can increment comparisons
    // - swap() can increment writes
    //
    // Reset to 0 at the start of quickSort().
    private static long comparisons = 0;
    private static long writes = 0;

    // ------------------------------------------------------------
    // Quick Sort with counters
    // ------------------------------------------------------------

    /**
     * Public entry point for Quick Sort.
     *
     * Resets step counters and sorts the array in-place.
     */
    public static void quickSort(int[] arr) {
        // Always reset counters before a run
        comparisons = 0;
        writes = 0;

        // Arrays of length 0 or 1 are already sorted
        if (arr.length > 1) {
            quickSortRec(arr, 0, arr.length - 1);
        }
    }

    /**
     * Recursive Quick Sort on the sub-array arr[left..right] inclusive.
     *
     * Base case:
     *   - if left >= right, the range has 0 or 1 element -> already sorted
     *
     * Recursive case:
     *   - partition around a pivot -> pivot ends up in its final correct place
     *   - recursively sort the left partition  [left .. p-1]
     *   - recursively sort the right partition [p+1 .. right]
     */
    private static void quickSortRec(int[] arr, int left, int right) {
        if (left < right) {
            int p = partition(arr, left, right);
            quickSortRec(arr, left, p - 1);
            quickSortRec(arr, p + 1, right);
        }
    }

    /**
     * Lomuto partition scheme.
     *
     * Picks pivot = arr[right] (last element).
     *
     * Invariant:
     *   - i tracks the next position where a value <= pivot should go
     *   - j scans from left to right-1
     *
     * For each j:
     *   - if arr[j] <= pivot, swap arr[i] and arr[j], then i++
     *
     * After the loop:
     *   - swap arr[i] with pivot (arr[right]) so pivot lands in its final spot
     *
     * Returns:
     *   - pivot's final index i
     *
     * Step counting:
     *   - comparisons++ each time we evaluate arr[j] <= pivot
     *   - writes counted in swap()
     */
    private static int partition(int[] arr, int left, int right) {
        int pivot = arr[right];
        int i = left;

        for (int j = left; j < right; j++) {
            comparisons++;                // counting arr[j] <= pivot comparison

            // NOTE:
            // Using <= means duplicates equal to pivot go on the "left" side.
            // Using < instead can reduce swaps for duplicates, but both are valid.
            if (arr[j] <= pivot) {
                swap(arr, i, j);          // write counting happens inside swap()
                i++;
            }
        }

        // Place pivot in its final position
        swap(arr, i, right);
        return i;
    }

    /**
     * Swap arr[i] and arr[j].
     *
     * Step counting:
     *   - treat a swap as 3 writes (tmp, arr[i], arr[j])
     *
     * Optimization:
     *   - if i == j, swapping does nothing and should not count as writes
     */
    private static void swap(int[] arr, int i, int j) {
        if (i == j) return; // no work performed => no writes counted

        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;

        // Count a swap as 3 writes (teaching model)
        writes += 3;
    }

    // ------------------------------------------------------------
    // File loading helper
    // ------------------------------------------------------------

    /**
     * Loads an integer file using multiple fallback search paths.
     *
     * Why do we do this?
     * - Students often run from different working directories (IDE vs terminal),
     *   and relative paths break easily.
     *
     * Strategy:
     * - Build a list of prefixes (candidate folders)
     * - Try each prefix + filename until we find an existing file
     * - If nothing is found, print diagnostics and exit
     *
     * Returns:
     * - int[] containing all integers in the file
     */
    private static int[] loadFile(String filename) {

        // Each prefix is prepended to filename to form a candidate path
        String[] prefixes = new String[] {
            "",   // current working directory (CWD)

            // ./data/ (common for some project layouts)
            "data" + File.separator,

            // ../data/ (common when lesson folder sits next to data/)
            ".." + File.separator + "data" + File.separator,

            // ../section12/data/ (if running from inside section subfolder)
            ".." + File.separator + "section12" + File.separator + "data" + File.separator,

            // code_samples/section12/data/ (if running from repository/project root)
            "code_samples" + File.separator + "section12" + File.separator + "data" + File.separator,

            // ../code_samples/section12/data/ (one level above project root)
            ".." + File.separator +
                "code_samples" + File.separator +
                "section12" + File.separator +
                "data" + File.separator
        };

        // Keep a record of attempted paths for error messages
        List<String> tried = new ArrayList<>();
        File found = null;

        // Try each candidate path until one exists
        for (String prefix : prefixes) {
            File f = new File(prefix + filename);
            tried.add(f.getPath());

            if (f.exists()) {
                System.out.println("Loaded: " + f.getPath());
                found = f;
                break;
            }
        }

        // If nothing found, print diagnostics and exit
        if (found == null) {
            System.out.println("Error reading: " + filename);
            System.out.println("Working directory: " + System.getProperty("user.dir"));
            System.out.println("Search paths attempted:");
            for (String p : tried) System.out.println("  " + p);
            System.out.println("Missing input file - aborting.");
            System.exit(1);
        }

        // Parse integers from the discovered file
        //
        // Scanner can read whitespace-separated tokens easily. We also guard
        // against non-integer tokens by skipping them.
        List<Integer> nums = new ArrayList<>();

        try (Scanner sc = new Scanner(found)) {
            while (sc.hasNext()) {
                if (sc.hasNextInt()) {
                    nums.add(sc.nextInt());
                } else {
                    // Skip any non-integer token (defensive)
                    sc.next();
                }
            }
        } catch (Exception e) {
            System.out.println("Error reading: " + found.getPath());
            e.printStackTrace();
            System.exit(1);
        }

        // Convert List<Integer> -> int[]
        int[] arr = new int[nums.size()];
        for (int i = 0; i < nums.size(); i++) arr[i] = nums.get(i);

        return arr;
    }

    // ------------------------------------------------------------
    // MAIN: load unordered / ordered, sort, compare, print stats
    // ------------------------------------------------------------

    public static void main(String[] args) {

        // 1) Load test data
        int[] unordered = loadFile("unordered.txt");
        int[] expected  = loadFile("ordered.txt");

        // 2) Basic sanity check: both files must be same length
        if (unordered.length != expected.length) {
            System.out.println("Length mismatch: unordered="
                    + unordered.length + ", ordered=" + expected.length);
            System.out.println("Aborting.");
            return;
        }

        // 3) Sort a copy (do NOT mutate original unordered[] input)
        int[] arr = unordered.clone();
        quickSort(arr);

        // 4) Validate: compare sorted result against expected array
        boolean ok = true;
        int badIndex = -1;

        for (int i = 0; i < arr.length; i++) {
            if (arr[i] != expected[i]) {
                ok = false;
                badIndex = i;
                break;
            }
        }

        // 5) Print results
        System.out.println();
        System.out.println("Quick Sort (Java)");
        System.out.println("-----------------");
        System.out.println("Elements:     " + arr.length);
        System.out.println("Comparisons:  " + comparisons);
        System.out.println("Writes:       " + writes);
        System.out.println("Correct?      " + (ok ? "YES ✔" : "NO ✘"));

        // If incorrect, show where it failed
        if (!ok) {
            System.out.println("First mismatch at index " + badIndex +
                    ": got " + arr[badIndex] +
                    ", expected " + expected[badIndex]);
        } else {
            // If correct, print a small sample for visual confirmation
            System.out.println("\nFirst 10 sorted values:");
            for (int i = 0; i < Math.min(10, arr.length); i++) {
                System.out.print(arr[i] + " ");
            }
            System.out.println();
        }
    }
}
