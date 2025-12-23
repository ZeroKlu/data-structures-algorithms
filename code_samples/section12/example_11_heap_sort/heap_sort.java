package code_samples.section12.example_11_heap_sort;

import java.io.*;
import java.nio.file.*;
import java.util.*;

/*
 * ============================================================================
 * Example 11 - Heap Sort (Java) with Step Counting + File-Based Tests
 * ============================================================================
 *
 * PURPOSE
 * -------
 * Demonstrate Heap Sort on an integer array while counting "steps" to help
 * illustrate time complexity in a measurable way.
 *
 * INPUT FILES
 * -----------
 * This program expects TWO text files that contain the SAME integers:
 *
 *   - unordered.txt : the integers in arbitrary (unsorted) order
 *   - ordered.txt   : the same integers sorted in ascending order
 *
 * Files may contain integers separated by any whitespace (spaces/newlines/tabs).
 *
 * PROGRAM FLOW
 * ------------
 * 1) Load unordered.txt and ordered.txt using a robust "try many paths" loader.
 * 2) Copy unordered data so the original stays unchanged.
 * 3) Run heap sort on the copy, while counting:
 *      - comparisons: number of value comparisons (arr[a] vs arr[b])
 *      - writes:      number of array writes (swap counted as 3 writes)
 * 4) Validate sorted output matches ordered.txt exactly.
 * 5) Print a report (elements, comparisons, writes, correct?).
 *
 * STEP COUNTING MODEL (IMPORTANT)
 * -------------------------------
 * comparisons:
 *   - We count comparisons of DATA values, not loop/index bounds checks.
 *   - Specifically, each time we evaluate:
 *       arr[left]  > arr[largest]
 *       arr[right] > arr[largest]
 *
 * writes:
 *   - We treat a swap as 3 writes (tmp, arr[i], arr[j]) to match the convention
 *     used in your C/C++ examples.
 *   - Note: Java variables aren't "array writes". We count only assignments
 *     into the array positions. Using a temp variable is still part of the
 *     swap operation, so we keep the same 3-write model for consistency.
 *
 * HEAP SORT NOTES
 * --------------
 * - We build a MAX-HEAP (largest element at index 0).
 * - Then we repeatedly swap the max element to the end of the array and
 *   heapify the remaining prefix.
 * - Time Complexity:  O(n log n)
 * - Extra Space:      O(1) (in-place), aside from file loading and the test copy.
 * ============================================================================
 */

public class heap_sort {

    // ------------------------------------------------------------------------
    // Step counters (global so heapify/swap can update them easily)
    // ------------------------------------------------------------------------
    private static long comparisons = 0;  // counts value comparisons
    private static long writes = 0;       // counts array writes (swap counted as 3)

    // ------------------------------------------------------------------------
    // Swap helper with write count
    // ------------------------------------------------------------------------
    /*
     * Swaps arr[i] and arr[j].
     *
     * Step counting:
     *   We treat a swap as 3 writes:
     *     1) temp  = arr[i]
     *     2) arr[i] = arr[j]
     *     3) arr[j] = temp
     *
     * This is a standard convention used across many algorithm step-count demos.
     */
    private static void swap(int[] arr, int i, int j) {
        int temp = arr[i];   // conceptually part of the swap operation
        arr[i] = arr[j];
        arr[j] = temp;

        // Count swap as 3 writes for consistency with your other languages.
        writes += 3;
    }

    // ------------------------------------------------------------------------
    // Heapify (restore max-heap property at subtree rooted at index i)
    // ------------------------------------------------------------------------
    /*
     * heapify(arr, n, i) assumes that:
     *   - arr represents a heap for all nodes BELOW i
     *   - but node i itself may violate the heap property
     *
     * It "sifts down" arr[i] until the subtree rooted at i becomes a valid
     * max-heap again.
     *
     * Parameters:
     *   arr : array containing heap elements
     *   n   : heap size (only indices 0..n-1 are considered "in the heap")
     *   i   : index of the subtree root we are fixing
     *
     * comparisons counting:
     *   - We count only comparisons between data values:
     *       arr[left]  > arr[largest]
     *       arr[right] > arr[largest]
     *   - We do NOT count bounds checks like (left < n).
     */
    private static void heapify(int[] arr, int n, int i) {
        int largest = i;       // assume the parent is currently largest
        int left  = 2 * i + 1; // left child index in 0-based heap
        int right = 2 * i + 2; // right child index in 0-based heap

        // If left child exists, compare its value with current largest
        if (left < n) {
            comparisons++; // comparing arr[left] > arr[largest]
            if (arr[left] > arr[largest]) {
                largest = left;
            }
        }

        // If right child exists, compare its value with current largest
        if (right < n) {
            comparisons++; // comparing arr[right] > arr[largest]
            if (arr[right] > arr[largest]) {
                largest = right;
            }
        }

        // If a child is larger than the parent, swap and continue heapifying
        if (largest != i) {
            swap(arr, i, largest);

            // After swapping, the child subtree might now violate heap property,
            // so we recursively heapify the affected subtree.
            heapify(arr, n, largest);
        }
    }

    // ------------------------------------------------------------------------
    // Heap Sort (build max heap, then extract max repeatedly)
    // ------------------------------------------------------------------------
    /*
     * In-place heap sort:
     *   1) Build a max-heap from the array.
     *   2) Repeatedly swap the root (max) with the last element in the heap,
     *      shrink heap size, and heapify the root again.
     *
     * Reset counters here so each call gives a clean measurement.
     */
    public static void heapSort(int[] arr) {
        comparisons = 0;
        writes = 0;

        int n = arr.length;

        // -------------------------
        // 1) Build the max heap
        // -------------------------
        // The last parent is at index (n/2 - 1). Everything after that is a leaf.
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i);
        }

        // -------------------------
        // 2) Extract elements
        // -------------------------
        // Move current max to the end and fix the heap on the remaining prefix.
        for (int end = n - 1; end > 0; end--) {
            // Swap max element at root (index 0) with element at 'end'
            swap(arr, 0, end);

            // Heapify reduced heap [0 .. end-1]
            heapify(arr, end, 0);
        }
    }

    // ------------------------------------------------------------------------
    // Robust file loader
    // ------------------------------------------------------------------------
    /*
     * Loads a whitespace-separated list of integers from a file, trying several
     * likely paths to handle "working directory" differences.
     *
     * Path strategy (in order):
     *   1) filename (as-is)                           e.g., "unordered.txt"
     *   2) code_samples/section12/data/filename       (from project root)
     *   3) data/filename                              
     *   4) ../data/filename                            (common when running from lesson folder)
     *   5) ../../data/filename
     *   6) ../section12/data/filename
     *   7) ../../section12/data/filename
     *
     * If no path works, prints diagnostics and exits.
     *
     * NOTE:
     *   We use Files.lines(...) + split("\\s+") so any whitespace works.
     */
    private static int[] loadFile(String filename) {
        String wd = System.getProperty("user.dir");

        // Candidate search paths (keep consistent with your other examples)
        Path[] attempts = new Path[] {
            Paths.get(filename),
            Paths.get("code_samples", "section12", "data", filename),
            Paths.get("data", filename),
            Paths.get("..", "data", filename),
            Paths.get("..", "..", "data", filename),
            Paths.get("..", "section12", "data", filename),
            Paths.get("..", "..", "section12", "data", filename),
        };

        // Try each candidate path until one exists and is readable
        for (Path path : attempts) {
            File f = path.toFile();
            if (f.exists()) {
                System.out.println("Loaded: " + f.getAbsolutePath());
                try {
                    return Files.lines(f.toPath())
                            // split each line into tokens
                            .flatMap(line -> Arrays.stream(line.trim().split("\\s+")))
                            // ignore empty tokens (e.g., blank lines)
                            .filter(s -> !s.isEmpty())
                            // parse ints
                            .mapToInt(Integer::parseInt)
                            .toArray();
                } catch (IOException e) {
                    System.out.println("Error reading: " + f.getAbsolutePath());
                    System.out.println("Exception: " + e.getMessage());
                    // keep trying other paths
                } catch (NumberFormatException e) {
                    System.out.println("Error parsing integers from: " + f.getAbsolutePath());
                    System.out.println("Exception: " + e.getMessage());
                    // keep trying other paths
                }
            }
        }

        // If we get here, we failed all attempts. Print a helpful report.
        System.out.println("Error reading input file: " + filename);
        System.out.println("Working directory: " + wd);
        System.out.println("Search paths attempted:");
        for (Path p : attempts) {
            System.out.println("  " + p.toAbsolutePath());
        }
        System.out.println("Missing input file - aborting.");
        System.exit(1);

        return null; // unreachable
    }

    // ------------------------------------------------------------------------
    // Main test driver
    // ------------------------------------------------------------------------
    /*
     * 1) Load unordered and expected arrays from files.
     * 2) Sort a copy of unordered using heap sort.
     * 3) Compare element-by-element to expected to confirm correctness.
     * 4) Print step counts and correctness result.
     */
    public static void main(String[] args) {

        // Load test data
        int[] unordered = loadFile("unordered.txt");
        int[] expected  = loadFile("ordered.txt");

        // Basic sanity check: both files should contain the same number of ints
        if (unordered.length != expected.length) {
            System.out.println("Size mismatch between unordered and ordered files!");
            System.out.println("unordered length = " + unordered.length);
            System.out.println("expected  length = " + expected.length);
            System.exit(1);
        }

        // Copy before sorting so we preserve the original unordered data
        int[] data = Arrays.copyOf(unordered, unordered.length);

        // Sort in place
        heapSort(data);

        // Verify correctness (find first mismatch, if any)
        boolean ok = true;
        int badIndex = -1;

        for (int i = 0; i < data.length; i++) {
            if (data[i] != expected[i]) {
                ok = false;
                badIndex = i;
                break;
            }
        }

        // Print report
        System.out.println("\nHeap Sort (Java)");
        System.out.println("----------------");
        System.out.println("Elements:     " + data.length);
        System.out.println("Comparisons:  " + comparisons);
        System.out.println("Writes:       " + writes);
        System.out.println("Correct?      " + (ok ? "YES ✓" : "NO ✗"));

        // If failing, show first mismatch to help debugging
        if (!ok) {
            System.out.println("First mismatch at index " + badIndex
                    + ": got " + data[badIndex]
                    + ", expected " + expected[badIndex]);
        } else {
            // Optional: show a small sample of sorted output
            System.out.println("\nFirst 10 sorted values:");
            for (int i = 0; i < Math.min(10, data.length); i++) {
                System.out.print(data[i] + (i + 1 < Math.min(10, data.length) ? " " : ""));
            }
            System.out.println();
        }
    }
}
