package code_samples.section12.example_8_merge_sort;

import java.io.IOException;
import java.nio.file.*;
import java.util.*;
import java.util.stream.Stream;

public class merge_sort {
    // void mergeSort(int[] arr) {
    //     int n = arr.length;
    //     int[] tmp = new int[n];
    //     mergeSortRec(arr, tmp, 0, n);
    // }

    // private void mergeSortRec(int[] arr, int[] tmp, int left, int right) {
    //     if (right - left <= 1) return;
    //     int mid = left + (right - left) / 2;
    //     mergeSortRec(arr, tmp, left, mid);
    //     mergeSortRec(arr, tmp, mid, right);
    //     merge(arr, tmp, left, mid, right);
    // }

    // private void merge(int[] arr, int[] tmp, int left, int mid, int right) {
    //     int i = left, j = mid, k = left;
    //     while (i < mid && j < right) {
    //         if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
    //         else                  tmp[k++] = arr[j++];
    //     }
    //     while (i < mid)  tmp[k++] = arr[i++];
    //     while (j < right) tmp[k++] = arr[j++];
    //     for (int p = left; p < right; p++) {
    //         arr[p] = tmp[p];
    //     }
    // }

    // --- step counters for "Big-O" illustration ---
    private long compareCount = 0;
    private long copyCount = 0;

    // Public getters so main() can print them
    public long getCompareCount() { return compareCount; }
    public long getCopyCount() { return copyCount; }

    // --- Merge sort API ---

    public void mergeSort(int[] arr) {
        int n = arr.length;
        int[] tmp = new int[n];
        compareCount = 0;
        copyCount = 0;
        mergeSortRec(arr, tmp, 0, n);
    }

    private void mergeSortRec(int[] arr, int[] tmp, int left, int right) {
        if (right - left <= 1) return;
        int mid = left + (right - left) / 2;
        mergeSortRec(arr, tmp, left, mid);
        mergeSortRec(arr, tmp, mid, right);
        merge(arr, tmp, left, mid, right);
    }

    private void merge(int[] arr, int[] tmp, int left, int mid, int right) {
        int i = left, j = mid, k = left;
        while (i < mid && j < right) {
            compareCount++; // arr[i] <= arr[j] comparison
            if (arr[i] <= arr[j]) {
                tmp[k++] = arr[i++];
            } else {
                tmp[k++] = arr[j++];
            }
            copyCount++; // wrote one element into tmp
        }
        while (i < mid) {
            tmp[k++] = arr[i++];
            copyCount++;
        }
        while (j < right) {
            tmp[k++] = arr[j++];
            copyCount++;
        }
        for (int p = left; p < right; p++) {
            arr[p] = tmp[p];
            copyCount++; // count copy back into original array
        }
    }

    // --- File loading helpers ---

    /**
     * Try to load a space-separated integer file from:
     *   1) ..\data\<name> relative to working dir
     *   2) C:\Users\<you>\Desktop\data-structures-algorithms\code_samples\section12\data\<name>
     */
    private static int[] loadFile(String name) throws IOException {
        // 1) relative path: ..\data\name (works when running from lesson dir)
        Path rel = Paths.get("..", "data", name).normalize();
        System.out.println("Attempting to read (relative): " + rel.toAbsolutePath());

        if (Files.exists(rel)) {
            return readInts(rel);
        }

        // 2) explicit full project path (your known layout)
        Path explicit = Paths.get(
            System.getProperty("user.home"),
            "Desktop",
            "data-structures-algorithms",
            "code_samples",
            "section12",
            "data",
            name
        ).normalize();
        System.out.println("Relative path not found, trying explicit: " + explicit.toAbsolutePath());

        if (Files.exists(explicit)) {
            return readInts(explicit);
        }

        throw new IOException("Missing file '" + name + "' at:\n  "
                + rel.toAbsolutePath() + "\n  " + explicit.toAbsolutePath());
    }

    private static int[] readInts(Path path) throws IOException {
        try (Stream<String> lines = Files.lines(path)) {
            return lines
                .flatMap(line -> Arrays.stream(line.trim().split("\\s+")))
                .filter(s -> !s.isEmpty())
                .mapToInt(Integer::parseInt)
                .toArray();
        }
    }

    // --- Test harness / demo ---

    public static void main(String[] args) {
        System.out.println("Working directory: " + Paths.get("").toAbsolutePath());

        try {
            int[] unordered = loadFile("unordered.txt");
            int[] expected  = loadFile("ordered.txt");

            System.out.println("Loaded unordered length: " + unordered.length);
            System.out.println("Loaded ordered   length: " + expected.length);

            merge_sort sorter = new merge_sort();
            int[] arr = Arrays.copyOf(unordered, unordered.length);

            sorter.mergeSort(arr);

            boolean ok = Arrays.equals(arr, expected);
            System.out.println("Sorted correctly? " + ok);

            System.out.println("Comparisons: " + sorter.getCompareCount());
            System.out.println("Copies:      " + sorter.getCopyCount());

            // show a small sample
            int show = Math.min(10, arr.length);
            System.out.print("First " + show + " sorted values: ");
            for (int i = 0; i < show; i++) {
                System.out.print(arr[i] + (i + 1 < show ? " " : ""));
            }
            System.out.println();

        } catch (IOException e) {
            System.err.println("Error reading input files:");
            System.err.println(e.getMessage());
        }
    }
}
