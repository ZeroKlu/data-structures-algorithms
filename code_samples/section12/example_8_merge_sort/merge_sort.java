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

    /*
        -------------------------------------------------------------------------
        COMMENTED-OUT (baseline) MERGE SORT
        -------------------------------------------------------------------------
        This commented block is the “plain” merge sort implementation without
        any step counting.

        It’s left here on purpose so students can compare:
          - the original algorithm (just sorting),
          - versus the instrumented version below that counts comparisons + copies.

        In a production codebase, you’d typically delete this block to reduce noise.
    */

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
    /*
        These counters instrument the algorithm to help illustrate time complexity.

        compareCount:
          - increments for each element-to-element comparison made during merging:
                arr[i] <= arr[j]

        copyCount:
          - increments for each write into tmp[] during merging
          - increments again for each write copying tmp[] back into arr[]

        Important:
          - These are instance fields, so the counts belong to a specific sorter object.
          - mergeSort() resets them to 0 each time you start a new sort.
    */
    private long compareCount = 0;
    private long copyCount = 0;

    // Public getters so main() can print them
    /*
        Simple accessor methods so callers (like main) can display the final counts.

        This avoids making the fields public while still allowing observation of the
        measured “work” done by the algorithm.
    */
    public long getCompareCount() { return compareCount; }
    public long getCopyCount() { return copyCount; }

    // --- Merge sort API ---
    /*
        mergeSort()
        ----------
        Public entry point:
          - Allocates a temporary buffer tmp[] the same size as arr
          - Resets counters (important for repeat runs)
          - Calls the recursive helper over the full half-open range [0, n)

        Range convention:
          - Uses half-open intervals: [left, right)
            where 'right' is exclusive.
    */
    public void mergeSort(int[] arr) {
        int n = arr.length;
        int[] tmp = new int[n];

        // Reset counters for a clean measurement run
        compareCount = 0;
        copyCount = 0;

        mergeSortRec(arr, tmp, 0, n);
    }

    /*
        mergeSortRec()
        -------------
        Recursively sorts arr[left..right).

        Base case:
          - If the range has 0 or 1 elements (right - left <= 1), it is already sorted.

        Recursive case:
          - Split at mid
          - Sort each half
          - Merge the sorted halves back together
    */
    private void mergeSortRec(int[] arr, int[] tmp, int left, int right) {
        if (right - left <= 1) return;

        int mid = left + (right - left) / 2;

        mergeSortRec(arr, tmp, left, mid);
        mergeSortRec(arr, tmp, mid, right);

        merge(arr, tmp, left, mid, right);
    }

    /*
        merge()
        -------
        Merges two sorted halves into one sorted range.

        Inputs (assumed sorted):
          - arr[left..mid)
          - arr[mid..right)

        Output:
          - arr[left..right) becomes sorted

        Implementation approach:
          1) Merge into tmp[left..right)
          2) Copy tmp[left..right) back into arr[left..right)

        Step counting:
          - compareCount++ for each arr[i] <= arr[j] decision
          - copyCount++ for each write into tmp
          - copyCount++ for each copy back into arr
    */
    private void merge(int[] arr, int[] tmp, int left, int mid, int right) {
        int i = left;   // pointer for left half
        int j = mid;    // pointer for right half
        int k = left;   // pointer for output region in tmp

        // Merge while both halves still have elements
        while (i < mid && j < right) {
            compareCount++; // arr[i] <= arr[j] comparison

            if (arr[i] <= arr[j]) {
                tmp[k++] = arr[i++];
            } else {
                tmp[k++] = arr[j++];
            }

            copyCount++; // wrote one element into tmp
        }

        // Copy remaining elements from left half (if any)
        while (i < mid) {
            tmp[k++] = arr[i++];
            copyCount++;
        }

        // Copy remaining elements from right half (if any)
        while (j < right) {
            tmp[k++] = arr[j++];
            copyCount++;
        }

        // Copy merged block back into original array
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
    /*
        loadFile()
        ----------
        Attempts to locate and load an integer file (unordered.txt / ordered.txt).

        Candidate locations:
          1) Relative: ../data/<name>
             - Works when the working directory is the lesson folder.

          2) Explicit path under Desktop:
             - Matches a known local project layout.
             - Uses user.home so it adapts per machine username.

        Behavior:
          - Prints what it’s trying (helpful debugging for path issues).
          - Throws IOException if neither candidate exists.
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

        // Neither location exists → fail with a clear error message
        throw new IOException("Missing file '" + name + "' at:\n  "
                + rel.toAbsolutePath() + "\n  " + explicit.toAbsolutePath());
    }

    /*
        readInts()
        ----------
        Reads a text file and parses integers separated by whitespace.

        Implementation notes:
          - Streams file line-by-line with Files.lines() (good for large files).
          - Splits each line on one-or-more whitespace characters (\\s+).
          - Filters out any empty tokens (extra safety).
          - Parses tokens to int and returns a primitive int[].

        try-with-resources:
          - Ensures the Stream (and underlying file handle) is closed automatically.
    */
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
    /*
        main()
        ------
        Script-style demo:
          1) Print working directory (helps diagnose relative path problems)
          2) Load unordered + ordered reference outputs
          3) Copy unordered into a working array (so original stays intact)
          4) Run merge sort with step counting
          5) Validate correctness by comparing to ordered.txt
          6) Print counters and a short sample output

        Error handling:
          - Any IOException from loadFile/readInts is caught and printed cleanly.
    */
    public static void main(String[] args) {
        System.out.println("Working directory: " + Paths.get("").toAbsolutePath());

        try {
            int[] unordered = loadFile("unordered.txt");
            int[] expected  = loadFile("ordered.txt");

            System.out.println("Loaded unordered length: " + unordered.length);
            System.out.println("Loaded ordered   length: " + expected.length);

            merge_sort sorter = new merge_sort();

            // Work on a copy so the original unordered array remains available if needed
            int[] arr = Arrays.copyOf(unordered, unordered.length);

            sorter.mergeSort(arr);

            // Validate: does our sorted output match the known-good ordered.txt?
            boolean ok = Arrays.equals(arr, expected);
            System.out.println("Sorted correctly? " + ok);

            // Print instrumentation counts
            System.out.println("Comparisons: " + sorter.getCompareCount());
            System.out.println("Copies:      " + sorter.getCopyCount());

            // Show a small sample of the sorted output
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
