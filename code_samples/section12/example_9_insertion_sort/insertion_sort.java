package code_samples.section12.example_9_insertion_sort;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class insertion_sort {

    // ------------------------------------------------------------------
    // Insertion Sort with step counting
    // ------------------------------------------------------------------
    //
    // What this function does:
    //   - Sorts the array in-place using INSERTION SORT.
    //   - Tracks how much "work" the algorithm performs using step counters.
    //
    // How insertion sort works (high level):
    //   - Think of the left side of the array as a growing "sorted region".
    //   - For each i from 1..end:
    //       1) Save arr[i] as key
    //       2) Shift larger values in the sorted region one position to the right
    //       3) Insert key into its correct position
    //
    // Step counters in this implementation:
    //   stats[0] = comparisons
    //       - Counts each comparison of arr[j] > key (i.e., "is this element too big?")
    //
    //   stats[1] = writes
    //       - Counts each write to the array:
    //            (a) shifting arr[j] -> arr[j+1]
    //            (b) inserting key into arr[j+1]
    //
    // NOTE:
    //   - We do NOT count the assignment "int key = arr[i]" as a write because it
    //     doesn't modify the array; it's a local variable copy.
    //
    // Complexity (Big-O):
    //   - Best case (already sorted):    O(n) comparisons, O(n) writes (just placing keys)
    //   - Average / Worst case:         O(n^2) comparisons and writes
    //
    static void insertionSort(int[] arr, long[] stats) {
        long comparisons = 0;
        long writes = 0;

        // i marks the first element of the "unsorted" region
        // arr[0..i-1] is treated as sorted after each iteration
        for (int i = 1; i < arr.length; i++) {

            // key is the value we are inserting into the sorted region
            int key = arr[i];

            // j scans leftwards through the sorted region
            int j = i - 1;

            // Shift larger elements one step to the right until the insertion
            // position for key is found.
            //
            // Loop invariant idea:
            //   - At each step, elements greater than key are moved right,
            //     making a hole where key will eventually go.
            while (j >= 0) {

                // We are about to evaluate: arr[j] > key
                comparisons++;

                if (arr[j] > key) {
                    // Shift element right by one position
                    arr[j + 1] = arr[j];
                    writes++;

                    // Move left to continue scanning
                    j--;
                } else {
                    // As soon as arr[j] <= key, key belongs just to the right of j
                    break;
                }
            }

            // Insert key into the hole (j+1)
            // This always happens once per outer-loop iteration.
            arr[j + 1] = key;
            writes++;
        }

        // Store results back into stats[] for the caller
        stats[0] = comparisons;
        stats[1] = writes;
    }

    // ------------------------------------------------------------------
    // File loader with fallback directories
    // ------------------------------------------------------------------
    //
    // Goal:
    //   Load integers from a whitespace-separated text file.
    //
    // Why "fallback" paths?
    //   Depending on how you run Java, the *working directory* (user.dir) can vary.
    //   These attempts make it more likely the program finds the data files.
    //
    // Search order:
    //   1) ./<name>                     (current folder)
    //   2) ./data/<name>                (local data folder)
    //   3) ../data/<name>               (sibling data folder)
    //   4) ../../data/<name>            (one more level up)
    //   5) ~/Desktop/data-structures-algorithms/code_samples/section12/data/<name>
    //      (explicit location used in your environment)
    //
    // Parsing approach:
    //   - Read all lines from the file
    //   - Split each line on whitespace
    //   - Convert tokens -> ints
    //   - Return int[] of all numbers in file
    //
    private static int[] loadFile(String name) throws IOException {
        List<String> attempts = new ArrayList<>();

        // 1) current folder (./unordered.txt, ./ordered.txt)
        attempts.add(name);

        // 2) ./data/unordered.txt
        attempts.add("data/" + name);

        // 3) ../data/unordered.txt
        attempts.add("../data/" + name);

        // 4) ../../data/unordered.txt
        attempts.add("../../data/" + name);

        // 5) explicit desktop project layout (Windows/Mac/Linux home dir)
        String home = System.getProperty("user.home");
        if (home != null) {
            attempts.add(home + "/Desktop/data-structures-algorithms/code_samples/section12/data/" + name);
        }

        IOException lastError = null;

        // Try each candidate path until one works
        for (String path : attempts) {
            try {
                Path p = Paths.get(path);

                // Read entire file (line-by-line) into memory
                List<String> lines = Files.readAllLines(p);

                // Parse integers from all lines
                List<Integer> nums = new ArrayList<>();
                for (String line : lines) {

                    // split on whitespace; trim() avoids leading/trailing whitespace issues
                    for (String part : line.trim().split("\\s+")) {
                        if (!part.isEmpty()) {
                            nums.add(Integer.parseInt(part));
                        }
                    }
                }

                // Convert List<Integer> -> int[]
                int[] arr = new int[nums.size()];
                for (int i = 0; i < nums.size(); i++) {
                    arr[i] = nums.get(i);
                }

                System.out.println("Loaded: " + p.toAbsolutePath());
                return arr;

            } catch (IOException ex) {
                // Keep the last IO error so we can display something useful later
                lastError = ex;
            } catch (NumberFormatException ex) {
                // If a file contains something that isn't an integer, treat as fatal
                throw new IOException("Failed to parse integers in: " + path + " (" + ex.getMessage() + ")");
            }
        }

        // If we reach here, all attempts failed
        System.err.println("Error reading input file: " + name);
        System.err.println("Paths attempted:");
        for (String a : attempts) {
            System.err.println("  " + Paths.get(a).toAbsolutePath());
        }

        if (lastError != null) {
            System.err.println("Last error: " + lastError.getMessage());
        }

        throw new IOException("Missing input file: " + name);
    }

    // ------------------------------------------------------------------
    // Main test harness
    // ------------------------------------------------------------------
    //
    // What main() does:
    //   1) Load unordered.txt and ordered.txt
    //   2) Copy unordered -> arr (so we don't destroy the original)
    //   3) Run insertionSort(arr, stats)
    //   4) Compare arr against expected (ordered.txt)
    //   5) Print step counts + PASS/FAIL
    //
    public static void main(String[] args) {
        try {
            // Load test data
            int[] unordered = loadFile("unordered.txt");
            int[] expected  = loadFile("ordered.txt");

            // Sanity check: if files contain different counts, comparisons are meaningless
            if (unordered.length != expected.length) {
                System.err.println("Size mismatch: unordered has "
                        + unordered.length + ", ordered has " + expected.length);
                return;
            }

            // Work on a copy (keep original unordered array unchanged)
            int[] arr = Arrays.copyOf(unordered, unordered.length);

            // stats[0] = comparisons, stats[1] = writes
            long[] stats = new long[2];

            // Sort + count steps
            insertionSort(arr, stats);

            // Verify output
            int mismatch = -1;
            for (int i = 0; i < arr.length; i++) {
                if (arr[i] != expected[i]) {
                    mismatch = i;
                    break;
                }
            }

            // Print results
            System.out.println("\nInsertion Sort (Java) Test");
            System.out.println("--------------------------");
            System.out.println("Elements:     " + arr.length);
            System.out.println("Comparisons:  " + stats[0]);
            System.out.println("Writes:       " + stats[1]);

            if (mismatch == -1) {
                System.out.println("Result:       PASS (sorted output matches ordered.txt)");
            } else {
                System.out.println("Result:       FAIL (sorted output does NOT match ordered.txt)");
                System.out.println("First mismatch at index " + mismatch
                        + ": got " + arr[mismatch]
                        + ", expected " + expected[mismatch]);
            }

        } catch (IOException ex) {
            // If loadFile() couldn't locate or parse the file(s)
            System.err.println("\nMissing input file - aborting.");
            System.err.println(ex.getMessage());
        }
    }
}
