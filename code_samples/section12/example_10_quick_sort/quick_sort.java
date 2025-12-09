package code_samples.section12.example_10_quick_sort;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class quick_sort {

    // Global counters for illustrating complexity
    private static long comparisons = 0;
    private static long writes = 0;

    // ------------------------------------------------------------
    // Quick Sort with counters
    // ------------------------------------------------------------

    public static void quickSort(int[] arr) {
        comparisons = 0;
        writes = 0;
        if (arr.length > 1) {
            quickSortRec(arr, 0, arr.length - 1);
        }
    }

    private static void quickSortRec(int[] arr, int left, int right) {
        if (left < right) {
            int p = partition(arr, left, right);
            quickSortRec(arr, left, p - 1);
            quickSortRec(arr, p + 1, right);
        }
    }

    // Lomuto partition
    private static int partition(int[] arr, int left, int right) {
        int pivot = arr[right];
        int i = left;

        for (int j = left; j < right; j++) {
            comparisons++;                // compare arr[j] <= pivot
            if (arr[j] <= pivot) {
                swap(arr, i, j);          // count writes inside swap
                i++;
            }
        }

        swap(arr, i, right);
        return i;
    }

    private static void swap(int[] arr, int i, int j) {
        if (i == j) return; // no writes if same index
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        writes += 3;        // treat swap as 3 writes
    }

    // ------------------------------------------------------------
    // File loading helper
    // ------------------------------------------------------------

    private static int[] loadFile(String filename) {

        String[] prefixes = new String[] {
            "",   // local folder

            // most common, matches your other languages
            "data" + File.separator,

            // relative to example_10_quick_sort/
            ".." + File.separator + "data" + File.separator,
            ".." + File.separator + "section12" + File.separator + "data" + File.separator,

            // relative if run from project root
            "code_samples" + File.separator + "section12" + File.separator + "data" + File.separator,
            ".." + File.separator +
                "code_samples" + File.separator +
                "section12" + File.separator +
                "data" + File.separator
        };

        List<String> tried = new ArrayList<>();
        File found = null;

        for (String prefix : prefixes) {
            File f = new File(prefix + filename);
            tried.add(f.getPath());
            if (f.exists()) {
                System.out.println("Loaded: " + f.getPath());
                found = f;
                break;
            }
        }

        if (found == null) {
            System.out.println("Error reading: " + filename);
            System.out.println("Working directory: " + System.getProperty("user.dir"));
            System.out.println("Search paths attempted:");
            for (String p : tried) System.out.println("  " + p);
            System.out.println("Missing input file - aborting.");
            System.exit(1);
        }

        List<Integer> nums = new ArrayList<>();

        try (Scanner sc = new Scanner(found)) {
            while (sc.hasNext()) {
                if (sc.hasNextInt()) nums.add(sc.nextInt());
                else sc.next();
            }
        } catch (Exception e) {
            System.out.println("Error reading: " + found.getPath());
            e.printStackTrace();
            System.exit(1);
        }

        int[] arr = new int[nums.size()];
        for (int i = 0; i < nums.size(); i++) arr[i] = nums.get(i);

        return arr;
    }

    // ------------------------------------------------------------
    // MAIN: load unordered / ordered, sort, compare, print stats
    // ------------------------------------------------------------

    public static void main(String[] args) {
        // Load test data
        int[] unordered = loadFile("unordered.txt");
        int[] expected  = loadFile("ordered.txt");

        if (unordered.length != expected.length) {
            System.out.println("Length mismatch: unordered="
                    + unordered.length + ", ordered=" + expected.length);
            System.out.println("Aborting.");
            return;
        }

        // Copy and sort
        int[] arr = unordered.clone();
        quickSort(arr);

        // Validate
        boolean ok = true;
        int badIndex = -1;
        for (int i = 0; i < arr.length; i++) {
            if (arr[i] != expected[i]) {
                ok = false;
                badIndex = i;
                break;
            }
        }

        System.out.println();
        System.out.println("Quick Sort (Java)");
        System.out.println("-----------------");
        System.out.println("Elements:     " + arr.length);
        System.out.println("Comparisons:  " + comparisons);
        System.out.println("Writes:       " + writes);
        System.out.println("Correct?      " + (ok ? "YES" : "NO"));

        if (!ok) {
            System.out.println("First mismatch at index " + badIndex +
                    ": got " + arr[badIndex] +
                    ", expected " + expected[badIndex]);
        } else {
            System.out.println("\nFirst 10 sorted values:");
            for (int i = 0; i < Math.min(10, arr.length); i++) {
                System.out.print(arr[i] + " ");
            }
            System.out.println();
        }
    }
}
