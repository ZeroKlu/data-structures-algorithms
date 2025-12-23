package code_samples.section12.example_7_bubble_sort;

import java.io.IOException;
import java.nio.file.*;

// ============================================================
// Bubble Sort (step counting + file-based verification)
// ============================================================
//
// This program:
//   1) Locates the project root (folder that contains "code_samples")
//   2) Loads unordered.txt (unsorted input) and ordered.txt (expected output)
//   3) Runs bubble sort while counting comparisons + swaps
//   4) Verifies the sorted result matches ordered.txt
//
public class bubble_sort {

    // ------------------------------------------------------------
    // Bubble Sort with step counting
    // ------------------------------------------------------------
    //
    // We store step counts in this simple "Result" object:
    //   - comparisons: counts adjacent comparisons (arr[i-1] > arr[i])
    //   - swaps: counts actual swaps performed
    //
    // Note: This is essentially the same pattern as other languages in
    // your lesson set: a small stats container passed into the sorter.
    //
    static class Result {
        long comparisons;
        long swaps;
    }

    // ------------------------------------------------------------
    // bubbleSort (IN PLACE) with step counting
    // ------------------------------------------------------------
    //
    // Bubble sort overview:
    //   - Repeatedly sweep through the array.
    //   - Compare adjacent items; swap if out of order.
    //   - The largest remaining item "bubbles" to the end each pass.
    //
    // Optimizations used:
    //   1) Early exit:
    //        If a full pass performs no swaps, the array is sorted.
    //   2) Shrinking range:
    //        After each pass, the last element is in its final position,
    //        so we can reduce n by 1 and skip re-checking it.
    //
    // Step counting:
    //   - stats.comparisons increments once per adjacent comparison
    //   - stats.swaps increments only when a swap occurs
    //
    void bubbleSort(int[] arr, Result stats) {
        int n = arr.length;
        boolean swapped = true;

        // Continue passes until a pass makes no swaps
        while (swapped) {
            swapped = false;

            // Compare adjacent pairs in the current unsorted range [0..n-1]
            for (int i = 1; i < n; i++) {
                stats.comparisons++; // count the comparison arr[i - 1] > arr[i]

                // Swap if out of order
                if (arr[i - 1] > arr[i]) {
                    int tmp = arr[i - 1];
                    arr[i - 1] = arr[i];
                    arr[i] = tmp;

                    stats.swaps++;   // count actual swap
                    swapped = true;  // indicates we must do another pass
                }
            }

            // After each pass, the last element in the current range is correct
            n--; // last element is now in place
        }
    }

    // ------------------------------------------------------------
    // Find project root (directory that contains "code_samples")
    // ------------------------------------------------------------
    //
    // Why this exists:
    //   - When running from different IDEs / terminals, user.dir may be:
    //       * the repo root
    //       * the specific lesson folder
    //       * some build/output folder
    //
    // This method walks upward from the current working directory until
    // it finds a directory that contains a "code_samples" folder.
    //
    // Returns:
    //   - Path to the detected project root, or
    //   - current directory as fallback if nothing is found (best effort)
    //
    private static Path findProjectRoot() {
        // Start at the current working directory (whatever user.dir is)
        Path cur = Paths.get("").toAbsolutePath();
        System.out.println("Current working directory: " + cur);

        // Walk upwards until we hit filesystem root (cur becomes null)
        while (cur != null) {
            // Heuristic: repo/project root contains "code_samples"
            if (Files.isDirectory(cur.resolve("code_samples"))) {
                System.out.println("Detected project root: " + cur);
                return cur;
            }
            cur = cur.getParent();
        }

        // fallback (probably wrong, but better than null)
        return Paths.get("").toAbsolutePath();
    }

    // ------------------------------------------------------------
    // Load file from code_samples/section12/data/<filename>
    // ------------------------------------------------------------
    //
    // Builds a path relative to the detected project root:
    //   <root>/code_samples/section12/data/<filename>
    //
    // Reads all text, splits on whitespace, parses integers.
    //
    // Returns:
    //   - int[] on success
    //   - null on failure
    //
    // Errors handled:
    //   - IOException (file missing, permissions, etc.)
    //   - NumberFormatException (non-integer tokens)
    //
    static int[] loadFile(String filename) {
        try {
            Path projectRoot = findProjectRoot();

            // Build full path to the data file
            Path full = projectRoot
                    .resolve("code_samples")
                    .resolve("section12")
                    .resolve("data")
                    .resolve(filename);

            // Diagnostic output helps confirm the real resolved path
            System.out.println("Attempting to read: " + full);

            // Read entire file and parse integers
            String text = Files.readString(full);
            String[] parts = text.trim().split("\\s+");

            int[] arr = new int[parts.length];
            for (int i = 0; i < parts.length; i++) {
                arr[i] = Integer.parseInt(parts[i]);
            }

            return arr;
        } catch (IOException e) {
            System.out.println("Error reading: " + filename);
            e.printStackTrace();
            return null;
        } catch (NumberFormatException e) {
            System.out.println("Error parsing integers from: " + filename);
            e.printStackTrace();
            return null;
        }
    }

    // ------------------------------------------------------------
    // Compare arrays (print a few mismatches)
    // ------------------------------------------------------------
    //
    // Compares two arrays element-by-element and counts mismatches.
    // Prints up to the first 10 mismatches so output remains readable.
    //
    // Defensive behavior:
    //   - compares only up to the smaller length (Math.min),
    //     though main() already checks for equal lengths before calling.
    //
    // Returns:
    //   - number of mismatched indices
    //
    static int compareArrays(int[] a, int[] b) {
        int mismatches = 0;
        int n = Math.min(a.length, b.length);

        for (int i = 0; i < n; i++) {
            if (a[i] != b[i]) {
                if (mismatches < 10) {
                    System.out.println("Mismatch at index " + i +
                            ": got " + a[i] + ", expected " + b[i]);
                }
                mismatches++;
            }
        }
        return mismatches;
    }

    // ------------------------------------------------------------
    // Main for testing
    // ------------------------------------------------------------
    //
    // Test workflow:
    //   1) Load unordered.txt and ordered.txt
    //   2) Validate both loaded and lengths match
    //   3) Run bubble sort with step counting
    //   4) Print counts and verify correctness against ordered reference
    //
    public static void main(String[] args) {
        bubble_sort sorter = new bubble_sort();

        // Load test input and reference output
        int[] unordered = loadFile("unordered.txt");
        int[] expected  = loadFile("ordered.txt");

        // Abort if either file could not be loaded
        if (unordered == null || expected == null) {
            System.out.println("Missing input file - aborting.");
            return;
        }

        // Abort if sizes differ (cannot do a 1:1 comparison)
        if (unordered.length != expected.length) {
            System.out.println("File size mismatch - aborting.");
            System.out.println("unordered length: " + unordered.length);
            System.out.println("expected  length: " + expected.length);
            return;
        }

        // Container for step counts
        Result stats = new Result();

        System.out.println("\n--- Bubble Sort Step Count ---");

        // Sort IN PLACE and accumulate comparisons + swaps
        sorter.bubbleSort(unordered, stats);

        // Print stats
        System.out.println("Comparisons: " + stats.comparisons);
        System.out.println("Swaps:       " + stats.swaps);

        // Verify that the sorted output matches the known correct ordering
        System.out.println("\nComparing result with ordered.txt...");
        int mismatches = compareArrays(unordered, expected);

        if (mismatches == 0) {
            System.out.println("SUCCESS - output matches!");
        } else {
            System.out.println("FAIL - mismatches found: " + mismatches);
        }
    }
}
