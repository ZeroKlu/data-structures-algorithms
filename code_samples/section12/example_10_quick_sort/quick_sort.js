// ------------------------------------------------------------
// Example 10 - Quick Sort (JavaScript / Node.js ES Modules)
// ------------------------------------------------------------
//
// PURPOSE
// -------
// Demonstrate Quick Sort on integer data while counting steps to help
// illustrate time complexity.
//
// This script expects TWO files containing the SAME integers:
//   - unordered.txt  (unsorted input)
//   - ordered.txt    (the same integers sorted ascending)
//
// Workflow:
//   1) Load unordered + ordered using a robust multi-path search
//   2) Quick-sort a COPY of unordered (do not mutate input)
//   3) Compare the sorted output against ordered.txt
//   4) Print: element count, comparisons, writes, PASS/FAIL
//
// INPUT FORMAT
// ------------
// Files contain integers separated by whitespace (spaces/newlines/tabs).
//
// STEP COUNTING MODEL
// -------------------
// We count two metrics:
//
//   comparisons:
//     - incremented each time we compare an array element to the pivot
//       inside partition()
//
//   writes:
//     - incremented when we mutate the array due to swaps
//
// NOTE ABOUT SWAP COUNTING (IMPORTANT)
// ----------------------------------
// This implementation uses destructuring assignment swaps:
//     [arr[i], arr[j]] = [arr[j], arr[i]]
//
// Conceptually, a swap writes to TWO array positions: arr[i] and arr[j].
// So we count that as 2 writes (not 3).
//
// In some other languages/examples you used "3 writes" because a swap
// typically uses a temp variable (tmp = a; a = b; b = tmp).
// In JavaScript, the temp is "hidden" by destructuring, but the array
// still gets two positions overwritten. So counting "2 writes" is a
// reasonable teaching model for JS.
//
// ALGORITHM NOTES
// ---------------
// - Partition scheme: Lomuto partition
// - Pivot choice: last element in the range (arr[right])
// - Average time: O(n log n), worst case: O(n^2) (e.g., bad pivot on
//   already-sorted data).
//
// ------------------------------------------------------------

import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";

// ------------------------------------------------------------
// GLOBAL STEP COUNTERS
// ------------------------------------------------------------
// Reset before each test run so you can compare outputs cleanly.
let comparisons = 0;
let writes = 0;

// ------------------------------------------------------------
// Quick Sort Implementation (returns a sorted COPY)
// ------------------------------------------------------------

/**
 * quickSort(arr)
 * --------------
 * Returns a sorted COPY of the input array.
 *
 * Why copy?
 * - So the caller still has the original unordered array unchanged.
 * - This makes debugging + testing easier.
 */
function quickSort(arr) {
    const copy = arr.slice(); // shallow copy of numbers
    quickSortRec(copy, 0, copy.length - 1);
    return copy;
}

/**
 * quickSortRec(arr, left, right)
 * ------------------------------
 * In-place recursive quick sort on arr[left..right].
 *
 * Base case:
 * - If left >= right, the sub-array has 0 or 1 element => already sorted.
 *
 * Recursive case:
 * - Partition around a pivot, getting pivot index p
 * - Sort left side  [left .. p-1]
 * - Sort right side [p+1 .. right]
 */
function quickSortRec(arr, left, right) {
    if (left >= right) return;

    const p = partition(arr, left, right);
    quickSortRec(arr, left, p - 1);
    quickSortRec(arr, p + 1, right);
}

/**
 * partition(arr, left, right)  (Lomuto partition)
 * -----------------------------------------------
 * Choose pivot = arr[right].
 *
 * i marks the next position where an element < pivot should be placed.
 * j scans through the range [left .. right-1].
 *
 * For each j:
 * - Compare arr[j] to pivot (counted in comparisons)
 * - If arr[j] < pivot:
 *     swap arr[i] and arr[j]
 *     i++
 *
 * Finally:
 * - swap arr[i] with pivot (arr[right]) so pivot lands at its final position
 *
 * Returns:
 * - the final pivot index i
 *
 * Step counting:
 * - comparisons++ for each arr[j] < pivot check
 * - writes += 2 for each swap (two array slots overwritten)
 */
function partition(arr, left, right) {
    const pivot = arr[right];
    let i = left;

    for (let j = left; j < right; j++) {
        comparisons++; // one comparison: arr[j] < pivot ?

        // NOTE: Using "<" puts values equal to pivot on the RIGHT side.
        // Using "<=" would move equals left, which may change behavior
        // with duplicates. Either is valid, but be consistent across languages.
        if (arr[j] < pivot) {
            // Swap arr[i] and arr[j]
            // This overwrites arr[i] and arr[j] => 2 writes
            [arr[i], arr[j]] = [arr[j], arr[i]];
            writes += 2;
            i++;
        }
    }

    // Final swap to place pivot in correct position
    // Overwrites arr[i] and arr[right] => 2 writes
    [arr[i], arr[right]] = [arr[right], arr[i]];
    writes += 2;

    return i;
}

// ------------------------------------------------------------
// File Loader with Robust Path Searching
// ------------------------------------------------------------

/**
 * loadFile(filename)
 * ------------------
 * Tries multiple likely locations so the script works regardless of where
 * you run it from (lesson folder, project root, etc.).
 *
 * We build absolute paths using:
 * - __dirname (directory of this script)
 * - process.cwd() (current working directory)
 *
 * Returns:
 * - array of integers parsed from the file
 *
 * Exits:
 * - prints diagnostics and process.exit(1) if file can't be found
 */
function loadFile(filename) {
    // __dirname for ES modules
    const __filename = fileURLToPath(import.meta.url);
    const __dirname = path.dirname(__filename);

    // Candidate search paths (absolute)
    const searchPaths = [
        // 1) Same folder as this script
        path.join(__dirname, filename),

        // 2) ./data relative to this script
        path.join(__dirname, "data", filename),

        // 3) ../data relative to this script (common: lesson folder next to data/)
        path.join(__dirname, "..", "data", filename),

        // 4) ../section12/data relative to this script
        path.join(__dirname, "..", "section12", "data", filename),

        // 5) ../../section12/data relative to this script
        path.join(__dirname, "..", "..", "section12", "data", filename),

        // 6) Running from project root: code_samples/section12/data/<file>
        path.join(process.cwd(), "code_samples", "section12", "data", filename),

        // 7) Running from project root with a top-level data folder
        path.join(process.cwd(), "data", filename),
    ];

    const tried = [];

    for (const p of searchPaths) {
        tried.push(p);

        if (fs.existsSync(p)) {
            console.log("Loaded:", p);

            // Read and parse integers
            const text = fs.readFileSync(p, "utf-8");

            // split on ANY whitespace; filter not needed if we trim() first
            return text
                .trim()
                .split(/\s+/)
                .map(Number);
        }
    }

    // If we get here, none of the candidates worked.
    console.error("Error reading:", filename);
    console.log("Working directory:", process.cwd());
    console.log("Search paths attempted:");
    for (const t of tried) console.log("  " + t);
    console.error("Missing input file — aborting.");
    process.exit(1);
}

// ------------------------------------------------------------
// Main / Test Harness
// ------------------------------------------------------------

/**
 * main()
 * ------
 * Loads test data, runs quick sort, verifies correctness, and prints stats.
 */
function main() {
    // Load input + expected output
    const unordered = loadFile("unordered.txt");
    const expected = loadFile("ordered.txt");

    // Reset counters for a clean run
    comparisons = 0;
    writes = 0;

    // Sort a COPY of unordered
    const result = quickSort(unordered);

    // Verify correctness:
    // - same length
    // - every element matches the expected sorted array
    const correct =
        result.length === expected.length &&
        result.every((v, i) => v === expected[i]);

    // Print summary
    console.log("\nQuick Sort (JavaScript)");
    console.log("-----------------------");
    console.log("Elements:     ", unordered.length);
    console.log("Comparisons:  ", comparisons);
    console.log("Writes:       ", writes);
    console.log("Correct?      ", correct ? "YES ✔" : "NO ✘");

    // If incorrect, show the first mismatch (helps debugging quickly)
    if (!correct) {
        console.log("First mismatch (if any):");
        for (let i = 0; i < result.length && i < expected.length; i++) {
            if (result[i] !== expected[i]) {
                console.log(
                    `  index ${i}: got ${result[i]}, expected ${expected[i]}`
                );
                break;
            }
        }
    } else {
        // If correct, show a small sample
        console.log("\nFirst 10 sorted values:");
        console.log(result.slice(0, 10).join(" "));
    }
}

// Run the test harness
main();
