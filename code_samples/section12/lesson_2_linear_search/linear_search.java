package code_samples.section12.lesson_2_linear_search;

import java.io.*;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

public class linear_search {

    int linearSearch(int[] arr, int target) {
        for (int i = 0; i < arr.length; i++) {
            if (arr[i] == target) {
                return i;
            }
        }
        return -1;
    }

    int linearSearchCountSteps(int[] arr, int target, int[] outSteps) {
        int steps = 0;
        for (int i = 0; i < arr.length; i++) {
            steps++;
            if (arr[i] == target) {
                outSteps[0] = steps;
                return i;
            }
        }
        outSteps[0] = steps;
        return -1;
    }

    int[] loadFile(Path path) {
        List<Integer> list = new ArrayList<>();
        File file = path.toFile();
        try (Scanner sc = new Scanner(file)) {
            while (sc.hasNextInt()) {
                list.add(sc.nextInt());
            }
        } catch (FileNotFoundException e) {
            System.out.println("Error reading: " + file.getPath());
            System.out.println("Working directory: " + System.getProperty("user.dir"));
            return new int[0];
        }
        return list.stream().mapToInt(i -> i).toArray();
    }

    void runDataset(String name, int[] arr) {
        System.out.println("=== " + name + " ===");
        System.out.println("Loaded " + arr.length + " integers\n");

        int[] steps = new int[1];

        int idx1 = linearSearchCountSteps(arr, arr[0], steps);
        System.out.printf("Search first (%d): index=%d, steps=%d%n",
                arr[0], idx1, steps[0]);

        int midVal = arr[arr.length / 2];
        int idx2 = linearSearchCountSteps(arr, midVal, steps);
        System.out.printf("Search middle (%d): index=%d, steps=%d%n",
                midVal, idx2, steps[0]);

        int lastVal = arr[arr.length - 1];
        int idx3 = linearSearchCountSteps(arr, lastVal, steps);
        System.out.printf("Search last (%d): index=%d, steps=%d%n",
                lastVal, idx3, steps[0]);

        int idx4 = linearSearchCountSteps(arr, 999999, steps);
        System.out.printf("Search missing (999999): index=%d, steps=%d%n%n",
                idx4, steps[0]);
    }

    public void runTests() {
        // From working dir C:\Users\smclean\Desktop\data-structures-algorithms
        // data lives in code_samples\section12\data
        Path orderedPath   = Paths.get("code_samples", "section12", "data", "ordered.txt");
        Path unorderedPath = Paths.get("code_samples", "section12", "data", "unordered.txt");

        int[] ordered   = loadFile(orderedPath);
        int[] unordered = loadFile(unorderedPath);

        if (ordered.length == 0 || unordered.length == 0) {
            System.out.println("Missing input files (see error messages above for paths).");
            return;
        }

        runDataset("Ordered Data", ordered);
        runDataset("Unordered Data", unordered);
    }

    public static void main(String[] args) {
        new linear_search().runTests();
    }
}
