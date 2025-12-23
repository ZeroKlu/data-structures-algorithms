// insertion_sort.js
// Node.js script for insertion sort with step counting and file-based tests.
//
// PURPOSE
// -------
// This program demonstrates **Insertion Sort** on a list of integers, while also
// counting "steps" to help visualize time complexity.
//
// It expects two text files containing the SAME integers:
//   - unordered.txt  (unsorted input)
//   - ordered.txt    (the correctly sorted output, ascending)
//
// Each file should contain integers separated by whitespace (spaces/newlines/tabs).
//
// HOW TO RUN (Node ES Modules)
// ----------------------------
// 1) Ensure Node is running in ES-module mode:
//    - Either set "type": "module" in package.json, OR
//    - Rename this file to insertion_sort.mjs
//
// 2) Run:
//      node insertion_sort.js
//
// STEP COUNTING MODEL
// -------------------
// We count two types of work:
//   1) comparisons: how many times we compare arr[j] > key
//   2) writes:      how many times we write into the array (shifts + final insert)
//
// Notes:
//   - We do NOT count "key = arr[i]" as an array write, because it does not
//     modify the array; it copies a value into a local variable.
//   - We DO count:
//       * arr[j + 1] = arr[j]   (a shift)
//       * arr[j + 1] = key      (final insertion)
//
// COMPLEXITY (Big-O)
// ------------------
// Best case (already sorted):
//   - comparisons ~ n-1
//   - writes ~ n-1 (one "insert" per i)
// Worst case (reverse sorted):
//   - comparisons ~ n(n-1)/2
//   - writes ~ n(n-1)/2 + (n-1)  (many shifts + one insert per i)

import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";

// Resolve directory of this script.
//
// In ES modules, __dirname is not defined by default.
// We reconstruct it so our file paths are reliable even if you run the script
// from a different working directory.
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// ------------------------------------------------------------
// Insertion Sort with step counting
// ------------------------------------------------------------
//
// This function sorts the array IN PLACE and returns an object with stats.
//
// Insertion sort idea:
//   - Treat arr[0..i-1] as a sorted region.
//   - Take arr[i] (key) and insert it into the correct position by shifting
//     larger elements to the right.
//
function insertionSort(arr) {
    let comparisons = 0;
    let writes = 0;

    // Start at i = 1 because a single-element prefix (arr[0]) is already "sorted".
    for (let i = 1; i < arr.length; i++) {
        // key is the element we want to insert into the sorted region.
        const key = arr[i];

        // j scans leftward through the sorted region [0..i-1].
        let j = i - 1;

        // While j is valid, compare arr[j] with key and shift if needed.
        while (j >= 0) {
            // Count the comparison we are about to make: arr[j] > key
            comparisons++;

            if (arr[j] > key) {
                // Shift arr[j] one position to the right.
                arr[j + 1] = arr[j];
                writes++;

                // Move left and continue shifting as long as elements are > key.
                j--;
            } else {
                // arr[j] <= key means the correct insertion point is j+1.
                break;
            }
        }

        // Insert key into the "hole" at position j+1.
        // This happens once per outer iteration.
        arr[j + 1] = key;
        writes++;
    }

    return { comparisons, writes };
}

// ------------------------------------------------------------
// File loader with fallback paths
// ------------------------------------------------------------
//
// Problem we're solving:
//   The *working directory* (process.cwd()) might not be the same directory
//   where this script is located. So we build several candidate paths and
//   try them in order.
//
// Search paths (in this order):
//   1) <script_dir>/<name>
//   2) <script_dir>/data/<name>
//   3) <script_dir>/../data/<name>
//   4) <script_dir>/../../data/<name>
//   5) <Desktop>/data-structures-algorithms/code_samples/section12/data/<name>
//
// If none succeed, we print all attempted paths and exit.
//
function loadFile(name) {
    const home = process.env.USERPROFILE || process.env.HOME;

    const attempts = [
        path.join(__dirname, name),
        path.join(__dirname, "data", name),
        path.join(__dirname, "..", "data", name),
        path.join(__dirname, "..", "..", "data", name),

        // Your known project location (works if your repo is on Desktop)
        home
            ? path.join(
                home,
                "Desktop",
                "data-structures-algorithms",
                "code_samples",
                "section12",
                "data",
                name
            )
            : null
    ].filter(Boolean); // remove null if HOME/USERPROFILE wasn't available

    for (const p of attempts) {
        try {
            // Read file contents (synchronous for simplicity in a learning demo).
            const text = fs.readFileSync(p, "utf8");

            // Print which path worked so you can diagnose path issues easily.
            console.log("Loaded:", p);

            // Parse integers from ANY whitespace.
            // - trim() removes leading/trailing whitespace.
            // - split(/\s+/) splits on one-or-more whitespace characters.
            // - map(Number) converts each token to a number.
            return text
                .trim()
                .split(/\s+/)
                .map(Number);
        } catch {
            // If this path fails (file not found, etc.), silently try the next.
        }
    }

    // If we got here, all attempts failed.
    console.error("Error: Could not load", name);
    console.error("Search paths were:");
    for (const p of attempts) console.error("  " + p);

    process.exit(1);
}

// ------------------------------------------------------------
// Test harness (main program)
// ------------------------------------------------------------
//
// Steps:
//   1) Load unordered + ordered
//   2) Copy unordered -> arr
//   3) Sort arr with insertion sort + collect stats
//   4) Compare arr to expected
//   5) Print results
//
function main() {
    const unordered = loadFile("unordered.txt");
    const expected = loadFile("ordered.txt");

    // Copy the input so we don't destroy the original "unordered" array.
    const arr = [...unordered];

    // Sort and collect step counts
    const stats = insertionSort(arr);

    // Verify correctness by scanning for the first mismatch.
    // (We could also use JSON stringify or array.every, but this is explicit
    // and lets us print the first failing index/value.)
    let mismatch = -1;
    for (let i = 0; i < arr.length; i++) {
        if (arr[i] !== expected[i]) {
            mismatch = i;
            break;
        }
    }

    // Print summary
    console.log("\nInsertion Sort (JavaScript)");
    console.log("---------------------------");
    console.log("Elements:     ", arr.length);
    console.log("Comparisons:  ", stats.comparisons);
    console.log("Writes:       ", stats.writes);

    if (mismatch === -1) {
        console.log("Result:       PASS ✔ (sorted output matches ordered.txt)");
    } else {
        console.log("Result:       FAIL ✘ (sorted output does NOT match ordered.txt)");
        console.log(
            `Mismatch at index ${mismatch}: got ${arr[mismatch]}, expected ${expected[mismatch]}`
        );
    }
}

main();
