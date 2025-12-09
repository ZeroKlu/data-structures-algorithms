package code_samples.section12.example_9_insertion_sort;

import java.io.*;
import java.nio.file.*;
import java.util.*;

public class insertion_sort {

    // ------------------------------------------------------------------
    // Insertion Sort with step counting
    // ------------------------------------------------------------------
    static void insertionSort(int[] arr, long[] stats) {
        // stats[0] = comparisons, stats[1] = writes
        long comparisons = 0;
        long writes = 0;

        for (int i = 1; i < arr.length; i++) {
            int key = arr[i];
            int j = i - 1;

            while (j >= 0) {
                comparisons++;
                if (arr[j] > key) {
                    arr[j + 1] = arr[j];
                    writes++;
                    j--;
                } else {
                    break;
                }
            }

            arr[j + 1] = key;
            writes++;
        }

        stats[0] = comparisons;
        stats[1] = writes;
    }

    // ------------------------------------------------------------------
    // File loader with fallback directories
    // ------------------------------------------------------------------
    private static int[] loadFile(String name) throws IOException {
        List<String> attempts = new ArrayList<>();

        // 1) Try local folder
        attempts.add(name);

        // 2) Try ./data/
        attempts.add("data/" + name);

        // 3) Try ../data/
        attempts.add("../data/" + name);

        // 4) Try ../../data/
        attempts.add("../../data/" + name);

        // 5) Try Desktop/data/
        String home = System.getProperty("user.home");
        if (home != null) {
            attempts.add(home + "/Desktop/data-structures-algorithms/code_samples/section12/data/" + name);
        }

        IOException lastError = null;

        for (String path : attempts) {
            try {
                Path p = Paths.get(path);
                List<String> tokens = Files.readAllLines(p);

                List<Integer> nums = new ArrayList<>();
                for (String line : tokens) {
                    for (String part : line.trim().split("\\s+")) {
                        if (!part.isEmpty()) {
                            nums.add(Integer.parseInt(part));
                        }
                    }
                }

                int[] arr = new int[nums.size()];
                for (int i = 0; i < nums.size(); i++) arr[i] = nums.get(i);

                System.out.println("Loaded: " + p.toAbsolutePath());
                return arr;

            } catch (IOException ex) {
                lastError = ex;
            }
        }

        System.err.println("Error reading input file: " + name);
        if (lastError != null) {
            System.err.println(lastError.getMessage());
        }
        throw new IOException("Missing input file: " + name);
    }

    // ------------------------------------------------------------------
    // Main test harness
    // ------------------------------------------------------------------
    public static void main(String[] args) {
        try {
            int[] unordered = loadFile("unordered.txt");
            int[] expected = loadFile("ordered.txt");

            if (unordered.length != expected.length) {
                System.err.println("Size mismatch: unordered has "
                        + unordered.length + ", ordered has " + expected.length);
                return;
            }

            int[] arr = Arrays.copyOf(unordered, unordered.length);
            long[] stats = new long[2]; // [comparisons, writes]

            insertionSort(arr, stats);

            // Verify
            int mismatch = -1;
            for (int i = 0; i < arr.length; i++) {
                if (arr[i] != expected[i]) {
                    mismatch = i;
                    break;
                }
            }

            System.out.println("\nInsertion Sort (Java) Test");
            System.out.println("--------------------------");
            System.out.println("Elements:     " + arr.length);
            System.out.println("Comparisons:  " + stats[0]);
            System.out.println("Writes:       " + stats[1]);

            if (mismatch == -1) {
                System.out.println("Result:       PASS");
            } else {
                System.out.println("Result:       FAIL");
                System.out.println("First mismatch at index " + mismatch +
                                   ": got " + arr[mismatch] +
                                   ", expected " + expected[mismatch]);
            }

        } catch (IOException ex) {
            System.err.println("\nMissing input file - aborting.");
        }
    }
}
