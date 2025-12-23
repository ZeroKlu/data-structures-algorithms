package code_samples.section12.example_6_selection_sort;

import java.nio.file.*;

// ============================================================
// Selection Sort example with step counting and verification
// ============================================================
//
// This program:
//  1) Loads an unordered list of integers from unordered.txt
//  2) Sorts it using Selection Sort while counting steps
//  3) Loads ordered.txt as the reference correct output
//  4) Verifies that the sorted result matches the reference
//
public class selection_sort {

    // ============================
    // Selection Sort statistics
    // ============================
    //
    // comparisons:
    //   Counts how many element-to-element comparisons are made
    //
    // swaps:
    //   Counts how many actual swaps occur
    //
    static class SortStats {
        long comparisons = 0;
        long swaps = 0;
    }

    // ============================
    // Selection Sort with step counting
    // ============================
    //
    // Algorithm:
    //  - For each index i:
    //      * Find the smallest element in the range [i .. n-1]
    //      * Swap it into position i
    //
    // Step counting:
    //  - Increment comparisons for each arr[j] < arr[minIdx] check
    //  - Increment swaps only when a swap is actually performed
    //
    // This method sorts the array IN PLACE.
    //
    void selectionSort(int[] arr, SortStats stats) {
        int n = arr.length;

        // Outer loop selects the next position to fill
        for (int i = 0; i < n; i++) {
            int minIdx = i;

            // Inner loop finds the minimum element
            // in the unsorted suffix [i+1 .. n-1]
            for (int j = i + 1; j < n; j++) {
                stats.comparisons++; // count comparison
                if (arr[j] < arr[minIdx]) {
                    minIdx = j;
                }
            }

            // Swap only if a smaller element was found
            if (minIdx != i) {
                int tmp = arr[i];
                arr[i] = arr[minIdx];
                arr[minIdx] = tmp;
                stats.swaps++; // count swap
            }
        }
    }

    // ============================
    // Load integers from a text file
    // ============================
    //
    // Reads a whitespace-delimited list of integers from a file
    // located relative to the project root:
    //
    //   <project-root>/code_samples/section12/data/<filename>
    //
    // Returns:
    //  - int[] on success
    //  - null on failure
    //
    static int[] loadFile(String relativePathInsideProject) {
        try {
            // Java working directory (usually project root in VS Code)
            String wd = System.getProperty("user.dir");

            // Build full path to the data file
            Path full = Paths.get(
                wd,
                "code_samples",
                "section12",
                "data",
                relativePathInsideProject
            );

            // Diagnostic output to show exactly what path is used
            System.out.println("Attempting to read: " + full);

            // Read entire file as one string
            String text = Files.readString(full);

            // Split on any whitespace
            String[] parts = text.trim().split("\\s+");

            // Convert tokens into integers
            int[] arr = new int[parts.length];
            for (int i = 0; i < parts.length; i++) {
                arr[i] = Integer.parseInt(parts[i]);
            }

            return arr;

        } catch (Exception e) {
            // Handles missing file, permission issues, parse errors, etc.
            System.out.println("Error reading file.");
            System.out.println(e);
            return null;
        }
    }

    // ============================
    // Main Test Harness
    // ============================
    //
    // Orchestrates:
    //  - File loading
    //  - Sorting
    //  - Step reporting
    //  - Verification against ordered.txt
    //
    public static void main(String[] args) {
        selection_sort sorter = new selection_sort();

        String unorderedPath = "..\\data\\unordered.txt";
        String orderedPath   = "..\\data\\ordered.txt";

        // --------------------------------------------------
        // Load unordered input
        // --------------------------------------------------
        System.out.println("Attempting to read: " + unorderedPath);
        int[] unordered = loadFile(unorderedPath);
        if (unordered == null) {
            System.out.println("Missing unordered input file - aborting.");
            return;
        }

        // --------------------------------------------------
        // Load ordered reference data
        // --------------------------------------------------
        System.out.println("Attempting to read: " + orderedPath);
        int[] ordered = loadFile(orderedPath);
        if (ordered == null) {
            System.out.println("Missing ordered input file - aborting.");
            return;
        }

        // Sanity check: both files should contain the same number of values
        if (unordered.length != ordered.length) {
            System.out.println("File lengths differ!");
            System.out.println("unordered length = " + unordered.length);
            System.out.println("ordered length   = " + ordered.length);
            return;
        }

        // --------------------------------------------------
        // Run selection sort
        // --------------------------------------------------
        SortStats stats = new SortStats();
        System.out.println("\n--- Selection Sort (Step Counting) ---");

        sorter.selectionSort(unordered, stats);

        System.out.println("Comparisons: " + stats.comparisons);
        System.out.println("Swaps:       " + stats.swaps);

        // ============================
        // Compare sorted output
        // ============================
        //
        // Verifies that the algorithm produced exactly
        // the same ordering as ordered.txt
        //
        int mismatches = 0;
        for (int i = 0; i < unordered.length; i++) {
            if (unordered[i] != ordered[i]) {
                // Print only the first few mismatches
                if (mismatches < 10) {
                    System.out.println(
                        "Mismatch at index " + i +
                        ": got " + unordered[i] +
                        ", expected " + ordered[i]
                    );
                }
                mismatches++;
            }
        }

        // Final verdict
        if (mismatches == 0) {
            System.out.println("SUCCESS: Output matches ordered.txt");
        } else {
            System.out.println("FAIL: " + mismatches + " mismatches found.");
        }
    }
}
