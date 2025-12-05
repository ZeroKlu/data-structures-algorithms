package code_samples.section12.lesson_4_jump_search;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class jump_search {

    // ============================================================
    // Jump Search (index only)
    // ============================================================
    int jumpSearch(int[] arr, int target) {
        int n = arr.length;
        if (n == 0) return -1;

        int step = (int)Math.sqrt(n);
        int prev = 0;
        int curr = step;

        // Jump ahead until arr[curr] >= target
        while (curr < n && arr[curr] < target) {
            prev = curr;
            curr += step;
        }

        if (curr >= n) curr = n - 1;

        // Linear search through block
        for (int i = prev; i <= curr; i++) {
            if (arr[i] == target) return i;
        }
        return -1;
    }

    // ============================================================
    // Jump Search WITH step counting
    // stepsOut[0] = number of comparisons
    // ============================================================
    int jumpSearchSteps(int[] arr, int target, int[] stepsOut) {
        int n = arr.length;
        int steps = 0;

        if (n == 0) {
            stepsOut[0] = 0;
            return -1;
        }

        int step = (int)Math.sqrt(n);
        int prev = 0;
        int curr = step;

        // Jump phase
        while (curr < n) {
            steps++; // compare arr[curr] < target

            if (arr[curr] >= target)
                break;

            prev = curr;
            curr += step;
        }

        if (curr >= n) curr = n - 1;

        // Linear search phase
        for (int i = prev; i <= curr; i++) {
            steps++; // compare arr[i] == target
            if (arr[i] == target) {
                stepsOut[0] = steps;
                return i;
            }
        }

        stepsOut[0] = steps;
        return -1;
    }

    // ============================================================
    // Load ordered.txt relative to project directory
    // ============================================================
    static int[] loadOrderedData() {
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
            System.out.println("Error reading ordered.txt");
            e.printStackTrace();
            return new int[0];
        }

        int[] arr = new int[list.size()];
        for (int i = 0; i < arr.length; i++) arr[i] = list.get(i);
        return arr;
    }

    // ============================================================
    // MAIN TESTS
    // ============================================================
    public static void main(String[] args) {
        int[] arr = loadOrderedData();

        if (arr.length == 0) {
            System.out.println("Missing input file — aborting.");
            return;
        }

        jump_search js = new jump_search();

        int[] steps = new int[1];
        int idx;

        System.out.println("=== Jump Search Tests ===");
        System.out.println("Loaded " + arr.length + " integers\n");

        // First element
        idx = js.jumpSearchSteps(arr, arr[0], steps);
        System.out.println("Search first  (" + arr[0] + "): index=" + idx + ", steps=" + steps[0]);

        // Middle element
        int midVal = arr[arr.length / 2];
        idx = js.jumpSearchSteps(arr, midVal, steps);
        System.out.println("Search middle (" + midVal + "): index=" + idx + ", steps=" + steps[0]);

        // Last element
        int lastVal = arr[arr.length - 1];
        idx = js.jumpSearchSteps(arr, lastVal, steps);
        System.out.println("Search last   (" + lastVal + "): index=" + idx + ", steps=" + steps[0]);

        // Missing element
        idx = js.jumpSearchSteps(arr, 999_999, steps);
        System.out.println("Search missing (999999): index=" + idx + ", steps=" + steps[0]);
    }
}
