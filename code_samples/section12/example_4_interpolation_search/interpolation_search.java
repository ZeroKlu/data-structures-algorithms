package code_samples.section12.example_4_interpolation_search;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class interpolation_search {

    // ======================================================
    // Interpolation Search (index-only)
    // ======================================================
    int interpolationSearch(int[] arr, int target) {
        int n = arr.length;
        if (n == 0) return -1;

        int lo = 0, hi = n - 1;

        while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {
            // All values same
            if (arr[hi] == arr[lo]) {
                return (arr[lo] == target) ? lo : -1;
            }

            // Estimate the position
            int pos = lo + (int)((double)(hi - lo) * (target - arr[lo])
                    / (arr[hi] - arr[lo]));

            if (pos < lo || pos > hi) return -1;

            if (arr[pos] == target) return pos;
            else if (arr[pos] < target) lo = pos + 1;
            else hi = pos - 1;
        }

        return -1;
    }

    // ======================================================
    // Interpolation Search WITH step counting
    // ======================================================
    int interpolationSearchSteps(int[] arr, int target, int[] stepsOut) {
        int steps = 0;
        int n = arr.length;
        if (n == 0) {
            stepsOut[0] = 0;
            return -1;
        }

        int lo = 0, hi = n - 1;

        while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {

            steps++; // Compare arr[hi] == arr[lo]
            if (arr[hi] == arr[lo]) {
                steps++; // Compare arr[lo] == target
                stepsOut[0] = steps;
                return (arr[lo] == target) ? lo : -1;
            }

            int pos = lo + (int)((double)(hi - lo) * (target - arr[lo])
                    / (arr[hi] - arr[lo]));

            if (pos < lo || pos > hi) {
                stepsOut[0] = steps;
                return -1;
            }

            steps++; // Compare arr[pos] == target
            if (arr[pos] == target) {
                stepsOut[0] = steps;
                return pos;
            }

            steps++; // Compare arr[pos] < target
            if (arr[pos] < target) lo = pos + 1;
            else hi = pos - 1;
        }

        stepsOut[0] = steps;
        return -1;
    }

    // ======================================================
    // Load ordered.txt relative to project directory
    // ======================================================
    static int[] loadOrderedData() {
        // Expected structure:
        // code_samples/section12/data/ordered.txt
        String root = System.getProperty("user.dir");
        String path = root
                + File.separator + "code_samples"
                + File.separator + "section12"
                + File.separator + "data"
                + File.separator + "ordered.txt";

        System.out.println("Attempting to read: " + path);

        List<Integer> list = new ArrayList<>();
        try (Scanner sc = new Scanner(new File(path))) {
            while (sc.hasNextInt()) {
                list.add(sc.nextInt());
            }
        } catch (FileNotFoundException e) {
            System.out.println("Error reading ordered.txt: " + e.getMessage());
            return new int[0];
        }

        // Convert list → array
        int[] arr = new int[list.size()];
        for (int i = 0; i < arr.length; i++) arr[i] = list.get(i);
        return arr;
    }

    // ======================================================
    // MAIN Test Harness
    // ======================================================
    public static void main(String[] args) {

        int[] arr = loadOrderedData();
        if (arr.length == 0) {
            System.out.println("Missing input file - aborting.");
            return;
        }

        interpolation_search is = new interpolation_search();
        int[] steps = new int[1];

        System.out.println("\n=== Interpolation Search Tests (sorted data only) ===");
        System.out.println("Loaded " + arr.length + " integers\n");

        // First element
        int idx = is.interpolationSearchSteps(arr, arr[0], steps);
        System.out.println("Search first  (" + arr[0] + "): index=" + idx + ", steps=" + steps[0]);

        // Middle element
        int midVal = arr[arr.length / 2];
        idx = is.interpolationSearchSteps(arr, midVal, steps);
        System.out.println("Search middle (" + midVal + "): index=" + idx + ", steps=" + steps[0]);

        // Last element
        int lastVal = arr[arr.length - 1];
        idx = is.interpolationSearchSteps(arr, lastVal, steps);
        System.out.println("Search last   (" + lastVal + "): index=" + idx + ", steps=" + steps[0]);

        // Missing element
        idx = is.interpolationSearchSteps(arr, 999_999, steps);
        System.out.println("Search missing (999999): index=" + idx + ", steps=" + steps[0]);
    }
}
