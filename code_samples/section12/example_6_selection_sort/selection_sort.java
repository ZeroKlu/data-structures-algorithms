package code_samples.section12.example_6_selection_sort;

import java.nio.file.*;

public class selection_sort {

    // ============================
    // Selection Sort with step count
    // ============================
    static class SortStats {
        long comparisons = 0;
        long swaps = 0;
    }

    void selectionSort(int[] arr, SortStats stats) {
        int n = arr.length;
        for (int i = 0; i < n; i++) {
            int minIdx = i;
            for (int j = i + 1; j < n; j++) {
                stats.comparisons++; // count comparison
                if (arr[j] < arr[minIdx]) {
                    minIdx = j;
                }
            }
            if (minIdx != i) {
                int tmp = arr[i];
                arr[i] = arr[minIdx];
                arr[minIdx] = tmp;
                stats.swaps++;
            }
        }
    }

    // ============================
    // Load integers from a text file
    // ============================
    static int[] loadFile(String relativePathInsideProject) {
        try {
            // Detect Java working directory (root of VS Code / project)
            String wd = System.getProperty("user.dir");

            // Build correct full path
            Path full = Paths.get(wd, "code_samples", "section12", "data", relativePathInsideProject);

            System.out.println("Attempting to read: " + full);

            String text = Files.readString(full);
            String[] parts = text.trim().split("\\s+");

            int[] arr = new int[parts.length];
            for (int i = 0; i < parts.length; i++) {
                arr[i] = Integer.parseInt(parts[i]);
            }
            return arr;

        } catch (Exception e) {
            System.out.println("Error reading file.");
            System.out.println(e);
            return null;
        }
    }

    // ============================
    // Main Test Harness
    // ============================
    public static void main(String[] args) {
        selection_sort sorter = new selection_sort();

        String unorderedPath = "..\\data\\unordered.txt";
        String orderedPath   = "..\\data\\ordered.txt";

        System.out.println("Attempting to read: " + unorderedPath);
        int[] unordered = loadFile(unorderedPath);
        if (unordered == null) {
            System.out.println("Missing unordered input file - aborting.");
            return;
        }

        System.out.println("Attempting to read: " + orderedPath);
        int[] ordered = loadFile(orderedPath);
        if (ordered == null) {
            System.out.println("Missing ordered input file - aborting.");
            return;
        }

        if (unordered.length != ordered.length) {
            System.out.println("File lengths differ!");
            System.out.println("unordered length = " + unordered.length);
            System.out.println("ordered length   = " + ordered.length);
            return;
        }

        SortStats stats = new SortStats();
        System.out.println("\n--- Selection Sort (Step Counting) ---");

        sorter.selectionSort(unordered, stats);

        System.out.println("Comparisons: " + stats.comparisons);
        System.out.println("Swaps:       " + stats.swaps);

        // ============================
        // Compare sorted output
        // ============================
        int mismatches = 0;
        for (int i = 0; i < unordered.length; i++) {
            if (unordered[i] != ordered[i]) {
                if (mismatches < 10) {
                    System.out.println(
                        "Mismatch at index " + i +
                        ": got " + unordered[i] +
                        ", expected " + ordered[i]);
                }
                mismatches++;
            }
        }

        if (mismatches == 0) {
            System.out.println("SUCCESS: Output matches ordered.txt");
        } else {
            System.out.println("FAIL: " + mismatches + " mismatches found.");
        }
    }
}
