package code_samples.section12.example_11_heap_sort;

import java.io.*;
import java.nio.file.*;
import java.util.*;

public class heap_sort {

    // -----------------------------
    // Step counters
    // -----------------------------
    private static long comparisons = 0;
    private static long writes = 0;

    // -----------------------------
    // Swap helper with write count
    // -----------------------------
    private static void swap(int[] arr, int i, int j) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;

        writes += 3;  // temp, arr[i], arr[j]
    }

    // -----------------------------
    // Heapify
    // -----------------------------
    private static void heapify(int[] arr, int n, int i) {
        int largest = i;
        int left  = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n) {
            comparisons++;
            if (arr[left] > arr[largest]) {
                largest = left;
            }
        }
        if (right < n) {
            comparisons++;
            if (arr[right] > arr[largest]) {
                largest = right;
            }
        }

        if (largest != i) {
            swap(arr, i, largest);
            heapify(arr, n, largest);
        }
    }

    // -----------------------------
    // Heap Sort
    // -----------------------------
    public static void heapSort(int[] arr) {
        comparisons = 0;
        writes = 0;

        int n = arr.length;

        // Build max heap
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i);
        }

        // Extract one by one
        for (int end = n - 1; end > 0; end--) {
            swap(arr, 0, end);
            heapify(arr, end, 0);
        }
    }

    // -----------------------------
    // Robust file loader
    // -----------------------------
    private static int[] loadFile(String filename) {
        String wd = System.getProperty("user.dir");

        // Candidate search paths (same strategy that fixed earlier Java issues)
        Path[] attempts = new Path[] {
            Paths.get(filename),
            Paths.get("code_samples", "section12", "data", filename),
            Paths.get("data", filename),
            Paths.get("..", "data", filename),
            Paths.get("..", "..", "data", filename),
            Paths.get("..", "section12", "data", filename),
            Paths.get("..", "..", "section12", "data", filename),
        };

        for (Path path : attempts) {
            File f = path.toFile();
            if (f.exists()) {
                System.out.println("Loaded: " + f.getAbsolutePath());
                try {
                    return Files.lines(f.toPath())
                                .flatMap(line -> Arrays.stream(line.trim().split("\\s+")))
                                .filter(s -> !s.isEmpty())
                                .mapToInt(Integer::parseInt)
                                .toArray();
                } catch (IOException e) {
                    System.out.println("Error reading: " + f.getAbsolutePath());
                }
            }
        }

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

    // -----------------------------
    // Main test driver
    // -----------------------------
    public static void main(String[] args) {

        int[] unordered = loadFile("unordered.txt");
        int[] expected  = loadFile("ordered.txt");

        if (unordered.length != expected.length) {
            System.out.println("Size mismatch between unordered and ordered files!");
            System.exit(1);
        }

        // Copy before sorting
        int[] data = Arrays.copyOf(unordered, unordered.length);

        heapSort(data);

        boolean ok = true;
        for (int i = 0; i < data.length; i++) {
            if (data[i] != expected[i]) {
                System.out.println("Mismatch at index " + i +
                                   ": got " + data[i] +
                                   ", expected " + expected[i]);
                ok = false;
                break;
            }
        }

        System.out.println("\nHeap Sort (Java)");
        System.out.println("----------------");
        System.out.println("Elements:     " + data.length);
        System.out.println("Comparisons:  " + comparisons);
        System.out.println("Writes:       " + writes);
        System.out.println("Correct?      " + (ok ? "YES" : "NO"));
    }
}
