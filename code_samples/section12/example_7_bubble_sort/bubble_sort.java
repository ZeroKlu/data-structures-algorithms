package code_samples.section12.example_7_bubble_sort;

import java.io.IOException;
import java.nio.file.*;

public class bubble_sort {

    // ------------------------------------------------------------
    // Bubble Sort with step counting
    // ------------------------------------------------------------
    static class Result {
        long comparisons;
        long swaps;
    }

    void bubbleSort(int[] arr, Result stats) {
        int n = arr.length;
        boolean swapped = true;
        while (swapped) {
            swapped = false;
            for (int i = 1; i < n; i++) {
                stats.comparisons++;
                if (arr[i - 1] > arr[i]) {
                    int tmp = arr[i - 1];
                    arr[i - 1] = arr[i];
                    arr[i] = tmp;
                    stats.swaps++;
                    swapped = true;
                }
            }
            n--; // last element is now in place
        }
    }

    // ------------------------------------------------------------
    // Find project root (directory that contains "code_samples")
    // ------------------------------------------------------------
    private static Path findProjectRoot() {
        Path cur = Paths.get("").toAbsolutePath(); // whatever user.dir is
        System.out.println("Current working directory: " + cur);

        while (cur != null) {
            if (Files.isDirectory(cur.resolve("code_samples"))) {
                System.out.println("Detected project root: " + cur);
                return cur;
            }
            cur = cur.getParent();
        }
        return Paths.get("").toAbsolutePath(); // fallback (probably wrong, but better than null)
    }

    // ------------------------------------------------------------
    // Load file from code_samples/section12/data/<filename>
    // ------------------------------------------------------------
    static int[] loadFile(String filename) {
        try {
            Path projectRoot = findProjectRoot();
            Path full = projectRoot
                    .resolve("code_samples")
                    .resolve("section12")
                    .resolve("data")
                    .resolve(filename);

            System.out.println("Attempting to read: " + full);

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
    public static void main(String[] args) {
        bubble_sort sorter = new bubble_sort();

        int[] unordered = loadFile("unordered.txt");
        int[] expected  = loadFile("ordered.txt");

        if (unordered == null || expected == null) {
            System.out.println("Missing input file - aborting.");
            return;
        }

        if (unordered.length != expected.length) {
            System.out.println("File size mismatch - aborting.");
            System.out.println("unordered length: " + unordered.length);
            System.out.println("expected  length: " + expected.length);
            return;
        }

        Result stats = new Result();

        System.out.println("\n--- Bubble Sort Step Count ---");
        sorter.bubbleSort(unordered, stats);

        System.out.println("Comparisons: " + stats.comparisons);
        System.out.println("Swaps:       " + stats.swaps);

        System.out.println("\nComparing result with ordered.txt...");
        int mismatches = compareArrays(unordered, expected);
        if (mismatches == 0) {
            System.out.println("SUCCESS - output matches!");
        } else {
            System.out.println("FAIL - mismatches found: " + mismatches);
        }
    }
}
