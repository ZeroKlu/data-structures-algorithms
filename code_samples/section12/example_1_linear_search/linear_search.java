package code_samples.section12.example_1_linear_search;

import java.io.*;              // Provides file I/O classes (File, FileNotFoundException)
import java.nio.file.Path;     // Represents file system paths
import java.nio.file.Paths;   // Utility class for creating Path instances
import java.util.*;            // Provides collections, Scanner, and utility classes

/**
 * Demonstrates linear search on integer datasets,
 * including step-counting to measure search cost.
 */
public class linear_search {

    /**
     * Performs a basic linear search on an integer array.
     *
     * @param arr     Array to search
     * @param target  Value to locate
     *
     * @return Index of the target if found; otherwise -1
     */
    int linearSearch(int[] arr, int target) {
        // Iterate through the array sequentially
        for (int i = 0; i < arr.length; i++) {

            // Check whether the current element matches the target
            if (arr[i] == target) {
                return i; // Return index immediately upon finding a match
            }
        }

        // Target was not found in the array
        return -1;
    }

    /**
     * Performs a linear search while counting comparison steps.
     *
     * @param arr       Array to search
     * @param target    Value to locate
     * @param outSteps  Single-element array used to return step count
     *
     * @return Index of the target if found; otherwise -1
     */
    int linearSearchCountSteps(int[] arr, int target, int[] outSteps) {
        int steps = 0; // Counts how many comparisons are made

        // Iterate through each element in order
        for (int i = 0; i < arr.length; i++) {
            steps++; // Count the comparison arr[i] == target

            // Check if current element matches the target
            if (arr[i] == target) {
                outSteps[0] = steps; // Store total steps taken
                return i;            // Return index of the found element
            }
        }

        // Target was not found after scanning entire array
        outSteps[0] = steps;         // Total comparisons equals array length
        return -1;
    }

    /**
     * Loads integers from a text file into an array.
     *
     * @param path  Path to the input file
     *
     * @return Array of integers read from the file,
     *         or an empty array if the file cannot be read
     */
    int[] loadFile(Path path) {
        List<Integer> list = new ArrayList<>(); // Temporary dynamic storage
        File file = path.toFile();               // Convert Path to File

        // Use try-with-resources to ensure the Scanner is closed automatically
        try (Scanner sc = new Scanner(file)) {

            // Read integers until no more are available
            while (sc.hasNextInt()) {
                list.add(sc.nextInt());
            }

        } catch (FileNotFoundException e) {
            // Provide diagnostic information if file cannot be found
            System.out.println("Error reading: " + file.getPath());
            System.out.println("Working directory: " + System.getProperty("user.dir"));
            return new int[0]; // Return empty array on failure
        }

        // Convert List<Integer> to primitive int[] array
        return list.stream().mapToInt(i -> i).toArray();
    }

    /**
     * Runs a standard set of linear search tests on a dataset.
     *
     * @param name  Descriptive name of the dataset (used in output)
     * @param arr   Integer array containing the dataset
     */
    void runDataset(String name, int[] arr) {
        System.out.println("=== " + name + " ===");
        System.out.println("Loaded " + arr.length + " integers\n");

        // Single-element array used to retrieve step count by reference
        int[] steps = new int[1];

        // Test 1: Search for the first element (best-case scenario)
        int idx1 = linearSearchCountSteps(arr, arr[0], steps);
        System.out.printf("Search first (%d): index=%d, steps=%d%n",
                arr[0], idx1, steps[0]);

        // Test 2: Search for the middle element (average-case scenario)
        int midVal = arr[arr.length / 2];
        int idx2 = linearSearchCountSteps(arr, midVal, steps);
        System.out.printf("Search middle (%d): index=%d, steps=%d%n",
                midVal, idx2, steps[0]);

        // Test 3: Search for the last element (worst-case successful search)
        int lastVal = arr[arr.length - 1];
        int idx3 = linearSearchCountSteps(arr, lastVal, steps);
        System.out.printf("Search last (%d): index=%d, steps=%d%n",
                lastVal, idx3, steps[0]);

        // Test 4: Search for a value guaranteed not to exist
        int idx4 = linearSearchCountSteps(arr, 999999, steps);
        System.out.printf("Search missing (999999): index=%d, steps=%d%n%n",
                idx4, steps[0]);
    }

    /**
     * Coordinates loading datasets and running search tests.
     */
    public void runTests() {
        // Expected working directory:
        //   C:\Users\smclean\Desktop\data-structures-algorithms
        // Data files live in:
        //   code_samples\section12\data

        // Build platform-independent paths to data files
        Path orderedPath   = Paths.get("code_samples", "section12", "data", "ordered.txt");
        Path unorderedPath = Paths.get("code_samples", "section12", "data", "unordered.txt");

        // Load both datasets from disk
        int[] ordered   = loadFile(orderedPath);
        int[] unordered = loadFile(unorderedPath);

        // Validate that both datasets were loaded successfully
        if (ordered.length == 0 || unordered.length == 0) {
            System.out.println("Missing input files (see error messages above for paths).");
            return; // Exit early if data could not be loaded
        }

        // Run linear search benchmarks on each dataset
        runDataset("Ordered Data", ordered);
        runDataset("Unordered Data", unordered);
    }

    /**
     * Program entry point.
     */
    public static void main(String[] args) {
        // Instantiate the class and execute test suite
        new linear_search().runTests();
    }
}
