// function mergeSort(arr) {
//     if (arr.length <= 1) return arr.slice();
//     const mid = Math.floor(arr.length / 2);
//     const left = mergeSort(arr.slice(0, mid));
//     const right = mergeSort(arr.slice(mid));
//     return merge(left, right);
// }

// function merge(left, right) {
//     const result = [];
//     let i = 0, j = 0;
//     while (i < left.length && j < right.length) {
//         if (left[i] <= right[j]) {
//             result.push(left[i++]);
//         } else {
//             result.push(right[j++]);
//         }
//     }
//     return result.concat(left.slice(i)).concat(right.slice(j));
// }

/*
    -------------------------------------------------------------------------
    COMMENTED-OUT (baseline) MERGE SORT
    -------------------------------------------------------------------------
    This block is the original “clean” merge sort that just returns a sorted
    copy of the input.

    It’s commented out so you can compare it to the instrumented version below,
    which adds step counting + file-based testing.

    In real projects you’d remove this, but for learning it’s useful context.
*/

// Example 8 - Merge Sort with Step Counting
import fs from "fs";
import path from "path";
import url from "url";

// ---------------------------------------------------------------------------
// Step counters (global for simplicity)
// ---------------------------------------------------------------------------
/*
    comparisons:
      - increments each time we compare left[i] <= right[j]

    writes:
      - increments each time we write a value into result[]
        (each push counts as one write)

    Note:
      - This version counts writes to the output array only.
      - It does NOT count the cost of slice() allocations, which also copy data.
        (That’s fine for a “Big-O illustration” but worth mentioning.)
*/
let comparisons = 0;
let writes = 0;

/** ------------------------------------------------------------------------
 * merge(left, right)
 * -------------------------------------------------------------------------
 * Merge two sorted arrays into one sorted array.
 *
 * Step counting:
 *  - comparisons++ for each left[i] <= right[j] check
 *  - writes++ for each push into result
 */
function merge(left, right) {
    const result = [];
    let i = 0, j = 0;

    // Merge until one side is exhausted
    while (i < left.length && j < right.length) {
        comparisons++;   // compare left[i] <= right[j]

        if (left[i] <= right[j]) {
            result.push(left[i++]);
        } else {
            result.push(right[j++]);
        }
        writes++;        // wrote into result[]
    }

    // Copy remaining elements from left
    while (i < left.length) {
        result.push(left[i++]);
        writes++;
    }

    // Copy remaining elements from right
    while (j < right.length) {
        result.push(right[j++]);
        writes++;
    }

    return result;
}

/** ------------------------------------------------------------------------
 * mergeSort(arr)
 * -------------------------------------------------------------------------
 * Recursive merge sort that returns a NEW sorted array (does not sort in-place).
 *
 * Base case:
 *  - length 0 or 1 is already sorted (return a copy with slice()).
 *
 * Recursive case:
 *  - split into halves, sort each, merge the results.
 */
function mergeSort(arr) {
    if (arr.length <= 1) return arr.slice(); // return copy

    const mid = Math.floor(arr.length / 2);
    const left = mergeSort(arr.slice(0, mid));
    const right = mergeSort(arr.slice(mid));

    return merge(left, right);
}

// ---------------------------------------------------------------------------
// File Loading Utility (same logic as bubble/selection sort JS examples)
// ---------------------------------------------------------------------------
/*
    loadFile(name)
    --------------
    Robust loader that tries two candidate locations:

      1) ../data/<name> relative to THIS script file
         - works when script lives in lesson folder next to ../data

      2) Explicit Desktop project layout
         - C:\Users\<you>\Desktop\data-structures-algorithms\code_samples\section12\data\<name>

    Why the __dirname stuff?
      - In ES modules, __dirname isn’t available by default, so we build it
        from import.meta.url.
*/
function loadFile(name) {
    const __dirname = path.dirname(url.fileURLToPath(import.meta.url));

    // 1) relative: ../data/<name>
    const rel = path.resolve(__dirname, "..", "data", name);
    console.log("Attempting to read:", rel);

    if (fs.existsSync(rel)) {
        return fs.readFileSync(rel, "utf8")
            .trim()
            .split(/\s+/)
            .map(Number);
    }

    // 2) explicit path (your known layout)
    const explicit = path.resolve(
        process.env.USERPROFILE,
        "Desktop",
        "data-structures-algorithms",
        "code_samples",
        "section12",
        "data",
        name
    );

    console.log("Relative path not found, trying:", explicit);

    if (fs.existsSync(explicit)) {
        return fs.readFileSync(explicit, "utf8")
            .trim()
            .split(/\s+/)
            .map(Number);
    }

    // Neither location worked
    throw new Error(`Missing file '${name}' in:\n  ${rel}\n  ${explicit}`);
}

// ---------------------------------------------------------------------------
// Main Execution / Test Harness
// ---------------------------------------------------------------------------
/*
    main()
    ------
    1) Load unordered.txt and ordered.txt
    2) Reset counters
    3) Sort unordered with merge sort (returns NEW array)
    4) Check correctness vs ordered.txt
    5) Print counters and a small sample output

    Correctness check:
      - Uses JSON stringify to compare arrays.
      - Works fine here because these are flat number arrays.
      - Alternative would be a loop compare (more efficient for large arrays).
*/
function main() {
    try {
        const unordered = loadFile("unordered.txt");
        const expected = loadFile("ordered.txt");

        console.log("Unordered length:", unordered.length);
        console.log("Expected length:", expected.length);

        // reset counters for this run
        comparisons = 0;
        writes = 0;

        const sorted = mergeSort(unordered);

        const correct = JSON.stringify(sorted) === JSON.stringify(expected);
        console.log("Sorted correctly?", correct);

        console.log("Comparisons:", comparisons);
        console.log("Writes:", writes);

        console.log("First 10 sorted elements:", sorted.slice(0, 10).join(" "));
    } catch (err) {
        console.error("Error reading input files:");
        console.error(err.message);
        console.log("Missing input file — aborting.");
    }
}

main();
