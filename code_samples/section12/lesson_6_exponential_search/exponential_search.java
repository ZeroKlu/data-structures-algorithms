package code_samples.section12.lesson_6_exponential_search;

import java.io.File;
import java.io.FileNotFoundException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class exponential_search {

    // --- Result holder for search index + step count ---
    static class Result {
        final int index;
        final int steps;
        Result(int index, int steps) {
            this.index = index;
            this.steps = steps;
        }
    }

    /**
     * Exponential search on a sorted array with step counting.
     * Steps count "array probes" (how many times we read arr[i]).
     */
    static Result exponentialSearchWithSteps(int[] arr, int target) {
        int n = arr.length;
        int steps = 0;
        if (n == 0) {
            return new Result(-1, steps);
        }

        // Check first element
        int val0 = arr[0];
        steps++;
        if (val0 == target) {
            return new Result(0, steps);
        }

        // Find range for binary search by repeated doubling
        int bound = 1;
        while (bound < n) {
            int v = arr[bound];
            steps++;
            if (v < target) {
                bound *= 2;
            } else {
                break;
            }
        }

        int left = bound / 2;
        int right = Math.min(bound, n - 1);

        // Binary search in [left, right] (reusing the existing steps count)
        return binarySearchRangeWithSteps(arr, left, right, target, steps);
    }

    /**
     * Binary search restricted to [lo, hi], continuing step count.
     */
    static Result binarySearchRangeWithSteps(int[] arr, int lo, int hi, int target, int stepsSoFar) {
        int steps = stepsSoFar;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int val = arr[mid];
            steps++; // one array probe

            if (val == target) {
                return new Result(mid, steps);
            } else if (val < target) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        return new Result(-1, steps);
    }

    // --- File loading helpers ------------------------------------------------

    /**
     * Try multiple candidate paths to load ordered.txt:
     *  1) ../data/ordered.txt  (when running from lesson_6_exponential_search)
     *  2) code_samples/section12/data/ordered.txt (when running from workspace root)
     */
    static int[] loadOrderedFile() {
        String[] candidates = new String[] {
            ".." + File.separator + "data" + File.separator + "ordered.txt",
            "code_samples" + File.separator + "section12" + File.separator + "data" + File.separator + "ordered.txt"
        };

        System.out.println("Working directory: " + System.getProperty("user.dir"));

        for (String rel : candidates) {
            Path p = Paths.get(rel).normalize().toAbsolutePath();
            System.out.println("Trying: " + p);
            if (Files.exists(p)) {
                try (Scanner sc = new Scanner(new File(p.toString()))) {
                    List<Integer> list = new ArrayList<>();
                    while (sc.hasNextInt()) {
                        list.add(sc.nextInt());
                    }
                    int[] arr = new int[list.size()];
                    for (int i = 0; i < list.size(); i++) {
                        arr[i] = list.get(i);
                    }
                    System.out.println("Loaded " + arr.length + " integers from " + p);
                    return arr;
                } catch (FileNotFoundException e) {
                    System.out.println("Unexpected FileNotFound after exists() for: " + p);
                }
            } else {
                System.out.println("  Not found at: " + p);
            }
        }

        System.out.println("Missing input file — could not locate ordered.txt via any candidate path.");
        return null;
    }

    // --- Demo / tests --------------------------------------------------------

    public static void main(String[] args) {
        int[] arr = loadOrderedFile();
        if (arr == null) {
            // Already printed diagnostics
            return;
        }

        int[] testTargets = new int[] {
            -5000,   // first element (best case)
            0,       // middle
            5000,    // last element
            6000     // not present
        };

        for (int target : testTargets) {
            Result res = exponentialSearchWithSteps(arr, target);
            System.out.printf("Target %d -> index = %d, steps = %d%n",
                    target, res.index, res.steps);
        }
    }
}
