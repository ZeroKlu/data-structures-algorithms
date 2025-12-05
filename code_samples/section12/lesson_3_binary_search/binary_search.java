package code_samples.section12.lesson_3_binary_search;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class binary_search {

    // ----------------------------------------------------
    // Given binary search
    // ----------------------------------------------------
    int binarySearch(int[] arr, int target) {
        int lo = 0, hi = arr.length - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (arr[mid] == target) {
                return mid;
            } else if (arr[mid] < target) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        return -1;
    }

    // Same algorithm, but counts how many iterations (steps)
    int binarySearchSteps(int[] arr, int target, int[] stepsOut) {
        int lo = 0, hi = arr.length - 1;
        int steps = 0;

        while (lo <= hi) {
            steps++;
            int mid = lo + (hi - lo) / 2;
            if (arr[mid] == target) {
                stepsOut[0] = steps;
                return mid;
            } else if (arr[mid] < target) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        stepsOut[0] = steps;
        return -1;
    }

    // ----------------------------------------------------
    // Load integers from file, using project root (user.dir)
    // ----------------------------------------------------
    static int[] loadOrderedFromProjectRoot() {
        String root = System.getProperty("user.dir");
        // This assumes your project structure:
        // <root>/code_samples/section12/data/ordered.txt
        File file = new File(root
                + File.separator + "code_samples"
                + File.separator + "section12"
                + File.separator + "data"
                + File.separator + "ordered.txt");

        System.out.println("Attempting to read: " + file.getAbsolutePath());

        List<Integer> list = new ArrayList<>();
        try (Scanner sc = new Scanner(file)) {
            while (sc.hasNextInt()) {
                list.add(sc.nextInt());
            }
        } catch (FileNotFoundException e) {
            System.out.println("Error reading ordered.txt");
            e.printStackTrace();
            return new int[0];
        }

        int[] arr = new int[list.size()];
        for (int i = 0; i < list.size(); i++) {
            arr[i] = list.get(i);
        }
        return arr;
    }

    // ----------------------------------------------------
    // MAIN TESTS
    // ----------------------------------------------------
    public static void main(String[] args) {
        int[] arr = loadOrderedFromProjectRoot();
        if (arr.length == 0) {
            System.out.println("Missing or empty ordered.txt, aborting.");
            return;
        }

        System.out.println("=== Binary Search Tests ===");
        System.out.println("Loaded " + arr.length + " integers\n");

        binary_search bs = new binary_search();
        int[] steps = new int[1];

        // First element
        int idx = bs.binarySearchSteps(arr, arr[0], steps);
        System.out.println("Search first  (" + arr[0] + "): index=" + idx + ", steps=" + steps[0]);

        // Middle element
        int midVal = arr[arr.length / 2];
        idx = bs.binarySearchSteps(arr, midVal, steps);
        System.out.println("Search middle (" + midVal + "): index=" + idx + ", steps=" + steps[0]);

        // Last element
        int lastVal = arr[arr.length - 1];
        idx = bs.binarySearchSteps(arr, lastVal, steps);
        System.out.println("Search last   (" + lastVal + "): index=" + idx + ", steps=" + steps[0]);

        // Missing element
        idx = bs.binarySearchSteps(arr, 999_999, steps);
        System.out.println("Search missing (999999): index=" + idx + ", steps=" + steps[0]);
    }
}
